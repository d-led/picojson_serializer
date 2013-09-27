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
        }
    };
}

TEST_CASE() {
    SECTION("serialization") {
        A a;
        a.ints.push_back(1);
        a.ints.push_back(2);
        picojson::value av = picojson::convert::to_value(a);
        std::string as = picojson::convert::to_string(a);

        CHECK(has<picojson::array>(av, "ints"));
        picojson::array const& arr = av.get<picojson::object>()["ints"].get<picojson::array>();
        CHECK(arr.size() == 2);
        CHECK(is<double>(arr, 0, 1));
        CHECK(is<double>(arr, 1, 2));
        SECTION("deserialization from value") {


            SECTION("deserialization from string") {

            }
        }
    }
}
