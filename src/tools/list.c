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
	node->prev = list->tail;
	list->tail = node;
	list->length++;

	return true;
}

bool list_remove(list_t* list, void* data) {
	if (list == NULL || list->eq == NULL)
		return false;

	list_node_t* node = list->head;
	while (node != NULL){
		if (list->eq(node->data, data)) {
			if (node->prev != NULL) {
				node->prev->next = node->next;
			} else {
				list->head = node->next;
			}
			
			if (node->next != NULL) {
				node->next->prev = node->prev;
			} else {
				list->tail = node->prev;
			}

			node_free(node);
			list->length--;
			return true;
		}

		node = node->next;
	}

	return false;
}

void list_filter(list_t* list, bool (*filter)(void*)) {
	if (list == NULL)
		return;

	list_node_t* node = list->head;
	while (node != NULL) {
		list_node_t* next = node->next;
		if (!filter(node->data)) {
			if (node->prev != NULL) {
				node->prev->next = node->next;
			} else {
				list->head = node->next;
			}
			
			if (node->next != NULL) {
				node->next->prev = node->prev;
			} else {
				list->tail = node->prev;
			}

			node_free(node);
			list->length--;
		}

		node = next;
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
	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
}

void list_free(list_t* list) {
	list_empty(list);
}

list_iter_t list_iter(list_t* list) {
	if (list == NULL)
		return (list_iter_t){
			.current = NULL
		};
	return (list_iter_t){
		.current = list->head
	};
}

void list_iter_next(list_iter_t* iter) {
	if (iter == NULL)
		return;

	if (iter->current != NULL) {
		iter->current = iter->current->next;
	}
}

bool list_iter_finished(list_iter_t* iter) {
	if (iter == NULL)
		return true;

	return iter->current == NULL;
}

void* list_iter_data(list_iter_t* iter) {
	if (iter == NULL || iter->current == NULL)
		return NULL;
	return iter->current->data;
}
