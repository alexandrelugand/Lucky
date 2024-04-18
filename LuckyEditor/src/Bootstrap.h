#pragma once

#ifndef EXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define EXPORT
#   else
#     define EXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define EXPORT __attribute__ ((visibility("default")))
#   else
#     define EXPORT
#   endif
# endif
#endif

/* calling conventions for Windows */
#ifndef STDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define STDCALL __stdcall
# else
#   define STDCALL
# endif
#endif
