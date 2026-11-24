#include "list.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

list_node_t* list_node_new(void* data) {
	list_node_t* node = (list_node_t*)malloc(sizeof(list_node_t));
	if (node == NULL)
		return NULL;

	node->data = data;
	node->next = NULL;
	node->prev = NULL;
	
	return node;
}
void node_free(list_node_t* node) {
	free(node);
}

list_t list_new(bool (*eq)(void*, void*)) {
	return (list_t) {
		.head = NULL,
		.tail = NULL,
		.length = 0,
		.eq = eq
	};
}

bool list_append(list_t* list, void* data) {
	if (list == NULL)
		return false;

	list_node_t* node = list_node_new(data);
	if (node == NULL)
		return false;

	if (list->tail == NULL) {
		list->head = node;
		list->tail = node;
		list->length++;

		return true;
	}

	list->tail->next = node;
	list->tail = node;
	list->length++;

	return true;
}
bool list_remove(list_t* list, void* data) {
	if (list == NULL)
		return false;

	list_node_t* prev = NULL;
	list_node_t* node = list->head;
	while (node != NULL){
		if (list->eq(node->data, data)) {
			if (node == list->head) {
				list->head = node->next;
				node_free(node);

				if (list->head == NULL)
					list->tail = NULL;

				list->length--;
				return true;
			}

			list->tail = prev;
			node_free(node);

			if (list->tail == NULL)
				list->head = NULL;

			list->length--;
			return true;
		}

		prev = node;
		node = node->next;
	}

	return false;
}
void list_filter(list_t* list, bool (*filter)(void*)) {
	if (list == NULL)
		return;

	list_node_t* prev = NULL;
	list_node_t* node = list->head;
	while (node != NULL){
		if (filter(node->data)) {
			if (node == list->head) {
				list->head = node->next;
				node_free(node);

				if (list->head == NULL)
					list->tail = NULL;
			} else {
				list->tail = prev;
				node_free(node);

				if (list->tail == NULL)
					list->head = NULL;
			}
			list->length--;
		}

		prev = node;
		node = node->next;
	}
}
void list_empty(list_t* list) {
	if (list == NULL)
		return;

	list_node_t* temp = NULL;
	list_node_t* node = list->head;
	while (node != NULL) {
		temp = node;
		node = node->next;
		node_free(temp);
	}
	list->length = 0;
}

void list_free(list_t* list) {
	list_empty(list);
}
