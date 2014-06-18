#include <catch.hpp>
#include <limits>

#include "../picojson_serializer.h"
#include "test_helpers.h"

TEST_CASE("const strings") {

    char const a[] = "Compile time-sized array of char\0with embedded null";
    picojson::value v = picojson::convert::to_value(a);
    std::string const as(a, sizeof(a)-1);
    CHECK(is<std::string>(v, as));

    v = picojson::convert::to_value("normal usage");
    CHECK(is<std::string>(v, "normal usage"));

    char const* p("Pointer to char");
    v = picojson::convert::to_value(p);
    CHECK(is<std::string>(v, p));

    char const* const pc("Const pointer to char");
    v = picojson::convert::to_value(pc);
    CHECK(is<std::string>(v, pc));

}
