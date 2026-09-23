#ifndef INI_H
#define INI_H

#include <stddef.h>
#include <stdbool.h>

typedef struct assoc_arr_item assoc_arr_item_t;

typedef struct assoc_arr {
	assoc_arr_item_t* head;
	assoc_arr_item_t* tail;

	size_t size;
} assoc_arr_t;

typedef assoc_arr_t ini_array_t;

typedef struct ini_table {
	char* name;

	assoc_arr_t table;
} ini_table_t;

typedef assoc_arr_item_t* ini_table_iter_t;
typedef struct ini ini_t;

void ini_add_table(ini_t* ini, const char* table);
void ini_remove_table(ini_t* ini, const char* table);

void ini_add_kv_pair(ini_t* ini, const char* table, const char* key, const char* value);

ini_table_t* ini_get_table(ini_t* ini, const char* table);
char* ini_table_get(ini_table_t* table, const char* key);

ini_table_iter_t ini_table_to_iter(ini_table_t* table);
ini_table_iter_t ini_table_first(ini_table_t* table);
ini_table_iter_t ini_table_last(ini_table_t* table);
ini_table_iter_t ini_table_next(ini_table_iter_t item);
ini_table_iter_t ini_table_prev(ini_table_iter_t item);
char* ini_table_item_key(ini_table_iter_t item);
char* ini_table_item_value(ini_table_iter_t item);

ini_t* ini_read(const char* ini_path);
void ini_cleanup(ini_t* ini);
bool ini_write(ini_t* ini);

#endif

