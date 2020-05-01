/*
 * List.c
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#include "List.h"

List* createList(uint64_t dataSize) {
	List *list = (List*) malloc(sizeof (List));
	list->head = (Node*) malloc(sizeof (Node));
	list->last = list->head;

	list->head->data = (uint16_t*) malloc(sizeof (uint16_t) * dataSize);
	list->head->next = NULL;

	list->lastFreeIndex = 0;
	list->dataSize = dataSize;

	return list;
}

Node* createNode(uint64_t dataSize) {
	Node *node = (Node*) malloc(sizeof (Node));

	node->data = (uint16_t*) malloc(sizeof (uint16_t) * dataSize);
	node->next = NULL;

	return node;
}

void addToList(List *list, uint16_t data) {
	if (list->lastFreeIndex >= list->dataSize) {
		list->last->next = createNode(list->dataSize);
		list->last = list->last->next;
		list->lastFreeIndex = 0;
	}

	list->last->data[list->lastFreeIndex] = data;
	++list->lastFreeIndex;
}

void freeList(List *list) {
	Node *current;
	Node *next = list->head;

	while (next != NULL) {
		current = next;
		next = current->next;

		free(current->data);
		free(current);
	}

	free(list);
}
