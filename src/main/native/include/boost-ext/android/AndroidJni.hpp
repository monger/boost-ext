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

/** Define this macro before including AndroidJni.hpp to use your own (subclassed) JNI */
#ifndef JNI_CLASS
    #define JNI_CLASS   boost_ext::AndroidJni
#endif

/** Call these macros to get/initialize the instance */
#define JNI_INSTANCE()      boost_ext::AndroidJni::inst<JNI_CLASS>()
#define JNI_INITIALIZE(...) JNI_INSTANCE().initialize(__VA_ARGS__)

/** Call these macros to start/end a JVM section */
#define JNI_START()         ANDROID_JNI_START(JNI_CLASS)
#define JNI_END()           ANDROID_JNI_END()
#define JNI_START_NOTRY()   ANDROID_JNI_START_NOTRY(JNI_CLASS)
#define JNI_END_NOTRY()     ANDROID_JNI_END_NOTRY()

#define ANDROID_JNI_START_NOTRY(T)                                                                      \
    {                                                                                                   \
        using namespace jace::proxy;                                                                    \
        boost_ext::auto_read_lock  __guard(boost_ext::AndroidJni::inst<T>().mtx());                     \
        if (!boost_ext::AndroidJni::inst<T>().initialized()) {                                          \
            BOOST_THROW_EXCEPTION(boost_ext::exception("JNI Not Initialized"));                         \
        }

#define ANDROID_JNI_END_NOTRY()                                                                         \
    }

#define ANDROID_JNI_START(T)                                                                            \
    ANDROID_JNI_START_NOTRY(T)                                                                          \
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
using namespace boost;

    class AndroidJni {
    public:
        AndroidJni() {}
        virtual ~AndroidJni() {
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
            m_loader.reset();
        }

        /** A static function for getting the singleton instance */
        template <typename T>
        static T& inst() { static T _inst; return _inst; }

        /** Returns the mutex so we can grab it in our scope macros */
        shared_mutex& mtx() { return m_mtx; }

        /** Initializes the JNI environment with a specific JVM */
        virtual bool initialize(JavaVM *pJvm = NULL) {
            auto_upgrade_lock           upGuard(m_mtx);
            if (jace::getJavaVm()) { return false; }
            /* Upgrade our lock to be a writeable one, and then set our values */
            auto_upgrade_unique_lock    guard(upGuard);

            try {
                if (pJvm) {
                    /* We use the Vm that is passed in */
                    m_loader.reset();
                    jace::setJavaVm(pJvm);
                } else {
                    /* We create a new VM */
                    m_loader.reset(new jace::DefaultVmLoader());
                    jace::createVm(*m_loader, jace::OptionList(), false);
                }
                return true;
            } catch (std::exception& e) {
                __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Error initializing VM: %s", e.what());
                return false;
            } catch (...) {
                __android_log_print(ANDROID_LOG_FATAL, "AndroidJni", "Unhandled exception initializing VM");
                return false;
            }
        }

        /** Returns whether or not the JNI environment has been initialized */
        bool initialized() {
            auto_read_lock  guard(m_mtx);
            return jace::getJavaVm();
        }

    private:
        shared_mutex                        m_mtx;
        shared_ptr<jace::DefaultVmLoader>   m_loader;
    };
}

#endif /* _AndroidJni_h_ */
