#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <QCache>
#include "GoogleTask.h"
#include "google/endpoint/ApiEndpoint.h"

namespace googleQt {

    template<class O>
    using CACHE_QUERY_RESULT_MAP  = std::map<QString, std::shared_ptr<O>>;
    template<class O>
    using CACHE_QUERY_RESULT_LIST = std::list<std::shared_ptr<O>>;
    template<class O>
	using CACHE_MAP = CACHE_QUERY_RESULT_MAP<O>;
    template<class O>
	using CACHE_ORD = CACHE_QUERY_RESULT_LIST<O>;

    enum class EDataState
    {
        snippet = 1,
            body = 2
            };

    class CacheData 
    {
    public:
        CacheData(EDataState state, QString id);

        virtual bool  isLoaded(EDataState st)const;
        virtual void  merge(CacheData* other) = 0;
        QString       id()const { return m_id; }
        int           aggState()const { return m_state_agg; }
    protected:
        int         m_state_agg;
        QString     m_id;
    };

    template<class O>
    class GoogleCacheBase
    {
    public:
        virtual bool mem_has_object(QString id)const = 0;
        virtual std::shared_ptr<O> mem_object(QString id) = 0;
        virtual void mem_insert(QString id, std::shared_ptr<O>) = 0;
        virtual size_t mem_size()const = 0;
        virtual void mem_clear() = 0;
        virtual void persistent_clear(const std::set<QString>& ids2delete) = 0;
        virtual void update_persistent_storage(EDataState state, CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids) = 0;
        
        void merge(EDataState state, 
            CACHE_QUERY_RESULT_MAP<O>& r, 
            const std::set<QString>& fetched_ids)
        {          
            
            for (typename CACHE_QUERY_RESULT_MAP<O>::iterator i = r.begin();
                 i != r.end(); i++) 
                {
                    QString id = i->first;
                    std::shared_ptr<O> new_obj = i->second;
                    std::shared_ptr<O> cache_obj = mem_object(id);
                    if (cache_obj)
                        {
                            cache_obj->merge(new_obj.get());
                        }
                    else
                        {
                            mem_insert(id, new_obj);
                        }
                }
            update_persistent_storage(state, r, fetched_ids);
        }
    };

    template <class O>
    class CacheDataList
    {
    public:
        CACHE_QUERY_RESULT_MAP<O> result_map;
        CACHE_QUERY_RESULT_LIST<O> result_list;
        EDataState state;
    };

    template <class O>
    using CacheTaskParent = googleQt::GoogleTask<CacheDataList<O>>;
    
    /**
        CacheQueryTask - we can query server and feed parsed results
        back to the cache. 
    */
    template <class O>
    class CacheQueryTask : public CacheTaskParent<O>
    {
    public:
        CacheQueryTask(EDataState load, ApiEndpoint& ept, std::shared_ptr<GoogleCacheBase<O>> c)
            :GoogleTask<CacheDataList<O>>(ept),
            m_cache(c){CacheTaskParent<O>::m_completed.reset(new CacheDataList<O>); CacheTaskParent<O>::m_completed->state = load; };


        virtual void fetchFromCloud_Async(const std::list<QString>& id_list) = 0;
        std::unique_ptr<CacheDataList<O>> waitForResultAndRelease() = delete;

        void notifyOnCompletedFromCache(){ m_query_completed = true;CacheTaskParent<O>::notifyOnFinished(); };
        void notifyFetchCompleted(CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids)        
        {
            m_query_completed = true;
            m_cache->merge(CacheTaskParent<O>::m_completed->state, r, fetched_ids);
            CacheTaskParent<O>::notifyOnFinished();
        }
        
        bool isCompleted()const override { return m_query_completed; }

        void add(std::shared_ptr<O> obj){ CacheTaskParent<O>::m_completed->result_map[obj->id()] = obj; CacheTaskParent<O>::m_completed->result_list.push_back(obj); };
        void inc_mem_cache_hit_count() { m_cache_hit_count++; }
        void set_db_cache_hit_count(size_t val) { m_db_cache_hit_count = val; }
        size_t mem_cache_hit_count()const { return m_cache_hit_count; }
        size_t db_cache_hit_count()const { return m_db_cache_hit_count; }

    protected:
        std::shared_ptr<GoogleCacheBase<O>> m_cache;
        size_t     m_cache_hit_count{0};
        size_t     m_db_cache_hit_count{0};
        bool       m_query_completed{ false };
    };
    
    template <class O, class R>
    class LocalPersistentStorage
    {
    public:
        virtual std::list<QString> load_db(EDataState state, 
                                           const std::list<QString>& id_list,
                                           R* cr) = 0;
        virtual void update_db(EDataState state, CACHE_QUERY_RESULT_LIST<O>& r) = 0;
        virtual void update_db(EDataState state,
            CACHE_QUERY_RESULT_MAP<O>& r, 
            const std::set<QString>& fetched_ids)
        {
            CACHE_QUERY_RESULT_LIST<O> lst;
            typedef typename CACHE_QUERY_RESULT_MAP<O>::iterator ITR;
            for (ITR i = r.begin(); i != r.end(); i++) 
                {
                    if (fetched_ids.find(i->first) != fetched_ids.cend())
                        {
                            lst.push_back(i->second);
                        }
                }
            update_db(state, lst);
        };
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

			m_ord.remove_if([&](const std::shared_ptr<O>& o) { return (ids2delete.find(o->id()) != ids2delete.end()); });

            if (m_localDB &&
                m_localDB->isValid())
                {
                    m_localDB->remove_db(ids2delete);
                }
        };

        void update_persistent_storage(
            EDataState state,
            CACHE_QUERY_RESULT_MAP<O>& r, 
            const std::set<QString>& fetched_ids)override
        {
            if(m_localDB &&
               m_localDB->isValid() &&
               !r.empty())
                {
                    m_localDB->update_db(state, r, fetched_ids);
                }            
        }

        void query_Async(EDataState load, const std::list<QString>& id_list, R* rfetcher)
        {
            std::list<QString> missed_cache;
            for (std::list<QString>::const_iterator i = id_list.begin(); i != id_list.end(); i++)
                {
                    QString id = *i;
                    std::shared_ptr<O> obj = mem_object(id);
                    if (obj && obj->isLoaded(load))
                        {
                            rfetcher->add(obj);
                            rfetcher->inc_mem_cache_hit_count();
                        }
                    else 
                        {
                            missed_cache.push_back(id);
                        }
                }//swipping cache

            if(m_localDB &&
               m_localDB->isValid() &&
               !missed_cache.empty())
                {
                    size_t items2load = missed_cache.size();
                    missed_cache = m_localDB->load_db(load, missed_cache, rfetcher);
                    items2load -= missed_cache.size();
                    rfetcher->set_db_cache_hit_count(items2load);
                }
            
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
    protected:
        ApiEndpoint&                m_endpoint;
        CACHE_MAP<O>                m_mcache;
        CACHE_ORD<O>                m_ord;
        LocalPersistentStorage<O, R>* m_localDB;
        bool                        m_valid{true};
    };
};
