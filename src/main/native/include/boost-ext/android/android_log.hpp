/**
 * Android-specific logging backend (redirects to logcat)
 */
#ifndef H_BOOST_EXT_ANDROID_LOG
#define H_BOOST_EXT_ANDROID_LOG

#include <android/log.h>
#include "boost/log/sinks/sync_frontend.hpp"
#include "boost/log/sinks/basic_sink_backend.hpp"

namespace boost_ext { namespace log {
    using namespace std;
    using namespace boost;
    using namespace boost::log;
    namespace sinks = boost::log::sinks;
    
    inline int android_priority(trivial::severity_level lvl) {
        switch(lvl) {
            case trivial::trace:    return ANDROID_LOG_VERBOSE;
            case trivial::debug:    return ANDROID_LOG_DEBUG;
            case trivial::info:     return ANDROID_LOG_INFO;
            case trivial::warning:  return ANDROID_LOG_WARN;
            case trivial::error:    return ANDROID_LOG_ERROR;
            case trivial::fatal:    return ANDROID_LOG_FATAL;
            default:                return ANDROID_LOG_UNKNOWN;
        }
    }
    
    class android_backend : public sinks::basic_formatted_sink_backend< char > {
        typedef sinks::basic_formatted_sink_backend< char > base_type;

    public:
        android_backend(const string& tag) : m_tag(tag) {}
        ~android_backend() {}

        typedef base_type::char_type char_type;
        typedef base_type::string_type string_type;
        void consume(const record_view& rec, const string_type& formatted_message) {
            __android_log_write(android_priority(rec[trivial::severity].get()),
                                m_tag.c_str(),
                                formatted_message.c_str());
        }

    private:
        string  m_tag;
    };

    /** This function adds the android logger */
    inline shared_ptr< sinks::synchronous_sink<android_backend> > add_android_log(const string& tag) {
        shared_ptr<android_backend> pBackend(new android_backend(tag));

        typedef sinks::synchronous_sink< android_backend> sink_t;
        shared_ptr<sink_t> pSink = make_shared<sink_t>(pBackend);

        core::get()->add_sink(pSink);
        return pSink;
    }
    
}}

#endif /* H_BOOST_EXT_ANDROID_LOG */
