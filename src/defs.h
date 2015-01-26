#ifndef DEFS_H
#define DEFS_H

/*
 * debug definitions
 */

#if !defined(_debug) && defined(DEBUG)
#	define _debug 1
#else
#	define _debug 0
#endif

#if !defined(_release) && defined(RELEASE)
#	define _release 1
#else
#	define _release 0
#endif

#if !_debug && !_release
#	define _test 1
#else
#	define _test 0
#endif

/* 
 * windows check 
 */ 
 
#if !defined(_windows_) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__))
#       define _windows_ 1 
#endif 

/*
 * library exportation definition
 */

#ifdef _windows_
#       define _export __declspec(dllexport)
#else
#       define _export __attribute__((visibility("default")))
#endif

/*
 * common headers
 */

#include <alloca.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>


/**
 * Variable argument list wrapper structure.
 *   @args: The internal argument list.
 */

struct arglist_t {
	va_list args;
};

#endif
