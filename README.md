#Boost-Ext Library

These are header-only libraries which add some common functionality to the boost library.

This project depends upon the [maven-native-oat](https://github.com/toonetown/maven-native-oat) project to build.

### boost-ext/test/unit_test.hpp

Including this file (instead of boost/test/unit_test.hpp) adds the ability to define parameter-based unit tests (tests 
that will run multiple times with a parameter set).  For example:

    // Use this include instead of "boost/test/unit_test.hpp"
    #include "boost-ext/test/unit_test.hpp"

    // Set up our suite fixture
    struct ExampleTestFixture {
        ExampleTestFixture() { }
        ~ExampleTestFixture() { }
    };

    // Register the fixture with our suite - just like "regular" BOOST_AUTO_TEST_CASE
    BOOST_FIXTURE_TEST_SUITE(ExampleTestSuite, ExampleTestFixture);

    // Just do the same to define a single test case
    BOOST_AUTO_TEST_CASE(testRunOnce) {
        BOOST_MESSAGE(" Running single test");
        BOOST_CHECK(true);
    }

    // And do this to run with parameters - define the parameters first
    BOOST_AUTO_TEST_PARAMS(testRunParam, int) { 1, 2, 3, 4, 5 };

    // And register the param-based test case.  Use the same name as in BOOST_AUTO_TEST_PARAMS
    BOOST_AUTO_PARAM_TEST_CASE(testRunParam, int, i) {
        BOOST_MESSAGE("Running param with " << i);
        BOOST_CHECK(true);
    }

    // You can also use any type you wish, just make sure things match
    BOOST_AUTO_TEST_PARAMS(testRunParamString, std::string) {
        "First", "Second", "Third"
    };
    BOOST_AUTO_PARAM_TEST_CASE(testRunParamString, std::string, str) {
        BOOST_MESSAGE("Running string with " << str);
        BOOST_CHECK(true);
    }

    // Remember to end your suite
    BOOST_AUTO_TEST_SUITE_END ();
    

### Boost Teamcity Library

These are just a repackaging of the libraries at https://github.com/JetBrains/teamcity-cpp/tree/master/boost for 
integrating boost unit tests with TeamCity.  Just include `boost-ext/test/unit_test.hpp` after setting the 
`BOOST_TEST_MODULE` macro, and then include this library as a dependency with scope `test` in order to have these tests 
output in a TeamCity-friendly format when running under TeamCity.  If you would like to include 
`boost-ext/test/unit_test.hpp` without linking in the TeamCity library, define `BOOST_UNIT_TEST_NO_TEAMCITY` before
including the file.
