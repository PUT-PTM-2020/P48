/*
 * List.h
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#ifndef INC_LIST_H_
#define INC_LIST_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct Node {
	uint16_t *data;
	struct Node *next;
} Node;

typedef struct List {
	Node *head;
	Node *last;

	uint64_t lastFreeIndex;
	uint64_t dataSize;
} List;

List* createList(uint64_t dataSize);
void addToList(List *list, uint16_t data);
void freeList(List *list);

#endif /* INC_LIST_H_ */
