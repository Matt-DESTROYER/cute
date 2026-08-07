#include "tools.h"
#include "file_io.h"
#include "new.h"

const char* CUTE_INI = "; cute package manager settings\n\
[cute]\n\
version = 0\n\
\n\
; your project settings\n\
[project]\n\
name = \"%s\"\n\
version = \"0.1.0\"\n\
\n\
; your project dependencies\n\
[dependencies]\n\
";

const char* MAIN_C = "#include <stdlib.h>\n\
#include <stdio.h>\n\
\n\
int main(int argc, char* argv[]) {\n\
	printf(\"Hello, world!\\n\");\n\
\n\
	return EXIT_SUCCESS;\n\
}\n\
";

new_project_result_t new_project(int argc, char* argv[]) {
	char* project_name = NULL;
	for (int i = 2; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (project_name != NULL)
				return INVALID_ARGS;

			project_name = argv[i];
			continue;
		}
	}

	if (project_name == NULL)
		return NO_PROJECT_NAME;

	char* directory = format("./%s", project_name);
	char* ini_path = format("%s/Cute.ini", directory);
	char* lock_path = format("%s/Cute.lock", directory);

	char* src_dir = format("%s/src", directory);
	char* main_path = format("%s/main.c", src_dir);

	char* include_dir = format("%s/.includes", directory);
	char* libraries_dir = format("%s/.libraries", directory);

	if (directory_exists(project_name)) {
		return PROJECT_DIR_ALREADY_EXISTS;
	}

	directory_create(project_name);

	file_t ini_file = file_open(ini_path, WRITE_BINARY);
	fprintf(ini_file, CUTE_INI, project_name);
	file_close(ini_file);

	file_t lock_file = file_open(lock_path, WRITE_BINARY);
	file_close(lock_file);

	directory_create(src_dir);

	file_t main_file = file_open(main_path, WRITE_BINARY);
	fprintf(main_file, MAIN_C);
	file_close(main_file);

	free(directory);
	free(ini_path);
	free(lock_path);
	free(src_dir);
	free(main_path);
	free(include_dir);
	free(libraries_dir);

	return SUCCESS;
}

