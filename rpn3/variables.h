#ifndef _RPN3_VARIABLES_H
#define _RPN3_VARIABLES_H

#include "rpn_container.h"

#include <stdlib.h>
#include <string.h>

#ifdef WIN32
typedef int _bool;
#define false 0
#define true 1
#endif

typedef struct variable_node
{
	char* name;
	double number;
	struct variable_node* next;
} variable_node;

void freeVariables(variable_node* last);
void addVariable(rpn_container* container, char* name, double value);
variable_node* getVariable(rpn_container* container, char* name);
_bool removeVariable(rpn_container* container, char* name);

#endif