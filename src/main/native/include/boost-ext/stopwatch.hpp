/**
 * A timer class for tracking elapsed time
 */
#ifndef H_BOOST_EXT_STOPWATCH
#define H_BOOST_EXT_STOPWATCH

#include "boost/noncopyable.hpp"
#include "boost-ext/make_shared_static.hpp"
#include "boost-ext/ticker.hpp"
namespace boost_ext {
    class stopwatch {
    public:
        stopwatch(boost::shared_ptr<ticker> pTicker = make_shared_static(systemTicker()))
        : m_pTicker(pTicker) {}

        /** Starts this stopwatch */
        stopwatch& start() {
            if (m_isRunning) {
                //ERROR
            }
            m_isRunning = true;
            m_startTick = m_pTicker->read();
            return *this;
        }

        /** Stops this stopwatch */
        stopwatch& stop() {
            boost::chrono::nanoseconds tick = m_pTicker->read();
            if (!m_isRunning) {
                //ERROR
            }
            m_isRunning = false;
            m_elapsedTime += (tick - m_startTick);
            return *this;
        }

        /** Resets this stopwatch */
        stopwatch& reset() {
            m_elapsedTime = boost::chrono::nanoseconds(0);
            m_isRunning = false;
            return *this;
        }

        bool isRunning() { return m_isRunning; }
        boost::chrono::nanoseconds elapsed() {
            return m_isRunning ? ((m_pTicker->read() - m_startTick) + m_elapsedTime) : m_elapsedTime;
        }
    private:
        bool                        m_isRunning;
        boost::chrono::nanoseconds  m_startTick;
        boost::chrono::nanoseconds  m_elapsedTime;
        boost::shared_ptr<ticker>   m_pTicker;
    };
}

#endif /* H_BOOST_EXT_STOPWATCH */
