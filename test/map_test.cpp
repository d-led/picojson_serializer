#include "../picojson_map_serializer.h"

#include <catch.hpp>
#include <map>

#include "test_helpers.h"

namespace {
    struct X {
        std::map< int, std::string > x;
        std::map< int, std::string > y;
        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar)
        {
            ar & picojson::convert::member("x", x);
            ar & picojson::convert::member("y", y);
        }
    };
}

TEST_CASE("map serialization") {
    X x;
    x.x[1]="42";
    x.x[3]="33";
    std::string xs=picojson::convert::to_string(x);
    picojson::value xv=picojson::convert::to_value(x);
    X y={};
    picojson::convert::from_value<X>(xv,y);
    CHECK( x.x == y.x );

    SECTION("const data") {
        X const xc(x);
        xs=picojson::convert::to_string(xc);
        y.x.clear();
        picojson::convert::from_value<X>(xv,y);
        CHECK( xc.x == y.x );
    }
}

TEST_CASE("map as root object") {
    std::map<int,int> m;
    m[1]=2;
    m[3]=4;
    picojson::value mv=picojson::convert::to_value(m);
    std::map<int,int> m_;
    picojson::convert::from_value(mv,m_);
    CHECK( m_[1] == 2 );
    CHECK( m_[3] == 4 );

    SECTION("const data") {
        std::map<int,int> const mc(m);
        mv=picojson::convert::to_value(mc);
        m_.clear();
        picojson::convert::from_value(mv,m_);
        CHECK( m_[1] == 2 );
        CHECK( m_[3] == 4 );
    }
}

TEST_CASE("multimap serialization") {
    X x;
    x.y.insert(std::make_pair(1,"42"));
    x.y.insert(std::make_pair(1,"11"));
    x.x[3]="33";
    std::string xs=picojson::convert::to_string(x);
    picojson::value xv=picojson::convert::to_value(x);
    X y={};
    picojson::convert::from_value<X>(xv,y);
    CHECK( x.x == y.x );
    CHECK( x.y == y.y );

    SECTION("const data") {
        X const xc(x);
        xs=picojson::convert::to_string(xc);
        y.x.clear();
        y.y.clear();
        picojson::convert::from_value<X>(xv,y);
        CHECK( xc.x == y.x );
        CHECK( xc.y == y.y );
    }
}

TEST_CASE("multimap as root object") {
    std::multimap<int,int> m;
    m.insert(std::make_pair(1,2));
    m.insert(std::make_pair(1,22));
    m.insert(std::make_pair(3,4));
    picojson::value mv=picojson::convert::to_value(m);
	std::string ms=picojson::convert::to_string(m);
    std::multimap<int,int> m_;
    picojson::convert::from_value(mv,m_);
    CHECK( m_ == m );

    SECTION("const data") {
        std::multimap<int,int> const mc(m);
        mv=picojson::convert::to_value(m);
	ms=picojson::convert::to_string(m);
        m_.clear();
        picojson::convert::from_value(mv,m_);
        CHECK( m_ == mc );
    }
}