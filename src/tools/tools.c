#include "tools.h"
#include "file_io.h"

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

bool fetch_package(const char *repo_url, const char *package_name, const char* version) {
	char* package_folder = format("./.libraries/%s", package_name);
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
	if (command == NULL) {
		return false;
	}

	int exit_code = system(command);

	free(command);

	if (exit_code != 0) {
		return false;
	}
	return true;
}

