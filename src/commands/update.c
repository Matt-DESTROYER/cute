#include <file-io.h>
#include <stdbool.h>
#include <string.h>

#include "../tools/list.h"
#include "../error.h"
#include "../cli-package.h"

#include "update.h"

bool package_name_match(void* x, void* y) {
	char* package_x = (char*)x;
	char* package_y = (char*)y;
	return strcmp(package_x, package_y) == 0;
}

cute_error_t update_packages(int argc, char **argv) {
	char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL)
		return (cute_error_t){
			.status_code = CUTE_ERROR_INVALID_ARGUMENT,
			.message = "Not within a Cute package."
		};

	list_t packages = list_new(package_name_match);

	for (int i = 2; i < argc; i++) {
		if (argv[i][0] == '-') {
			return (cute_error_t){
				.status_code = CUTE_ERROR_INVALID_FLAG,
				.message = "Invalid flag supplied"
			};
		}

		list_append(&packages, (void*)argv[i]);
	}

	if (packages.length == 0) {
		printf("TODO: update all packages to maximum compatible versions");

		return (cute_error_t){
			.status_code = CUTE_SUCCESS,
			.message = ""
		};
	}

	printf("TODO: update selected packages to maximum compatible versions");

	list_free(&packages);

	return (cute_error_t){
		.status_code = CUTE_SUCCESS,
		.message = ""
	};
}
