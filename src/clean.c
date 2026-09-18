#include <file-io.h>

#include "clean.h"

#include <stdlib.h>

void clean() {
    char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL) {
		printf("Not a Cute project...\n");
		return;
	}

    free(project_root);

    // TODO: add funcTions to remove directories recursively etc in file-io
    printf("Not yet implemented...\n");
}

