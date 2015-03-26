#ifndef _RPN3_RPN_CONTAINER_H_
#define _RPN3_RPN_CONTAINER_H_

#ifdef WIN32
typedef int _bool;
#define false 0
#define true 1
#else
#include <stdbool.h>
typedef bool _bool;
#endif

typedef struct rpn_container
{
	struct queue_node* inst_queue;
	struct stack_node* data_stack;
	struct variable_node* variable_list;
	struct function_node* function_list;
	unsigned int status;
	_bool running;
	_bool execute;
	int lengthStack;
} rpn_container;

#include "stack.h"
#include "queue.h"
#include "variables.h"

#endif