/**
 * This file sets "platform detection" macros as follows.  These macros should be used in all code, rather than the
 * compiler-specific macros, to ensure that the correct values are being detected.
 *
 *      _IS_OS_WINDOWS_    Set when building for Windows   (-Dnative.os=windows)
 *      _IS_OS_MACOSX_     Set when building for Mac OS X  (-Dnative.os=macosx)
 *      _IS_OS_IOS_        Set when building for iOS       (-Dnative.os=ios)
 *      _IS_OS_ANDROID_    Set when building for Android   (-Dnative.os=android)
 *      _IS_OS_LINUX_      Set when building for Linux     (-Dnative.os=linux)
 *
 *      _IS_OS_APPLE_      Set when building for either Mac OS X *or* iOS
 *
 * The following architectures are detected as well.  Note that these are exclusive of each other (i.e. _ARCH_ARMV7_
 * is *NOT* defined when _ARCH_ARMV7S_ is defined.
 *
 *      _IS_ARCH_X86_64_   Set when building for x86_64    (-Dnative.arch=x86_64)
 *      _IS_ARCH_I386_     Set when building for i386      (-Dnative.arch=i386)
 *      _IS_ARCH_PPC_      Set when building for ppc       (-Dnative.arch=ppc)
 *      _IS_ARCH_ARM64_    Set when building for arm64     (-Dnative.arch=arm64)
 *      _IS_ARCH_ARMV7S_   Set when building for armv7s    (-Dnative.arch=armv7s)
 *      _IS_ARCH_ARMV7_    Set when building for armv7     (-Dnative.arch=armv7)
 *      _IS_ARCH_ARMV6_    Set when building for armv6     (-Dnative.arch=armv6)
 *      _IS_ARCH_ARM_      Set when building for arm       (-Dnative.arch=arm)
 *
 * In addition, the combination macros "_IS_<OS>_<ARCH>_" are also set where <OS> is the OS value and <ARCH> is the
 * architecture value.
 *
 * This file also sets _INC_DIR_OS(), _INC_DIR_ARCH(), and _INC_DIR_OS_ARCH() macros to point to a specific directory
 * for include files.  These macros do *NOT* include a trailing slash, so one must be added
 *
 */
#ifndef H_BOOST_EXT_PLATFORM_DETECT
#define H_BOOST_EXT_PLATFORM_DETECT

#include "boost-ext/common.hpp"

/* Do our detection */
#if defined(_WIN32)

    #define _IS_OS_WINDOWS_                 1
    #define _NATIVE_OS_NAME_                windows

    #if defined(_M_AMD64)
        #define _IS_ARCH_X86_64_            1
        #define _IS_WINDOWS_X86_64_         1
    #elif defined(_M_IX86)
        #define _IS_ARCH_I386_              1
        #define _IS_WINDOWS_I386_           1
    #else
        #error Unsupported Windows Architecture
    #endif

#elif defined(__APPLE__)
    // Use the appropriate header file
    #include <TargetConditionals.h>
    #define _IS_OS_APPLE_                   1

    #if TARGET_OS_IPHONE

        #define _IS_OS_IOS_                 1
        #define _NATIVE_OS_NAME_            ios

        #if defined(__x86_64__)
            #define _IS_ARCH_X86_64_        1
            #define _IS_IOS_X86_64_         1
        #elif defined(__i386__)
            #define _IS_ARCH_I386_          1
            #define _IS_IOS_I386_           1
        #elif defined(__ARM64_ARCH_8__)
            #define _IS_ARCH_ARM64_         1
            #define _IS_IOS_ARM64_          1
        #elif defined(__ARM_ARCH_7S__)
            #define _IS_ARCH_ARMV7S_        1
            #define _IS_IOS_ARMV7S_         1
        #elif defined(__ARM_ARCH_7A__)
            #define _IS_ARCH_ARMV7_         1
            #define _IS_IOS_ARMV7_          1
        #elif defined(__ARM_ARCH_6K__)
            #define _IS_ARCH_ARMV6_         1
            #define _IS_IOS_ARMV6_          1
        #else
            #error Unsupported IOS Architecture
        #endif

    #elif TARGET_OS_MAC

        #define _IS_OS_MACOSX_              1
        #define _NATIVE_OS_NAME_            macosx

        #if defined(__x86_64__)
            #define _IS_ARCH_X86_64_        1
            #define _IS_MACOSX_X86_64_      1
        #elif defined(__i386__)
            #define _IS_ARCH_I386_          1
            #define _IS_MACOSX_I386_        1
        #elif defined(__ppc__)
            #define _IS_ARCH_PPC_           1
            #define _IS_MACOSX_PPC_         1
        #else
            #error Unsupported Mac OS X Architecture
        #endif

    #else
        #error Unsupported Apple OS
    #endif
#elif defined(__ANDROID__)

    #define _IS_OS_ANDROID_                 1
    #define _NATIVE_OS_NAME_                android

    #if defined(__i386__)
        #define _IS_ARCH_I386_              1
        #define _IS_ANDROID_I386_           1
    #elif defined(__ARM_ARCH_7A__)
        #define _IS_ARCH_ARMV7_             1
        #define _IS_ANDROID_ARMV7_          1
    #elif defined(__ARM_ARCH_5TE__)
        #define _IS_ARCH_ARM_               1
        #define _IS_ANDROID_ARM_            1
    #else
        #error Unsupported Android Architecture
    #endif

#elif defined(__linux__)

    #define _IS_OS_LINUX_                   1
    #define _NATIVE_OS_NAME_                linux

    #if defined(__x86_64__)
        #define _IS_ARCH_X86_64_            1
        #define _IS_LINUX_X86_64_           1
    #elif defined(__i386__)
        #define _IS_ARCH_I386_              1
        #define _IS_LINUX_I386_             1
    #else
        #error Unsupported Mac OS X Architecture
    #endif

#else
    #error Unsupported OS
#endif

/* Get our architecture name */
#if _IS_ARCH_X86_64_
    #define _NATIVE_ARCH_NAME_      x86_64
#elif _IS_ARCH_I386_
    #define _NATIVE_ARCH_NAME_      i386
#elif _IS_ARCH_PPC_
    #define _NATIVE_ARCH_NAME_      ppc
#elif _IS_ARCH_ARM64_
    #define _NATIVE_ARCH_NAME_      arm64
#elif _IS_ARCH_ARMV7S_
    #define _NATIVE_ARCH_NAME_      armv7s
#elif _IS_ARCH_ARMV7_
    #define _NATIVE_ARCH_NAME_      armv7
#elif _IS_ARCH_ARMV6_
    #define _NATIVE_ARCH_NAME_      armv6
#elif _IS_ARCH_ARM_
    #define _NATIVE_ARCH_NAME_      arm
#else
    #error Unsupported Architecture
#endif

/* Calculate our include dirs */
#define _INC_DIR_OS(_FNAME_)        QUOTE(plat/os-_NATIVE_OS_NAME_/_FNAME_)
#define _INC_DIR_ARCH(_FNAME_)      QUOTE(plat/arch-_NATIVE_ARCH_NAME_/_FNAME_)
#define _INC_DIR_OS_ARCH(_FNAME_)   QUOTE(plat/os-_NATIVE_OS_NAME_/arch-_NATIVE_ARCH_NAME_/_FNAME_)

#endif /* H_BOOST_EXT_PLATFORM_DETECT */
