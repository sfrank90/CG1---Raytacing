#ifndef __COMMON_H__
#define __COMMON_H__   

/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/

#if defined(__WIN32__) || defined(_MSC_VER)
#   define PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__unix__)
#   define PLATFORM_LINUX
#elif defined(__APPLE__) || defined(MACOSX)
#   define PLATFORM_MACOSX // unsupported yet
#   error The application is not supported under "Apple MacOSX" yet!
#elif defined(__FreeBSD__)
#   define PLATFORM_FREEBSD // unsupported yet
#   error The application is not supported under "FreeBSD" yet!
#elif defined(__sparc__) || defined(__sun__)
#   define PLATFORM_SOLARIS // unsupported yet
#   error The application is not supported under "Sun Solaris" yet!
#else
#   error Unknown platform, no support!
#endif


#if defined(__GNUC__)
#   define COMPILER_GCC  // GNU Compiler Collection
#elif defined(_MSC_VER)
#   define COMPILER_VC   // VisualC++
#endif

#if defined(COMPILER_GCC)
// optimization here
#elif defined(COMPILER_VC) //Only on windows
// optimization here
#pragma warning( disable: 4251 ) 

#endif

namespace constants {
	static const double pi          = 3.14159265358979323846264338327950288;
	static const double two_pi      = pi*2.0;
	static const double half_pi     = pi/2.0;
	static const double quarter_pi  = pi/2.0;
	static const double inv_pi      = 1.0/pi;
	static const double sqrd_pi     = pi*pi;
	static const double infinity    = 10e+30;
	static const double epsilon     = 1.0e-4;
}


#endif
