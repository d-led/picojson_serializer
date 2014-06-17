#include <catch.hpp>

#include "../picojson_serializer.h"
#include "test_helpers.h"

namespace {
	struct Example {
		enum Status {
			NONE = 0,
			SOME,
			SOME_OTHER
		};

		Status status;

		friend class picojson::convert::access;
		template<class Archive>
		void json(Archive & ar)
		{
			ar & picojson::convert::member("status", status);
		}
	};
}

namespace picojson {
	namespace convert {

		template<> struct value_converter<Example::Status> {
			static value to_value(Example::Status v) { return value(static_cast<double>(v)); }
			static void from_value(value const& ov, Example::Status& v) { if ( ov.is<double>() ) v = Example::Status(static_cast<int>(ov.get<double>())); }
		};


	}
}

TEST_CASE("custom serializer example") {
	Example e = { Example::NONE };

	picojson::value ev = picojson::convert::to_value(e);
	CHECK(has<double>(ev, "status", 0));

	e.status=Example::SOME;
	ev = picojson::convert::to_value(e);
	CHECK(has<double>(ev, "status", static_cast<int>(Example::SOME)));

	e.status=Example::SOME_OTHER;
	ev = picojson::convert::to_value(e);
	CHECK(has<double>(ev, "status", static_cast<int>(Example::SOME_OTHER)));

	SECTION("const data") {
		Example const ec = { Example::SOME };
		ev = picojson::convert::to_value(ec);
		CHECK(has<double>(ev, "status", static_cast<int>(Example::SOME)));
	}

}
