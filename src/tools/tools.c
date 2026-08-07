#include "tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

char* format(const char* template_string, ...) {
	if (template_string == NULL)
		return NULL;

	va_list args;

	va_start(args, template_string);
	int length = vsnprintf(NULL, 0, template_string, args);
	va_end(args);

	if (length < 1)
		return NULL;

	char* buffer = (char*)malloc(sizeof(char) * (length + 1));
	if (buffer == NULL)
		return NULL;

	va_start(args, template_string);
	int written = vsnprintf(buffer, length + 1, template_string, args);
	va_end(args);

	if (written != length) {
		free(buffer);
		return NULL;
	}

	return buffer;
}

