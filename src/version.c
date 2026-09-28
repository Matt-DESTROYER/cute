#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "version.h"

int buffer_to_int(const char* buffer, size_t length) {
	int num = 0;
	for (size_t i = 0; i < length; i++) {
		num = num * 10 + (int)(buffer[i] - '0');
	}
	return num;
}

version_parse_status_t parse_version(const char* string, version_t* version) {
	version->type = VERSION_RELEASE;
	version->numbers = (int*)malloc(sizeof(int) * 3);
	version->length = 3;

	if (version->numbers == NULL) {
		version->length = 0;
		return VERSION_MEMORY_ERROR;
	}

	size_t numbers_idx = 0;

	size_t i = 0;
	size_t buffer_size = 5;
	size_t buffer_idx = 0;
	char* buffer = (char*)malloc(sizeof(char) * buffer_size);
	if (buffer == NULL) {
		free(version->numbers);
		version->numbers = NULL;
		version->length = 0;
		return VERSION_MEMORY_ERROR;
	}

	while (string[i] != '\0') {
		if (isdigit((unsigned char)string[i])) {
			if (buffer_idx == buffer_size) {
				buffer_size *= 2;
				char* new_buffer = (char*)realloc(buffer, sizeof(char) * buffer_size);
				// NOTE: alternatively could just continue here,
				// drop the chars and continue gracefull-ish-ly
				if (new_buffer == NULL) {
					free(buffer);
					free(version->numbers);
					version->numbers = NULL;
					version->length = 0;
					return VERSION_MEMORY_ERROR;
				}
				buffer = new_buffer;
			}

			buffer[buffer_idx] = string[i];
			buffer_idx++;

			i++;
			continue;
		}

		if (buffer_idx > 0) {
			if (numbers_idx >= version->length) {
				version->length *= 2;
				int* new_buffer = (int*)realloc(version->numbers, sizeof(int) * version->length);
				if (new_buffer == NULL) {
					free(buffer);
					free(version->numbers);
					version->numbers = NULL;
					version->length = 0;
					return VERSION_MEMORY_ERROR;
				}
				version->numbers = new_buffer;
			}
			int num = buffer_to_int(buffer, buffer_idx);
			buffer_idx = 0;
			version->numbers[numbers_idx] = num;
			numbers_idx++;
		}

		// TODO: better + more accurate handling of version types
		if (string[i] == 'a') {
			version->type = VERSION_ALPHA;
		} else if (string[i] == 'b') {
			version->type = VERSION_BETA;
		} else if (string[i] == 'r') {
			version->type = VERSION_CANDIDATE;
		}

		i++;
	}

	free(buffer);

	if (version->length != numbers_idx) {
		int* new_buffer = (int*)realloc(version->numbers, sizeof(int) * numbers_idx);
		if (new_buffer == NULL) {
			free(version->numbers);
			version->numbers = NULL;
			version->length = 0;
			return VERSION_MEMORY_ERROR;
		}
		version->numbers = new_buffer;
		version->length = numbers_idx;
	}

	return VERSION_VALID;
}

int version_cmp(const version_t* x, const version_t* y) {
	if (x->type != y->type)
		return x->type - y->type;

	size_t min_length = x->length < y->length ? x->length : y->length;
	for (size_t i = 0; i < min_length; i++) {
		if (x->numbers[i] != y->numbers[i])
			return x->numbers[i] - y->numbers[i];
	}

	if (x->length != y->length)
		return x->length - y->length;

	return 0;
}

int _version_cmp(const void* x, const void* y) {
	return version_cmp((const version_t*)x, (const version_t*)y);
}

void sort_versions(version_t* versions, size_t count) {
	qsort(versions, count, sizeof(version_t), _version_cmp);
}
