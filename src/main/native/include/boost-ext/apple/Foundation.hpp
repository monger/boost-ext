/**
 * Foundation helper macros and functions
 */
#ifndef H_BOOST_EXT_APPLE_FOUNDATION
#define H_BOOST_EXT_APPLE_FOUNDATION

#ifndef __OBJC__
    #error This file can only be used when compiling for objective-c++
#endif

#import <Foundation/Foundation.h>
#include <string>
#include "boost-ext/collections.hpp"
#include "boost-ext/scoped_type.hpp"

/** Macros which allow for arc functionality */
#if __has_feature(objc_arc)
    #define __ARC(stmt)               stmt
    #define __NON_ARC(stmt)
    #define __ARC_OR(stmt1, stmt2)    stmt1
#else
    #define __ARC(stmt)
    #define __NON_ARC(stmt)  stmt
    #define __ARC_OR(stmt1, stmt2)    stmt2
#endif

#define __ARC_COMMA       ,
#define __ARC_BRIDGE      __ARC(__bridge)

/* NOTE: all these __ARC_* functions contain semicolons where needed - you do not place them after the macro */
#define __ARC_SYNTH(v)    __NON_ARC(@synthesize v = _ ## v;)
#define __ARC_PVAR(t, v)  __NON_ARC(t _ ## v;)
#define __ARC_RETAIN(v)   __ARC_OR(v;,       [v retain];)
#define __ARC_RELEASE(v)  __ARC_OR(v = nil;, [v release] __ARC_COMMA v = nil;)
#define __ARC_DEALLOC()   __NON_ARC([super dealloc];)


namespace boost_ext {
using namespace std;
using namespace boost;

    /** Call this macro to cast an objective-c object pointer */
    #define NSO_PVOID(o)         ((__ARC_BRIDGE void*)(o))

    /** Converts the given objective-c object to a c_str */
    #define NSO_CSTR(o)    ([boost_ext::NSObjectToString(o) UTF8String])

    /** Safely casts an NSObject to the given class - or nil if it doesn't cast */
    #define NSO_CAST(o, t) (([o isKindOfClass:[t class]]) ? (t*) o : nil) 

    /** Some "toString" functions on common NSObjects */
    #define NSO_TO_STRING(t, v, r) inline NSString* NSObjectToString(t* v) { return r; }
    NSO_TO_STRING(NSObject,          o, [o description])
    NSO_TO_STRING(NSString,          s, s)
    NSO_TO_STRING(NSError,           e, [e localizedDescription])
    NSO_TO_STRING(NSURL,             u, [u absoluteString])
    NSO_TO_STRING(NSURLRequest,      r, NSObjectToString([r URL]))
    NSO_TO_STRING(NSURLConnection,   c, NSObjectToString([c currentRequest]))
    #undef NSO_TO_STRING
    inline NSString* NSObjectToString(NSURLResponse* r) {
        NSHTTPURLResponse *h = NSO_CAST(r, NSHTTPURLResponse);
        return (h == nil) ? 
                    [r description] : 
                    [NSString stringWithFormat:@"HTTP %d (%@)", 
                                    h.statusCode,
                                    [NSHTTPURLResponse localizedStringForStatusCode:h.statusCode]];
    }

    /** Appends the data from pData to the given vector.  Returns the passed-in vector */
    inline byte_vector& CopyNSDataToVector(NSData* pData, byte_vector& vec) {
        if (pData != nil && [pData length] > 0) {
            uint8_t         *pBytes = (uint8_t*)([pData bytes]);
            size_t          len = [pData length];
            vec.insert(vec.end(), pBytes, pBytes + len);
        }
        return vec;
    }

    /** Appends the data from the given vector into the given NSData pointer.  Returns the passed-in NSData */
    inline NSMutableData* CopyVectorToNSData(NSMutableData* pData, const byte_vector& vec) {
        if (vec.size() > 0) {
            [pData appendBytes:(&vec.front()) length:vec.size()];
        }
        return pData;
    }
    inline NSData* ToNSData(const byte_vector& vec) {
        if (vec.empty() > 0) {
            return [NSData data];
        }
        return [NSData dataWithBytes:(&vec.front()) length:vec.size()];
    }

    /** Creates an NSString from the given string */
    inline NSString* ToNSString(const char *pszStr) { return [NSString stringWithUTF8String:pszStr]; }
    inline NSString* ToNSString(const string& str) { return ToNSString(str.c_str()); }

    /** Creates an NSURL from the given string */
    template<typename T> inline NSURL* ToNSURL(const T& str) { return [NSURL URLWithString:ToNSString(str)]; }

    /** A scoped_type wrapper that will call release when an object leaves scope */
    template<typename T>
    class ScopedNSObject {
    public:
        typedef scoped_type<T, ScopedNSObject<T> > type;
    protected:
        void _destroy(T& obj) const { __ARC_RELEASE(obj) }
        T _get_invalid_value() const { return nil; }
    };

    /** Shortcut for our scoped object type */
    #define SCOPED_NSO(t) boost_ext::ScopedNSObject<t*>::type
}

#endif /* H_BOOST_EXT_APPLE_FOUNDATION */
