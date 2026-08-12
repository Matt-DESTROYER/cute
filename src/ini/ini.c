#include "tools.h"
#include "file_io.h"
#include "ini.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct assoc_arr_item assoc_arr_item_t;
struct assoc_arr_item {
	char* key;
	char* value;

	assoc_arr_item_t* next;
	assoc_arr_item_t* prev;
};

typedef struct assoc_arr {
	assoc_arr_item_t* head;
	assoc_arr_item_t* tail;

	size_t size;
} assoc_arr_t;

typedef struct ini_table {
	char* name;

	assoc_arr_t table;
} ini_table_t;

typedef struct ini_table_item ini_table_item_t;
struct ini_table_item {
	ini_table_t table;

	ini_table_item_t* next;
	ini_table_item_t* prev;
};

typedef struct ini_table_arr {
	ini_table_item_t* head;
	ini_table_item_t* tail;

	size_t size;
} ini_table_arr_t;

// typedefed in header (this should be the only publicly exposed struct)
struct ini {
	char* file_path;

	bool succesfully_initialised;

	ini_table_arr_t table;
};

assoc_arr_t assoc_arr_new() {
	assoc_arr_t assoc_arr = {
		.head = NULL,
		.tail = NULL,
		.size = 0
	};
	return assoc_arr;
}

void assoc_arr_push(assoc_arr_t* assoc_arr, char* key, char* value) {
	assoc_arr_item_t* item = malloc(sizeof(assoc_arr_item_t));
	item->key = key;
	item->value = value;
	item->next = NULL;
	item->prev = NULL;

	if (assoc_arr->head == NULL) {
		assoc_arr->head = item;
		assoc_arr->tail = item;
		assoc_arr->size++;
		return;
	}

	assoc_arr->tail->next = item;
	item->prev = assoc_arr->tail;
	assoc_arr->tail = item;

	assoc_arr->size++;
}

void assoc_arr_pop(assoc_arr_t* assoc_arr) {
	if (assoc_arr->tail == NULL)
		return;

	assoc_arr_item_t* temp = assoc_arr->tail;

	assoc_arr->tail = assoc_arr->tail->prev;
	if (assoc_arr->tail == NULL) {
		assoc_arr->head = NULL;
	} else {
		assoc_arr->tail->next = NULL;
	}
	assoc_arr->size--;

	free(temp);
}

void assoc_arr_remove(assoc_arr_t* assoc_arr, const char* key) {
	if (assoc_arr->head == NULL)
		return;

	assoc_arr_item_t* current = assoc_arr->head;

	while (current != NULL &&
			strcmp(current->key, key) != 0) {
		current = current->next;
	}

	if (current == NULL)
		return;

	if (current->prev != NULL) {
		current->prev->next = current->next;
	} else {
		assoc_arr->head = current->next;
	}

	if (current->next != NULL) {
		current->next->prev = current->prev;
	} else {
		assoc_arr->tail = current->prev;
	}

	free(current);
}

char* assoc_arr_search(assoc_arr_t assoc_arr, const char* key) {
	if (assoc_arr.head == NULL)
		return NULL;

	assoc_arr_item_t* current = assoc_arr.head;

	while (current != NULL &&
			strcmp(current->key, key) != 0) {
		current = current->next;
	}

	if (current == NULL)
		return NULL;

	return current->value;
}

ini_table_t ini_table_new(char* name) {
	ini_table_t table = {
		.name = name,
		.table = assoc_arr_new()
	};
	return table;
}

void ini_table_push(ini_table_t* ini_table, char* key, char* value) {
	assoc_arr_push(&ini_table->table, key, value);
}

void ini_table_pop(ini_table_t* ini_table) {
	assoc_arr_pop(&ini_table->table);
}

void ini_table_remove(ini_table_t* ini_table, const char* key) {
	assoc_arr_remove(&ini_table->table, key);
}

char* ini_table_search(ini_table_t ini_table, const char* key) {
	return assoc_arr_search(ini_table.table, key);
}

ini_table_arr_t ini_table_arr_new() {
	ini_table_arr_t table_arr = {
		.head = NULL,
		.tail = NULL,
		.size = 0
	};
	return table_arr;
}

ini_table_t* ini_table_arr_get_last(ini_table_arr_t table_arr) {
	return &table_arr.tail->table;
}

void ini_table_arr_add_table(ini_table_arr_t* table_arr, ini_table_t table) {
	ini_table_item_t* item = (ini_table_item_t*)malloc(sizeof(ini_table_item_t));
	item->table = table;
	item->next = NULL;
	item->prev = NULL;

	if (table_arr->head == NULL) {
		table_arr->head = item;
		table_arr->tail = item;
		table_arr->size++;
		return;
	}

	table_arr->tail->next = item;
	item->prev = table_arr->tail;
	table_arr->tail = item;

	table_arr->size++;
}

void ini_table_arr_remove_table(ini_table_arr_t* table_arr, const char* table) {
	if (table_arr->head == NULL)
		return;

	ini_table_item_t* current = table_arr->head;

	while (current != NULL &&
			strcmp(current->table.name, table) != 0) {
		current = current->next;
	}

	if (current == NULL)
		return;

	if (current->prev != NULL) {
		current->prev->next = current->next;
	} else {
		table_arr->head = current->next;
	}

	if (current->next != NULL) {
		current->next->prev = current->prev;
	} else {
		table_arr->tail = current->prev;
	}

	free(current);
}

ini_table_t* ini_table_arr_search_tables(ini_table_arr_t table_arr, const char* table) {
	if (table_arr.head == NULL)
		return NULL;

	ini_table_item_t* current = table_arr.head;

	while (current != NULL &&
			strcmp(current->table.name, table) != 0) {
		current = current->next;
	}

	if (current == NULL)
		return NULL;

	return &current->table;
}

void ini_table_arr_add_to_table(ini_table_arr_t* table_arr, const char* table, char* key, char* value) {
	if (table_arr->head == NULL)
		return;

	ini_table_t* _table = ini_table_arr_search_tables(*table_arr, table);
	ini_table_push(_table, key, value);
}

void ini_table_arr_remove_from_table(ini_table_arr_t* table_arr, const char* table, const char* key) {
	if (table_arr->head == NULL)
		return;

	ini_table_t* _table = ini_table_arr_search_tables(*table_arr, table);
	ini_table_remove(_table, key);
}

bool valid_table_char(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c == '-' || c == '_');
}

bool valid_table_name(const char* name, size_t length) {
	if (name == NULL)
		return false;

	for (size_t i = 0; i < length; i++) {
		if (!valid_table_char(name[i]))
			return false;
	}

	return true;
}

bool valid_key_char(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c == '-' || c == '_');
}

bool valid_key(const char* name, size_t length) {
	if (name == NULL)
		return false;

	for (size_t i = 0; i < length; i++) {
		if (!valid_key_char(name[i]))
			return false;
	}

	return true;
}

bool is_whitespace(char c) {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

void ini_add_table(ini_t* ini, char* table) {
	ini_table_arr_add_table(&ini->table, ini_table_new(table));
}

void ini_remove_table(ini_t* ini, const char* table) {
	ini_table_arr_remove_table(&ini->table, table);
}

ini_table_t* ini_search_table(ini_t ini, const char* table) {
	return ini_table_arr_search_tables(ini.table, table);
}

void ini_add_kv_pair(ini_t* ini, const char* table, char* key, char* value) {
	ini_table_arr_add_to_table(&ini->table, table, key, value);
}

ini_t* ini_read(char* ini_path) {
	ini_t* ini = (ini_t*)malloc(sizeof(ini_t));
	ini->file_path = ini_path;
	ini->succesfully_initialised = true;
	ini->table = ini_table_arr_new();

	file_t ini_file = file_open(ini->file_path, READ);
	if (ini_file == NULL) {
		ini->succesfully_initialised = false;
		return ini;
	}

	char* ini_file_content;
	size_t ini_file_content_length = file_read(ini_file, &ini_file_content);

	file_close(ini_file);

	if (ini_file_content_length == 0) {
		ini->succesfully_initialised = false;
		return ini;
	}

	for (size_t i = 0; i < ini_file_content_length; i++) {
		// comments
		if (ini_file_content[i] == ';') {
			while (i < ini_file_content_length &&
					ini_file_content[i] != '\n') i++;
			continue;
		}
		// table headers
		else if (ini_file_content[i] == '[') {
			i++;
			size_t table_start = i;
			while (i < ini_file_content_length &&
					ini_file_content[i] != ']' &&
					!is_whitespace(ini_file_content[i])) {
				i++;
			}

			if (i >= ini_file_content_length) {
				ini->succesfully_initialised = false;
				return ini;
			}

			size_t table_length = i - table_start;
			char* table_header = bounded_strdup(ini_file_content, table_start, table_length);
			if (table_header == NULL) {
				free(table_header);

				ini->succesfully_initialised = false;
				return ini;
			}

			if (!valid_table_name(table_header, table_length)) {
				free(table_header);

				ini->succesfully_initialised = false;
				return ini;
			}

			ini_add_table(ini, table_header);

			continue;
		}
		// random whitespace
		else if (is_whitespace(ini_file_content[i]))
			continue;
		// table key-value pairs
		else {
			// read the key
			size_t key_start = i;
			while (i < ini_file_content_length &&
					!is_whitespace(ini_file_content[i]) &&
					ini_file_content[i] != '=')
				i++;

			if (i >= ini_file_content_length) {
				ini->succesfully_initialised = false;
				return ini;
			}

			size_t key_length = i - key_start;
			char* key = bounded_strdup(ini_file_content, key_start, key_length);
			if (key == NULL) {
				free(key);

				ini->succesfully_initialised = false;
				return ini;
			}

			// skip whitespace
			while (i < ini_file_content_length &&
					is_whitespace(ini_file_content[i])) i++;

			if (i >= ini_file_content_length) {
				free(key);

				ini->succesfully_initialised = false;
				return ini;
			}

			// we expect '=', confirm valid formatting
			if (ini_file_content[i] != '=') {
				free(key);

				ini->succesfully_initialised = false;
				return ini;
			}

			// whitespace again
			while (i < ini_file_content_length &&
					is_whitespace(ini_file_content[i])) i++;

			if (i >= ini_file_content_length) {
				free(key);

				ini->succesfully_initialised = false;
				return ini;
			}

			// now we read the value
			while (i < ini_file_content_length &&
					ini_file_content[i] != '"') i++;
			i++;

			if (i >= ini_file_content_length) {
				free(key);

				ini->succesfully_initialised = false;
				return ini;
			}

			size_t value_start = i;

			while (i < ini_file_content_length &&
					(ini_file_content[i] != '"' ||
					ini_file_content[i - 1] == '\\')) i++;

			if (i >= ini_file_content_length) {
				free(key);

				ini->succesfully_initialised = false;
				return ini;
			}

			size_t value_length = i - value_start;
			char* value = bounded_strdup(ini_file_content, value_start, value_length);

			if (ini->table.size == 0) {
				ini_table_arr_add_table(&ini->table, ini_table_new(bounded_strdup("global", 0, 6)));
			}

			ini_table_t* table = ini_table_arr_get_last(ini->table);
			ini_table_arr_add_to_table(&ini->table, table->name, key, value);

			continue;
		}
	}

	free(ini_file_content);

	return ini;
}

void ini_cleanup(ini_t* ini) {
	if (ini == NULL)
		return;

	ini_table_item_t* current_table = ini->table.head;

	while (current_table != NULL) {
		assoc_arr_item_t* kv_pair = current_table->table.table.head;

		while (kv_pair != NULL) {
			free(kv_pair->key);
			free(kv_pair->value);

			assoc_arr_item_t* temp = kv_pair;
			kv_pair = kv_pair->next;
			free(temp);
		}

		free(current_table->table.name);

		ini_table_item_t* temp = current_table;
		current_table = current_table->next;
		free(temp);
	}

	free(ini);
}

bool ini_write(ini_t* ini) {
	if (ini == NULL || ini->file_path == NULL)
		return false;

	file_t ini_file = file_open(ini->file_path, WRITE_BINARY);
	if (ini_file == NULL)
		return false;

	ini_table_item_t* current_table = ini->table.head;

	while (current_table != NULL) {
		fprintf(ini_file, "[%s]\n", current_table->table.name);

		assoc_arr_item_t* kv_pair = current_table->table.table.head;

		while (kv_pair != NULL) {
			fprintf(ini_file, "%s = \"%s\"\n", kv_pair->key, kv_pair->value);

			kv_pair = kv_pair->next;
		}

		fprintf(ini_file, "\n");
		current_table = current_table->next;
	}

	file_close(ini_file);

	return true;
}

