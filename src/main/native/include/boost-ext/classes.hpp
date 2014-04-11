/**
 * Common class helper macros
 */
#ifndef H_BOOST_EXT_CLASSES
#define H_BOOST_EXT_CLASSES

#include <string>
#include <ostream>

/* 
 * For the GETTER, SETTER, and ACCESSOR macros, the M_* variant will automatically link funtcions called "var()" to
 * the member variable called "m_var".
 */

/* Place this macro to provide "getter" functionality */
#define GETTER(t, var, fx) t fx() const { return var; }
#define M_GETTER(t, fx) GETTER(t, m_ ## fx, fx)

/* Place this macro to provide "setter" functionality */
#define SETTER(cls, t, var, fx) cls& fx(t val) { var = val; return *this; }
#define M_SETTER(cls, t, fx) SETTER(cls, t, m_ ## fx, fx)

/* Place this macro to provide both getter and setter functionality */
#define ACCESSOR(cls, t, var, fx) GETTER(t, var, fx) SETTER(cls, t, var, fx)
#define M_ACCESSOR(cls, t, fx) ACCESSOR(cls, t, m_ ## fx, fx)

/* Place this macro to define a "friend" transformation for ostream redirection */
#define OSTR_OPER(cls, v, asStr)                                                \
        std::ostream& operator<< (std::ostream& _str, const cls& v) {           \
            _str << asStr; return _str;                                         \
        }

/* Place this macro to provide "toString"-like functionality */
#define TO_STRING(cls, fx)                                                      \
        operator const std::string() const { return fx(); }                     \
        friend OSTR_OPER(cls, o, ((std::string) o))

/* This macro behaves like above, but allows you to define the function inline */
#define TO_STRING_FX(cls)                                                       \
        TO_STRING(cls, __toString_ ## cls)                                      \
        const std::string __toString_ ## cls() const

/* Place this macro to provide a static "singleton" functionality */
#define SINGLETON(t, fx) static t& fx() { static t _fx; return _fx; }
/* This macro behaves as above, but passes arguments to the constructor */
#define SINGLETON_A(t, fx, ...) static t& fx() { static t _fx(__VA_ARGS__); return _fx; }
            
#endif /* H_BOOST_EXT_CLASSES */
