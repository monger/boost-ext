/**
 * Helper macros (unused, quote, and the NOOP function)
 */
#ifndef H_BOOST_EXT_COMMON
#define H_BOOST_EXT_COMMON

/* UNUSED macro (for parameters and return values) and NOOP functions */
#if !defined(UNUSED)
	#define UNUSED(x) (void)(x)
	__inline void NOOP(...){}
#endif

/* QUOTE macros */
#if !defined(QUOTE)
	#define QUOTE_(x)	#x
	#define QUOTE(x)	QUOTE_(x)

    /* Wide version - don't use this unless needed */
	#define WQUOTE_(x)	L ## x
	#define WQUOTE(x)	WQUOTE_(x)
#endif

#endif /* H_BOOST_EXT_COMMON */
