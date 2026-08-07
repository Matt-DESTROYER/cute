#ifndef TOOLS_H
#define TOOLS_H

#if defined(_WIN32) || defined(_WIN64)
	#define __WINDOWS 1
#else
	#define __WINDOWS 0
#endif

#if !__WINDOWS
	#define _POSIX_C_SOURCE 200112L
	#define _FILE_OFFSET_BITS 64
#endif

char* format(const char* template_string, ...);

#endif

