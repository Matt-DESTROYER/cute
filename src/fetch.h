#ifndef CUTE_FETCH_H
#define CUTE_FETCH_H

#include "error.h"

char* installed_package_version(const char* package_name);
cute_error_t fetch_packages_from_ini(const char* ini_path);

#endif
