#include "network.h"

#include "new.h"
#include "add.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("No arguments supplied... doing nothing!\n");
		return EXIT_SUCCESS;
	}

	network_init();
	
	if (strcmp(argv[1], "new") == 0) {
		/*new_project_result_t res = */new_project(argc, argv);
	} else if ((strcmp(argv[1], "add") == 0) ||
			(strcmp(argv[1], "install") == 0)) {
		/*add_package_result_t res = */add_package(argc, argv);
	} else if ((strcmp(argv[1], "remove") == 0) ||
			(strcmp(argv[1], "uninstall") == 0)) {
		printf("Not yet implemented...");
	} else {
		printf("Unknown argument supplied... doing nothing!\n");
	}

	network_cleanup();

	return EXIT_SUCCESS;
}

