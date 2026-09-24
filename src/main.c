#include <file-io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools/tools.h"
#include "error.h"
#include "new.h"
#include "add.h"
#include "build.h"
#include "fetch.h"
#include "clean.h"
#include "version.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("No arguments supplied... doing nothing!\n");
		return EXIT_SUCCESS;
	}

	// handle actual command
	cute_error_t err = (cute_error_t){
		.status_code = CUTE_SUCCESS,
		.message = ""
	};
	if (strcmp(argv[1], "version") == 0
			|| strcmp(argv[1], "--version") == 0) {
		printf("%s\n", VERSION);
	} else if (strcmp(argv[1], "new") == 0) {
		err = new_project(argc, argv);
	} else if (strcmp(argv[1], "add") == 0) {
		err = add_package(argc, argv);
	} else if (strcmp(argv[1], "remove") == 0) {
		// TODO
		printf("Not yet implemented...\n");
	} else if (strcmp(argv[1], "update") == 0) {
		// TODO
		printf("Note yet implemented\n");
	} else if (strcmp(argv[1], "clean") == 0) {
		err = clean();
	} else if (strcmp(argv[1], "build") == 0) {
		err = build(argc, argv);
	} else if (strcmp(argv[1], "fetch") == 0) {
		char* project_root = file_root_by_file("Cute.ini");
		if (project_root == NULL) {
			printf("Not a Cute project...\n");
			return EXIT_SUCCESS;
		}

		printf("Fetching dependencies...\n");
		char* cute_ini = format("%s/Cute.ini", project_root);
		err = fetch_packages_from_ini(cute_ini);
		free(cute_ini);
	} else {
		printf("Unknown argument supplied... doing nothing!\n");
	}

	if (err.status_code != CUTE_SUCCESS) {
		printf("Error: %s\n", err.message);

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
