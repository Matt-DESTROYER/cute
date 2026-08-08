#ifndef ADD_H
#define ADD_H

typedef enum add_package_result {
    ADD_SUCCESS,
    ADD_INVALID_ARGS,
    ADD_NO_PACKAGE_NAME,
    ADD_FETCH_FAILED
} add_package_result_t;

add_package_result_t add_package(int argc, char* argv[]);

#endif

