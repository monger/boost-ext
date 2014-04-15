/*
 * Unit test for scheduler
 */

#include "boost-ext/test/unit_test.hpp"
#include "boost-ext/thread_pool.hpp"
#include "boost-ext/log.hpp"

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

/* the test scheduled_task we will schedule */
class SchedulerExecutor : public scheduled_task {
public:
    SchedulerExecutor() : counter(0) { }

    ~SchedulerExecutor() { }

    void operator()() {
        counter++;
    }
    void on_error(const boost::system::error_code& error) {
        LOG(error) << "got error code: " << error;
    }

    unsigned int counter;
};

/* ensure that scheduled tasks are executed asynchronously */
BOOST_AUTO_TEST_CASE(testScheduler) {
    boost::shared_ptr<SchedulerExecutor> ex(new SchedulerExecutor());
    BOOST_CHECK_EQUAL(ex->counter, 0);
    MyThreadPool::inst().schedule(ex, boost::chrono::milliseconds(500));
    BOOST_CHECK_EQUAL(ex->counter, 0);
    this_thread::sleep(posix_time::seconds(1));
    BOOST_CHECK_EQUAL(ex->counter, 1);
}

BOOST_AUTO_TEST_SUITE_END ();
