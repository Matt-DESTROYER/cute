#ifndef FILE_IO_H
#define FILE_IO_H

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

bool directory_create(const char* path);

file_t file_open(const char* file, FileOptions_t options);
void file_close(file_t file);

size_t file_read(file_t file, char** out);
void file_write(file_t file, const char* buffer, size_t size);

#endif

