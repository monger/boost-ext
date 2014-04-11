/*
 * Unit test for JNI
 */

/* 32-bit OS X doesn't have a JVM anymore - so don't do any of these tests */
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if !(TARGET_OS_MAC) || defined(__x86_64__)

#include "boost-ext/test/unit_test.hpp"

#include "boost-ext/jni.hpp"
#include "jace/proxy/java/lang/String.h"
#include "jace/proxy/java/lang/Exception.h"

#include "boost-ext/thread_pool.hpp"

/* Create setup and teardown functions */
struct JniFixture {
    boost::shared_ptr<jace::DefaultVmLoader> m_loader;

	JniFixture() {
		/* Common setup before test cases here */
	}

	~JniFixture() {
		/* Common tear down after test cases here. */
	}
};

namespace JniThreadTest {
    using namespace std;
    using namespace boost;
    using namespace boost_ext;

    typedef future<const string> FutureString;

    BOOST_EXT_THREAD_POOL_WITH_SIZE(MyThreadPool, 10);

    static string getString(const string& p1) {
        JNI_START() {
            java::lang::String s1(p1);
            this_thread::sleep(posix_time::seconds(1));
            return s1.substring(6, 11);
        } JNI_END()
    }

    static string getString_ptrs(shared_ptr<const string> pP1) { return getString(*pP1); }

    static FutureString runFuture(const string& p1) {
        shared_ptr<const string> pP1(new string(p1));
        boost::function<const string()> asyncFx = bind(getString_ptrs, pP1);
        return MyThreadPool::inst().post(asyncFx);
    }
}

/* Register the fixture with our test suite - follow the naming convention */
BOOST_FIXTURE_TEST_SUITE(JniTest, JniFixture);

/* Tests cases follow - as many as you like (the more the merrier) - follow the naming convention */
BOOST_AUTO_TEST_CASE(testJni) {
    JNI_START() {
        java::lang::String s1("Hello World!");
        BOOST_CHECK_EQUAL(s1.substring(6, 11), "World");
    } JNI_END()
}

BOOST_AUTO_TEST_CASE(testJniThrow) {
    JNI_START() {
        java::lang::String s1("!");
        try {
            s1.substring(2);
            BOOST_CHECK(false);
        } catch (java::lang::Exception& e) {
            BOOST_CHECK_NE(e.getMessage(), "");
        }
    } JNI_END()
}

BOOST_AUTO_TEST_CASE(testJniThreading) {
    JniThreadTest::FutureString future = JniThreadTest::runFuture("Hello World!");
    BOOST_CHECK(!future.is_ready());
    future.wait();
    BOOST_CHECK(future.is_ready());
    BOOST_CHECK_EQUAL(future.get(), "World");
}

/* Make sure you end the test suite last thing in the file. */
BOOST_AUTO_TEST_SUITE_END ();

#endif /* __APPLE__ && __i386__ */
