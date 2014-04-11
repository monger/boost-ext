/**
 * Logging macros and definitions
 */
#ifndef H_BOOST_EXT_LOG
#define H_BOOST_EXT_LOG

#include "boost/noncopyable.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/support/date_time.hpp"
#include "boost/log/utility/setup/console.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"

#include "boost-ext/platform_detect.hpp"
#include "boost-ext/classes.hpp"

#if (_IS_OS_ANDROID_)
    /* Include our android-specific implementation */
    #include "boost-ext/android/android_log.hpp"
    #define SYSTEM_LOGGER   boost_ext::log::add_android_log
#endif

namespace boost_ext {

/** Call this to generate log messages - basically it is just BOOST_LOG_TRIVIAL for now */
#define LOG(lvl)                BOOST_LOG_TRIVIAL(lvl)

/** Returns the integer value of the given log level */
#define LOG_LEVEL(lvl)          boost::log::trivial::lvl

/** Call this to set the log level */
#define SET_LOG_LEVEL_VAL(i)    boost_ext::log::Logger::get().setLevel(i)
#define SET_LOG_LEVEL(lvl)      SET_LOG_LEVEL_VAL(LOG_LEVEL(lvl))

/** Call this macro to not log during the given calls */
#define LOG_DISABLE(S)  {                                                                                   \
                            boost_ext::log::AutoLogPause _autoPause;                                        \
                            S;                                                                              \
                        }

#define ENABLE_LOGGING(t, c, ...)   boost_ext::log::Logger::get().enable(t, c(__VA_ARGS__))
#define DISABLE_LOGGING(t)          boost_ext::log::Logger::get().disable(t)
                        
#define ENABLE_CONSOLE_LOGGING(...) ENABLE_LOGGING("console", boost_ext::log::add_console_log, __VA_ARGS__)
#define DISABLE_CONSOLE_LOGGING()   DISABLE_LOGGING("console")

#ifdef SYSTEM_LOGGER
    #define ENABLE_SYSTEM_LOGGING(...)  ENABLE_LOGGING("system", SYSTEM_LOGGER, __VA_ARGS__)
    #define DISABLE_SYSTEM_LOGGING()    DISABLE_LOGGING("system")
#else
    #define ENABLE_SYSTEM_LOGGING(...)
    #define DISABLE_SYSTEM_LOGGING()
#endif
                        
namespace log {
    using namespace std;
    using namespace boost;
    using namespace boost::log;
    namespace sinks = boost::log::sinks;
    
    /** A class which will pause logging on construction and resume it on destruction */
    struct AutoLogPause : noncopyable {
        AutoLogPause()  { core::get()->set_logging_enabled(false); }
        ~AutoLogPause() { core::get()->set_logging_enabled(true); }
    };
    
    class Logger : noncopyable {
    public:
        typedef shared_ptr<sinks::sink> sink_ptr;
        
        SINGLETON(Logger, get);
        
        void setLevel(int lvl) {
            core::get()->set_filter(boost::log::trivial::severity >= lvl);
        }
        
        void enable(const string& key, sink_ptr sink) {
            disable(key);
            m_sinks[key] = sink;
        }
        
        void disable(const string& key) {
            sink_ptr p = m_sinks[key];
            if (p) {
                core::get()->remove_sink(p);
                p.reset();
            }
        }

    private:
        Logger() : m_sinks() { boost::log::add_common_attributes(); }
        ~Logger() {}
        
        typedef map<string, sink_ptr>   sink_map;
        sink_map                        m_sinks;
    };


    /** This is our definition of a "standard" format */
    #define STD_LOG_FORMAT boost::log::keywords::format = (                                                         \
        boost::log::expressions::stream                                                                             \
            << "[" <<                                                                                               \
                boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp",                    \
                                                                                    "%Y-%m-%d %H:%M:%S.%f")         \
            << "] ["                                                                                                \
                << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") \
            << "] ["                                                                                                \
                << std::setw(7) << boost::log::trivial::severity                                                    \
            << "] " << boost::log::expressions::smessage                                                            \
    )
    #define STD_LOG_STREAM std::clog

    template<typename CharT, typename ArgsT> 
    Logger::sink_ptr add_console_log(basic_ostream<CharT>& strm, const ArgsT& args) {
        return boost::log::add_console_log(strm, args);
    }
    template<typename ArgsT> Logger::sink_ptr add_console_log(const ArgsT& args) {
        return boost::log::add_console_log(STD_LOG_STREAM, args);
    }
    template<typename CharT> Logger::sink_ptr add_console_log(basic_ostream<CharT>& strm) {
        return boost::log::add_console_log(strm, STD_LOG_FORMAT);
    }
    inline Logger::sink_ptr add_console_log() { 
        return boost::log::add_console_log(STD_LOG_STREAM, STD_LOG_FORMAT); 
    }

} /* namespace log */

} /* namespace boost_ext */


#endif /* H_BOOST_EXT_LOG */
