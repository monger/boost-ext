/**
 * A function which will make a shared_ptr from a static variable
 */
#ifndef H_BOOST_EXT_MAKE_SHARED_STATIC
#define H_BOOST_EXT_MAKE_SHARED_STATIC

#include "boost/shared_ptr.hpp"
namespace boost_ext {

    struct shared_static_null_deleter { void operator ()(void const *) const {} };

    template <typename T>
    inline boost::shared_ptr<T> make_shared_static(T& obj) {
        boost::shared_ptr<T> pObj(&obj, shared_static_null_deleter());
        return pObj;
    }
}

#endif /* H_BOOST_EXT_MAKE_SHARED_STATIC */
