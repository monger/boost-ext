/**
 * Main execution entry for unit tests
 */

/* Since this is the main entry point, we define this value before including our headers */
#define BOOST_TEST_MODULE "Boost-Ext Tests"
#include "boost-ext/test/unit_test.hpp"

/* Global configuration */
struct GlobalConfig {
    GlobalConfig() {}
    ~GlobalConfig() {}
};
BOOST_GLOBAL_FIXTURE(GlobalConfig);
