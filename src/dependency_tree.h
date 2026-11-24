#ifndef DEPENDENCY_TREE_H
#define DEPENDENCY_TREE_H

#include "tools/list.h"
#include "version.h"

#include <stddef.h>

typedef struct dependency_tree {
    const char* package;
    version_t version;
    list_t versions;
    list_t dependants;
    bool resolved;

} dependency_tree_t;

#endif
