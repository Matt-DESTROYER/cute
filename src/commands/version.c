#include <file-io.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <semver.h>

#include "../tools/tools.h"
#include "../ini/ini.h"
#include "../error.h"

#include "version.h"

cute_error_t check_version() {
	char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL) {
		return (cute_error_t){
			.status_code = CUTE_ERROR_INVALID_ARGUMENT,
			.message = "Not within a Cute package."
		};
	}

	char* cute_path = format("%s/Cute.ini", project_root);
	ini_t* ini = ini_read(cute_path);
	free(cute_path);

	ini_table_t* table = ini_get_table(ini, "project");
	char* project_version = ini_table_get(table, "version");
	if (project_root != NULL) {
		printf("v%s\n", project_version);
	}

	free(project_root);
	ini_cleanup(ini);

	return (cute_error_t){
		.status_code = CUTE_SUCCESS,
		.message = ""
	};
}
