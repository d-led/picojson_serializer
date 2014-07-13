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
#include <set>
#include <algorithm>

namespace picojson {
    namespace convert{

        template<typename Key, class Compare, class Allocator> struct value_converter< std::set<Key, Compare, Allocator> > {
            typedef typename std::set<Key, Compare, Allocator> set_type;

            static value to_value(set_type const& v) {
                picojson::array a;
				std::transform(
					v.begin(),
					v.end(),
					std::back_inserter(a),
					operators::to_value_c<Key>
				);
                return value(a);
            }

            static void from_value(value const& ov, set_type& v) {
                if ( !ov.is<picojson::array>() )
                    return;
				picojson::array const& a = ov.get<picojson::array>();
				std::transform(
					a.begin(),
					a.end(),
					std::inserter(v, v.end()),
					operators::from_value<Key>);
            }
        };


        template<typename Key, class Compare, class Allocator> struct value_converter< std::multiset<Key, Compare, Allocator> > {
            typedef typename std::multiset<Key, Compare, Allocator> set_type;

            static value to_value(set_type const& v) {
                picojson::array a;
				std::transform(
					v.begin(),
					v.end(),
					std::back_inserter(a),
					operators::to_value_c<Key>
				);
                return value(a);
            }

            static void from_value(value const& ov, set_type& v) {
                if ( !ov.is<picojson::array>() )
                    return;
				picojson::array const& a = ov.get<picojson::array>();
				std::transform(
					a.begin(),
					a.end(),
					std::inserter(v, v.end()),
					operators::from_value<Key>);
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
