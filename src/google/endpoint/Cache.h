#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <QCache>
#include "GoogleTask.h"
#include "google/endpoint/ApiEndpoint.h"

#ifdef API_QT_AUTOTEST
#define VERIFY_CACHE EXPECT((m_mem_cache.size() == m_order_cache.size()), QString("Inconsistant cache structure %1/%2").arg(m_mem_cache.size()).arg(m_order_cache.size()));
#else
#define VERIFY_CACHE 
#endif

namespace googleQt {

	template<class O>
	using CACHE_QUERY_RESULT_MAP  = std::map<QString, std::shared_ptr<O>>;
    template<class O>
    using CACHE_QUERY_RESULT_LIST = std::list<std::shared_ptr<O>>;

    enum class EDataState
    {
        snippet	= 1,
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
        int			m_state_agg;
        QString     m_id;
    };

	template<class O>
	class GoogleCacheBase
	{
	public:
        virtual bool mem_has_object(QString id)const = 0;
        virtual std::shared_ptr<O> mem_object(QString id) = 0;
        virtual bool mem_insert(QString id, std::shared_ptr<O>) = 0;
        virtual size_t mem_size()const = 0;
        virtual void mem_clear() = 0;
        virtual void persistent_clear(const std::set<QString>& ids2delete) = 0;
        virtual void update_persistent_storage(EDataState state, CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids) = 0;
        
		void merge(EDataState state, CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids)
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
    class CacheQueryResult : public EndpointRunnable 
    {
    public:
        CacheQueryResult(EDataState load, ApiEndpoint& ept, GoogleCacheBase<O>* c) 
			:EndpointRunnable(ept), m_state(load), m_cache(c){};

        virtual void fetchFromCloud_Async(const std::list<QString>& id_list) = 0;
		void notifyOnCompletedFromCache() { m_completed = true;notifyOnFinished(); };
		void notifyFetchCompleted(CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids)
		{
            m_completed = true;
			m_cache->merge(m_state, r, fetched_ids);
			notifyOnFinished();
		}

		std::map<QString, std::shared_ptr<O>> waitForResultAndRelease()
		{
			if (!isFinished())
			{
				m_in_wait_loop = true;
				waitUntillFinishedOrCancelled();
			}

			deleteLater();
			return std::move(m_result);
		}

        std::list<std::shared_ptr<O>> waitForResultListAndRelease()
        {
            if (!isFinished())
            {
                m_in_wait_loop = true;
                waitUntillFinishedOrCancelled();
            }

            deleteLater();
            return std::move(m_result_list);
        }

        bool isCompleted()const override { return m_completed; }

        void add(std::shared_ptr<O> obj) { m_result[obj->id()] = obj; m_result_list.push_back(obj); };
        void inc_mem_cache_hit_count() { m_mem_cache_hit_count++; }
        void set_db_cache_hit_count(size_t val) { m_db_cache_hit_count = val; }
        size_t mem_cache_hit_count()const { return m_mem_cache_hit_count; }
        size_t db_cache_hit_count()const { return m_db_cache_hit_count; }

    protected:
		CACHE_QUERY_RESULT_MAP<O> m_result;
        CACHE_QUERY_RESULT_LIST<O> m_result_list;
        EDataState m_state;
		GoogleCacheBase<O>* m_cache;
        size_t     m_mem_cache_hit_count{0};
        size_t     m_db_cache_hit_count{0};
        bool       m_completed{ false };
    };
    
    template <class O, class R>
    class LocalPersistentStorage
    {
    public:
        virtual std::list<QString> load(EDataState state, 
                                        const std::list<QString>& id_list,
                                        R* cr) = 0;
        virtual void update(EDataState state, CACHE_QUERY_RESULT_LIST<O>& r) = 0;
        virtual void update(EDataState state, CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids)
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
            update(state, lst);
        };
        virtual void remove(const std::set<QString>& ids2remove) = 0;
        virtual bool isValid()const = 0;
		virtual void setStarred(QString msg_id, bool starred_on) = 0;
    };
    
    template <class O, class R>
    class GoogleCache : public GoogleCacheBase<O>
    {
    public:
        GoogleCache(ApiEndpoint& ept): m_endpoint(ept){};
        void setupLocalStorage(LocalPersistentStorage<O, R>* localDB){m_localDB .reset(localDB);};
        bool hasLocalPersistentStorate()const { return(m_localDB.get() != nullptr); };
		//virtual R* produceCloudResultFetcher(EDataState load, ApiEndpoint& ept) = 0;

        bool mem_has_object(QString id)const override 
        {
            return (m_mem_cache.find(id) != m_mem_cache.end());
        };

        std::shared_ptr<O> mem_object(QString id)override
        {
            std::shared_ptr<O> rv;
            auto i = m_mem_cache.find(id);
            if (i != m_mem_cache.end())
                rv = i->second;
            return rv;
        }
        bool mem_insert(QString id, std::shared_ptr<O> o)override
        {
            if (m_mem_cache.insert(std::make_pair(id, o)).second)
            {
                m_order_cache.push_back(o);
            }
            VERIFY_CACHE;
            return true;
        }

        size_t mem_size()const override 
        {
            return m_mem_cache.size();
        }

        void mem_clear()override 
        {
            VERIFY_CACHE;
            m_mem_cache.clear();
            m_order_cache.clear();
        };

        
        void persistent_clear(const std::set<QString>& ids2delete) override
        {
            for (auto i = ids2delete.cbegin(); i != ids2delete.cend(); i++)
            {
                m_mem_cache.erase(*i);
            }

            m_order_cache.remove_if([&](const std::shared_ptr<O>& o) { return (ids2delete.find(o->id()) != ids2delete.end()); });

            VERIFY_CACHE;

            if (m_localDB != nullptr &&
                m_localDB->isValid())
            {
                m_localDB->remove(ids2delete);
            }
        };

        void update_persistent_storage(EDataState state, CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids)override
        {
            if(m_localDB != nullptr &&
               m_localDB->isValid() &&
               !r.empty())
                {
                    m_localDB->update(state, r, fetched_ids);
                }            
        }
        
		void update_persistent_starred(QString msg_id, bool starred_on) 
		{
			if (m_localDB != nullptr &&
				m_localDB->isValid())
			{
				m_localDB->setStarred(msg_id, starred_on);
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
                    missed_cache = m_localDB->load(load, missed_cache, rfetcher);
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

        void cacheData(R* rfetcher, int number2load)
        {
            int count = 0;
            for (auto& i : m_order_cache)
            {
                rfetcher->add(i);
                if (number2load > 0)
                {
                    if (++count >= number2load)
                        break;
                }
            }
            rfetcher->notifyOnCompletedFromCache();
        }

    protected:
        ApiEndpoint&                m_endpoint;
        CACHE_QUERY_RESULT_MAP<O>   m_mem_cache;
        CACHE_QUERY_RESULT_LIST<O>  m_order_cache;
        std::unique_ptr<LocalPersistentStorage<O, R>> m_localDB;
    };
};
