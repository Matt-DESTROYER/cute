#include <file-io.h>
#include <stddef.h>

#include "fetch.h"

#include "tools/tools.h"
#include "ini/ini.h"

void fetch_packages_from_ini(const char* ini_path) {
	ini_t* ini;
	if (ini_path == NULL) {
		char* root = file_root_by_file("Cute.ini");
		char* cute_path = format("%s/Cute.ini", root);

		ini = ini_read(ini_path);

		free(cute_path);
		free(root);
	} else {
		ini = ini_read(ini_path);
	}

	ini_table_t* dependencies_table = ini_get_table(ini, "dependencies");
	if (dependencies_table == NULL) {
		ini_cleanup(ini);
		return;
	}

	ini_table_iter_t deps_iter = ini_table_first(dependencies_table);

	while (deps_iter != NULL) {
		// TODO: check if dep is installed
		// if not: install
		// if so:
		//   check dep version
		//   if correct or not specified:
		//     do nothing
		//   if not:
		//     error

		deps_iter = ini_table_next(deps_iter);
	}

	ini_cleanup(ini);
}
