#include "boost/test/unit_test.hpp"
#include "boost/test/parameterized_test.hpp"

#if !defined(BOOST_UNIT_TEST_NO_TEAMCITY)
    #include "boost-ext/test/teamcity_boost.hpp"
#endif

//____________________________________________________________________________//

// ************************************************************************** //
// **************         BOOST_FIXTURE_PARAM_TEST_CASE        ************** //
// ************************************************************************** //
#define BOOST_FIXTURE_PARAM_TEST_CASE( test_name,                       \
                                       val_type, p, test_vals, val_len, \
                                       F )                              \
struct test_name : public F { void test_method(val_type p); };          \
                                                                        \
static void BOOST_AUTO_TC_INVOKER( test_name )(val_type p)              \
{                                                                       \
    test_name t;                                                        \
    t.test_method(p);                                                   \
}                                                                       \
                                                                        \
struct BOOST_AUTO_TC_UNIQUE_ID( test_name ) {};                         \
                                                                        \
BOOST_AUTO_TU_REGISTRAR( test_name )(                                   \
    boost::unit_test::make_test_case(                                   \
        &BOOST_AUTO_TC_INVOKER( test_name ), #test_name,                \
        (test_vals), (test_vals + val_len) ) );                         \
                                                                        \
void test_name::test_method(val_type p)

//____________________________________________________________________________//

// ************************************************************************** //
// **************             BOOST_AUTO_TC_PARAMS             ************** //
// ************************************************************************** //
#define BOOST_AUTO_TC_PARAMS( test_name ) BOOST_JOIN( test_name, _params )

// ************************************************************************** //
// **************            BOOST_AUTO_PARAM_SIZE             ************** //
// ************************************************************************** //
#define BOOST_AUTO_PARAM_SIZE( test_vals )                              \
(sizeof(test_vals) / sizeof(test_vals[0]))

//____________________________________________________________________________//

// ************************************************************************** //
// **************            BOOST_AUTO_TEST_PARAMS            ************** //
// ************************************************************************** //
#define BOOST_AUTO_TEST_PARAMS( test_name, val_type)                    \
static val_type BOOST_AUTO_TC_PARAMS( test_name )[] =

//____________________________________________________________________________//

// ************************************************************************** //
// **************        BOOST_AUTO_PARAM_TEST_CASE_DATA       ************** //
// ************************************************************************** //
#define BOOST_AUTO_PARAM_TEST_CASE_DATA( test_name, data, val_type, p ) \
BOOST_FIXTURE_PARAM_TEST_CASE( test_name,                               \
                               val_type, p,                             \
                               data,                                    \
                               BOOST_AUTO_PARAM_SIZE(data),             \
                               BOOST_AUTO_TEST_CASE_FIXTURE )

//____________________________________________________________________________//

// ************************************************************************** //
// **************          BOOST_AUTO_PARAM_TEST_CASE          ************** //
// ************************************************************************** //
#define BOOST_AUTO_PARAM_TEST_CASE( test_name, val_type, p )            \
BOOST_AUTO_PARAM_TEST_CASE_DATA( test_name,                             \
                                 BOOST_AUTO_TC_PARAMS( test_name ),     \
                                 val_type, p )
