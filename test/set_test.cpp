#include "../picojson_set_serializer.h"

#include <catch.hpp>

#include "test_helpers.h"

namespace {
    struct X {
        int x;
        X() : x(0) { }
        X(int x) : x(x) { }
        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar) const
        {
            ar & picojson::convert::member("x", x);
        }
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
        class Compare = std::less<Key>,
        class Allocator = std::allocator<Key>
        > class S>
    struct A {
        S< int > x;
        S< X > y;
        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar) const
        {
            ar & picojson::convert::member("x", x);
            ar & picojson::convert::member("y", y);
        }
        template<class Archive>
        void json(Archive & ar)
        {
            ar & picojson::convert::member("x", x);
            ar & picojson::convert::member("y", y);
        }
    };
}

TEST_CASE("set serialization") {
    A<std::set> x;
    x.x.insert(1);
    x.x.insert(3);
    x.y.insert(X(123));
    x.y.insert(X(321));
    std::string xs=picojson::convert::to_string(x);
    picojson::value xv=picojson::convert::to_value(x);
    A<std::set> y={};
    picojson::convert::from_value<A<std::set> >(xv,y);
    CHECK( x.x == y.x );
    CHECK( x.y == y.y );

    SECTION("const data") {
        A<std::set> const xc(x);
        xs=picojson::convert::to_string(xc);
        y.x.clear();
        y.y.clear();
        picojson::convert::from_value<A<std::set> >(xv,y);
        CHECK( xc.x == y.x );
        CHECK( xc.y == y.y );
    }
}

TEST_CASE("set as root object") {
    std::set<int> s;
    s.insert(1);
    s.insert(3);
    picojson::value sv=picojson::convert::to_value(s);
    std::set<int> s_;
    picojson::convert::from_value(sv,s_);
    CHECK( s_.count(1) == 1u );
    CHECK( s_.count(3) == 1u );

    SECTION("const data") {
        std::set<int> const sc(s);
        sv=picojson::convert::to_value(sc);
        s_.clear();
        picojson::convert::from_value(sv,s_);
        CHECK( s_.count(1) == 1u );
        CHECK( s_.count(3) == 1u );
    }
}

TEST_CASE("multiset serialization") {
    A<std::multiset> x;
    x.x.insert(1);
    x.x.insert(1);
    x.x.insert(3);
    x.y.insert(X(123));
    x.y.insert(X(123));
    x.y.insert(X(321));
    std::string xs=picojson::convert::to_string(x);
    picojson::value xv=picojson::convert::to_value(x);
    A<std::multiset> y={};
    picojson::convert::from_value<A<std::multiset> >(xv,y);
    CHECK( x.x == y.x );
    CHECK( x.y == y.y );

    SECTION("const data") {
        A<std::multiset> const xc(x);
        xs=picojson::convert::to_string(xc);
        y.x.clear();
        y.y.clear();
        picojson::convert::from_value<A<std::multiset> >(xv,y);
        CHECK( xc.x == y.x );
        CHECK( xc.y == y.y );
    }
}

TEST_CASE("multiset as root object") {
    std::multiset<int> s;
    s.insert(1);
    s.insert(1);
    s.insert(3);
    picojson::value sv=picojson::convert::to_value(s);
    std::multiset<int> s_;
    picojson::convert::from_value(sv,s_);
    CHECK( s_.count(1) == 2u );
    CHECK( s_.count(3) == 1u );

    SECTION("const data") {
        std::multiset<int> const sc(s);
        sv=picojson::convert::to_value(sc);
        s_.clear();
        picojson::convert::from_value(sv,s_);
        CHECK( s_.count(1) == 2u );
        CHECK( s_.count(3) == 1u );
    }
}