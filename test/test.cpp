#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../picojson_serializer.h"

namespace {
    struct Point {
        double x, y, z;

        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar)
        {
            ar & picojson::convert::member("x", x);
            ar & picojson::convert::member("y", y);
            ar & picojson::convert::member("z", z);
        }
    };

    struct NamedPoint {
        std::string name;
        Point point;

        friend class picojson::convert::access;
        template<class Archive>
        void json(Archive & ar)
        {
            ar & picojson::convert::member("name", name);
            ar & picojson::convert::member("point", point);
        }
    };

    template <typename T>
    bool has(picojson::object const& o, std::string const& key) {
        picojson::object::const_iterator found = o.find(key);
        if ( found == o.end() )
            return false;

        if ( !found->second.is<T>() )
            return false;

        return true;
    }

    template <typename T, typename TValue>
    bool has(picojson::object const& o, std::string const& key, TValue v) {
        if ( !has<T>(o, key) )
            return false;
        return o.find(key)->second.get<T>() == v;
    }

    template <typename T, typename TValue>
    bool has(picojson::value const& ov, std::string const& key, TValue v) {
        if ( !ov.is<picojson::object>() )
            return false;

        return has<T>(ov.get<picojson::object>(), key, v);
    }

    template <typename T>
    bool has(picojson::value const& ov, std::string const& key) {
        if ( !ov.is<picojson::object>() )
            return false;

        return has<T>(ov.get<picojson::object>(), key);
    }
}

TEST_CASE() {

    SECTION("serialization") {

        Point p = { 1, 2, 3 };
        picojson::value pv = picojson::convert::to_value(p);
        CHECK(has<double>(pv, "x", 1));
        CHECK(has<double>(pv, "y", 2));
        CHECK(has<double>(pv, "z", 3));
        
        std::string test_point_name("test point");
        NamedPoint np = { test_point_name , p };

        picojson::value npv = picojson::convert::to_value(np);
        CHECK(has<std::string>(npv, "name", test_point_name));
        CHECK(has<picojson::object>(npv, "point"));

        SECTION("deserialization") {
            Point np = { 0, 0, 0 };
            picojson::convert::from_value(pv, np);
            CHECK(np.x == 1);
            CHECK(np.y == 2);
            CHECK(np.z == 3);
            
            np.x = np.y = np.z = 0;
            NamedPoint nnp = { "", np };
            picojson::convert::from_value(npv, nnp);
            CHECK(nnp.name == test_point_name);
            CHECK(nnp.point.x == 1);
            CHECK(nnp.point.y == 2);
            CHECK(nnp.point.z == 3);

            //std::string pvs = picojson::convert::to_string(p);
            //picojson::convert::from_string<Point>(pvs, np);
        }
    }
}
