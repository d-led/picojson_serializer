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

        /// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
        template<typename T>
        class has_json_member
        {
            struct Fallback { int json; };
            struct Derived : T, Fallback { };

            template<typename U, U> struct Check;

            typedef char ArrayOfOne[1];  // typedef for an array of size one.
            typedef char ArrayOfTwo[2];  // typedef for an array of size two.

            template<typename U> 
            static ArrayOfOne & func(Check<int Fallback::*, &U::json> *);

            template<typename U> 
            static ArrayOfTwo & func(...);

        public:
            typedef has_json_member type;
            enum { value = sizeof(func<Derived>(0)) == 2 };
        };

        // pre-c++11  enable_if (http://en.cppreference.com/w/cpp/types/enable_if)
        template <bool B, class T = void> struct enable_if {};
        template <class T> struct enable_if<true, T> { typedef T type; };

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

        class access {
            object o;

        public:
            template<typename T>
            void operator & (key_value<T> kv);

            inline std::string serialize() const;

            inline value get_value() const;
        };

        template <typename T>
        class write_access {
            T& t;
            value const& v;

        public:
            write_access(value const& v_,T& t_);

            template<typename TT>
            void operator & (key_value<TT> kv);
        };

        template<typename T,class Enable = void> struct value_converter {
            static value to_value(T& v) {
                access a;
                json(a,v);
                return a.get_value();
            }

            static void from_value(value const& ov, T& v) {
                write_access<T> a(ov,v);
                json(a,v);
            }
        };

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

        template<typename T>
        void access::operator & (key_value<T> kv) {
            o[kv.key] = to_value(kv.value); 
        }

        inline std::string access::serialize() const {
            return get_value().serialize();
        }

        inline value access::get_value() const {
            return value(o);
        }

        template <typename T>
		write_access<T>::write_access(value const& v_,T& t_) :t(t_),v(v_){}

        template <typename T>
		template<typename TT>
		void write_access<T>::operator & (key_value<TT> kv) {
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
				return;
			}

			value_converter<TT>::from_value(found->second, kv.value);
		}

        template <typename T>
        struct value_converter<T, typename enable_if< has_json_member<T>::value >::type> {
            static value to_value(T& v) {
                access a;
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
            return to_value(t).serialize();
        }

        template < typename T>
        void from_string(std::string const& json,T& t) {
            picojson::value v;
			std::string err;
            picojson::parse(v, json.begin(), json.end(),&err);
            from_value(v, t);
        }

		namespace operators {

			template<typename T>
			T from_value(picojson::value const& v) {
				T t{}; //Fixes unititialized data warnings from GCC
				value_converter<T>::from_value(v, t);
				return t;
			}

			template <typename KeyType,typename ValueType>
			value to_value(std::pair< KeyType, ValueType >const& p) {
				picojson::object o;
				o["Key"]=value_converter<KeyType>::to_value(const_cast<KeyType&>(p.first));
				o["Value"]=value_converter<ValueType>::to_value(p.second);
				return value(o);
			}

			template <typename KeyType,typename ValueType>
			std::pair<KeyType,ValueType> from_value(picojson::value const& v) {
				return std::pair<KeyType,ValueType>();//todo
			}

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
