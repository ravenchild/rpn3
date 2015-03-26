#ifndef _RPN3_QUEUE_H_
#define _RPN3_QUEUE_H_

#include "rpn_container.h"

#include <stdlib.h>

#ifdef WIN32
typedef int _bool;
#define false 0
#define true 1
#endif

typedef struct queue_node
{
	struct queue_node* next; // Towards top item. (Newest item)
	struct queue_node* prev; // Towards base item. (Oldest item)
	unsigned int instruction;
	char* raw;
	size_t lengthRaw;
	char* line;
	size_t lengthLine;
	double number;
	_bool allocated;
	struct rpn_container* user_function;
	struct rpn_container* container;
} queue_node;

// Functions

void freeQueue(queue_node* node);
int lengthQueue(queue_node* node);
void pointToTop(rpn_container* container);

queue_node* putInst(rpn_container* container, char* raw, char* line, double number, unsigned int instruction, _bool allocated);
queue_node* peekInst(rpn_container* container);
void removeInst(queue_node* node);
queue_node* insertAfter(rpn_container* container, queue_node* position, char* raw, char* line, double number, unsigned int instruction, _bool allocated);

#endif

