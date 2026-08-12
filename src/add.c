#include "tools.h"
#include "ini.h"

#include "add.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

add_package_result_t add_package(int argc, char **argv) {
	char* package_name = NULL;
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (package_name != NULL)
				return ADD_INVALID_ARGS;

			package_name = argv[i];
			continue;
		}
	}

	if (package_name == NULL)
		return ADD_NO_PACKAGE_NAME;

	char* package_url = format("https://github.com/%s.git", package_name);

	bool res = fetch_package(package_url, package_name, NULL);

	free(package_url);

	if (!res) {
		return ADD_FETCH_FAILED;
	}

	ini_t* ini = ini_read("./Config.ini");

	ini_add_kv_pair(ini, "dependencies", package_name, "");

	ini_cleanup(ini);

	return ADD_SUCCESS;
}

