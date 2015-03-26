#include "variables.h"

void freeVariables(variable_node* last)
{
	variable_node* next = NULL;
	for (;last != NULL;)
	{
		next = last->next;
		free(last->name);
		free(last);
		last = next;
	}
}

void addVariable(rpn_container* container, char* name, double value)
{
	char* var_name;
	variable_node* new_variable;
	variable_node* last = getVariable(container, name);
	if (last != NULL)
	{
		last->number = value;
		return;
	}
	
	last = container->variable_list;
	for (;last != NULL;)
	{
		if (last->next == NULL)
			break;
		last = last->next;
	}
	
	var_name = (char*) malloc(sizeof(char) * strlen(name) + 1);
	memcpy(var_name, name, sizeof(char) * strlen(name) + 1);
	
	new_variable = (variable_node*) malloc(sizeof(variable_node));
	new_variable->name = var_name;
	new_variable->number = value;
	new_variable->next = NULL;
	
	if (last == NULL)
		container->variable_list = new_variable;
	else
		last->next = new_variable;

}

variable_node* getVariable(rpn_container* container, char* name)
{
	variable_node* last = container->variable_list;
	for (;last != NULL;)
	{
		if (strcmp(name, last->name) == 0)
			break;
		last = last->next;
	}
	return last;
}

_bool removeVariable(rpn_container* container, char* name)
{
	variable_node* last = container->variable_list;
	variable_node* prev = NULL;
	for (;last != NULL;)
	{
		if (strcmp(name, last->name) == 0)
			break;
		prev = last;
		last = last->next;
	}
	if (last == NULL)
		return false;
	
	if (prev == NULL)
		container->variable_list = last->next;
	else
		prev->next = last->next;
	
	free(last->name);
	free(last);
	
	return true;
}