#ifndef NEW_H
#define NEW_H

typedef enum new_project_result {
    PROJECT_SUCCESS,
    PROJECT_INVALID_ARGS,
    PROJECT_NO_NAME,
    PROJECT_DIR_ALREADY_EXISTS
} new_project_result_t;

new_project_result_t new_project(int argc, char* argv[]);

#endif

