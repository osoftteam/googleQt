#include "Cache.h"

using namespace googleQt;

///CacheData
googleQt::CacheData::CacheData(googleQt::EDataState state, QString id)
    :m_id(id)
{
    m_flags.flags = 0;
    m_flags.agg_state = static_cast<int>(state);
};

int googleQt::CacheData::aggState()const 
{
    return m_flags.agg_state;
};

bool googleQt::CacheData::isLoaded(EDataState st)const
{
    bool rv = (m_flags.agg_state & static_cast<int>(st)) != 0;
    return rv;
};

bool googleQt::CacheData::hasDbRecord()const 
{
    bool rv = (m_flags.has_db_record == 1);
    return rv;
};

void googleQt::CacheData::markDbRecord() 
{
    m_flags.has_db_record = 1;
};

bool googleQt::CacheData::userFlag1()const
{
    return (m_flags.user_flag1 == 1 ? true : false);
};

void googleQt::CacheData::setUserFlag1(bool val)
{
    m_flags.user_flag1 = val ? 1 : 0;
};

///CacheDataWithHistory
googleQt::CacheDataWithHistory::CacheDataWithHistory(QString id, quint64 hid) 
    :CacheData(EDataState::snippet, id),
    m_history_id(hid)
{

};
