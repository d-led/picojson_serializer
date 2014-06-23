#include "../picojson_map_serializer.h"

#include <catch.hpp>
#include <map>

#include "test_helpers.h"

namespace {
    struct X {
        int x;
        X() : x(0) { }
        X(int x) : x(x) { }
        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar)
        {
            ar & picojson::convert::member("x", x);
        }
    };

    bool operator==(X const& lhs, X const& rhs) { return lhs.x == rhs.x; }
    bool operator<(X const& lhs, X const& rhs) { return lhs.x < rhs.x; }

    template<template<
        class Key,
        class T,
        class Compare = std::less<Key>,
        class Allocator = std::allocator<std::pair<const Key, T> >
        > class M>
    struct A {
        M< int, std::string > x;
        M< int, X > y;
        M< X, X > z;
        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar)
        {
            ar & picojson::convert::member("x", x);
            ar & picojson::convert::member("y", y);
            ar & picojson::convert::member("z", z);
        }
    };
}

TEST_CASE("map serialization") {
    A<std::map> x;
    x.x[1]="42";
    x.x[3]="33";
    x.y[2]=X(123);
    x.y[4]=X(321);
    x.z[X(0)]=X(5);
    x.z[X(5)]=X(0);
    std::string xs=picojson::convert::to_string(x);
    picojson::value xv=picojson::convert::to_value(x);
    A<std::map> y={};
    picojson::convert::from_value<A<std::map> >(xv,y);
    CHECK( x.x == y.x );
    CHECK( x.y == y.y );
    CHECK( x.z == y.z );

    SECTION("const data") {
        A<std::map> const xc(x);
        xs=picojson::convert::to_string(xc);
        y.x.clear();
        y.y.clear();
        y.z.clear();
        picojson::convert::from_value<A<std::map> >(xv,y);
        CHECK( xc.x == y.x );
        CHECK( xc.y == y.y );
        CHECK( xc.z == y.z );
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
    A<std::multimap> x;
    x.x.insert(std::make_pair(1, "42"));
    x.x.insert(std::make_pair(1, "11"));
    x.x.insert(std::make_pair(3, "33"));
    x.y.insert(std::make_pair(2, X(123)));
    x.y.insert(std::make_pair(2, X(0)));
    x.y.insert(std::make_pair(4, X(321)));
    x.z.insert(std::make_pair(X(0), X(5)));
    x.z.insert(std::make_pair(X(0), X(2)));
    x.z.insert(std::make_pair(X(5), X(0)));
    std::string xs=picojson::convert::to_string(x);
    picojson::value xv=picojson::convert::to_value(x);
    A<std::multimap> y={};
    picojson::convert::from_value<A<std::multimap> >(xv,y);
    CHECK( x.x == y.x );
    CHECK( x.y == y.y );
    CHECK( x.z == y.z );

    SECTION("const data") {
        A<std::multimap> const xc(x);
        xs=picojson::convert::to_string(xc);
        y.x.clear();
        y.y.clear();
        y.z.clear();
        picojson::convert::from_value<A<std::multimap> >(xv,y);
        CHECK( xc.x == y.x );
        CHECK( xc.y == y.y );
        CHECK( xc.z == y.z );
    }
}

TEST_CASE("multimap as root object") {
    std::multimap<int,int> m;
    m.insert(std::make_pair(1,2));
    m.insert(std::make_pair(1,22));
    m.insert(std::make_pair(3,4));
    picojson::value mv=picojson::convert::to_value(m);
    std::multimap<int,int> m_;
    picojson::convert::from_value(mv,m_);
    CHECK( m_ == m );

    SECTION("const data") {
        std::multimap<int,int> const mc(m);
        mv=picojson::convert::to_value(mc);
        m_.clear();
        picojson::convert::from_value(mv,m_);
        REQUIRE( m_.count(1) == 2u);
        CHECK( m_.find(1)->second >= 2);
        CHECK( m_.find(1)->second <= 3);
        REQUIRE( m_.count(3) == 1u );
        CHECK( m_.find(3)->second == 4 );
    }
}