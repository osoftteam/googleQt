#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <QCache>
#include "GoogleTask.h"
#include "google/endpoint/ApiEndpoint.h"

namespace googleQt {

	template<class O>
	using CACHE_QUERY_RESULT_MAP = std::map<QString, std::shared_ptr<O>>;

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
    protected:
        int			m_state_agg;
        QString     m_id;
    };

	template<class O>
	class GoogleCacheBase
	{
	public:
        virtual std::shared_ptr<O>* mem_object(QString id) = 0;
        virtual bool mem_insert(QString id, std::shared_ptr<O>*) = 0;
        virtual void update_persistent_storage(CACHE_QUERY_RESULT_MAP<O>& r) = 0;
        
		void merge(CACHE_QUERY_RESULT_MAP<O>& r) 
		{
            
			for (typename CACHE_QUERY_RESULT_MAP<O>::iterator i = r.begin();
                 i != r.end(); i++) 
                {
                    QString id = i->first;
                    std::shared_ptr<O> new_obj = i->second;
                    std::shared_ptr<O>* cache_obj = mem_object(id);
                    if (cache_obj != nullptr)
                        {
                            (*cache_obj)->merge(new_obj.get());
                        }
                    else
                        {
                            std::shared_ptr<O>* ptr = new std::shared_ptr<O>(new_obj);
                            mem_insert(id, ptr);
                        }
                }
		}
	};

    template <class O>
    class CacheQueryResult : public EndpointRunnable 
    {
    public:
        CacheQueryResult(EDataState load, ApiEndpoint& ept, GoogleCacheBase<O>* c) 
			:EndpointRunnable(ept), m_load(load), m_cache(c){};

        virtual void fetchFromCloud_Async(const std::list<QString>& id_list) = 0;
		void notifyOnCompletedFromCache() { notifyOnFinished(); };
		void notifyFetchCompleted(CACHE_QUERY_RESULT_MAP<O>& r) 
		{
			m_cache->merge(r);
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
			return m_result;
		}

		void add(QString id, std::shared_ptr<O> obj) { m_result[id] = obj; };

    protected:
		CACHE_QUERY_RESULT_MAP<O> m_result;
        EDataState m_load;
		GoogleCacheBase<O>* m_cache;
    };
    
    template <class O, class R>
    class LocalPersistentStorage
    {
    public:
        //        LocalPersistentStorage(GoogleCacheBase<O> c){}
        virtual std::list<QString> load(const std::list<QString>& id_list,
                                        std::unique_ptr<R>& cr) = 0;
        virtual void update(CACHE_QUERY_RESULT_MAP<O>& r) = 0;
        virtual bool isValid()const = 0;
    };
    
    template <class O, class R>
    class GoogleCache : public GoogleCacheBase<O>
    {
    public:
        GoogleCache(ApiEndpoint& ept, int maxSize = 1000): m_endpoint(ept), m_mem_cache(maxSize){};
        void setupLocalStorage(LocalPersistentStorage<O, R>* localDB){m_localDB = localDB;};
		virtual std::unique_ptr<R> produceCloudResultFetcher(EDataState load, ApiEndpoint& ept) = 0;

        std::shared_ptr<O>* mem_object(QString id)override
        {
            return m_mem_cache.object(id);
        }
        bool mem_insert(QString id, std::shared_ptr<O>* o)override
        {
            bool rv = m_mem_cache.insert(id, o);
            if(!rv)
                {
                    qWarning() << "cache insert failed" << m_mem_cache.size() << m_mem_cache.maxCost() << id;
                    delete o;
                }
            return rv;
        }

        void update_persistent_storage(CACHE_QUERY_RESULT_MAP<O>& r)override
        {
            if(m_localDB != nullptr &&
               m_localDB->isValid() &&
               !r.empty())
                {
                    m_localDB->update(r);
                }            
        }
        
        std::unique_ptr<R> query_Async(EDataState load, const std::list<QString>& id_list)
        {
			std::unique_ptr<R> rv = produceCloudResultFetcher(load, m_endpoint);

            std::list<QString> missed_cache;
            for (std::list<QString>::const_iterator i = id_list.begin(); i != id_list.end(); i++)
            {
                QString id = *i;
				std::shared_ptr<O>* obj = m_mem_cache.object(id);
                if (obj != NULL && (*obj)->isLoaded(load))
                {
                    rv->add(id, *obj);
                }
                else 
                {
                    missed_cache.push_back(id);
                }
            }//swipping cache

            if(m_localDB != nullptr &&
               m_localDB->isValid() &&
               !missed_cache.empty())
                {
                    missed_cache = m_localDB->load(missed_cache, rv);
                }
            
            if (!missed_cache.empty())
                {
                    rv->fetchFromCloud_Async(missed_cache);
                }
            else 
				{
					rv->notifyOnCompletedFromCache();
				}            
            return rv;
        };
    protected:
        ApiEndpoint& m_endpoint;
        QCache<QString, std::shared_ptr<O>> m_mem_cache;
        LocalPersistentStorage<O, R>* m_localDB {nullptr};
    };
};
