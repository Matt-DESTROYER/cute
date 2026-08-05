#include "new.h"

#include "file_io.h"

new_project_result_t new_project(int argc, char* argv[]) {
	char* project_name = NULL;
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (project_name != NULL)
				return INVALID_ARGS;

			project_name = argv[i];
			continue;
		}

		printf("...");
	}

	return SUCCESS;
}

