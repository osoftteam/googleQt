#include "Cache.h"

using namespace googleQt;

///CacheData
googleQt::CacheData::CacheData(googleQt::EDataState state, QString id)
	:m_state_agg(static_cast<int>(state)), m_id(id)
{

};

bool googleQt::CacheData::isLoaded(EDataState st)const
{
	bool rv = (m_state_agg & static_cast<int>(st)) != 0;
	return rv;
};

/*
template <class O>
CacheQueryTask::CacheQueryTask(EDataState load, ApiEndpoint& ept, GoogleCacheBase<O>* c) 
    :GoogleTask<CacheDataList<O>>(ept), m_cache(c)
{
    m_completed.reset(new CacheDataList<O>);
    m_completed->state = load;
};

void googleQt::CacheQueryTask::notifyOnCompletedFromCache()
{
    m_query_completed = true;
    notifyOnFinished();
};

void googleQt::CacheQueryTask::notifyFetchCompleted(CACHE_QUERY_RESULT_MAP<O>& r, const std::set<QString>& fetched_ids)
{
    m_query_completed = true;
    m_cache->merge(m_completed->state, r, fetched_ids);
    notifyOnFinished();
}

void googleQt::CacheQueryTask::add(std::shared_ptr<O> obj)
{
    m_completed->result_map[obj->id()] = obj;
    m_completed->result_list.push_back(obj);
};
*/
