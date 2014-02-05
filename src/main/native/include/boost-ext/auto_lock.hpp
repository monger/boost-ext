/**
 * Automatic locking/unlocking typedefs
 */
#ifndef H_BOOST_EXT_AUTO_LOCK
#define H_BOOST_EXT_AUTO_LOCK

#include "boost/thread/mutex.hpp"
#include "boost/thread/shared_mutex.hpp"
#include "boost/thread/locks.hpp"

namespace boost_ext {

    /** Automatic mutex locking type */
    typedef boost::unique_lock<boost::mutex>                    auto_lock;

    /** Automatic mutex read lock type for a shared mutex */
    typedef boost::shared_lock<boost::shared_mutex>             auto_read_lock;

    /** Automatic mutex write lock type for a shared mutex */
    typedef boost::unique_lock<boost::shared_mutex>             auto_write_lock;

    /** Automatic mutex upgradable lock type for a shared mutex */
    typedef boost::upgrade_lock<boost::shared_mutex>            auto_upgrade_lock;
    /** Automatic mutex upgradable lock type for a shared mutex */
    typedef boost::upgrade_to_unique_lock<boost::shared_mutex>  auto_upgrade_unique_lock;
}

#endif /* H_BOOST_EXT_AUTO_LOCK */
