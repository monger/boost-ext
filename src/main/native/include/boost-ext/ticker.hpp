/**
 * Classes which can be used in a stopwatch for timing things
 */
#ifndef H_BOOST_EXT_TICKER
#define H_BOOST_EXT_TICKER

#include "boost/noncopyable.hpp"
#include "boost/chrono/duration.hpp"
#include "boost-ext/classes.hpp"

namespace boost_ext {

    /** This abstract class can be extended in order to create a customized ticker */
    class ticker : public boost::noncopyable {
    public:
        virtual boost::chrono::nanoseconds read() =0;
    };

    /** A timer that is based upon a system clock */
    template <typename Clock>
    class clock_ticker : public ticker {
    public:
        typedef clock_ticker<Clock> type_t;

        /* Constructor */
        clock_ticker() : m_start(Clock::now()) {}

        /* Implementation */
        boost::chrono::nanoseconds read() { return Clock::now() - m_start; }
    private:
        /* The time this clock was started */
        typename Clock::time_point  m_start;
    };

    /** Inline functions for getting the standard system ticker */
    template <typename Clock> inline SINGLETON(clock_ticker<Clock>, clockTicker)
    inline ticker& systemTicker() { return clockTicker<boost::chrono::high_resolution_clock>(); }
}

#endif /* H_BOOST_EXT_TICKER */
