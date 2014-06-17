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
#include "picojson_serializer.h"
#include <vector>
#include <algorithm>

namespace picojson {
    namespace convert{

        template<typename T,typename Allocator> struct value_converter< std::vector<T,Allocator> > {
            static value to_value(typename std::vector<T, Allocator> const& v) {
				picojson::array a;
				std::transform(
					v.begin(),
					v.end(),
					std::back_inserter(a),
					value_converter<T>::to_value);
                return value(a);
            }

            static void from_value(value const& ov, typename std::vector<T, Allocator>& v) {
                if ( !ov.is<picojson::array>() )
                    return;
				picojson::array const& a = ov.get<picojson::array>();
				std::transform(
					a.begin(),
					a.end(),
					std::back_inserter(v),
					operators::from_value<T>);
            }
        };
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
