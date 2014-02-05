/**
 * Common exception class (for easy throwing)
 */
#ifndef H_BOOST_EXT_EXCEPTION
#define H_BOOST_EXT_EXCEPTION

#include "boost/throw_exception.hpp"
namespace boost_ext {
    struct exception : virtual boost::exception, virtual std::exception {
        exception(const std::string &message) : m_message(message) {}
        ~exception() throw() {}
        const char* what() const throw() { return m_message.c_str(); }
        std::string m_message;
    };
}

#endif /* H_BOOST_EXT_EXCEPTION */
