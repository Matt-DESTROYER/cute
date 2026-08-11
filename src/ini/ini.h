#ifndef INI_H
#define INI_H

#include <stdbool.h>

typedef struct ini ini_t;

bool ini_read(ini_t* ini, char* ini_path);
void ini_add_table(ini_t* ini, char* table);
void ini_remove_table(ini_t* ini, const char* table);
void ini_add_kv_pair(ini_t* ini, const char* table, char* key, char* value);
void ini_cleanup(ini_t* ini);
bool ini_write(ini_t* ini);

#endif

