/*
 * Unit test for scheduler
 */

#include "boost-ext/test/unit_test.hpp"
#include "boost-ext/thread_pool.hpp"

 using namespace std;
 using namespace boost;
 using namespace boost_ext;

 /* Create setup and teardown functions */
 struct SchedulerFixture {
   SchedulerFixture() {
     /* Common setup before test cases here */
   }

   ~SchedulerFixture() {
     /* Common tear down after test cases here. */
   }
 };

BOOST_FIXTURE_TEST_SUITE(SchedulerTest, SchedulerFixture);
BOOST_EXT_THREAD_POOL_WITH_SIZE(MyThreadPool, 2);

struct SchedulerExecutor {
    unsigned int counter;

    SchedulerExecutor() : counter(0) { }

    void operator() (const boost::system::error_code& error) {
        if (!error) {
            counter++;
        } else {
            std::cout << "Boost error: " << error << std::endl;
        }
    }
};

BOOST_AUTO_TEST_CASE(testScheduler) {
    int myVar = 0;
    SchedulerExecutor exInst;
    BOOST_CHECK_EQUAL(exInst.counter, 0);
    MyThreadPool::inst().schedule(exInst, boost::chrono::milliseconds(500));
    BOOST_CHECK_EQUAL(exInst.counter, 0);
    this_thread::sleep(posix_time::seconds(1));
    BOOST_CHECK_EQUAL(exInst.counter, 1);
}

BOOST_AUTO_TEST_SUITE_END ();
