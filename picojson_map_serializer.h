/**
* picojson_serializer - a simple serialization solution based on picojson
* --------------------------------------------------------
* Copyright 2013, Dmitry Ledentsov (d.ledentsov@gmail.com)
* Copyright 2014, project contributors
*
* This software is distributed under the MIT License. See notice at the end
* of this file.
*
* This work requires picojson: https://github.com/kazuho/picojson
*/
#pragma once
#include "picojson_serializer.h"
#include <map>
#include <algorithm>

namespace picojson {
    namespace convert{

        template<typename ValueType, typename Key, class Compare, class Allocator> struct value_converter< std::map<Key, ValueType, Compare, Allocator> > {
            typedef typename std::map<Key, ValueType, Compare, Allocator> map_type;

            static value to_value(map_type const& v) {
                picojson::array a;
				std::transform(
					v.begin(),
					v.end(),
					std::back_inserter(a),
					operators::to_value_c<Key,ValueType>
				);
                return value(a);
            }

            static value to_value(map_type& v) {
                picojson::array a;
				std::transform(
					v.begin(),
					v.end(),
					std::back_inserter(a),
					operators::to_value_nc<Key,ValueType>
				);
                return value(a);
            }

            static void from_value(value const& ov, map_type& m) {
                if ( !ov.is<picojson::array>() )
                    return;
                picojson::array const& a=ov.get<picojson::array>();
                for (picojson::array::const_iterator it=a.begin();
                     it!=a.end();
                     ++it) {
                        if ( !it->is<picojson::object>() )
                            continue;
                        picojson::object const& o=it->get<picojson::object>();
                        
                        picojson::object::const_iterator key=o.find("Key");
                        if ( key == o.end() )
                            continue;

                        picojson::object::const_iterator value=o.find("Value");
                        if ( value == o.end() )
                            continue;

                    	Key key_(operators::from_value<Key>(key->second));
                    	ValueType value_(operators::from_value<ValueType>(value->second));
                    	m[key_]=value_;
                }
            }
        };


        template<typename ValueType, typename Key, class Compare, class Allocator> struct value_converter< std::multimap<Key, ValueType, Compare, Allocator> > {
            typedef typename std::multimap<Key, ValueType, Compare, Allocator> map_type;

            static value to_value(map_type const& v) {
                picojson::array a;
				std::transform(
					v.begin(),
					v.end(),
					std::back_inserter(a),
					operators::to_value_c<Key,ValueType>
				);
                return value(a);
            }

            static value to_value(map_type& v) {
                picojson::array a;
				std::transform(
					v.begin(),
					v.end(),
					std::back_inserter(a),
					operators::to_value_nc<Key,ValueType>
				);
                return value(a);
            }

            static void from_value(value const& ov, map_type& m) {
                if ( !ov.is<picojson::array>() )
                    return;
                picojson::array const& a=ov.get<picojson::array>();
                for (picojson::array::const_iterator it=a.begin();
                     it!=a.end();
                     ++it) {
                        if ( !it->is<picojson::object>() )
                            continue;
                        picojson::object const& o=it->get<picojson::object>();
                        
                        picojson::object::const_iterator key=o.find("Key");
                        if ( key == o.end() )
                            continue;

                        picojson::object::const_iterator value=o.find("Value");
                        if ( value == o.end() )
                            continue;

                    	Key key_(operators::from_value<Key>(key->second));
                    	ValueType value_(operators::from_value<ValueType>(value->second));
                    	m.insert(std::make_pair(key_,value_));
                }
            }
        };        
    }
}

/**
* Copyright 2013 Dmitry Ledentsov
* Copyright 2014, project contributors
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
