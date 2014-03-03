/**
 * Main execution entry for unit tests
 */

/* Since this is the main entry point, we define this value before including our headers */
#define BOOST_TEST_MODULE "Boost-Ext Tests"
#include "boost-ext/test/unit_test.hpp"

#include "boost-ext/log.hpp"
#include "boost-ext/jni.hpp"

/* Global configuration */
struct GlobalConfig {
    GlobalConfig() {
        /* We need to initialize JNI and our logging */
        SET_LOG_LEVEL(warning);
        JNI_INITIALIZE();        
    }
    ~GlobalConfig() {
        /* We need to uninitialize JNI */
        JNI_UNINITIALIZE();        
    }
};
BOOST_GLOBAL_FIXTURE(GlobalConfig);
