#include "stack.h"

void freeStack(stack_node* node)
{
	if (node == NULL)
		return;
	
	for (;node->next != NULL;)
		node = node->next;
	
	for (;node->prev != NULL;)
	{
		node = node->prev;
		free(node->next);
		node->next = NULL;
	}
	
	free(node);
	node = NULL;
}

int lengthStack(stack_node* node)
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

void pointToHead(rpn_container* container)
{
	if (container->data_stack != NULL)
		for (;container->data_stack->next != NULL;)
			container->data_stack = container->data_stack->next;
}

stack_node* pushDataToTop(rpn_container* container, double data)
{
	stack_node* new_node;
	stack_node* node = container->data_stack;

	if (container->data_stack != NULL)
		for (;node->prev != NULL;)
			node = node->prev;
	
	new_node = (stack_node*) malloc(sizeof(stack_node));
	new_node->next = node;
	new_node->data = data;
	new_node->prev = NULL;
	new_node->container = container;
	
	if (node != NULL)
		node->prev = new_node;
	
	pointToHead(container);
	container->data_stack = new_node;
	container->lengthStack++;
	
	return new_node;
}

stack_node* pushData(rpn_container* container, double data)
{
	stack_node* new_node;

	pointToHead(container);
	
	new_node = (stack_node*) malloc(sizeof(stack_node));
	new_node->next = NULL;
	new_node->data = data;
	new_node->prev = container->data_stack;
	new_node->container = container;
	
	if (container->data_stack != NULL)
		container->data_stack->next = new_node;
	container->data_stack = new_node;
	container->lengthStack++;
	
	return new_node;
}

double popData(rpn_container* container)
{
	stack_node* old_node;
	double old_data;

	if (container->data_stack == NULL)
		return 0.0f;
	
	pointToHead(container);
	old_node = container->data_stack;
	old_data = old_node->data;
	container->data_stack = old_node->prev;
	
	if (container->data_stack != NULL)
		container->data_stack->next = NULL;
	
	free(old_node);
	old_node = NULL;
	container->lengthStack--;
	
	return old_data;
}
