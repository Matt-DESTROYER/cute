#ifndef FILE_IO_H
#define FILE_IO_H

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
#include <stdio.h>
#include <stdlib.h>

typedef FILE* file_t;

typedef enum FileOptions {
	READ,
	READ_BINARY,
	READ_WRITE,
	READ_WRITE_BINARY,
	WRITE,
	WRITE_BINARY,
	WRITE_READ,
	WRITE_READ_BINARY,
	APPEND,
	APPEND_BINARY,
	APPEND_READ,
	APPEND_READ_BINARY
} FileOptions_t;

bool file_exists(const char* file);
bool directory_exists(const char* file);
bool path_exists(const char* path);

file_t file_open(const char* file, FileOptions_t options);
void file_close(file_t file);

size_t file_read(file_t file, char** out);
void file_write(file_t file, const char* buffer, size_t size);

#endif

