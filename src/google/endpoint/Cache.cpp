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