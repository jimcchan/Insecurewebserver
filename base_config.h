//
// Created by philip metzger on 20.09.2020.
//

#ifndef INSECUREWEBSERVER_BASE_CONFIG_H
#define INSECUREWEBSERVER_BASE_CONFIG_H
#if defined(__linux__)
#define OS_LINUX 1
// Include a system header to pull in features.h for glibc/uclibc macros.
#include <unistd.h>
#if defined(__GLIBC__) && !defined(__UCLIBC__)
// We really are using glibc, not uClibc pretending to be glibc.
#define LIBC_GLIBC 1
#endif
#elif defined(_WIN32)
#define OS_WIN 1
#elif defined(__Fuchsia__)
#define OS_FUCHSIA 1
#elif defined(__FreeBSD__)
#define OS_FREEBSD 1
#elif defined(__NetBSD__)
#define OS_NETBSD 1
#elif defined(__OpenBSD__)
#define OS_OPENBSD 1
#elif defined(__sun)
#define OS_SOLARIS 1
#elif defined(__QNXNTO__)
#define OS_QNX 1
#elif defined(_AIX)
#define OS_AIX 1
#endif
#if defined(OS_FREEBSD) || defined(OS_NETBSD) || defined(OS_OPENBSD)
#define OS_BSD 1
#define UNIX_LIKE 1
#endif

// For access to standard POSIXish features, use OS_POSIX instead of a
// more specific macro.
#if defined(OS_AIX) || defined(OS_ANDROID) || defined(OS_ASMJS) ||      \
        defined(OS_FREEBSD) || defined(OS_IOS) || defined(OS_LINUX) ||  \
        defined(OS_CHROMEOS) || defined(OS_MAC) || defined(OS_NACL) ||  \
        defined(OS_NETBSD) || defined(OS_OPENBSD) || defined(OS_QNX) || \
        defined(OS_SOLARIS)
#define OS_POSIX 1
#endif

// Compiler detection. Note: clang masquerades as GCC on POSIX and as MSVC on
// Windows.
#if defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#elif defined(__clang__)
#define COMPILER_CLANG 1
#endif
#ifdef COMPILER_MSVC
#define WEBSERVER_INTERNAL static
#define WEBSERVER_PRINFLIKE(a, b)
#endif
#ifdef COMPILER_CLANG
#define WEBSERVER_INTERNAL __attribute__((visibility("hidden")))
#define WEBSERVER_PRINTFLIKE(a, b) __attribute__((format(printf,(a),(b)))
#endif
#ifdef COMPILER_GCC
#define WEBSERVER_INTERNAL __attribute__((visibility("hidden")))
#define WEBSERVER_PRINTFLIKE(a, b) __attribute__((format(printf, (a), (b))))
#endif
#ifndef NDEBUG
#include <assert.h>
#define assert_unreachable() assert(false)
#endif
#endif//INSECUREWEBSERVER_BASE_CONFIG_H
