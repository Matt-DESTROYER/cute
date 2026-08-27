//#include "network.h"

#include "new.h"
#include "add.h"
#include "clean.h"
#include "build.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.1"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("No arguments supplied... doing nothing!\n");
		return EXIT_SUCCESS;
	}

	//network_init();

	// handle actual command
	if (strcmp(argv[1], "version") == 0) {
		printf("%s\n", VERSION);
	} else if (strcmp(argv[1], "new") == 0) {
		/*new_project_result_t res = */new_project(argc, argv);
	} else if ((strcmp(argv[1], "add") == 0) ||
			(strcmp(argv[1], "install") == 0)) {
		/*add_package_result_t res = */add_package(argc, argv);
	} else if ((strcmp(argv[1], "remove") == 0) ||
			(strcmp(argv[1], "uninstall") == 0)) {
		// TODO
		printf("Not yet implemented...\n");
	} else if (strcmp(argv[1], "update") == 0) {
		// TODO
		printf("Note yet implemented\n");
	} else if (strcmp(argv[1], "clean") == 0) {
		clean(argc, argv);
	} else if (strcmp(argv[1], "build") == 0) {
		build(argc, argv);
	} else {
		printf("Unknown argument supplied... doing nothing!\n");
	}

	//network_cleanup();

	return EXIT_SUCCESS;
}

