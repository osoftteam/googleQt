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
	using CACHE_MAP = std::map<int, CACHE_QUERY_RESULT_MAP<O>>;
	template<class O>
	using CACHE_ORD = std::map<int, CACHE_QUERY_RESULT_LIST<O>>;

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
        virtual bool mem_has_object(int accId, QString id)const = 0;
        virtual std::shared_ptr<O> mem_object(int accId, QString id) = 0;
        virtual void mem_insert(int accId, QString id, std::shared_ptr<O>) = 0;
        virtual size_t mem_size()const = 0;
        virtual void mem_clear() = 0;
        virtual void persistent_clear(int accId, const std::set<QString>& ids2delete) = 0;
        virtual void update_persistent_storage(int accId, EDataState state, CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids) = 0;
        
        void merge(int accId,
			EDataState state, 
			CACHE_QUERY_RESULT_MAP<O>& r, 
			const std::set<QString>& fetched_ids)
        {          
            
            for (typename CACHE_QUERY_RESULT_MAP<O>::iterator i = r.begin();
                 i != r.end(); i++) 
                {
                    QString id = i->first;
                    std::shared_ptr<O> new_obj = i->second;
                    std::shared_ptr<O> cache_obj = mem_object(accId, id);
                    if (cache_obj)
                        {
                            cache_obj->merge(new_obj.get());
                        }
                    else
                        {
                            mem_insert(accId, id, new_obj);
                        }
                }
            update_persistent_storage(accId, state, r, fetched_ids);
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
		CacheQueryTask - account-based class that can query server and feed parsed results
		back to the cache. Sort of cache-helper, key thing is that it's account-based
		For gmail accoundId is DB-based ID of userId, for APIs that don't support userId
		same accountId can be used, '0' for example. '-1' means invalid accountId
	*/
    template <class O>
    class CacheQueryTask : public CacheTaskParent<O>
    {
    public:
        CacheQueryTask(int accId, EDataState load, ApiEndpoint& ept, std::shared_ptr<GoogleCacheBase<O>> c)
            :GoogleTask<CacheDataList<O>>(ept),
            m_accountId(accId),
            m_cache(c){CacheTaskParent<O>::m_completed.reset(new CacheDataList<O>); CacheTaskParent<O>::m_completed->state = load; };


        virtual void fetchFromCloud_Async(const std::list<QString>& id_list) = 0;
        std::unique_ptr<CacheDataList<O>> waitForResultAndRelease() = delete;

        void notifyOnCompletedFromCache(){ m_query_completed = true;CacheTaskParent<O>::notifyOnFinished(); };
        void notifyFetchCompleted(CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids)        
        {
            m_query_completed = true;
            m_cache->merge(m_accountId, CacheTaskParent<O>::m_completed->state, r, fetched_ids);
            CacheTaskParent<O>::notifyOnFinished();
        }
        
		int accountId()const { return m_accountId; }
        bool isCompleted()const override { return m_query_completed; }

        void add(std::shared_ptr<O> obj){ CacheTaskParent<O>::m_completed->result_map[obj->id()] = obj; CacheTaskParent<O>::m_completed->result_list.push_back(obj); };
        void inc_mem_cache_hit_count() { m_cache_hit_count++; }
        void set_db_cache_hit_count(size_t val) { m_db_cache_hit_count = val; }
        size_t mem_cache_hit_count()const { return m_cache_hit_count; }
        size_t db_cache_hit_count()const { return m_db_cache_hit_count; }

    protected:
		int		m_accountId;
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
        virtual void update_db(int accId, EDataState state, CACHE_QUERY_RESULT_LIST<O>& r) = 0;
        virtual void update_db(int accId, 
			EDataState state,
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
            update_db(accId, state, lst);
        };
        virtual void remove_db(int accId, const std::set<QString>& ids2remove) = 0;
        virtual bool isValid()const = 0;
    };
    
    template <class O, class R>
    class GoogleCache : public GoogleCacheBase<O>
    {
    public:
        GoogleCache(ApiEndpoint& ept): m_endpoint(ept){};
        void setupLocalStorage(LocalPersistentStorage<O, R>* localDB){m_localDB.reset(localDB);};
        bool hasLocalPersistentStorate()const { return(m_localDB.get() != nullptr); };

        bool mem_has_object(int accId, QString id)const override
        {
			bool rv = false;
			auto i = m_mcache.find(accId);
			if (i != m_mcache.end()) {
				rv = (i->second.find(id) != i->second.end());
			}
			return rv;
            //return (m_mem_cache.find(id) != m_mem_cache.end());
        };

        std::shared_ptr<O> mem_object(int accId, QString id)override
        {
            std::shared_ptr<O> rv;
			auto i = m_mcache.find(accId);
			if (i != m_mcache.end()) {
				auto j = i->second.find(id);
				if (j != i->second.end()) {
					rv = j->second;
				}
				//rv = (i->second.find(id) != i->second.end());
			}
			/*
            auto i = m_mem_cache.find(id);
            if (i != m_mem_cache.end())
                rv = i->second;
				*/
            return rv;
        }
        void mem_insert(int accId, QString id, std::shared_ptr<O> o)override
        {
			auto i = m_mcache.find(accId);
			if (i != m_mcache.end()) {
				if (i->second.insert(std::make_pair(id, o)).second) {
					m_ord[accId].push_back(o);
					//m_order_cache.push_back(o);
				}
			}
			else {
				CACHE_QUERY_RESULT_MAP<O> mres;
				mres[id] = o;
				m_mcache[accId] = mres;
	//			m_order_cache.push_back(o);
				m_ord[accId].push_back(o);
			}

			/*
            if (m_mem_cache.insert(std::make_pair(id, o)).second)
                {
                    m_order_cache.push_back(o);
                }
				
            return true;*/
        }

        size_t mem_size()const override 
        {
			size_t rv = 0;
			for (auto i = m_mcache.begin(); i != m_mcache.end(); i++) {
				rv += i->second.size();
			}
			return rv;
         //   return m_mem_cache.size();
        }

        void mem_clear()override 
        {
			m_mcache.clear();
			m_ord.clear();
            //m_order_cache.clear();
        };

        
        void persistent_clear(int accId, const std::set<QString>& ids2delete) override
        {
			auto k = m_mcache.find(accId);
			if (k != m_mcache.end()) {
				for (auto i = ids2delete.cbegin(); i != ids2delete.cend(); i++)
				{
					k->second.erase(*i);
				}
			}

			auto r = m_ord.find(accId);
			if (r != m_ord.end()) {
				r->second.remove_if([&](const std::shared_ptr<O>& o) { return (ids2delete.find(o->id()) != ids2delete.end()); });
			}

            //m_order_cache.remove_if([&](const std::shared_ptr<O>& o) { return (ids2delete.find(o->id()) != ids2delete.end()); });

            if (m_localDB != nullptr &&
                m_localDB->isValid())
                {
                    m_localDB->remove_db(accId, ids2delete);
                }
        };

        void update_persistent_storage(int accId, 
			EDataState state,
			CACHE_QUERY_RESULT_MAP<O>& r, 
			const std::set<QString>& fetched_ids)override
        {
            if(m_localDB != nullptr &&
               m_localDB->isValid() &&
               !r.empty())
                {
                    m_localDB->update_db(accId, state, r, fetched_ids);
                }            
        }

        void query_Async(EDataState load, const std::list<QString>& id_list, R* rfetcher)
        { //cache-lookup makes sense only against account defined in 'fetcher' so cache and fetcher 
		//	should be compatible, using userId/accId
            std::list<QString> missed_cache;
            for (std::list<QString>::const_iterator i = id_list.begin(); i != id_list.end(); i++)
                {
                    QString id = *i;
                    std::shared_ptr<O> obj = mem_object(rfetcher->accountId(), id);
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

    protected:
        ApiEndpoint&                m_endpoint;
		CACHE_MAP<O>				m_mcache;
		CACHE_ORD<O>				m_ord;
        std::unique_ptr<LocalPersistentStorage<O, R>> m_localDB;
    };
};
