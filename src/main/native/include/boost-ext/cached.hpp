/**
 * Caching macros
 */
#ifndef H_BOOST_EXT_CACHED
#define H_BOOST_EXT_CACHED

/* Calls a function and caches it in a static value (which is returned, if it is already set) */
#define CACHED_FX(t, fx)                                        \
    static t fx ## _load();                                     \
    const t& fx() {                                             \
        static t v;                                             \
        static bool l;                                          \
        if (!l) { v = fx ## _load(), l = true; }                \
        return v;                                               \
    }                                                           \
    static t fx ## _load()

/* Macro which can be used in a header file to define a cached function */
#define CACHED_FX_DEF(t, fx) const t& fx()
            
            
#endif /* H_BOOST_EXT_CACHED */
