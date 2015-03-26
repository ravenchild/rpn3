#ifndef _RPN3_STACK_H_
#define _RPN3_STACK_H_

#include "rpn_container.h"

#include <stdlib.h>

typedef struct stack_node
{
	struct stack_node* next; // Towards top item. (Newest item)
	struct stack_node* prev; // Towards base item. (Oldest item)
	double data;
	struct rpn_container* container;
} stack_node;

// Functions

void freeStack(stack_node* node);
int lengthStack(stack_node* node);
void pointToHead(rpn_container* container);

stack_node* pushDataToTop(rpn_container* container, double data);
stack_node* pushData(rpn_container* container, double data);
double popData(rpn_container* container);

#endif

