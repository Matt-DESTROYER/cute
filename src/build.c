#include <file-io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tools/tools.h"
#include "fetch.h"
#include "error.h"

#include "build.h"

#define RELEASE "Release"
#define DEBUG   "Debug"

cute_error_t build(int argc, char* argv[]) {
	bool release = false;
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "--release") == 0) {
			release = true;
			break;
		}
	}

	char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL)
		return (cute_error_t){
			.status_code = CUTE_ERROR_INVALID_ARGUMENT,
			.message = "Not within a Cute package."
		};

	printf("Fetching dependencies...\n");
	char* cute_ini = format("%s/Cute.ini", project_root);
	fetch_packages_from_ini(cute_ini);
	free(cute_ini);

	int exit_code;

	char* prepare = format("cmake -S \"%s\" -B \"%s/build\" -DCMAKE_BUILD_TYPE=%s",
		project_root, project_root, release ? RELEASE : DEBUG);
	exit_code = system(prepare);
	free(prepare);
	if (exit_code != 0) {
		free(project_root);
		return (cute_error_t){
			.status_code = CUTE_ERROR_BUILD,
			.message = "CMake build error!"
		};
	}

	char* build = format("cmake --build \"%s/build\" --config %s",
		project_root, release ? RELEASE : DEBUG);
	exit_code = system(build);
	free(build);
	free(project_root);
	if (exit_code != 0)
		return (cute_error_t){
			.status_code = CUTE_ERROR_BUILD,
			.message = "CMake build error!"
		};

	return (cute_error_t){
		.status_code = CUTE_SUCCESS,
		.message = ""
	};
}

