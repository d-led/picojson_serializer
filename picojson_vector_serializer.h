#pragma once
#include "picojson_serializer.h"
#include <vector>

namespace picojson {
    namespace convert{

        template<typename T,typename Allocator> struct value_converter< std::vector<T,Allocator> > {
            static value to_value(std::vector<T, Allocator>& v) {
                picojson::array a;
                for ( std::vector<T, Allocator>::iterator it = v.begin();
                    it != v.end();
                    ++it ) {
                        a.push_back(value_converter<T>::to_value(*it));
                }
                return value(a);
            }

            static void from_value(value const& ov, std::vector<T, Allocator>& v) {
                if ( !ov.is<picojson::array>() )
                    return;
                picojson::array const& a = ov.get<picojson::array>();
                for ( picojson::array::const_iterator it = a.begin();
                    it != a.end();
                    ++it ) {
                        T t;
                        value_converter<T>::from_value(*it, t);
                        v.push_back(t);
                }
            }
        };
    }
}

