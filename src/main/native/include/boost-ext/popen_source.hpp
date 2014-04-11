/**
 * A boost::iostreams source that handle popen
 */
#ifndef H_BOOST_EXT_POPEN_SOURCE
#define H_BOOST_EXT_POPEN_SOURCE

#include <stdio.h>
#include <string>
#include <stdexcept>
#include <iosfwd>

#include "boost/utility.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/iostreams/categories.hpp"
#include "boost/iostreams/device/file_descriptor.hpp"

namespace boost_ext { namespace iostreams {
    using namespace std;
    using namespace boost;
    using namespace boost::iostreams;

    namespace impl {

        /** A wrapper around our FILE handle and the underlying file_descriptor_source */
        class popen_wrapper : noncopyable {
        public:
            explicit popen_wrapper(const string& cmd) : m_stream(popen(cmd.c_str(), "r")), m_source(), m_status(-1) {
                if (m_stream) { m_source.open(fileno(m_stream), never_close_handle); }
            }
            ~popen_wrapper() { close(); }

            /** Passes through to the underlying m_source */
            streamsize read(char* s, streamsize n) { return m_source.read(s, n); }
            streampos seek(stream_offset off, BOOST_IOS::seekdir way) { return m_source.seek(off, way); }
            bool is_open() const { return m_stream && m_source.is_open(); }
            int return_status() {
                /* Close this process - which means we wait for it to finish */
                if (m_status == -1 && m_stream) { close(); }
                return m_status;
            }
            
        private:
            void close() {
                m_source.close();
                if (m_stream) { m_status = pclose(m_stream), m_stream = NULL; }
            }

        private:
            FILE*                   m_stream;
            file_descriptor_source  m_source;
            int                     m_status;
        };
    }

    /* A class which can be used to handle popen calls */
    class popen_source {
    public:
        typedef char      char_type;
        struct category : input_seekable, device_tag, closable_tag { };

        popen_source() : m_wrapped() {}
        popen_source(const string& cmd) : m_wrapped() { open(cmd); }
        ~popen_source() { close(); }

        void open(const string& cmd) { m_wrapped.reset(new impl::popen_wrapper(cmd)); }
        void close() { m_wrapped.reset(); }

        /** Passes through to the underlying m_source */
        streamsize read(char_type* s, streamsize n) {
            return m_wrapped ? m_wrapped->read(s, n) : -1;
        }
        streampos seek(stream_offset off, BOOST_IOS::seekdir way) {
            return m_wrapped ? m_wrapped->seek(off, way) : streampos(-1);
        }
        bool is_open() const {
            return m_wrapped && m_wrapped->is_open();
        }
        int return_status() const {
            return m_wrapped ? m_wrapped->return_status() : -1;
        }

    private:
        shared_ptr<impl::popen_wrapper> m_wrapped;
    };

}}

#endif /* H_BOOST_EXT_POPEN_SOURCE */
