picojson serializer
===================

This is a lightweight header-only solution for serializing objects to and from json using the header-only library [picojson](https://github.com/kazuho/picojson).

[![Build Status](https://travis-ci.org/d-led/picojson_serializer.png?branch=master)](https://travis-ci.org/d-led/picojson_serializer) [![Coverage Status](https://coveralls.io/repos/d-led/picojson_serializer/badge.png?branch=master)](https://coveralls.io/r/d-led/picojson_serializer?branch=master) [![Coverity Status](https://scan.coverity.com/projects/3010/badge.svg)](https://scan.coverity.com/projects/3010)

Current release: [v0.9.0](https://github.com/d-led/picojson_serializer/releases/tag/v0.9.0)

a more feature-rich alternative: [cereal](https://github.com/USCiLab/cereal)

usage
=====

building
--------

Make sure `picojson.h` can be found, include `picojson_serialization.h`, no extra build steps necessary.

[Premake5](https://premake.github.io/) is included and can be used to generate build files in the Build folder. To build and run the tests, do:

    [path_to]/premake5 gmake
    make -C Build

declaring objects as serializable
---------------------------------

The API is similar to [hiberlite](https://github.com/paulftw/hiberlite)'s or [Boost.Serialization](http://www.boost.org/doc/libs/1_54_0/libs/serialization/doc/tutorial.html#serializablemembers)'s:

````cpp
/// A JSON serializable class
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

/// Composed serialization is possible
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
````


serializing
-----------

````cpp
NamedPoint np = { "test point" , { 1 , 2 , 3 } };
picojson::value npv = picojson::convert::to_value(np);
std::string nps = picojson::convert::to_string(np);
````

resulting in
````js
{"name":"test point","point":{"x":1,"y":2,"z":3}}
````

deserializing
-------------

````cpp
NamedPoint np;
std::string json=...
picojson::convert::from_string(json,np);
````

Currently, if deserialization fails for a member, that member is not modified.

non-intrusive serialization
---------------------------

A class that cannot be extended, but the internals are accessible (following to the [Boost.Serialization](http://www.boost.org/doc/libs/1_55_0/libs/serialization/doc/serialization.html#free) api):

````cpp
struct Untouchable {
    int value;
};
````

With a free function defined in the `::picojson::convert` namespace,

````cpp
namespace picojson {
    namespace convert {

        template <class Archive>
        void json(Archive &ar, Untouchable &u) {
            ar & picojson::convert::member("value", u.value);
        }

    }
}
````
serialization is again possible:

````cpp
Untouchable example = { 42 };
std::string example_string( picojson::convert::to_string(example) );

Untouchable example_deserialized = { 0 };
picojson::convert::from_string( example_string, example_deserialized );
CHECK( example.value == example_deserialized.value );
````

serializing containers
----------------------

To enable `std::vector` serialization, use the header `picojson_vector_serializer.h` and likewise for the other supported container types.

### List of supported standard library containers ###

- vector
- map
- multimap
- set
- multiset

serializing const data
----------------------

To serialize `const` data types (including the keys of `std::map`, `std::multimap`, `std::set`, and `std::multiset`), the `json()` member must be overloaded for `const` objects. The normal `void(Archive&)` function template as explained above works on non-`const` objects (`Point` and `NamedPoint` in the above example). If a `const` object is to be serialized, an additional `const` version of the `json()` function must be defined. E.g.

````cpp
struct NamedPoint {
    // in addition to the non-const version
    template<class Archive>
    void json(Archive & ar) const
    {
        ar & picojson::convert::member("name", name);
        ar & picojson::convert::member("point", point);
    }
};
````

Free function version can also be overloaded

````cpp
template <class Archive>
void json(Archive &ar, Point const &p) {
    ar & ...
}
````

initializing the object upon serialization
------------------------------------------

as a convenience, the function `picojson::convert::init_from_string` can be used instead of `from_string`
to default-initialize the object before deserialization.


implementing custom value converters
------------------------------------

The serialization can be easily customized for types that are not default-convertible. Example class:

````cpp
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
````

Attempting to serialize instances of `Example` should lead to compile error:

````cpp
Example e = { Example::NONE };
picojson::value ev = picojson::convert::to_value(e);
````

However, you can specialize the value converter for the enum, i.e.:

````cpp
namespace picojson {
namespace convert {

	template<> struct value_converter<Example::Status> {
		static value to_value(Example::Status v) {
			return value(static_cast<double>(v));
		}

		static void from_value(value const& ov, Example::Status& v) {
			if ( ov.is<double>() ) v = Example::Status(static_cast<int>(ov.get<double>()));
		}
	};
}
}
````

and the test passes:

````cpp
Example e = { Example::SOME };
picojson::value ev = picojson::convert::to_value(e);
CHECK(has<double>(ev, "status", static_cast<int>(Example::SOME)));
````


mapping between unrelated types
-------------------------------

If you have serializable types that may be unrelated, such as a logic component and a data transfer object, you can 'project' the data from one object to another simply by mapping the values through the serialization like so:

````cpp
Class1 c1=...;
Class2 c2=picojson::project::from(c1).onto<Class2>();
````

license
=======

- Copyright 2013, Dmitry Ledentsov
- Copyright 2014, project contributors
- [MIT License](http://www.opensource.org/licenses/mit-license.php)


dependencies
============
- [picojson](https://github.com/kazuho/picojson) as the main dependency
- [msinttypes](https://code.google.com/p/msinttypes/) for in64 in MSVC
- [premake](industriousone.com/premake) for generating build files

Dependencies retain their respective licenses
