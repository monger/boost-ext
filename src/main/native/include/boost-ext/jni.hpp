/**
 * A class template that wraps JNI functionality
 */
#ifndef H_BOOST_EXT_JNI
#define H_BOOST_EXT_JNI

#include "jace/Jace.h"
#include "jace/DefaultVmLoader.h"
#include "jace/OptionList.h"

#include "boost/utility.hpp"
#include "boost-ext/auto_lock.hpp"
#include "boost-ext/exception.hpp"
#include "boost-ext/classes.hpp"

/** Define this macro before including jni.hpp to use a different logger */
#ifndef JNI_LOGGER
    #include "boost-ext/log.hpp"
    #define JNI_LOGGER()   LOG(fatal)
#endif

/** Call these macros to get/initialize/uninitialize the instance */
#define JNI()               boost_ext::Jni::inst()
#define JNI_INITIALIZE(...) JNI().initialize(__VA_ARGS__)
#define JNI_UNINITIALIZE()  JNI().uninitialize()

/** Call these macros to start/end a JVM section */
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
        SINGLETON(Jni, inst)

        /** Returns whether or not the JNI environment has been initialized */
        bool initialized() {
            auto_read_lock              guard(m_mtx);
            return jace::getJavaVm();
        }
        
        /**
         * You can call inititialize with either a jvm or an option list (or neither) - but not both.
         */
        bool initialize(JavaVM *pJvm = NULL) {
            return initialize(pJvm, jace::OptionList());
        }
        
        bool initialize(const jace::OptionList& list) {
            return initialize(NULL, list);
        }

        /** 
         * Uninitializes this class.  Note, it is safe to call this multiple times, but you cannot call initialize 
         * after uninitialization.
         */
        void uninitialize() {
            auto_write_lock   guard(m_mtx);
            if (jace::getJavaVm()) {
                try {
                    jace::resetJavaVm();
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

        /** 
         * Initializes this class.  Note, it is safe to call this multiple times, but only the first one will be 
         * honored.  This is private to prevent people from unintentionally calling the function with BOTH a JavaVM
         * AND an OptionList - the public functions above handle that restriction.
         */
        bool initialize(JavaVM *pJvm, const jace::OptionList& list) {
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
                    jace::createJavaVm(m_loader, list);
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
        
    private:
        bool            m_uninitialized;
        shared_mutex    m_mtx;
        jace::Loader    m_loader;

    };
}

#endif /* H_BOOST_EXT_JNI */
