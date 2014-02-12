/**
 * Caching macros
 */
#ifndef H_BOOST_EXT_CACHED
#define H_BOOST_EXT_CACHED

#define _CACHED_FX_EMPTY
#define _CACHED_FX_STATIC static

/* Calls a function and caches it in a static value (which is returned, if it is already set) */
#define CACHED_FX(t, fx)            CACHED_FX_IMPL(_CACHED_FX_EMPTY, t, fx)
#define CACHED_STATIC_FX(t, fx)     CACHED_FX_IMPL(_CACHED_FX_STATIC, t, fx)

/* Macro which can be used in a header file to define a cached function */
#define CACHED_FX_DEF(t, fx)        const t& fx()

/* Use this macro if you want to reference the load function directly */
#define CACHED_LOAD_FX_NAME(fx)     fx ## _load

/* Used internally for forward defining the load function */
#define CACHED_LOAD_FX_DEF(t, fx)   static t CACHED_LOAD_FX_NAME(fx)()
/* Macro with will work with either the static or non-static version */
#define CACHED_FX_IMPL(s, t, fx)                                \
    CACHED_LOAD_FX_DEF(t, fx);                                  \
    s CACHED_FX_DEF(t, fx) {                                    \
        static t v; static bool l;                              \
        if (!l) { v = CACHED_LOAD_FX_NAME(fx)(), l = true; }    \
        return v;                                               \
    }                                                           \
    CACHED_LOAD_FX_DEF(t, fx)
            
#endif /* H_BOOST_EXT_CACHED */
