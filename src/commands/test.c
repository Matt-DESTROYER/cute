#include <file-io.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../tools/tools.h"
#include "../tools/list.h"
#include "fetch.h"
#include "../error.h"

#include "test.h"

bool test_name_eq(void* x, void* y) {
	return strcmp((char*)x, (char*)y) == 0;
}

cute_error_t test(int argc, char* argv[]) {
	bool release = false;
	list_t tests = list_new(test_name_eq);
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (!list_append(&tests, argv[i]))
				return (cute_error_t){
					.status_code = CUTE_ERROR_OUT_OF_MEMORY,
					.message = "Failed to append test case."
				};
		} else if (strcmp(argv[i], "--release") == 0) {
			release = true;
		} else {
			return (cute_error_t){
				.status_code = CUTE_ERROR_INVALID_FLAG,
				.message = "Invalid argument found."
			};
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
		"cmake --build \"%s/build\" --config %s",
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

	char* run_tests;
	if (tests.length == 0) {
		run_tests = format(
			"cmake -E chdir \"%s/build\" ctest --output-on-failure --no-tests=error",
			project_root
		);
	} else if (tests.length == 1) {
		run_tests = format(
			"cmake -E chdir \"%s/build\" ctest --output-on-failure --no-tests=error -R \"^(%s)$\"",
			project_root, (char*)tests.head->data
		);
	} else {
		size_t combined_length = 0;
		list_iter_t iter = list_iter(&tests);
		while (!list_iter_finished(&iter)) {
			combined_length += strlen((char*)list_iter_data(&iter));
			list_iter_next(&iter);
		}
	
		char* tests_buffer = (char*)malloc(sizeof(char) * (combined_length + tests.length));
		if (tests_buffer == NULL)
			return (cute_error_t){
				.status_code = CUTE_ERROR_OUT_OF_MEMORY,
				.message = "Failed to allocate memory to build test string."
			};
		iter = list_iter(&tests);
		size_t tests_buffer_idx = 0;
		while (!list_iter_finished(&iter)) {
			char* buff = (char*)list_iter_data(&iter);
			size_t i = 0;
			for (; buff[i] != '\0'; i++) {
				tests_buffer[tests_buffer_idx + i] = buff[i];
			}
			tests_buffer_idx += i;

			if (iter.current->next != NULL) {
				tests_buffer[tests_buffer_idx] = '|';
				tests_buffer_idx++;
			}

			list_iter_next(&iter);
		}
		tests_buffer[tests_buffer_idx] = '\0';

		run_tests = format(
			"cmake -E chdir \"%s/build\" ctest --output-on-failure --no-tests=error -R \"^(%s)$\"",
			project_root, tests_buffer
		);
		free(tests_buffer);
	}

	exit_code = system(run_tests);
	free(run_tests);
	free(project_root);

	list_free(&tests);

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

