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

///CacheDataWithHistory
googleQt::CacheDataWithHistory::CacheDataWithHistory(QString id, quint64 hid) 
	:CacheData(EDataState::snippet, id),
	m_history_id(hid)
{

};
