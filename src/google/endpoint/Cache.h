#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <QCache>
#include "GoogleTask.h"

namespace googleQt {

    enum class EDataState
    {
        partialyLoaded,
        completlyLoaded
    };

    class CacheData 
    {
    public:
        CacheData(EDataState state, QString id) :m_state(state), m_id(id) {};

        virtual bool isLoaded(EDataState st)const { return (m_state == st); };

        QString         id()const { return m_id; }
        EDataState      state()const { return   m_state; }
    protected:
        EDataState      m_state;
        QString         m_id;
    };

    template <class O>
    class CacheQueryResult : public EndpointRunnable 
    {
    public:
        CacheQueryResult(EDataState load, ApiEndpoint& ept) :EndpointRunnable(ept), m_load(load) {};

        virtual void fetchFromCloud_Async(const std::list<QString>& id_list) = 0;
    //protected:
        std::map<QString, O*> m_results;
        EDataState m_load;
    };

    template <class O>
    class GoogleCache //: public EndpointRunnable
    {
    public:
        GoogleCache(ApiEndpoint& ept) :m_endpoint(ept) {};

        CacheQueryResult<O>* query_Async(EDataState load, const std::list<QString>& id_list)
        {
            CacheQueryResult<O>* rv = new CacheQueryResult<O>(load, m_endpoint);

            std::list<QString> missed_cache;
            for (std::list<QString>::const_iterator i = id_list.begin(); i != id_list.end(); i++)
            {
                QString id = *i;
                O* obj = m_mem_cache.object(id);
                if (obj != NULL && obj->isLoaded(load))
                {
                    rv->m_results[id] = obj;
                }
                else 
                {
                    missed_cache.push_back(id);
                }

                if (!missed_cache.empty()) 
                {
                    rv->fetchFromCloud_Async(missed_cache);
                }
            }
            return rv;
        };
        //CacheQueryResult<O>* queryComplete_Async(const std::list<QString>& id_list);

    protected:
        QCache<QString, O> m_mem_cache;
        ApiEndpoint& m_endpoint;
    };
};
