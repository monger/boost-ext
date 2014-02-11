/**
 * A class template that wraps android and JNI functionality
 */
#ifndef _AndroidJni_h_
#define _AndroidJni_h_

#include "jace/Jace.h"
#include "jace/DefaultVmLoader.h"
#include "jace/OptionList.h"
#include "jace/proxy/java/lang/Throwable.h"

#include "boost/shared_ptr.hpp"
#include "boost-ext/auto_lock.hpp"
#include "boost-ext/exception.hpp"

#include "android/log.h"

/** Call these macros to start/end a JVM section */
#define ANDROID_JNI_START_NOTRY(t)                                                                      \
    {                                                                                                   \
        using namespace jace::proxy;                                                                    \
        boost_ext::auto_read_lock  __guard(boost_ext::AndroidJni<t>::inst().mtx());                     \
        if (!boost_ext::AndroidJni<t>::inst().initialized()) {                                          \
            BOOST_THROW_EXCEPTION(boost_ext::exception("JNI Not Initialized"));                         \
        }

#define ANDROID_JNI_END_NOTRY()                                                                         \
    }

#define ANDROID_JNI_START(t)                                                                            \
    ANDROID_JNI_START_NOTRY(t)                                                                          \
    try {

#define ANDROID_JNI_END()                                                                               \
    } catch (jace::proxy::java::lang::Throwable& t) {                                                   \
        t.printStackTrace();                                                                            \
        __android_log_print(ANDROID_LOG_FATAL, "AndroidJni",                                            \
                            "Unhandled java exception in JNI: %s", t.what());                           \
    } catch (std::exception& e) {                                                                       \
        __android_log_print(ANDROID_LOG_FATAL, "AndroidJni",                                            \
                            "Unhandled c++ exception in JNI: %s", e.what());                            \
    } catch (...) {                                                                                     \
        __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Unhandled exception in JNI");             \
    }                                                                                                   \
    ANDROID_JNI_END_NOTRY()

namespace boost_ext {
using namespace std;
using namespace boost;

    template <typename T>
    class AndroidJni {
    public:
        AndroidJni() {}
        ~AndroidJni() {
            auto_write_lock   guard(m_mtx);
            if (jace::getJavaVm()) {
                try {
                    /* Check if we created it (and should call destroy) or set it (and should call reset) */
                    if (m_loader) {
                        jace::destroyVm();
                    } else {
                        jace::resetJavaVm();
                    }
                } catch (std::exception& e) {
                    __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Error cleaning up: %s", e.what());
                } catch (...) {
                    __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Unhandled exception cleaning up");
                }
            }
            m_ctx.reset();
            m_loader.reset();
        }

        /** A static function for getting the singleton instance */
        static AndroidJni& inst() { static AndroidJni _inst; return _inst; }

        /** Returns the mutex so we can grab it in our scope macros */
        shared_mutex& mtx() { return m_mtx; }

        /** Initializes the JNI environment with a specific JVM and a context */
        bool initialize(JavaVM *pJvm, shared_ptr<T> ctx = shared_ptr<T>()) {
            auto_upgrade_lock           upGuard(m_mtx);
            if (jace::getJavaVm()) { return false; }
            /* Upgrade our lock to be a writeable one, and then set our values */
            auto_upgrade_unique_lock    guard(upGuard);

            try {
                jace::setJavaVm(pJvm);
                m_ctx = ctx;
                return true;
            } catch (std::exception& e) {
                __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Error setting VM: %s", e.what());
                return false;
            } catch (...) {
                __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Unhandled exception setting VM");
                return false;
            }
        }

        /** Initializes the JNI environment with a newly-created JVM */
        bool initialize(shared_ptr<T> ctx = shared_ptr<T>()) {
            auto_upgrade_lock           upGuard(m_mtx);
            if (jace::getJavaVm()) { return false; }
            /* Upgrade our lock to be a writeable one, and then set our values */
            auto_upgrade_unique_lock    guard(upGuard);

            try {
                m_loader.reset(new jace::DefaultVmLoader());
                jace::createVm(*m_loader, jace::OptionList(), false);
                m_ctx = ctx;
                return true;
            } catch (std::exception& e) {
                __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Error creating VM: %s", e.what());
                return false;
            } catch (...) {
                __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Unhandled exception creating VM");
                return false;
            }
        }

        /** Returns whether or not the JNI environment has been initialized */
        bool initialized() {
            auto_read_lock  guard(m_mtx);
            return jace::getJavaVm();
        }

        /** Returns the context */
        shared_ptr<const T> ctx() {
            auto_read_lock  guard(m_mtx);
            return m_ctx;
        }

    private:
        shared_mutex                        m_mtx;
        shared_ptr<jace::DefaultVmLoader>   m_loader;
        shared_ptr<const T>                 m_ctx;
    };

    void runTest();
}

#endif /* _AndroidJni_h_ */
