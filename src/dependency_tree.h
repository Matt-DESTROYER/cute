#ifndef DEPENDENCY_TREE_H
#define DEPENDENCY_TREE_H

#include <semver.h>

#include "tools/list.h"

#include <stddef.h>

typedef struct dependency_tree_node {
	const char* package;
	cute_version_t version;

	list_t available_versions;
	list_t dependants;

	bool resolved;
} dependency_tree_node_t;

typedef struct dependency_tree {
	list_t packages;
} dependency_tree_t;

#endif
