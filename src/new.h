#ifndef NEW_H
#define NEW_H

typedef enum new_project_result {
    SUCCESS,
    INVALID_ARGS
} new_project_result_t;

new_project_result_t new_project(int argc, char* argv[]);

#endif

