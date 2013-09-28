#include "../picojson_vector_serializer.h"

#include <catch.hpp>
#include <vector>

#include "test_helpers.h"

namespace {
    struct X {
        int x;
        friend class picojson::convert::access;
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
            //CHECK_A(nav);

            SECTION("deserialization from string") {

            }
        }
    }
}
