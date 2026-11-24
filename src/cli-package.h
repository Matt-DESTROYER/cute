#ifndef CLI_PACKAGE_H
#define CLI_PACKAGE_H

#include <stddef.h>

#define VERSION_SEPARATOR '@'

typedef struct cli_package {
	const char* name;
	const char* version;
	size_t name_length;
	size_t version_length;
} cli_package_t;

cli_package_t cli_extract_package_details(const char* unnamed_arg);

#endif
