#ifndef BIX_PUBLIC_H
#define BIX_PUBLIC_H

#define BIX_UNUSED(x) (void)x;

#ifdef BIX_STATIC_DEFINE
#  define BIX_PUBLIC
#  define BIX_NO_EXPORT
#else
#  ifndef BIX_PUBLIC
#    ifdef BIX_EXPORTS
/* We are building this library */
#      define BIX_PUBLIC __declspec(dllexport)
#    else
/* We are using this library */
#      define BIX_PUBLIC __declspec(dllimport)
#    endif
#  endif

#  ifndef BIX_NO_EXPORT
#    define BIX_NO_EXPORT 
#  endif
#endif

#ifndef BIX_DEPRECATED
#  define BIX_DEPRECATED __declspec(deprecated)
#endif

#ifndef BIX_DEPRECATED_EXPORT
#  define BIX_DEPRECATED_EXPORT BIX_PUBLIC BIX_DEPRECATED
#endif

#ifndef BIX_DEPRECATED_NO_EXPORT
#  define BIX_DEPRECATED_NO_EXPORT BIX_NO_EXPORT BIX_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef BIX_NO_DEPRECATED
#    define BIX_NO_DEPRECATED
#  endif
#endif


#endif /* BIX_PUBLIC_H */