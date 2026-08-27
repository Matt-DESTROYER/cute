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

#include <stdbool.h>
#include <stdlib.h>

char* bounded_strdup(const char* string, size_t start, size_t end);

char* format(const char* template_string, ...);

void flatten_directory_name(char* buffer);

bool fetch_package(const char* repo_url, const char* package_name, const char* version);

#endif

