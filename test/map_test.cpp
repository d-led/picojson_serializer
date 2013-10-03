#include "../picojson_map_serializer.h"

#include <catch.hpp>
#include <map>

#include "test_helpers.h"

namespace {
    struct X {
        std::map< int, std::string > x;
        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar)
        {
            ar & picojson::convert::member("x", x);
        }
    };
}

TEST_CASE() {
    SECTION("serialization") {
        X x;
        x.x[1]="42";
        x.x[3]="33";
        std::string xs=picojson::convert::to_string(x);
        std::cout<<xs<<std::endl;
    }
}
