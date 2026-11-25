#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct list_node list_node_t;
struct list_node {
	void* data;
	list_node_t* next;
	list_node_t* prev;
};

typedef struct list {
	list_node_t* head;
	list_node_t* tail;
	size_t length;
	bool (*eq)(void*, void*);
} list_t;

typedef struct list_iter {
	list_node_t* current;
} list_iter_t;

list_t list_new(bool (*eq)(void*, void*));
bool list_append(list_t* list, void* data);
bool list_remove(list_t* list, void* data);
void list_filter(list_t* list, bool (*filter)(void*));
void list_empty(list_t* list);
void list_free(list_t* list);

list_iter_t list_iter(list_t* list);
void list_iter_next(list_iter_t* iter);
bool list_iter_finished(list_iter_t* iter);
void* list_iter_data(list_iter_t* iter);

#endif
