#pragma once

namespace googleQt{

	class Endpoint;

    class GoogleRouteBase
    {        
    public:
        GoogleRouteBase(Endpoint* ep):m_end_point(ep){}

    protected:
        Endpoint* m_end_point;
    };

}
