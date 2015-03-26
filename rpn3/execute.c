#include "execute.h"

void execute(rpn_container* container)
{
	
	int i;
	_bool working, hit_function_end;
	FILE* source;
	char* line;
	char* buffer;
	variable_node* var1;
	rpn_container* function_container;
	function_node* user_func;
	queue_node* node, *temp_node;
	
	// Need to run through the stack backwards and also free the instruction structs.
	pointToHead(container);
	pointToTop(container);
	
	if (container->inst_queue != NULL)
	{
		parseTokens(container);
		
		node = peekInst(container);
		
		if (!container->execute)
		{
			working = true;
			while (working)
			{
				if (node->instruction == INST_EXECUTE)
				{
					container->execute = true;
					removeInst(node);
				}
				else if (node->instruction == INST_DISPLAY)
				{
					display(container);
					removeInst(node);
				}
				else if (node->instruction == INST_PRINT)
				{
					displayOne(container);
					removeInst(node);
				}
				else if (peekInst(container)->instruction == INST_QUIT)
				{
					container->running = false;
					removeInst(node);
				}
				else
				{
					working = false;
				}
			}
		}
		
		node = peekInst(container);
		for (;node != NULL && container->execute && container->running;)
		{
			switch (node->instruction)
			{
				case INST_DATA:
					pushData(container, node->number);
					break;
					
				case INST_DISPLAY:
					display(container);
					break;
					
				case INST_PRINT:
					displayOne(container);
					break;
					
				case INST_DISPLAY_ALL:
					displayall(container);
					break;
					
				case INST_QUIT:
					container->running = false;
					break;
					
				case INST_POP:
					if (container->lengthStack < 1)
					{
						error_msg("Nothing to pop.\n");
						break;
					}
					popData(container);
					break;
					
				case INST_STACK_LEN:
					pushData(container, container->lengthStack);
					break;
					
				case INST_SWAP:
					if (container->lengthStack < 2)
					{
						error_msg("You must have at least two items to swap.\n");
						break;
					}
					else
					{
						double swap1 = popData(container);
						double swap2 = popData(container);
						pushData(container, swap1);
						pushData(container, swap2);
					}
					break;
					
				case INST_ADD:
					if (container->lengthStack < 2)
					{
						error_msg("You must have at least two items to add.\n");
						break;
					}
					else
					{
						pushData(container, popData(container) + popData(container));
					}
					break;
					
				case INST_SUBTRACT:
					if (container->lengthStack < 2)
					{
						error_msg("You must have at least two items to subtract.\n");
						break;
					}
					else
					{
						double sub = popData(container);
						pushData(container, popData(container) - sub);
					}
					break;
					
				case INST_MULTIPLY:
					if (container->lengthStack < 2)
					{
						error_msg("You must have at least two items to multiply.\n");
						break;
					}
					else
					{
						pushData(container, popData(container) * popData(container));
					}
					break;
					
				case INST_DIVIDE:
					if (container->lengthStack < 2)
					{
						error_msg("You must have at least two items to divide.\n");
						break;
					}
					else
					{
						double div1 = popData(container);
						if (div1 == 0.0)
						{
							pushData(container, div1);
							error_msg("Cannot destroy the world.\n");
							
							break;
						}
						pushData(container, popData(container) / div1);
					}
					break;
					
				case INST_CLEAR:
					freeStack(container->data_stack);
					container->data_stack = NULL;
					break;
					
				case INST_HELP:
					error_msg("RPNv3 - Eric Roth\n\n");
					error_msg("q            - Exit.\n");
					error_msg("d            - Display Stack.\n");
					error_msg("s            - Swap two items.\n");
					error_msg("p            - Pop last item.\n");
					error_msg("x            - Clear Stack.\n");
					error_msg("c            - Copy top stack item.\n");
					error_msg("h            - This help.\n");
					error_msg("#            - Comment remaining line.\n");
					error_msg("+,-,/,*,^    - Operators.\n");
					error_msg(">,<,>=,<=,=  - Comparison Operators.\n");
					error_msg("?            - If pop<1, skip next instruction.\n");
					
					error_msg("%%file.ext    - Parse a file in place.\n\n");
					error_msg("      WARNING: Files must end in whitespace.\n");
					error_msg("               Also, recursion is possible.\n\n");
					
					error_msg("$varname     - Push a variable value onto the stack.\n");
					error_msg(":varname     - Pop a stack value and save as a variable.\n\n");
					error_msg("      WARNING: Variable names must end in whitespace.\n");
					error_msg("               Variable names can be anything but `#'.\n\n");
					
					error_msg("[name:1 ]    - Empty function declaration. Takes one stack item.\n");
					error_msg("               Any items left in the stack are returned.\n\n");
					error_msg("      WARNING: It is possible to override built-ins.\n");
					error_msg("               You must finish with a `]'.\n\n");
					
					error_msg("Built-in Functions:\n");
					error_msg("pow, mod, abs, floor, ceil, sqrt\n");
					error_msg("sin, cos, tan, sinh, cosh, tanh\n");
					error_msg("ln, log, exp\n\n");
					error_msg("Constants:\n");
					error_msg("e, pi\n\n");
					error_msg("This is a valid line:\n99 1.5 .5^log dc:var p5d$var >?%%file.rpn q#Comment\n\n");
					
					break;
					
				case INST_FUNC_POW:
					if (container->lengthStack < 2)
					{
						error_msg("You must have at least 9002 items to raise the power.\n");
						break;
					}
					else
					{
						double pow1 = popData(container);
						pushData(container, pow(popData(container), pow1));
					}
					break;
					
				case INST_FUNC_MOD:
					if (container->lengthStack < 2)
					{
						error_msg("You must have at least 2 items for modulus.\n");
						break;
					}
					else
					{
						double mod1 = popData(container);
						pushData(container, fmod(popData(container), mod1));
					}
					break;
					
				case INST_FUNC_SIN:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for sine.\n");
						break;
					}
					else
					{
						pushData(container, sin(popData(container)));
					}
					break;
					
				case INST_FUNC_COS:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for cosine.\n");
						break;
					}
					else
					{
						pushData(container, cos(popData(container)));
					}
					break;
					
				case INST_FUNC_TAN:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for tangent.\n");
						break;
					}
					else
					{
						pushData(container, tan(popData(container)));
					}
					break;
					
				case INST_FUNC_SINH:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for hyperbolic sine.\n");
						break;
					}
					else
					{
						pushData(container, sinh(popData(container)));
					}
					break;
					
				case INST_FUNC_COSH:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for hyperbolic cosine.\n");
						break;
					}
					else
					{
						pushData(container, cosh(popData(container)));
					}
					break;
					
				case INST_FUNC_TANH:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for hyperbolic tangent.\n");
						break;
					}
					else
					{
						pushData(container, tanh(popData(container)));
					}
					break;
					
				case INST_FUNC_LN:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for natural log.\n");
						break;
					}
					else
					{
						pushData(container, log(popData(container)));
					}
					break;
					
				case INST_FUNC_LOG:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for log base 10.\n");
						break;
					}
					else
					{
						pushData(container, log10(popData(container)));
					}
					break;
					
				case INST_FUNC_EXP:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for exp.\n");
						break;
					}
					else
					{
						pushData(container, exp(popData(container)));
					}
					break;
					
				case INST_FUNC_ABS:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for absolute value.\n");
						break;
					}
					else
					{
						pushData(container, fabs(popData(container)));
					}
					break;
					
				case INST_FUNC_FLOOR:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for floor.\n");
						break;
					}
					else
					{
						pushData(container, floor(popData(container)));
					}
					break;
					
				case INST_FUNC_CEIL:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for ceiling.\n");
						break;
					}
					else
					{
						pushData(container, ceil(popData(container)));
					}
					break;
					
				case INST_FUNC_SQRT:
					if (container->lengthStack < 1)
					{
						error_msg("You must have at least 1 item for square root.\n");
						break;
					}
					else
					{
						pushData(container, sqrt(popData(container)));
					}
					break;
					
				case INST_CONST_E:
					pushData(container, 2.71828182845904523536028747135266249775724709369995);
					break;
					
				case INST_CONST_PI:
					pushData(container, 3.14159265358979323846264338327950288419716939937510);
					break;
					
				case INST_FILE:
					if (node->lengthRaw < 2)
					{
						error_msg("You must provide the file name with the file operator.\n");
						break;
					}
					source = fopen((char*) (node->raw + 1), "r");
					if (source == NULL)
					{
						perror((char*) (node->raw + 1));
						error_msg("The file `%s` could not be opened.\n", (char*) (node->raw + 1));
						break;
					}

					buffer = (char*) malloc(sizeof(char)*(RPN_BUFFER_LENGTH+1));
					
					if (buffer == NULL)
					{
						error_msg("Could not allocate buffer of size %d.\n", RPN_BUFFER_LENGTH);
						break;
					}
					
					temp_node = node;
					for(;(line = readLine(source, buffer, RPN_BUFFER_LENGTH));)
						temp_node = tokenizeLine(container, line, temp_node);
					
					fclose(source);
					source = NULL;
					free(buffer);
					buffer = NULL;
					
					parseTokens(container);
					
					break;
					
				case INST_GT:
					if (container->lengthStack < 1)
					{
						error_msg("You must have 2 items for greater than.\n");
						break;
					}
					else
					{
						double gt1 = popData(container);
						pushData(container, popData(container) > gt1 ? 1.0 : 0.0);
					}
					break;
					
				case INST_LT:
					if (container->lengthStack < 1)
					{
						error_msg("You must have 2 items for less than.\n");
						break;
					}
					else
					{
						double lt1 = popData(container);
						pushData(container, popData(container) < lt1 ? 1.0 : 0.0);
					}
					break;
					
				case INST_GTE:
					if (container->lengthStack < 1)
					{
						error_msg("You must have 2 items for greater than or equal.\n");
						break;
					}
					else
					{
						double gte1 = popData(container);
						pushData(container, popData(container) >= gte1 ? 1.0 : 0.0);
					}
					break;
					
				case INST_LTE:
					if (container->lengthStack < 1)
					{
						error_msg("You must have 2 items for less than or equal.\n");
						break;
					}
					else
					{
						double lte1 = popData(container);
						pushData(container, popData(container) <= lte1 ? 1.0 : 0.0);
					}
					break;
					
				case INST_EQUAL:
					if (container->lengthStack < 1)
					{
						error_msg("You must have 2 items for equal.\n");
						break;
					}
					else
					{
						double equal1 = popData(container);
						pushData(container, popData(container) == equal1 ? 1.0 : 0.0);
					}
					break;
					
				case INST_QUESTION:
					if (container->lengthStack < 1 || lengthQueue(node) < 2)
					{
						error_msg("You must have 1 data item and an additional instruction on the stack.\n");
						break;
					}
					else
					{
						if (popData(container) < 1)
							removeInst(node->next);
					}
					break;
					
				case INST_COPY:
					if (container->lengthStack < 1)
					{
						error_msg("You must have 1 data item to copy.\n");
						break;
					}
					else
					{
						double copy1 = popData(container);
						pushData(container, copy1);
						pushData(container, copy1);
					}
					break;
					
				case INST_VAR:
					var1 = getVariable(container, node->raw);
					if (var1 == NULL)
					{
						error_msg("Variable not found `%s' on line \"%s\"\n", node->raw, node->line);
						break;
					}
					else
					{
						pushData(container, var1->number);
					}
					break;
					
				case INST_SETVAR:
					if (container->lengthStack < 1)
					{
						error_msg("You must have 1 data item to set a variable.\n");
						break;
					}
					else
					{
						if (node->lengthRaw > 0)
							addVariable(container, node->raw, popData(container));
						else
							error_msg("Bad variable anme `%s' on line \"%s\"\n", node->raw, node->line);
					}
					break;
					
				case INST_COMMENT:
					printf("%s\n", node->raw);
					break;
					
				case INST_USERFUNC:
					user_func = getFunction(container, node->raw);
					if (user_func == NULL)
					{
						error_msg("Unknown user function `%s`.\n", node->raw);
						break;
					}
					
					if (container->lengthStack < user_func->stack_copy)
					{
						error_msg("You must have %d stack item(s) for user function `%s`.\n", user_func->stack_copy, node->raw);
						break;
					}
					
					if (user_func->stack_copy > 0)
					{
						function_container = (rpn_container*) malloc(sizeof(rpn_container));
						function_container->inst_queue = NULL;
						function_container->data_stack = NULL;
						function_container->variable_list = NULL;
						function_container->function_list = container->function_list;
						function_container->running = true;
						function_container->execute = true;
						function_container->lengthStack = 0;
						
						for (i = 0; i < user_func->stack_copy; i++)
							pushDataToTop(function_container, popData(container));
						
						addInstructionsFromFunction(function_container, user_func);
						
						execute(function_container);
						container->lengthStack = lengthStack(container->data_stack);
						function_container->lengthStack = lengthStack(function_container->data_stack);
						pushNewFunctionData(container, function_container);
						cleanup(function_container);
						function_container = NULL;
					}
					else
					{
						addInstructionsFromFunctionInline(container, user_func, node);
					}

					user_func = NULL;
					break;
					
				case INST_USERFUNC_B:
					user_func = addFunction(container, node->raw, (int)node->number);
					removeInst(node);
					hit_function_end = false;
					while ((node = peekInst(container)))
					{
						if (node->instruction != INST_USERFUNC_E)
						{
							addFunctionInstruction(user_func, node);
							removeInst(node);
						}
						else
						{
							hit_function_end = true;
							break;
						}
					}
					if (hit_function_end == false)
					{
						error_msg("User function `%s` did not end properly. Remember the ending ']'.\n", user_func->name);
						break;
					}
					break;
					
				case INST_UNKNOWN:
				default:
					if (node->lengthRaw > 0)
					{
						error_msg("Unable to parse `%s'(%d) on line \"%s\".\n", node->raw, node->lengthRaw, node->line);
						//container->execute = false;
					}
			}
			removeInst(node);
			node = peekInst(container);
		}		
	}
}

void cleanup(rpn_container* container)
{
	pointToHead(container);
	pointToTop(container);
	
	freeQueue(container->inst_queue);
	container->inst_queue = NULL;
	freeStack(container->data_stack);
	container->data_stack = NULL;
	freeVariables(container->variable_list);
	free(container);
	container = NULL;
}
