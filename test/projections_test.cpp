#include <catch.hpp>
#include "../picojson_vector_serializer.h"
#include "../picojson_projections.h"
#include <string>

namespace {

	namespace {
		struct Class1 {
			int a;
			double b;
			std::string c;
			size_t spoiler__;

			friend class picojson::convert::access;
			template<class Archive>
			void json(Archive & ar)
			{
				ar & picojson::convert::member("a", a);
				ar & picojson::convert::member("b", b);
				ar & picojson::convert::member("c", c);
			}
		};

		struct Class2 {
			std::string c;
			double b;
			int a;
			unsigned char spoiler__;

			friend class picojson::convert::access;
			template<class Archive>
			void json(Archive & ar)
			{
				ar & picojson::convert::member("c", c);
				ar & picojson::convert::member("b", b);
				ar & picojson::convert::member("a", a);
			}
		};
        
    struct Class3 {
			std::string d;
            int c;
			int b;
			int bla;
			unsigned char spoiler__;
            
			friend class picojson::convert::access;
			template<class Archive>
			void json(Archive & ar)
			{
				ar & picojson::convert::member("d", d);
				ar & picojson::convert::member("c", c);
				ar & picojson::convert::member("b", b);
				ar & picojson::convert::member("bla", bla);
			}
		};
	}

	template <typename C>
	void CHECK_123(C const& c) {
		CHECK( c.a == 1 );
		CHECK( c.b == 2 );
		CHECK( c.c == "3" );
	}

	SCENARIO("simplest case: 1-to-1 mapping") {
		GIVEN("an serializable object with some predefined members") {
			Class1 c1={1,2,"3"};
			CHECK_123(c1);

			WHEN("I map that object onto an unrelated serializable object") {
				Class2 c2=picojson::project::from(c1).onto<Class2>();
				Class2 c2_={};
				picojson::project::from(c1).onto(c2_);


				THEN("The members will be mapped as if the types were the same") {
					CHECK_123(c2);
					CHECK_123(c2);
					CHECK_123(c2_);
				}
			}

		}
	}
    
    SCENARIO("renaming fields") {
    	GIVEN("a couple of objects with incompatible member names") {
	        Class3 c3={};
	        Class1 c1={};
	        c3.c=42;
	        c3.b=33;
	        c3.bla=77;

	        WHEN("I map some member names onto others") {
		        picojson::project::from(c3)
		        	.remap_key("c","b") // shadows c3.b=77
		        	.remap_key("bla","b")
		        	.remap_key("b","a")
		        	.onto(c1);

		        THEN("the values are mapped correctly") {
	        		CHECK( c1.a == 33 );

	        		AND_THEN("any the first value in definition is mapped") {
	        			CHECK( c1.b == 42.0 );
	        		}
	        	}
	        }
		}
    }
}
