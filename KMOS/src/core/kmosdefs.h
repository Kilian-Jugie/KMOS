#ifndef _KMOSDEFS_H_GUARD
#define _KMOSDEFS_H_GUARD

#if defined(_WIN32) || defined(__linux__)
#error This project is create to and only to KMOS and need not to be distribued on another OS or compiled with another OS as target
#endif

#define KMOS

#define KMOS_FORCE_UTF8

#if defined(KMOS_USE_UNICODE) && !defined(KMOS_FORCE_UTF8)
#define UNICODE
#define _UNICODE
using char_t wchar_t
#define _T(x) L##x
#else
using char_t = char;
#define _T(x) x
#endif // KMOS_USE_UNICODE

#define STR(x) #x
#define LSTR(x) L##(STR(x))

#ifndef NULL
#define NULL 0LL
#endif // !NULL

//using uint32_t = uint32_t;




#endif