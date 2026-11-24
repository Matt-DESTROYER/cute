#include <file-io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../tools/tools.h"
#include "fetch.h"
#include "../error.h"

#include "test.h"

cute_error_t test(int argc, char* argv[]) {
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

	char* test_file = format("%s/test.c", project_root);
	if (!file_exists(test_file)) {
		free(test_file);
		free(project_root);

		return (cute_error_t){
			.status_code = CUTE_ERROR_INVALID_ARGUMENT,
			.message = "No test.c found."
		};
	}
	free(test_file);

	printf("Fetching dependencies...\n");

	char* cute_ini = format("%s/Cute.ini", project_root);
	cute_error_t fetch_error = fetch_packages_from_ini(cute_ini);
	free(cute_ini);
	if (fetch_error.status_code != CUTE_SUCCESS) {
		free(project_root);
		return fetch_error;
	}

	const char* config = release ? "Release" : "Debug";

	char* configure = format(
		"cmake -S \"%s\" -B \"%s/build\" -DCMAKE_BUILD_TYPE=%s -DBUILD_TESTING=ON",
		project_root, project_root, config
	);

	int exit_code = system(configure);
	free(configure);

	if (exit_code != 0) {
		free(project_root);
		return (cute_error_t){
			.status_code = CUTE_ERROR_BUILD,
			.message = "CMake configuration error!"
		};
	}

	char* build = format(
		"cmake --build \"$s/build\" --config %s",
		project_root, config
	);

	exit_code = system(build);
	free(build);

	if (exit_code != 0) {
		free(project_root);
		return (cute_error_t){
			.status_code = CUTE_ERROR_BUILD,
			.message = "Test build error!"
		};
	}

	char* run_tests = format(
		"cmake -E chdir \"%s/build\" ctest --output-on-failure",
		project_root
	);

	exit_code = system(run_tests);
	free(run_tests);
	free(project_root);

	if (exit_code != 0) {
		return (cute_error_t){
			.status_code = CUTE_ERROR_BUILD,
			.message = "Tests failed!"
		};
	}

	return (cute_error_t){
		.status_code = CUTE_SUCCESS,
		.message = "Tests passed!"
	};
}

