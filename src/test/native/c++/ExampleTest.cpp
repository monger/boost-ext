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
