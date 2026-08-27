#include "tools.h"
#include "file_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

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

// (End not inclusive)
char* bounded_strdup(const char* string, size_t start, size_t end) {
	if (string == NULL || start == end)
		return NULL;

	int reverse = 0;
	if (start > end) {
		reverse = 1;

		size_t temp = start;
		start = end;
		end = temp;
	}

	size_t new_length = end - start;
	char* new_string = malloc(sizeof(char) * (new_length + 1));
	if (new_string == NULL)
		return NULL;

	memcpy(new_string, string + start, sizeof(char) * new_length);
	new_string[new_length] = '\0';

	if (reverse == 1) {
		for (size_t i = 0; i < new_length / 2; i++) {
			size_t end_idx = new_length - i - 1;
			char temp = new_string[i];
			new_string[i] = new_string[end_idx];
			new_string[end_idx] = temp;
		}
	}

	return new_string;
}

void flatten_directory_name(char* buffer) {
	size_t i = 0;
	while (buffer[i] != '\0') {
		if (buffer[i] == '/')
			buffer[i] = '_';

		i++;
	}
}

bool fetch_package(const char *repo_url, const char *package_name, const char* version) {
	char* _package_name = bounded_strdup(package_name, 0, strlen(package_name));
	if (_package_name == NULL) {
		return false;
	}
	flatten_directory_name(_package_name);
	char* package_folder = format("./.libraries/%s", _package_name);
	free(_package_name);
	if (package_folder == NULL)
		return false;

	if (directory_exists(package_folder)) {
		free(package_folder);
		return true;
	}

	char* command;
	if (version == NULL) {
		command = format("git clone --depth 1 -q \"%s\" \"%s\"", repo_url, package_folder);
	} else {
		command = format("git clone --depth 1 --branch v%s -q \"%s\" \"%s\"", repo_url, package_folder, version);
	}

	free(package_folder);
	if (command == NULL)
		return false;

	int exit_code = system(command);

	free(command);

	return exit_code == 0;
}

