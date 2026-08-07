#ifndef NEW_H
#define NEW_H

#include <stdio.h>
#include <stdlib.h>

typedef enum new_project_result {
    SUCCESS,
    INVALID_ARGS,
    NO_PROJECT_NAME,
    PROJECT_DIR_ALREADY_EXISTS
} new_project_result_t;

new_project_result_t new_project(int argc, char* argv[]);

#endif

