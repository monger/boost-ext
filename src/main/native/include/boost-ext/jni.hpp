/**
 * A class template that wraps JNI functionality
 */
#ifndef H_BOOST_EXT_JNI
#define H_BOOST_EXT_JNI

#include "jace/Jace.h"
#include "jace/DefaultVmLoader.h"
#include "jace/OptionList.h"

// #include "jace/proxy/java/lang/Throwable.h"
// 
// #include "boost/atomic.hpp"
#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "boost-ext/auto_lock.hpp"
#include "boost-ext/exception.hpp"

/** Define this macro before including jni.hpp to use a different logger */
#ifndef JNI_LOGGER
    #include "boost-ext/log.hpp"
    #define JNI_LOGGER()   LOG(fatal)
#endif

// /** Define this macro before including jni.hpp to use your own (subclassed) JNI */
// #ifndef JNI_CLASS
//     #define JNI_CLASS   boost_ext::Jni
// #endif
// 
/** Call these macros to get/initialize/uninitialize the instance */
#define JNI()               boost_ext::Jni::inst()
#define JNI_INITIALIZE(...) JNI().initialize(__VA_ARGS__)
#define JNI_UNINITIALIZE()  JNI().uninitialize()

/** Call these macros to start/end a JVM section */
// #define JNI_START()         BOOST_EXT_JNI_START(JNI_CLASS)
// #define JNI_END()           BOOST_EXT_JNI_END()
// 

#define JNI_START()                                                                                     \
    {                                                                                                   \
        using namespace jace::proxy;                                                                    \
        boost_ext::auto_read_lock  __guard(JNI().mtx());                                                \
        if (!JNI().initialized()) {                                                                     \
            BOOST_THROW_EXCEPTION(boost_ext::exception("JNI Not Initialized"));                         \
        }

#define JNI_END()                                                                                       \
    }

namespace boost_ext {
using namespace boost;

    class Jni : public noncopyable {
    public:
        /** Returns a singleton instance of the JNI class */
        static Jni& inst() { static Jni _inst; return _inst; }

        /** Returns whether or not the JNI environment has been initialized */
        bool initialized() {
            auto_read_lock              guard(m_mtx);
            return jace::getJavaVm();
        }
        
        /** Initializes this class.  Note, it is safe to call this multiple times */
        bool initialize(JavaVM *pJvm = NULL) {
            auto_upgrade_lock           upGuard(m_mtx);

            /* Only initialize if we aren't already initialized, and have not been uninitialized */
            if (jace::getJavaVm()) { return false; }
            if (m_uninitialized) {
                JNI_LOGGER() << "Cannot reinitialize VM";
                return false;
            }

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
                JNI_LOGGER() << "Error initializing VM: " << e.what();
                return false;
            } catch (...) {
                JNI_LOGGER() << "Unhandled exception initializing VM";
                return false;
            }
        }
        
        /** 
         * Uninitializes this class.  Note, it is safe to call this multiple times, but you cannot call initialize 
         * after uninitialization.
         */
        void uninitialize() {
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
                    JNI_LOGGER() << "Error cleaning up: " << e.what();
                } catch (...) {
                    JNI_LOGGER() << "Unhandled exception cleaning up";
                }
                m_loader.reset();
            }
        }
        
        /** Returns the mutex so we can grab it in our scope macros */
        shared_mutex& mtx() { return m_mtx; }
        
    private:
        /* Constructors and destructors are private */
        Jni() : m_uninitialized(false) {}
        ~Jni() {}
        
    private:
        bool                                m_uninitialized;
        shared_mutex                        m_mtx;
        shared_ptr<jace::DefaultVmLoader>   m_loader;
    };
}

#endif /* H_BOOST_EXT_JNI */
