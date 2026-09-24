#include <file-io.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "tools/tools.h"
#include "ini/ini.h"
#include "error.h"

#include "fetch.h"

char* installed_package_version(const char* package_name) {
	(void)package_name;
	return NULL;
}

cute_error_t fetch_packages_from_ini(const char* ini_path) {
	ini_t* ini;

	char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL) {
		return (cute_error_t){
			.status_code = CUTE_ERROR_INVALID_ARGUMENT,
			.message = "Not within a Cute package."
		};
	}

	char* libraries_dir = format("%s/.libraries", project_root);

	if (ini_path == NULL) {
		char* cute_path = format("%s/Cute.ini", project_root);
		ini = ini_read(cute_path);
		free(cute_path);
	} else {
		ini = ini_read(ini_path);
	}

	ini_table_t* deps_table = ini_get_table(ini, "dependencies");
	if (deps_table == NULL) {
		free(libraries_dir);
		free(project_root);

		ini_cleanup(ini);

		return (cute_error_t){
			.status_code = CUTE_SUCCESS,
			.message = ""
		};
	}

	ini_table_iter_t deps_iter = ini_table_to_iter(deps_table);

	while (deps_iter != NULL) {
		char* package_name = ini_table_item_key(deps_iter);

		char* package_dir = bounded_strdup(package_name, 0, strlen(package_name));
		flatten_directory_name(package_dir);

		char* cute_ini_location = format("%s/%s/Cute.ini", libraries_dir, package_dir);
		free(package_dir);

		bool cute_ini_exists = file_exists(cute_ini_location);
		free(cute_ini_location);

		if (!cute_ini_exists) {
			printf("Installing %s...\n", package_name);
			char* repo_url = format("https://github.com/%s.git", package_name);
			fetch_package(repo_url, package_name, libraries_dir, NULL);
			free(repo_url);
		}

		deps_iter = ini_table_next(deps_iter);
	}

	free(libraries_dir);
	free(project_root);

	ini_cleanup(ini);

	return (cute_error_t){
		.status_code = CUTE_SUCCESS,
		.message = ""
	};
}
