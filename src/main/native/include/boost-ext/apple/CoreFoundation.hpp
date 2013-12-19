/**
 * CoreFoundation helper macros and functions
 */
#ifndef H_BOOST_EXT_APPLE_CORE_FOUNDATION
#define H_BOOST_EXT_APPLE_CORE_FOUNDATION

#include <CoreFoundation/CoreFoundation.h>
#include "boost-ext/scoped_type.hpp"

namespace boost_ext {
using namespace boost;

    /** Converts the given CFStringRef to a c_str */
    #define CF_CSTR(cf)    CFStringGetCStringPtr((CFStringRef) cf, kCFStringEncodingUTF8)

    /** A scoped_type wrapper that will call CFRelease when an object leaves scope */
    template<typename T>
    class ScopedCFRef {
    public:
        typedef based_scoped_type<CFTypeRef, T, ScopedCFRef<T> > type;
    protected:
        void _destroy(CFTypeRef& obj) const { if (obj) { CFRelease(obj), obj = NULL; } }
        CFTypeRef _get_invalid_value() const { return NULL; }
    };

    /** Shortcut for our scoped object type */
    #define SCOPED_CFR(t) boost_ext::ScopedCFRef<t>::type

}

#endif /* H_BOOST_EXT_APPLE_CORE_FOUNDATION */
