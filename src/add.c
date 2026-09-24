#include <file-io.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tools/tools.h"
#include "ini/ini.h"
#include "error.h"

#include "add.h"

cute_error_t add_package(int argc, char **argv) {
	char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL)
		return (cute_error_t){
			.status_code = CUTE_ERROR_INVALID_ARGUMENT,
			.message = "Not within a Cute package."
		};

	char* package_name = NULL;
	char* package_version = NULL;

	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (package_name == NULL) {
				package_name = argv[i];
			} else if (package_version == NULL) {
				package_version = argv[i];
			} else {
			 	return (cute_error_t){
					.status_code = CUTE_ERROR_INVALID_ARGUMENT,
					.message = "Invalid argument supplied."
				};
			}

			continue;
		}
	}

	if (package_name == NULL)
		return (cute_error_t){
			.status_code = CUTE_ERROR_INVALID_ARGUMENT,
			.message = "No package name supplied."
		};

	char* package_url = format("https://github.com/%s.git", package_name);
	char* package_location = format("%s/.libraries/", project_root);

	bool res = fetch_package(package_url, package_name, package_location, package_version);

	free(package_location);
	free(package_url);

	if (!res)
		return (cute_error_t){
			.status_code = CUTE_ERROR_PACKAGE,
			.message = "Failed to fetch package."
		};

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

	return (cute_error_t){
		.status_code = CUTE_SUCCESS,
		.message = ""
	};
}

