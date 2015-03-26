#include "queue.h"

void freeQueue(queue_node* node)
{
	if (node == NULL)
		return;
	
	for (;node->next != NULL;)
		node = node->next;
	
	for (;node->prev != NULL;)
	{
		node = node->prev;
		if (node->next->allocated)
		{
			free(node->next->raw);
			node->next->raw = NULL;
			free(node->next->line);
			node->next->line = NULL;
		}
		free(node->next);
		node->next = NULL;
	}
	
	if (node->allocated)
	{
		free(node->raw);
		node->raw = NULL;
		free(node->line);
		node->line = NULL;
	}
	free(node);
	node = NULL;
}

int lengthQueue(queue_node* node)
{
	int count = 1;

	if (node == NULL)
		return 0;
	
	for (;node->prev != NULL;)
		node = node->prev;
	for (;node->next != NULL; count++)
		node = node->next;
	
	return count;
}

void pointToTop(rpn_container* container)
{
	if (container->inst_queue != NULL)
		for (;container->inst_queue->prev != NULL;)
			container->inst_queue = container->inst_queue->prev;
}

queue_node* putInst(rpn_container* container, char* raw, char* line, double number, unsigned int instruction, _bool allocated)
{
	queue_node* node;
	queue_node* new_node = (queue_node*) malloc(sizeof(queue_node));

	new_node->container = container;
	new_node->allocated = allocated;
	new_node->raw = raw;
	new_node->lengthRaw = strlen(new_node->raw);
	new_node->line = line;
	new_node->lengthLine = strlen(new_node->line);
	new_node->number = number;
	new_node->instruction = instruction;
	new_node->next = NULL;
	new_node->prev = NULL;
	
	if (container->inst_queue != NULL)
	{
		node = container->inst_queue;
		for (;node->next != NULL;)
			node = node->next;
		node->next = new_node;
		new_node->prev = node;
	}
	else
	{
		container->inst_queue = new_node;
	}

	return new_node;
}

queue_node* peekInst(rpn_container* container)
{
	pointToTop(container);
	return container->inst_queue;
}

void removeInst(queue_node* node)
{
	if (node == NULL)
		return;
	
	if (node->prev != NULL && node->next != NULL)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		if (node->container->inst_queue == node)
			node->container->inst_queue = node->prev;
	}
	else if (node->prev == NULL && node->next != NULL)
	{
		node->next->prev = NULL;
		if (node->container->inst_queue == node)
			node->container->inst_queue = node->next;
	}
	else if (node->prev != NULL && node->next == NULL)
	{
		node->prev->next = NULL;
		if (node->container->inst_queue == node)
			node->container->inst_queue = node->prev;
	}
	else
	{
		node->container->inst_queue = NULL;
	}
	
	if (node->allocated)
	{
		free(node->raw);
		node->raw = NULL;
		free(node->line);
		node->line = NULL;
	}
	free(node);
	node = NULL;
}

queue_node* insertAfter(rpn_container* container, queue_node* position, char* raw, char* line, double number, unsigned int instruction, _bool allocated)
{	
	queue_node* new_node;

	if (position == NULL || container->inst_queue == NULL)
		return putInst(container, raw, line, number, instruction, allocated);
	
	new_node = (queue_node*) malloc(sizeof(queue_node));
	new_node->container = container;
	new_node->allocated = allocated;
	new_node->raw = raw;
	new_node->lengthRaw = strlen(new_node->raw);
	new_node->line = line;
	new_node->lengthLine = strlen(new_node->line);
	new_node->number = number;
	new_node->instruction = instruction;
	new_node->next = position->next;
	new_node->prev = position;
	
	if (position->next != NULL)
		position->next->prev = new_node;
	position->next = new_node;
	
	return new_node;
}
