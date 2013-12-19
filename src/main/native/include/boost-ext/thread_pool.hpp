/**
 * A simple implementation of a thread pool.
 */
#ifndef H_BOOST_EXT_THREAD_POOL
#define H_BOOST_EXT_THREAD_POOL

#include <string>
#include "boost/noncopyable.hpp"
#include "boost/make_shared.hpp"
#include "boost/asio.hpp"
#include "boost/thread.hpp"

#if !defined(BOOST_EXT_THREAD_POOL_DEFAULT_SIZE)
    #define BOOST_EXT_THREAD_POOL_DEFAULT_SIZE   5
#endif

#if defined(BOOST_EXT_THREAD_POOL_NO_LOGGING)
    #include "boost/test/utils/nullstream.hpp"
    namespace boost_ext { static boost::onullstream _thread_pool_nullstream; }
    #define BOOST_EXT_THREAD_POOL_LOG(lvl)  boost_ext::_thread_pool_nullstream
#else
    #include "boost/log/trivial.hpp"
    #define BOOST_EXT_THREAD_POOL_LOG(lvl) BOOST_LOG_TRIVIAL(lvl)
#endif

namespace boost_ext {

template<typename T>
class thread_pool : public boost::noncopyable {

public:
    typedef boost::function<T()>    fx_t;
    thread_pool(std::string name = "", int numThreads = BOOST_EXT_THREAD_POOL_DEFAULT_SIZE) : m_name(name),
                                                                                              m_work(m_service) {
        if (name.size() > 0) { m_qualifier = " (" + name + ")"; }

        /* Create our threads */
        BOOST_EXT_THREAD_POOL_LOG(info) << "Creating " << numThreads << " threads" << m_qualifier;
        for (int i = 0; i < numThreads; i++) {
            m_threads.create_thread(boost::bind(&boost::asio::io_service::run, &m_service));
        }
    }
    ~thread_pool() {
        /* Clean up and wait for our threads */
        BOOST_EXT_THREAD_POOL_LOG(info) << "Cleaning up threads" << m_qualifier << "...";
        m_service.stop();
        m_threads.join_all();
        BOOST_EXT_THREAD_POOL_LOG(debug) << "Done cleaning up threads" << m_qualifier;
    }

    /** Posts work to the pool, and returns the associated future */
    boost::future<T> post(fx_t& asyncFx) {
        BOOST_EXT_THREAD_POOL_LOG(trace) << "Posting task" << m_qualifier;

        /* Create a packaged task */
        typedef boost::packaged_task<T>   task_t;
        boost::shared_ptr<task_t>         task = boost::make_shared<task_t>(asyncFx);
        boost::future<T>                  future = task->get_future();

        /* Post to our service and return the future */
        m_service.post(boost::bind(&task_t::operator(), task));
        return boost::move(future);
    }

private:
    std::string                     m_name;
    std::string                     m_qualifier;
    boost::asio::io_service         m_service;
    boost::thread_group             m_threads;
    boost::asio::io_service::work   m_work;
};

/** Creates a singleton thread pool instance that is lazily initialized and will be cleaned up on exit */
#define BOOST_EXT_THREAD_POOL_WITH_SIZE(n, t, s)                      \
struct n : public boost::noncopyable {                                \
    typedef boost::function<t()>    fx_t;                             \
    static boost_ext::thread_pool<t>& inst() {                        \
        static boost_ext::thread_pool<t> me(BOOST_STRINGIZE(n), s);   \
        return me;                                                    \
    }                                                                 \
}

#define BOOST_EXT_THREAD_POOL(n, t) BOOST_EXT_THREAD_POOL_WITH_SIZE(n, t, BOOST_EXT_THREAD_POOL_DEFAULT_SIZE)

}

#endif /* H_BOOST_EXT_THREAD_POOL */
