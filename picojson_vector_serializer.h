#pragma once
#include "picojson_serializer.h"
#include <vector>

namespace picojson {
    namespace convert{

        template<typename T,typename Allocator> struct value_converter< std::vector<T,Allocator> > {
            static value to_value(std::vector<T, Allocator> const& v) {
                picojson::array a;
                for ( std::vector<T, Allocator>::const_iterator it = v.begin();
                    it != v.end();
                    ++it ) {
                        a.push_back(value_converter<T>::to_value(*it));
                }
                return value(a);
            }

            static void from_value(value const& ov, std::vector<T, Allocator>& v) {
                //if ( ov.is<std::string>() ) v = ov.get<std::string>();
            }
        };
    }
}

