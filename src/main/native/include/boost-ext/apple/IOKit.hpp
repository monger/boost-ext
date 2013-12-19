/**
 * IOKit helper macros and functions
 */
#ifndef H_BOOST_EXT_APPLE_IOKIT
#define H_BOOST_EXT_APPLE_IOKIT

#include <IOKit/IOKitLib.h>
#include "boost-ext/scoped_type.hpp"

namespace boost_ext {
using namespace boost;

    /** A scoped_type wrapper that will call IOObjectRelease when an object leaves scope */
    template<typename T>
    class ScopedIOObject {
    public:
        typedef based_scoped_type<io_object_t, T, ScopedIOObject<T> > type;
    protected:
        void _destroy(io_object_t& obj) const { IOObjectRelease(obj), obj = IO_OBJECT_NULL; }
        io_object_t _get_invalid_value() const { return IO_OBJECT_NULL; }
    };

    /** Shortcut for our scoped object type */
    #define SCOPED_IOOBJ(t) boost_ext::ScopedIOObject<t>::type

}

#endif /* H_BOOST_EXT_APPLE_IOKIT */
