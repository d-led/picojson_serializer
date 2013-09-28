#pragma once

template <typename T>
bool has(picojson::object const& o, std::string const& key) {
    picojson::object::const_iterator found = o.find(key);
    if ( found == o.end() )
        return false;

    if ( !found->second.is<T>() )
        return false;

    return true;
}

template <typename T, typename TValue>
bool has(picojson::object const& o, std::string const& key, TValue v) {
    if ( !has<T>(o, key) )
        return false;
    return o.find(key)->second.get<T>() == v;
}

template <typename T, typename TValue>
bool has(picojson::value const& ov, std::string const& key, TValue v) {
    if ( !ov.is<picojson::object>() )
        return false;

    return has<T>(ov.get<picojson::object>(), key, v);
}

template <typename T>
bool has(picojson::value const& ov, std::string const& key) {
    if ( !ov.is<picojson::object>() )
        return false;

    return has<T>(ov.get<picojson::object>(), key);
}

template <typename T>
bool is(picojson::array const& a, size_t pos, T const& t)
{
    if ( pos >= a.size() )
        return false;
    
    if ( a[pos].is<T>() )
        return a[pos].get<T>() == t;

    return false;
}

template <typename T>
bool is(picojson::array const& a, size_t pos)
{
    if ( pos >= a.size() )
        return false;

    if ( a[pos].is<T>() )
        return true;

    return false;
}