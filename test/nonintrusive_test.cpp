#include <catch.hpp>

#include "../picojson_serializer.h"
#include "test_helpers.h"

namespace {
struct Point {
    double x, y, z;
};

struct NamedPoint {
    std::string name;
    Point point;
};

struct Untouchable {
    int value;
};

}

namespace picojson {
namespace convert {

template <class Archive>
void json(Archive &ar, Point const &p) {
    ar &picojson::convert::member("x", p.x);
    ar &picojson::convert::member("y", p.y);
    ar &picojson::convert::member("z", p.z);
}

template <class Archive>
void json(Archive &ar, Point &p) {
    ar &picojson::convert::member("x", p.x);
    ar &picojson::convert::member("y", p.y);
    ar &picojson::convert::member("z", p.z);
}

template <class Archive>
void json(Archive &ar, NamedPoint const &p) {
    ar &picojson::convert::member("name", p.name);
    ar &picojson::convert::member("point", p.point);
}

template <class Archive>
void json(Archive &ar, NamedPoint &p) {
    ar &picojson::convert::member("name", p.name);
    ar &picojson::convert::member("point", p.point);
}

template <class Archive>
void json(Archive &ar, Untouchable const &u) {
    ar &picojson::convert::member("value", u.value);
}

template <class Archive>
void json(Archive &ar, Untouchable &u) {
    ar &picojson::convert::member("value", u.value);
}

}
}

TEST_CASE() {

    SECTION("readme") {
        Untouchable example = { 42 };
        std::string example_string( picojson::convert::to_string(example) );

        Untouchable example_deserialized = { 0 };
        picojson::convert::from_string( example_string, example_deserialized );
        CHECK( example.value == example_deserialized.value );

        SECTION("const data") {
            Untouchable const c_example = { 67 };
            example_string = picojson::convert::to_string(c_example);

            example_deserialized.value = 0;
            picojson::convert::from_string( example_string, example_deserialized );
            CHECK( c_example.value == example_deserialized.value );
        }

    }

    SECTION("nonintrusive serialization") {

        Point p = { 1, 2, 3 };
        picojson::value pv = picojson::convert::to_value(p);
        CHECK(has<double>(pv, "x", 1));
        CHECK(has<double>(pv, "y", 2));
        CHECK(has<double>(pv, "z", 3));

        std::string test_point_name("test point");
        NamedPoint np = { test_point_name, p };

        picojson::value npv = picojson::convert::to_value(np);
        CHECK(has<std::string>(npv, "name", test_point_name));
        CHECK(has<picojson::object>(npv, "point"));

        SECTION("deserialization from value") {
            Point np_ = { 0, 0, 0 };
            picojson::convert::from_value(pv, np_);
            CHECK(np_.x == 1);
            CHECK(np_.y == 2);
            CHECK(np_.z == 3);

            NamedPoint nnp = { "", { 0, 0, 0 } };
            picojson::convert::from_value(npv, nnp);
            CHECK(nnp.name == test_point_name);
            CHECK(nnp.point.x == 1);
            CHECK(nnp.point.y == 2);
            CHECK(nnp.point.z == 3);

            SECTION("deserialization from string") {
                std::string ps = picojson::convert::to_string(p);
                Point pss = { 0, 0, 0 };
                picojson::convert::from_string(ps, pss);
                CHECK(pss.x == 1);
                CHECK(pss.y == 2);
                CHECK(pss.z == 3);

                std::string nps = picojson::convert::to_string(np);
                NamedPoint npss = { "", { 0, 0, 0 } };
                picojson::convert::from_string(nps, npss);
                CHECK(npss.name == test_point_name);
                CHECK(npss.point.x == 1);
                CHECK(npss.point.y == 2);
                CHECK(npss.point.z == 3);
            }
        }

        SECTION("const data") {
            Point const pc = { 1, 2, 3 };
            pv = picojson::convert::to_value(pc);
            CHECK(has<double>(pv, "x", 1));
            CHECK(has<double>(pv, "y", 2));
            CHECK(has<double>(pv, "z", 3));

            NamedPoint const npc = { test_point_name, p };

            npv = picojson::convert::to_value(npc);
            CHECK(has<std::string>(npv, "name", test_point_name));
            CHECK(has<picojson::object>(npv, "point"));
        }

    }
}
