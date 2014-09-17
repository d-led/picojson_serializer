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

#include <string>
#include <map>

namespace picojson {

    namespace project {

    	typedef std::map< std::string, std::string > key_map;

		template <typename TIn>
		class projector {
			TIn& original;
			key_map key_remap;

		public:
			projector(TIn& o):original(o){}

		public:
			template <typename TOut>
			TOut onto() {
				TOut result = {};
				onto(result);
				return result;
			}

			template <typename TOut>
			void onto(TOut& result) {
				value original_serialized = ::picojson::convert::to_value(original);
				remap_keys_in(original_serialized);
				::picojson::convert::from_value(original_serialized,result);
			}

			projector& remap_key(std::string const& from_key, std::string const& to_key) {
				if ( from_key.length() == 0 ||
					 to_key.length() == 0 )
					return *this;

				key_remap[from_key] = to_key;

				return *this;
			}

		private:
			void remap_keys_in(value& v) {
				if ( !v.is<picojson::object>() )
					return;
				picojson::object& o = v.get<picojson::object>();
				// an untidy version
				for (key_map::const_iterator it = key_remap.begin(); it != key_remap.end(); ++it) {
					o[it->second]=o[it->first];
					o.erase(it->first);
				}
			}

		};

		template <typename TIn>
		projector<TIn> from(TIn& o) {
			return projector<TIn>(o);
		}

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
