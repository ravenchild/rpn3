#include "functions.h"

void freeFunctions(function_node* last)
{
	function_node* next = NULL;
	for (;last != NULL;)
	{
		next = last->next;
		freeQueue(last->instructions);
		free(last->name);
		free(last);
		last = next;
	}
}

function_node* addFunction(rpn_container* container, char* name, int stack_copy)
{
	size_t name_length;
	function_node* new_func;
	function_node* func = getFunction(container, name);

	if (func)
		removeFunction(container, name);
	
	func = container->function_list;
	name_length = strlen(name);
	if (func != NULL)
		for (; func->next != NULL;)
			func = func->next;
	
	new_func = (function_node*) malloc(sizeof(function_node));
	new_func->name = (char*) malloc(sizeof(char) * (name_length + 1));
	memcpy(new_func->name, name, name_length + 1);
	new_func->stack_copy = stack_copy;
	new_func->next = NULL;
	new_func->instructions = NULL;
	
	if (func == NULL)
		container->function_list = new_func;
	else
		func->next = new_func;

	return new_func;
}

void addFunctionInstruction(function_node* function, queue_node* instruction)
{
	queue_node* node;
	queue_node* new_node;

	new_node = (queue_node*) malloc(sizeof(queue_node));
	new_node->allocated = true;
	new_node->container = NULL;
	new_node->number = instruction->number;
	new_node->instruction = instruction->instruction;
	new_node->next = NULL;
	new_node->prev = NULL;
	
	new_node->raw = (char*) malloc(sizeof(char) * (instruction->lengthRaw + 1));
	memcpy(new_node->raw, instruction->raw, sizeof(char) * (instruction->lengthRaw + 1));
	new_node->lengthRaw = instruction->lengthRaw;
	
	new_node->line = (char*) malloc(sizeof(char) * (instruction->lengthLine + 1));
	memcpy(new_node->line, instruction->line, sizeof(char) * (instruction->lengthLine + 1));
	new_node->lengthLine = instruction->lengthLine;
	
	if (function->instructions != NULL)
	{
		node = function->instructions;
		for (;node->next != NULL;)
			node = node->next;
		node->next = new_node;
		new_node->prev = node;
	}
	else
	{
		function->instructions = new_node;
	}
}

function_node* getFunction(rpn_container* container, char* name)
{
	function_node* func = container->function_list;
	for (; func!=NULL;)
	{
		if (strcmp(func->name, name) == 0)
			break;
		else
			func = func->next;
	}
	return func;
}

void addInstructionsFromFunction(rpn_container* func_container, function_node* function)
{
	queue_node* func_inst = function->instructions;
	
	char* inst_raw;
	char* inst_line;
	
	if (func_inst != NULL)
		for (; func_inst->prev != NULL;)
			func_inst = func_inst->prev;
	
	for (; func_inst != NULL;)
	{
		inst_raw = (char*) malloc(sizeof(char) * (func_inst->lengthRaw + 1));
		memcpy(inst_raw, func_inst->raw, sizeof(char) * (func_inst->lengthRaw + 1));
		
		inst_line = (char*) malloc(sizeof(char) * (func_inst->lengthLine + 1));
		memcpy(inst_line, func_inst->line, sizeof(char) * (func_inst->lengthLine + 1));
		
		putInst(func_container, inst_raw, inst_line, func_inst->number, func_inst->instruction, true);
		func_inst = func_inst->next;
	}
}

void addInstructionsFromFunctionInline(rpn_container* func_container, function_node* function, queue_node* node)
{
	queue_node* func_inst = function->instructions;
	
	char* inst_raw;
	char* inst_line;
	
	if (func_inst != NULL)
		for (; func_inst->prev != NULL;)
			func_inst = func_inst->prev;
	
	for (; func_inst != NULL;)
	{
		inst_raw = (char*) malloc(sizeof(char) * (func_inst->lengthRaw + 1));
		memcpy(inst_raw, func_inst->raw, sizeof(char) * (func_inst->lengthRaw + 1));
		inst_line = (char*) malloc(sizeof(char) * (func_inst->lengthLine + 1));
		memcpy(inst_line, func_inst->line, sizeof(char) * (func_inst->lengthLine + 1));
		
		insertAfter(func_container, node, inst_raw, inst_line, func_inst->number, func_inst->instruction, true);
		node = node->next;
		func_inst = func_inst->next;
	}
}

_bool removeFunction(rpn_container* container, char* name)
{
	function_node* func_last = NULL;
	function_node* func = container->function_list;
	for (; func != NULL;)
	{
		if (strcmp(func->name, name) == 0)
		{
			freeQueue(func->instructions);
			free(func->name);
			if (func_last != NULL)
				func_last->next = func->next;
			else
				container->function_list = func->next;
			break;
		}
		else
		{
			func_last = func;
			func = func->next;
		}
	}
	
	return true;
}

void pushNewFunctionData(rpn_container* container, rpn_container* function_container)
{
	double data;

	if (function_container->lengthStack == 0)
		return;
	
	data = popData(function_container);
	pushNewFunctionData(container, function_container);
	pushData(container, data);
}