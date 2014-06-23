#include "../picojson_vector_serializer.h"

#include <catch.hpp>
#include <vector>

#include "test_helpers.h"

namespace {
    struct X {
        int x;
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

    struct A {
        std::vector<int> ints;
        std::vector<X> xx;
        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar) const
        {
            ar & picojson::convert::member("ints", ints);
            ar & picojson::convert::member("xx", xx);
        }
        template<class Archive>
        void json(Archive & ar)
        {
            ar & picojson::convert::member("ints", ints);
            ar & picojson::convert::member("xx", xx);
        }
    };
}

void CHECK_A(picojson::value& av) {
    REQUIRE(has<picojson::array>(av, "ints"));
    picojson::array const& arr = av.get<picojson::object>()["ints"].get<picojson::array>();
    REQUIRE(arr.size() == 2);
    CHECK(is<double>(arr, 0, 1));
    CHECK(is<double>(arr, 1, 2));
    REQUIRE(has<picojson::array>(av, "xx"));
    picojson::array& xarr = av.get<picojson::object>()["xx"].get<picojson::array>();
    REQUIRE(xarr.size() == 1);
    CHECK(is<picojson::object>(xarr, 0));
    picojson::object& xxx = xarr[0].get<picojson::object>();
    REQUIRE(xxx["x"].is<double>());
    CHECK(static_cast<int>(xxx["x"].get<double>()) == 42);
}

TEST_CASE() {
    SECTION("serialization") {
        A a;
        a.ints.push_back(1);
        a.ints.push_back(2);
        X x = { 42 };
        a.xx.push_back(x);
        picojson::value av = picojson::convert::to_value(a);
        std::string as = picojson::convert::to_string(a);
        CHECK_A(av);
        SECTION("deserialization from value") {
            A na;
            picojson::convert::from_value(av,na);
            picojson::value nav = picojson::convert::to_value(na);
            CHECK_A(nav);
            SECTION("deserialization from string") {
				A nna;
				picojson::convert::from_string(as,nna);
				picojson::value nnav=picojson::convert::to_value(nna);
				CHECK_A(nnav);
            }
        }

        SECTION("const data") {
            A const ac(a);
            av = picojson::convert::to_value(ac);
            CHECK_A(av);
        }
    }
}

TEST_CASE("vector as root object") {
    std::vector<X> v;
    X x = {1};
    v.push_back(x);
    x.x = 2;
    v.push_back(x);
    picojson::value vv=picojson::convert::to_value(v);
    std::vector<X> v_;
    picojson::convert::from_value(vv,v_);
    CHECK( v.size() == v_.size() );
    CHECK( v[1].x == v_[1].x );

    SECTION("const data") {
        std::vector<X> const vc(v);
        vv=picojson::convert::to_value(vc);
        v_.clear();
        picojson::convert::from_value(vv,v_);
        CHECK( vc.size() == v_.size() );
        CHECK( vc[1].x == v_[1].x );
    }
}
