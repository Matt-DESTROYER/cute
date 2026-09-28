#ifndef VERSION_H
#define VERSION_H

#define VERSION "v0.14.2"

#include <stddef.h>

typedef enum version_restriction {
	VERSION_MIN,
	VERSION_EQUALS,
	VERSION_MAX,
	VERSION_RANGE
} version_restriction_t;

typedef enum version_type {
	VERSION_RELEASE = 0,
	VERSION_CANDIDATE = 1,
	VERSION_BETA = 2,
	VERSION_ALPHA = 3
} version_type_t;

typedef enum version_parse_status {
	VERSION_VALID,
	VERSION_INVALID,
	VERSION_MEMORY_ERROR
} version_parse_status_t;

typedef struct version {
	version_type_t type;
	int* numbers;
	size_t length;
} version_t;

version_parse_status_t parse_version(const char* string, version_t* version);

int version_cmp(const version_t* x, const version_t* y);

void sort_versions(version_t* versions, size_t count);

#endif
