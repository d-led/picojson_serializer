#include <catch.hpp>

#define PICOJSON_USE_INT64
#include "../picojson_serializer.h"
#include "test_helpers.h"

TEST_CASE("64-bit integers") {

    // 9th Mersenne prime
    int64_t m9((int64_t(1)<<61) - 1);
    picojson::value v = picojson::convert::to_value(m9);
    CHECK(is<int64_t>(v, m9));

        SECTION("deserialization") {
            int64_t m9_(0);
            picojson::convert::from_value(v, m9_);
            CHECK(m9_ == m9);
        }

}
