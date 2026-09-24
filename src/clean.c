#include <file-io.h>

#include "clean.h"
#include "error.h"

#include <stdlib.h>

cute_error_t clean() {
    char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL)
		return (cute_error_t){
			.status_code = CUTE_ERROR_INVALID_ARGUMENT,
			.message = "Not within a Cute package."
		};

    free(project_root);

    // TODO: add funcTions to remove directories recursively etc in file-io
    printf("Not yet implemented...\n");

    return (cute_error_t){
		.status_code = CUTE_SUCCESS,
		.message = ""
	};
}

