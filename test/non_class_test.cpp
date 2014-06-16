#include <catch.hpp>
#include <limits>

#include "../picojson_serializer.h"
#include "test_helpers.h"

namespace picojson {
    namespace convert {

        template<typename T>
        struct value_converter<T,
                               typename enable_if<std::numeric_limits<T>::is_iec559>::type> {
            static value to_value(T v) {
                return value_converter<double>::to_value(v);
            }
            static void from_value(value const& ov, T& v) {
                double v2(0.);
                value_converter<double>::from_value(ov, v2);
                v = v2;
            }
        };

    }
}

TEST_CASE("Non-class type") {

    SECTION("serialization") {

        float f(1.2f);
        picojson::value pv = picojson::convert::to_value(f);
        CHECK(is<double>(pv, static_cast<double>(1.2f)));

        SECTION("deserialization from value") {
            float f_(0.f);
            picojson::convert::from_value(pv, f_);
            CHECK(f_ == f);

            SECTION("deserialization from string") {
                std::string fs = picojson::convert::to_string(f);
                float f__(0.f);
                picojson::convert::from_string(fs, f__);
                CHECK(f__ == f);
            }
        }

    }

}
