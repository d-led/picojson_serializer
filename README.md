picojson serializer
===================

This is a lightweight header-only solution for serializing objects to and from json using the header-only library [picojson](https://github.com/kazuho/picojson). 

usage
=====

building
--------

Make sure `picojson.h` can be found, include `picojson_serialization.h`, no extra build steps necessary.

A Visual Studio solution can be found in the project root with the test. 

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

To enable `std::vector` serialization, use the header `picojson_vector_serializer.h`

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

status
------

Lots of to-dos.

license
=======

Copyright 2013, Dmitry Ledentsov
MIT License: http://www.opensource.org/licenses/mit-license.php
