/**
* picojson_serializer - a simple serialization solution based on picojson
* --------------------------------------------------------
* Copyright (C) 2013, Dmitry Ledentsov (d.ledentsov@gmail.com)
*
* This software is distributed under the MIT License. See notice at the end
* of this file.
*
* This work requires picojson: https://github.com/kazuho/picojson
*/
#pragma once

#include "picojson.h"
#include <string>

namespace picojson {
    namespace convert {

        template<typename T>
        struct key_value {
            std::string key;
            T& value;
        };

        template <typename T>
        key_value<T> member(std::string const& k, T& v) {
            key_value<T> res = { k, v };
            return res;
        }

        template<typename T> struct value_converter;

        /// standard types
        template<> struct value_converter<double> {
            static value to_value(double v) { return value(v); }
            static void from_value(value const& ov, double& v) { if ( ov.is<double>() ) v = ov.get<double>(); }
        };
        template<> struct value_converter< std::string > {
            static value to_value(std::string const& v) { return value(v); }
            static void from_value(value const& ov, std::string& v) { if ( ov.is<std::string>() ) v = ov.get<std::string>(); }
        };
        template<> struct value_converter< int > {
            static value to_value(int v) { return value(static_cast<double>(v)); }
            static void from_value(value const& ov, int& v) { if ( ov.is<double>() ) v = static_cast<int>(ov.get<double>()); }
        };
        template<> struct value_converter< bool > {
            static value to_value(bool v) { return value(v); }
            static void from_value(value const& ov, bool& v) { if ( ov.is<bool>() ) v = ov.get<bool>(); }
        };

        template <typename T>
        value to_value(T& t) {
            return value_converter<T>::to_value(t);
        };

        template <typename T>
        void from_value(value const& v, T& t) {
            value_converter<T>::from_value(v, t);
        };

        class access {
            bool do_serialize;
            object o;

        public:
            access(bool w) :do_serialize(w){}

            template<typename T>
            void operator & (key_value<T>& kv) {
                if ( do_serialize ) {
                    o[kv.key] = to_value(kv.value);
                }
                else {
                    from_value(o[kv.key], kv.value);
                }
            }

            std::string serialize() const {
                return get_value().serialize();
            }

            value get_value() const {
                return value(o);
            }
        };

        template <typename V>
        struct standard_value;
        template<> struct standard_value<int> { static void get(value const& v_, int& v){ from_value(v_, v); } };
        template<> struct standard_value<double> { static void get(value const& v_, double& v){ from_value(v_, v); } };
        template<> struct standard_value<bool> { static void get(value const& v_, bool& v){ from_value(v_, v); } };
        template<> struct standard_value<std::string> { static void get(value const& v_, std::string& v){ from_value(v_, v); } };
        template <typename V>
        struct standard_value { static void get(value const& v_, V& v){} };

        template <typename T>
        class write_access {
            T& t;
            value const& v;

        public:
            write_access(value const& v_,T& t_) :t(t_),v(v_){}

            template<typename TT>
            void operator & (key_value<TT>& kv) {
                if ( !v.is<picojson::object>() )
                    return;

                picojson::object const& o = v.get<picojson::object>();
                picojson::object::const_iterator found =
                    o.find(kv.key);
                if ( found == o.end() )
                    return;

                if ( found->second.is<picojson::object>() ) {
                    from_value(found->second, kv.value);
                    return;
                }

				if ( found->second.is<picojson::array>() ) {
					from_value(found->second,kv.value);
				}

                standard_value<TT>::get(found->second, kv.value);
            }
        };

        template <typename T>
        struct value_converter {
            static value to_value(T& v) {
                access a(true);
                v.json(a);
                return a.get_value();
            }

            static void from_value(value const& ov, T& v) {
                write_access<T> a(ov,v);
                v.json(a);
            }
        };

        template < typename T>
        std::string to_string(T& t) {
            access a(true);
            t.json(a);
            return a.serialize();
        }

        template < typename T>
        void from_string(std::string const& json,T& t) {
            picojson::value v;
            picojson::parse(v, json.begin(), json.end());
            from_value(v, t);
        }
    }
}

/**
* Copyright (c) 2013 Dmitry Ledentsov
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
