#include "tools.h"
#include "file_io.h"
#include "build.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RELEASE "Release"
#define DEBUG   "Debug"

void build(int argc, char* argv[]) {
	bool release = false;
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "--release") == 0) {
			release = true;
			break;
		}
	}

	char* project_root = file_root_by_file("Cute.ini");
	if (project_root == NULL) {
		printf("Not a Cute project...\n");
		return;
	}

	int exit_code;

	char* prepare = format("cmake -S \"%s\" -B \"%s/build\" -DCMAKE_BUILD_TYPE=%s",
		project_root, project_root, release ? RELEASE : DEBUG);
	exit_code = system(prepare);
	free(prepare);
	if (exit_code != 0) {
		free(project_root);
		return;
	}

	char* build = format("cmake --build \"%s/build\" --config %s",
		project_root, release ? RELEASE : DEBUG);
	exit_code = system(build);
	free(build);
	free(project_root);
	if (exit_code != 0)
		return;

	return;
}

