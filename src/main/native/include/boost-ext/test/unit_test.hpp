#include "boost/test/unit_test.hpp"
#include "boost/test/parameterized_test.hpp"
#include "boost-ext/collections.hpp"

#ifndef H_BOOST_EXT_UNIT_TEST
#define H_BOOST_EXT_UNIT_TEST

#if !defined(BOOST_UNIT_TEST_NO_TEAMCITY)
    #include "boost-ext/test/teamcity_boost.hpp"
#endif

/** These are the environment variables that will be read */
#if !defined(BOOST_TEST_INCLUDE_GROUPS)
    #define BOOST_TEST_INCLUDE_GROUPS "BOOST_TEST_INCLUDE_GROUPS"
#endif
#if !defined(BOOST_TEST_EXCLUDE_GROUPS)
    #define BOOST_TEST_EXCLUDE_GROUPS "BOOST_TEST_EXCLUDE_GROUPS"
#endif

//____________________________________________________________________________//

// ************************************************************************** //
// **************           auto_test_unit_registrar           ************** //
// ************************************************************************** //
namespace boost_ext {
namespace unit_test {
namespace ut_detail {

struct BOOST_TEST_DECL auto_test_unit_registrar
{
    // Constructors
    auto_test_unit_registrar( const std::string& groups, test_case* tc, counter_t exp_fail ) {
        if(should_register(groups)) { boost::unit_test::ut_detail::auto_test_unit_registrar r(tc, exp_fail); }
    }
    auto_test_unit_registrar( const std::string& groups, const_string ts_name ) {
        if(should_register(groups)) { boost::unit_test::ut_detail::auto_test_unit_registrar r(ts_name); }
    }
    auto_test_unit_registrar( const std::string& groups, test_unit_generator const& tc_gen ) {
        if(should_register(groups)) { boost::unit_test::ut_detail::auto_test_unit_registrar r(tc_gen); }
    }
    auto_test_unit_registrar( const std::string& groups, int i) {
        if(should_register(groups)) { boost::unit_test::ut_detail::auto_test_unit_registrar r(i); }
    }
private:
    static bool should_register(const std::string& groupStr) {
        string_set groups   = boost_ext::trim_split<string_set>(groupStr);
        string_set includes = boost_ext::trim_split<string_set>(getenv(BOOST_TEST_INCLUDE_GROUPS));
        string_set excludes = boost_ext::trim_split<string_set>(getenv(BOOST_TEST_EXCLUDE_GROUPS));
        
        if (boost_ext::contains_any(groups, excludes)) {
            return false;
        } else if (includes.size() > 0 && contains_any(groups, includes)) {
            return true;
        }
        return includes.size() == 0;
    }
};

}
}
}

#define BOOST_EXT_AUTO_TU_REGISTRAR( test_name )                                                                  \
static boost_ext::unit_test::ut_detail::auto_test_unit_registrar BOOST_JOIN( BOOST_JOIN( test_name, _registrar ), \
                                                                             __LINE__ )

//____________________________________________________________________________//

// ************************************************************************** //
// **************            BOOST_FIXTURE_TEST_CASE           ************** //
// ************************************************************************** //

#define BOOST_EXT_FIXTURE_TEST_CASE( groups, test_name, F )             \
struct test_name : public F { void test_method(); };                    \
                                                                        \
static void BOOST_AUTO_TC_INVOKER( test_name )()                        \
{                                                                       \
    test_name t;                                                        \
    t.test_method();                                                    \
}                                                                       \
                                                                        \
struct BOOST_AUTO_TC_UNIQUE_ID( test_name ) {};                         \
                                                                        \
BOOST_EXT_AUTO_TU_REGISTRAR( test_name )(                               \
    groups,                                                             \
    boost::unit_test::make_test_case(                                   \
        &BOOST_AUTO_TC_INVOKER( test_name ), #test_name ),              \
    boost::unit_test::ut_detail::auto_tc_exp_fail<                      \
        BOOST_AUTO_TC_UNIQUE_ID( test_name )>::instance()->value() );   \
                                                                        \
void test_name::test_method()

//____________________________________________________________________________//

// ************************************************************************** //
// **************         BOOST_FIXTURE_PARAM_TEST_CASE        ************** //
// ************************************************************************** //
#define BOOST_FIXTURE_PARAM_TEST_CASE( groups,                          \
                                       test_name,                       \
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
BOOST_EXT_AUTO_TU_REGISTRAR( test_name )(                               \
    groups,                                                             \
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
// **************      BOOST_AUTO_GRP_PARAM_TEST_CASE_DATA     ************** //
// ************************************************************************** //
#define BOOST_AUTO_GRP_PARAM_TEST_CASE_DATA( groups, test_name, data, val_type, p ) \
BOOST_FIXTURE_PARAM_TEST_CASE( groups, test_name,                                   \
                               val_type, p,                                         \
                               data,                                                \
                               BOOST_AUTO_PARAM_SIZE(data),                         \
                               BOOST_AUTO_TEST_CASE_FIXTURE )

//____________________________________________________________________________//

// ************************************************************************** //
// **************        BOOST_AUTO_GRP_PARAM_TEST_CASE        ************** //
// ************************************************************************** //
#define BOOST_AUTO_GRP_PARAM_TEST_CASE( groups, test_name, val_type, p )     \
BOOST_AUTO_GRP_PARAM_TEST_CASE_DATA( groups, test_name,                      \
                                     BOOST_AUTO_TC_PARAMS( test_name ),      \
                                     val_type, p )

//____________________________________________________________________________//

// ************************************************************************** //
// **************        BOOST_AUTO_PARAM_TEST_CASE_DATA       ************** //
// ************************************************************************** //
#define BOOST_AUTO_PARAM_TEST_CASE_DATA( test_name, data, val_type, p ) \
BOOST_AUTO_GRP_PARAM_TEST_CASE_DATA( "", test_name, data, val_type, p )

//____________________________________________________________________________//

// ************************************************************************** //
// **************          BOOST_AUTO_PARAM_TEST_CASE          ************** //
// ************************************************************************** //
#define BOOST_AUTO_PARAM_TEST_CASE( test_name, val_type, p )            \
BOOST_AUTO_GRP_PARAM_TEST_CASE( "", test_name, val_type, p )

// ************************************************************************** //
// **************           BOOST_AUTO_GRP_TEST_CASE           ************** //
// ************************************************************************** //

#define BOOST_AUTO_GRP_TEST_CASE( groups, test_name )                   \
BOOST_EXT_FIXTURE_TEST_CASE( groups, test_name, BOOST_AUTO_TEST_CASE_FIXTURE )

// ************************************************************************** //
// **************             BOOST_AUTO_TEST_CASE             ************** //
// ************************************************************************** //

#ifdef BOOST_AUTO_TEST_CASE
    #undef BOOST_AUTO_TEST_CASE
#endif
#define BOOST_AUTO_TEST_CASE(test_name ) BOOST_AUTO_GRP_TEST_CASE( "", test_name )


#endif