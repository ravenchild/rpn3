#include "display.h"

void display(rpn_container* container)
{
	stack_node* current_data = NULL;
	int i;
	
	pointToHead(container);
	
	if (container->data_stack == NULL)
	{
		printf("No data items.\n");
		return;
	}
	
	// Printing Data
	
	current_data = container->data_stack;
	if (container->data_stack != NULL)
		for (current_data = container->data_stack; current_data->prev != NULL;)
			current_data = current_data->prev;
	
	for (i = 1; current_data != NULL; i++)
	{
		printf("%f\n", current_data->data);
		current_data = current_data->next;
	}
	
}

void displayOne(rpn_container* container)
{
	
	pointToHead(container);
	
	if (container->data_stack == NULL)
	{
		printf("No data items.\n");
		return;
	}
	
	// Printing Data
	printf("%f\n", container->data_stack->data);
}

void displayall(rpn_container* container)
{
	stack_node* current_data = NULL;
	queue_node* current_inst = NULL;
	int i;
	
	pointToHead(container);
	pointToTop(container);
	
	if (container->data_stack == NULL && container->inst_queue == NULL)
	{
		printf("No data or instruction items.\n");
		return;
	}
	
	// Printing Data
	
	current_data = container->data_stack;
	if (container->data_stack != NULL)
		for (current_data = container->data_stack; current_data->prev != NULL;)
			current_data = current_data->prev;
	
	printf("Data:\n-----\n");
	if (current_data == NULL)
		printf("|000|\tNone\n");
	for (i = 1; current_data != NULL; i++)
	{
		printf("|%03d|\t%f\n", i, current_data->data);
		current_data = current_data->next;
	}
	printf("-----\n");
	
	// Printing Instructions
	
	current_inst = container->inst_queue;
	if (container->inst_queue != NULL)
		for (current_inst = container->inst_queue; current_inst->prev != NULL;)
			current_inst = current_inst->prev;
	
	printf("Instructions:\n-----\n");
	if (current_inst == NULL)
		printf("|000|\tNone\n");
	for (i = 1; current_inst != NULL; i++)
	{
		printf("|%03d|\t% 2d\t%0.2f\t\"%s\"|\"%s\"\n", i, current_inst->instruction, current_inst->number,current_inst->raw, current_inst->line);
		current_inst = current_inst->next;
	}
	printf("-----\n");
}