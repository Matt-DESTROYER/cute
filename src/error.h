#ifndef CUTE_ERROR_H
#define CUTE_ERROR_H

typedef enum cute_status_code {
	CUTE_SUCCESS = 0,

	CUTE_ERROR_INVALID_ARGUMENT,
	CUTE_ERROR_OUT_OF_MEMORY,
	CUTE_ERROR_FILE,
	CUTE_ERROR_DIRECTORY,
	CUTE_ERROR_GIT,
	CUTE_ERROR_CONFIG,
	CUTE_ERROR_PACKAGE,
	CUTE_ERROR_BUILD
} cute_status_code_t;

typedef struct cute_error {
	cute_status_code_t status_code;
	const char* message;
} cute_error_t;

#endif
