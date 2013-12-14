#ifndef H_BOOST_EXT_COLLECTIONS
#define H_BOOST_EXT_COLLECTIONS

#include <set>
#include <vector>
#include <string>

#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"

namespace boost_ext {
    typedef std::set<std::string>       string_set;
    typedef std::vector<std::string>    string_vector;

    template <typename T, typename U>
    static bool contains_any(const T& v1, const U& v2) {
        if (v1.size() == 0) { return false; }
        BOOST_FOREACH( std::string value, v2 ) {
            if (std::find(v1.begin(), v1.end(), value) != v1.end()) { return true; }
        }
        return false;
    }
    template <typename T, typename U>
    static bool contains_all(const T& v1, const U& v2) {
        if (v1.size() == 0) { return false; }
        BOOST_FOREACH( std::string value, v2 ) {
            if (std::find(v1.begin(), v1.end(), value) == v1.end()) { return false; }
        }
        return true;
    }

    template <typename T>
    static T trim_split(std::string s, const std::string& chars = "\t, ") {
        T v;
        boost::trim_if(s, boost::is_any_of(chars));
        if (s.size() > 0) {
            boost::split(v, s, boost::is_any_of(chars), boost::token_compress_on);
        }
        return v;
    }
    template <typename T>
    static T trim_split(const char *s, const std::string& chars = "\t, ") {
        return boost_ext::trim_split<T>((std::string) (s ? s : ""), chars);
    }
}

#endif
