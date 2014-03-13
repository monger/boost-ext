/**
 * Automatic locking/unlocking typedefs
 */
#ifndef H_BOOST_EXT_SLEEP
#define H_BOOST_EXT_SLEEP

#include "boost/thread/thread_only.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

/** 
 * Sleeps for the given number (t) of units (u).  The appropriate prefix is added to the units, so just use, 
 * i.e. SLEEP(5, seconds) 
 */
#define SLEEP(t, u) boost::this_thread::sleep(boost::posix_time::u(t))

#endif /* H_BOOST_EXT_AUTO_LOCK */
