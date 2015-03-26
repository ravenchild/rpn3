#ifndef _RPN3_FUNCTIONS_H_
#define _RPN3_FUNCTIONS_H_

#include "rpn_container.h"
#include "queue.h"
#include "stack.h"

#include <stdlib.h>
#include <string.h>

#ifdef WIN32
typedef int _bool;
#define false 0
#define true 1
#endif

typedef struct function_node
{
	char* name;
	int stack_copy;
	struct queue_node* instructions;
	struct function_node* next;
} function_node;

void freeFunctions(function_node* last);
function_node* addFunction(rpn_container* container, char* name, int stack_copy);
void addFunctionInstruction(function_node* function, queue_node* instruction);
function_node* getFunction(rpn_container* container, char* name);
void addInstructionsFromFunction(rpn_container* func_container, function_node* function);
void addInstructionsFromFunctionInline(rpn_container* func_container, function_node* function, queue_node* node);
_bool removeFunction(rpn_container* container, char* name);
void pushNewFunctionData(rpn_container* container, rpn_container* function_container);

#endif