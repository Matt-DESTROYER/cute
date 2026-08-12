#ifndef INI_H
#define INI_H

#include <stdbool.h>

typedef struct ini ini_t;

void ini_add_table(ini_t* ini, const char* table);
void ini_remove_table(ini_t* ini, const char* table);
void ini_add_kv_pair(ini_t* ini, const char* table, const char* key, const char* value);
ini_t* ini_read(const char* ini_path);
void ini_cleanup(ini_t* ini);
bool ini_write(ini_t* ini);

#endif

