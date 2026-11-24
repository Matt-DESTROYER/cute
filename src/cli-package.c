#include "cli-package.h"
#include "tools/tools.h"

#include <stddef.h>

cli_package_t cli_extract_package_details(const char* unnamed_arg) {
	cli_package_t package = (cli_package_t) {
		.name = unnamed_arg,
		.version = NULL,
		.name_length = 0,
		.version_length = 0
	};

	if (unnamed_arg == NULL)
		return package;

	size_t i = slice_until(unnamed_arg, VERSION_SEPARATOR);
	package.name_length = i;

	if (unnamed_arg[i] == '\0')
		return package;

	i++; // skip VERSION_SEPARATOR

	if (unnamed_arg[i] == 'v')
		i++;

	package.version = (const char*)(unnamed_arg + i);
	package.version_length = slice_until(package.version, '\0');

	if (package.version_length == 0)
		package.version = NULL;

	return package;
}
