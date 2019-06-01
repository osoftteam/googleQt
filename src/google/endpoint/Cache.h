#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <QCache>
#include "GoogleTask.h"
#include "google/endpoint/ApiEndpoint.h"

namespace googleQt {

    template<class O>
    using CACHE_MAP = std::map<QString, std::shared_ptr<O>>;
    template<class O>
    using CACHE_LIST = std::vector<std::shared_ptr<O>>;
    template<class O>
    using CACHE_ARR = std::vector<std::shared_ptr<O>>;

    enum class EDataState
    {
        labels  = 0,
        snippet = 1,
        body    = 2
    };

    struct HistId 
    {
        QString id;
        quint64 hid;
    };

    class CacheData 
    {
    public:
        CacheData(EDataState state, QString id);
        virtual ~CacheData(){}

        virtual bool    isLoaded(EDataState st)const;
        virtual void    merge(CacheData* other) = 0;
        QString         id()const { return m_id; }
        int             aggState()const;// { return m_state_agg; }
        bool            hasDbRecord()const;
        void            markDbRecord();
        /// userPtr - custom user data pointer
        void*           userPtr()const {return m_user_ptr;}
        void            setUserPtr(void* p)const { m_user_ptr = p; }
        
    protected:
        union STATE
        {
            uint64_t flags;
            struct
            {
                unsigned agg_state : 4;
                unsigned has_db_record : 1;
            };
        } m_flags;
        QString        m_id;
        mutable void*  m_user_ptr{nullptr};
    };

    class CacheDataWithHistory: public CacheData
    {
    public:
        CacheDataWithHistory(QString id, quint64 hid);

        quint64 historyId()const { return m_history_id; }
    protected:
        quint64 m_history_id;
    };

    template<class O>
    class GoogleCacheBase
    {
    public:
        virtual ~GoogleCacheBase(){}
        virtual bool mem_has_object(QString id)const = 0;
        virtual std::shared_ptr<O> mem_object(QString id) = 0;
        virtual void mem_insert(QString id, std::shared_ptr<O>) = 0;
        virtual size_t mem_size()const = 0;
        virtual void mem_clear() = 0;
        virtual void persistent_clear(const std::set<QString>& ids2delete) = 0;
        virtual void update_persistent_storage(EDataState state, CACHE_LIST<O>& r) = 0;
        virtual void insert_persistent_storage(EDataState state, CACHE_LIST<O>& r) = 0;
        virtual void reorder_data_on_completed_fetch(const CACHE_LIST<O>& from_cloud) = 0;

        void merge(EDataState state, CACHE_LIST<O>& r)
        {
            CACHE_LIST<O> updated_list;
            CACHE_LIST<O> new_list;
               
            for (typename CACHE_LIST<O>::iterator i = r.begin();
                 i != r.end(); i++) 
                {
                    std::shared_ptr<O> obj = *i;
                    QString id = obj->id();
                    std::shared_ptr<O> cache_obj = mem_object(id);
                    if (cache_obj)
                        {
                            cache_obj->merge(obj.get());
                            updated_list.push_back(obj);
                        }
                    else
                        {
                            mem_insert(id, obj);
                            new_list.push_back(obj);
                        }
                }

            if (!updated_list.empty()) {
                update_persistent_storage(state, updated_list);
            }

            if (!new_list.empty()) {
                insert_persistent_storage(state, new_list);
            }
        }
    };

    template <class O>
    class CacheDataResult
    {
    public:
        CACHE_MAP<O> result_map;
        CACHE_LIST<O> result_list;
        CACHE_LIST<O> from_cloud;
        EDataState state;
    };

    template <class O>
    using CacheTaskParent = googleQt::GoogleTask<CacheDataResult<O>>;
    
    
    template <class O, class R>
    class LocalPersistentStorage
    {
    public:
        virtual void update_db(EDataState state, CACHE_LIST<O>& r) = 0;
        virtual void insert_db(EDataState state, CACHE_LIST<O>& r) = 0;
        virtual void remove_db(const std::set<QString>& ids2remove) = 0;
        virtual bool isValid()const = 0;
    };
    
    template <class O, class R>
    class GoogleCache : public GoogleCacheBase<O>
    {
    public:
        GoogleCache(ApiEndpoint& ept): m_endpoint(ept){};
        void setupLocalStorage(LocalPersistentStorage<O, R>* localDB) { m_localDB = localDB; };

        bool mem_has_object(QString id)const override
        {
            bool rv = (m_mcache.find(id) != m_mcache.end());
            return rv;
        };

        std::shared_ptr<O> mem_object(QString id)override
        {
            std::shared_ptr<O> rv;
            auto i = m_mcache.find(id);
            if (i != m_mcache.end()) {
                rv = i->second;
            }
            return rv;
        }
        void mem_insert(QString id, std::shared_ptr<O> o)override
        {
            if (m_mcache.insert(std::make_pair(id, o)).second) {
                m_ord.push_back(o);
            }
        }

        size_t mem_size()const override 
        {
            size_t rv = m_mcache.size();
            return rv;
        }

        void mem_clear()override 
        {
            m_mcache.clear();
            m_ord.clear();
        };

        
        void persistent_clear(const std::set<QString>& ids2delete) override
        {
            for (auto i = ids2delete.cbegin(); i != ids2delete.cend(); i++)
            {
                m_mcache.erase(*i);
            }

            m_ord.erase(std::remove_if(m_ord.begin(), m_ord.end(), 
                [&](const std::shared_ptr<O>& o) { return (ids2delete.find(o->id()) != ids2delete.end()); }
                ), 
                m_ord.end());
            //m_ord.remove_if([&](const std::shared_ptr<O>& o) { return (ids2delete.find(o->id()) != ids2delete.end()); });

            if (m_localDB &&
                m_localDB->isValid())
                {
                    m_localDB->remove_db(ids2delete);
                }
        };

        void update_persistent_storage(EDataState state, CACHE_LIST<O>& r)override
        {
            if (m_localDB &&
                m_localDB->isValid() &&
                !r.empty())
            {
                m_localDB->update_db(state, r);
            }
        }

        void insert_persistent_storage(EDataState state, CACHE_LIST<O>& r)override
        {
            if (m_localDB &&
                m_localDB->isValid() &&
                !r.empty())
            {
                m_localDB->insert_db(state, r);
            }
        }


        void query_Async(EDataState load, const STRING_LIST& id_list, R* rfetcher)
        {
			if (!isValid()) {
				std::unique_ptr<GoogleException> ex(new GoogleException("Can't lookup on invalid DB cache."));
				rfetcher->failed_callback(std::move(ex));
				return;
			}

            STRING_LIST missed_cache;
            for (STRING_LIST::const_iterator i = id_list.begin(); i != id_list.end(); i++)
                {
                    QString id = *i;
                    std::shared_ptr<O> obj = mem_object(id);
                    if (obj && obj->isLoaded(load))
                        {
                            rfetcher->add_result(obj, false);
                            rfetcher->inc_mem_cache_hit_count();
                        }
                    else 
                        {
                            missed_cache.push_back(id);
                        }
                }//swipping cache
            
            if (!missed_cache.empty())
                {
                    rfetcher->fetchFromCloud_Async(missed_cache);
                }
            else 
                {
                    rfetcher->notifyOnCompletedFromCache();
                }            
        };

        void queryWithHistory_Async(const std::vector<HistId>& id_list, R* rfetcher)
        {
			if (!isValid()) {
				std::unique_ptr<GoogleException> ex(new GoogleException("Can't lookup on invalid DB cache."));
				rfetcher->failed_callback(std::move(ex));
				return;
			}

            STRING_LIST missed_cache;
            for (std::vector<HistId>::const_iterator i = id_list.begin(); i != id_list.end(); i++)
            {
                QString id = i->id;
                quint64 hid = i->hid;
                std::shared_ptr<O> obj = mem_object(id);
                if (obj)
                {
                    bool locatedHist = (hid == 0) || (hid != 0 && obj->historyId() == hid);
                    if(locatedHist){
                        rfetcher->add_result(obj, false);
                        rfetcher->inc_mem_cache_hit_count();
                    }else{
                        missed_cache.push_back(id);
                    }
                }
                else
                {
                    missed_cache.push_back(id);
                }
            }//swipping cache

            if (!missed_cache.empty())
            {
                rfetcher->fetchFromCloud_Async(missed_cache);
            }
            else
            {
                rfetcher->notifyOnCompletedFromCache();
            }
        };

        ApiEndpoint&  endpoint() { return m_endpoint; }

        bool isValid()const { return m_valid; };
        void invalidate()
        {
            m_mcache.clear();
            m_ord.clear();
            m_valid = false;
        };

        LocalPersistentStorage<O, R>* localDB() {return m_localDB;}
    protected:
        ApiEndpoint&                m_endpoint;
        CACHE_MAP<O>                m_mcache;
        CACHE_ARR<O>                m_ord;
        LocalPersistentStorage<O, R>* m_localDB{nullptr};
        bool                        m_valid{true};
    };//GoogleCache


    /**
    CacheQueryTask - we can query server and feed parsed results
    back to the cache.
    */
    template <class O>
    class CacheQueryTask : public CacheTaskParent<O>
    {
    public:
        CacheQueryTask(EDataState load, ApiEndpoint& ept, std::shared_ptr<GoogleCacheBase<O>> c)
            :GoogleTask<CacheDataResult<O>>(ept),
            m_cache(c) {
            CacheTaskParent<O>::m_completed.reset(new CacheDataResult<O>); CacheTaskParent<O>::m_completed->state = load;
        };

        virtual void fetchFromCloud_Async(const STRING_LIST& id_list) = 0;
        std::unique_ptr<CacheDataResult<O>> waitForResultAndRelease() = delete;

        virtual void notifyOnCompletedFromCache() 
        { 
            m_query_completed = true; 
            CacheTaskParent<O>::notifyOnFinished(); 
        };

        void notifyFetchCompletedWithMergeRequest(CACHE_LIST<O>& r)
        {
            m_cache->merge(CacheTaskParent<O>::m_completed->state, r);
            notifyOnCompletedFromCache();
        }

        bool isCompleted()const override { return m_query_completed; }

        void add_result(std::shared_ptr<O> obj, bool from_cloud) { 
            CacheTaskParent<O>::m_completed->result_map[obj->id()] = obj; 
            CacheTaskParent<O>::m_completed->result_list.push_back(obj); 
            if (from_cloud)CacheTaskParent<O>::m_completed->from_cloud.push_back(obj);
            auto p = CacheTaskParent<O>::progressNotifier();
            if (p) {
                p->setValue(CacheTaskParent<O>::m_completed->result_list.size());
            }
        };
        void inc_mem_cache_hit_count() { m_cache_hit_count++; }
        void set_db_cache_hit_count(size_t val) { m_db_cache_hit_count = val; }
        size_t mem_cache_hit_count()const { return m_cache_hit_count; }
        size_t db_cache_hit_count()const { return m_db_cache_hit_count; }

    protected:
        std::shared_ptr<GoogleCacheBase<O>> m_cache;
        size_t     m_cache_hit_count{ 0 };
        size_t     m_db_cache_hit_count{ 0 };
        bool       m_query_completed{ false };
    };//CacheQueryTask

};
