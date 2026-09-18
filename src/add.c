#include <file-io.h>

#include "tools/tools.h"
#include "ini/ini.h"

#include "add.h"

#include <stdlib.h>
#include <stdbool.h>

add_package_result_t add_package(int argc, char **argv) {
	char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL) {
		printf("Not a Cute project...\n");
		return ADD_NOT_CUTE_PROJECT;
	}

	char* package_name = NULL;
	char* package_version = NULL;

	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (package_name == NULL) {
				package_name = argv[i];
			} else if (package_version == NULL) {
				package_version = argv[i];
			} else {
			 	return ADD_INVALID_ARGS;
			}

			continue;
		}
	}

	if (package_name == NULL)
		return ADD_NO_PACKAGE_NAME;

	char* package_url = format("https://github.com/%s.git", package_name);
	char* package_location = format("%s/.libraries/", project_root);

	bool res = fetch_package(package_url, package_name, package_location, package_version);

	free(package_location);
	free(package_url);

	if (!res)
		return ADD_FETCH_FAILED;

	// TODO: properly locate root directory, we may be in a nested dir
	char* ini_path = format("%s/Cute.ini", project_root);
	ini_t* ini = ini_read(ini_path);
	free(project_root);
	free(ini_path);

	if (package_version == NULL) {
		ini_add_kv_pair(ini, "dependencies", package_name, "");
	} else {
		ini_add_kv_pair(ini, "dependencies", package_name, package_version);
	}

	ini_write(ini);

	ini_cleanup(ini);

	return ADD_SUCCESS;
}

