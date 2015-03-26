#include "instructions.h"

void parseTokens(rpn_container* container)
{
	queue_node* node = container->inst_queue;
	
	if (node == NULL)
		return;
	
	for (;node->prev != NULL;)
		node = node->prev;
	
	for (;node != NULL;)
	{
		if (node->raw[0] == 0)
		{
			node = node->next;
			continue;
		}
		
		// Parse it.
		if (node->instruction == INST_NONE)
		{
			//TODO: Decide how I want to do this.
			//splitSpecials(container, node);
			
			switch (PARSE_MAP[(int)node->raw[0]])
			{
			
				// Operators
				case PARSE_DASH:
					node = parseDash(container, node);
					break;
					
				case PARSE_MULTIPLY:
					node = parseMultiply(container, node);
					break;
				
				case PARSE_GREATER:
				case PARSE_LESS:
				case PARSE_QUESTION:
				case PARSE_EQUALS:
				case PARSE_PLUS:
				case PARSE_DIVIDE:
				case PARSE_CARROT:
					node = parseOperator(container, node);
					break;
					
				// Files
				case PARSE_PERCENT:
					node = parseFile(container, node);
					break;
				
				// Variables
				case PARSE_COLON:
				case PARSE_DOLLAR:
					node = parseVariable(container, node);
					break;
					
				// Numbers
				case PARSE_PERIOD:
				case PARSE_NUMBER:
					node = parseNumber(container, node, false);
					break;
					
				// User functions
				case PARSE_BRACKET_L:
				case PARSE_BRACKET_R:
					node = parseUserFunction(container, node);
					break;
					
				// Functions
				case PARSE_LETTER:
				case PARSE_UNDERSCORE:
					node = parseFunction(container, node);
					break;
				
				// Issue characters (Ones that shouldn't be used yet)
				case PARSE_SEMICOLON:
				case PARSE_SPACE:
				case PARSE_UNUSED:
				default:
					node->instruction = INST_UNKNOWN;
					break;
			}
		}
		
		// else, go to the next
		node = node->next;
	}
}

void splitSpecials(rpn_container* container, queue_node* node)
{
	size_t i = 0u, length = node->lengthRaw;
	i = length - 1;
	
	if (length <= 2)
		return;

	for (;i > 0;i--)
	{
		switch (PARSE_MAP[(int)node->raw[i]])
		{
			case PARSE_EQUALS:
				if (PARSE_MAP[(int)node->raw[i-1]] == PARSE_GREATER || PARSE_MAP[(int)node->raw[i-1]] == PARSE_LESS)
					i--;
			case PARSE_DASH:
			case PARSE_MULTIPLY:
			case PARSE_GREATER:
			case PARSE_LESS:
			case PARSE_QUESTION:
			case PARSE_PLUS:
			case PARSE_DIVIDE:
			case PARSE_CARROT:
			case PARSE_DOLLAR:
			case PARSE_AT:
				splitToken(container, node, i, length);
				return;
		}
	}
}

void splitToken(rpn_container* container, queue_node* node, size_t i, size_t length)
{
	char* old_raw = node->raw;
	char* new_raw;
	char* next_raw;
	char* next_line;
		
	new_raw = (char*) malloc(sizeof(char) * (i+1));
	memcpy(new_raw, old_raw, i);
	new_raw[i] = 0;
	node->lengthRaw = i;
	
	next_raw = (char*) malloc(sizeof(char) * (length - i + 1));
	memcpy(next_raw, old_raw + i, length - i);
	next_raw[length - i] = 0;
	
	next_line = (char*) malloc(sizeof(char) * (node->lengthLine + 1));
	memcpy(next_line, node->line, node->lengthLine);
	next_line[node->lengthLine] = 0;
	
	free(node->raw);
	node->raw = new_raw;
	
	insertAfter(container, node, next_raw, next_line, 0.0f, INST_NONE, true);
}

queue_node* parseHelp(rpn_container* container, queue_node* node)
{
	if (node->lengthRaw > 1)
		splitToken(container, node, 1, node->lengthRaw);
	
	node->instruction = INST_HELP;
	
	return node;
}

queue_node* parseDash(rpn_container* container, queue_node* node)
{
	if (node->lengthRaw > 1)
	{
		
		if (PARSE_MAP[(int)node->raw[1]] == PARSE_NUMBER || PARSE_MAP[(int)node->raw[1]] == PARSE_PERIOD)
		{
			splitToken(container, node, 1, node->lengthRaw);
			node = node->next;
			removeInst(node->prev);
			return parseNumber(container, node, true);
		}
		splitToken(container, node, 1, node->lengthRaw);
		
	}
	
	node->instruction = INST_SUBTRACT;
	
	return node;
}

queue_node* parseMultiply(rpn_container* container, queue_node* node)
{
	if (node->lengthRaw > 1)
		splitToken(container, node, 1, node->lengthRaw);
	
	node->instruction = INST_MULTIPLY;
	
	return node;
}

queue_node* parseOperator(rpn_container* container, queue_node* node)
{
	if (node->lengthRaw > 1)
		splitToken(container, 
				   node, 
				   (node->raw[1] == '=' && (node->raw[0] == '<' || node->raw[0] == '>')) 
						? 2 
						: 1, 
				   node->lengthRaw);
	
	switch (node->raw[0])
	{
		case '+':
			node->instruction = INST_ADD;
			break;
		case '/':
			node->instruction = INST_DIVIDE;
			break;
		case '^':
			node->instruction = INST_FUNC_POW;
			break;
		case '?':
			node->instruction = INST_QUESTION;
			break;
		case '=':
			node->instruction = INST_EQUAL;
			break;
		case '<':
			node->instruction = INST_LT;
			if (node->raw[1] == '=')
				node->instruction = INST_LTE;
			break;
		case '>':
			node->instruction = INST_GT;
			if (node->raw[1] == '=')
				node->instruction = INST_GTE;
			break;
		default:
			node->instruction = INST_UNKNOWN;
	}
	
	return node;
}

queue_node* parseFile(rpn_container* container, queue_node* node)
{
	node->instruction = INST_FILE;
	return node;
}

queue_node* parseVariable(rpn_container* container, queue_node* node)
{
	splitToken(container, node, 1, node->lengthRaw);
	
	if (node->raw[0] == '$')
		node->next->instruction = INST_VAR;
	else
		node->next->instruction = INST_SETVAR;
	
	node = node->next;
	removeInst(node->prev);
	
	return node;
}

queue_node* parseUserFunction(rpn_container* container, queue_node* node)
{
	char* func_name = NULL;
	size_t func_name_length = 0;
	char* func_stack_raw = NULL;
	int func_stack;
	size_t func_stack_length = 0;
	size_t pos = 0, name_pos = 0;
	
	if (PARSE_MAP[(int)node->raw[0]] == PARSE_BRACKET_R)
	{
		if (node->lengthRaw > 1)
			splitToken(container, node, 1, node->lengthRaw);
		node->instruction = INST_USERFUNC_E;
		return node;
	}
	
	for (;pos < node->lengthRaw; pos++)
		if (PARSE_MAP[(int)node->raw[pos]] == PARSE_COLON)
			break;
	
	if (pos > 1 && pos < node->lengthRaw - 1)
	{
		func_name = (char*) malloc(sizeof(char) * (pos));
		memcpy(func_name, node->raw + 1, pos - 1);
		func_name[pos - 1] = 0;
		func_name_length = pos - 1;
		
		if (PARSE_MAP[(int)func_name[0]] != PARSE_LETTER && PARSE_MAP[(int)func_name[0]] != PARSE_UNDERSCORE)
			error_msg("User functions should start with a letter or an underscore.\n");
		name_pos = strspn(func_name, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
		if (name_pos != func_name_length)
			error_msg("Invalid function name \"%s\" at character '%c'.\n", func_name, func_name[name_pos]);
		
		pos++;
		for (;pos < node->lengthRaw; pos++)
			if (PARSE_MAP[(int)node->raw[pos]] != PARSE_NUMBER)
				break;
		
		if (pos == 2 + func_name_length) //The stack copy length is not defined.
			func_stack = 0;
		else //Determine the stack copy length
		{
			func_stack_length = pos - (2 + func_name_length);
			func_stack_raw = (char*) malloc(sizeof(char) * (func_stack_length + 1) );
			memcpy(func_stack_raw, node->raw + 2 + func_name_length, func_stack_length);
			func_stack_raw[func_stack_length] = 0;
			
			func_stack = atoi(func_stack_raw);
			
			free(func_stack_raw);
			func_stack_raw = NULL;
		}
		
		if (pos < node->lengthRaw)
			splitToken(container, node, pos, node->lengthRaw);
		
		node->instruction = INST_USERFUNC_B;
		free(node->raw);
		node->raw = func_name;
		node->number = func_stack;
		// Create placeholder function.
		addFunction(container, node->raw, func_stack);
	}
	else
	{
		node->instruction = INST_UNKNOWN;
	}
	
	return node;
}

queue_node* parseFunction(rpn_container* container, queue_node* node)
{
	function_node* user_func;
	int i = 0;

	for (i = 0; i < node->lengthRaw; i++)
		if (PARSE_MAP[(int)node->raw[i]] != PARSE_LETTER && PARSE_MAP[(int)node->raw[i]] != PARSE_UNDERSCORE)
			break;
	
	//If i doesn't equal the length then there are more things to parse.  Make a new token.
	if (i != node->lengthRaw)
		splitToken(container, node, i, node->lengthRaw);

	//To lower.
	//for (i = 0; i < length; i++)
	//	node->raw[i] += (node->raw[i] >= 'A' && node->raw[i] <= 'Z' ? 32 : 0); 
	
	user_func = getFunction(container, node->raw);
	if (user_func)
	{
		node->instruction = INST_USERFUNC;
		node->number = user_func->stack_copy;
		return node;
	}
	
	switch (node->lengthRaw)
	{
		case 1:
			switch (node->raw[0])
			{
				case 's':
					node->instruction = INST_SWAP;
					break;
				case 'o':
					node->instruction = INST_POP;
					break;
				case 'p':
					node->instruction = INST_PRINT;
					break;
				case 'l':
					node->instruction = INST_STACK_LEN;
					break;
				case 'd':
					node->instruction = INST_DISPLAY;
					break;
				case 'a':
					node->instruction = INST_DISPLAY_ALL;
					break;
				case 'q':
					node->instruction = INST_QUIT;
					break;
				case 'x':
					node->instruction = INST_CLEAR;
					break;
				case 'h':
					node->instruction = INST_HELP;
					break;
				case 'e':
					node->instruction = INST_CONST_E;
					break;
				case 'c':
					node->instruction = INST_COPY;
					break;
			}
			break;
		case 2:
			if (strcmp("ln", node->raw) == 0)
				node->instruction = INST_FUNC_LN;
			else if (strcmp("pi", node->raw) == 0)
				node->instruction = INST_CONST_PI;
			break;
		case 3:
			if (strcmp("pow", node->raw) == 0)
				node->instruction = INST_FUNC_POW;
			else if (strcmp("mod", node->raw) == 0)
				node->instruction = INST_FUNC_MOD;
			else if (strcmp("sin", node->raw) == 0)
				node->instruction = INST_FUNC_SIN;
			else if (strcmp("cos", node->raw) == 0)
				node->instruction = INST_FUNC_COS;
			else if (strcmp("tan", node->raw) == 0)
				node->instruction = INST_FUNC_TAN;
			else if (strcmp("log", node->raw) == 0)
				node->instruction = INST_FUNC_LOG;
			else if (strcmp("exp", node->raw) == 0)
				node->instruction = INST_FUNC_EXP;
			else if (strcmp("abs", node->raw) == 0)
				node->instruction = INST_FUNC_ABS;
			break;
		case 4:
			if (strcmp("ceil", node->raw) == 0)
				node->instruction = INST_FUNC_CEIL;
			else if (strcmp("sqrt", node->raw) == 0)
				node->instruction = INST_FUNC_SQRT;
			else if (strcmp("sinh", node->raw) == 0)
				node->instruction = INST_FUNC_SINH;
			else if (strcmp("cosh", node->raw) == 0)
				node->instruction = INST_FUNC_COSH;
			else if (strcmp("tanh", node->raw) == 0)
				node->instruction = INST_FUNC_TANH;
			break;
		case 5:
			if (strcmp("floor", node->raw) == 0)
				node->instruction = INST_FUNC_FLOOR;
			break;
	}
	
	if (node->instruction == INST_NONE && node->lengthRaw > 1)
	{
		splitToken(container, node, 1, node->lengthRaw);
		node = parseFunction(container, node);
	}
	else if (node->instruction == INST_NONE && node->lengthRaw == 1)
		node->instruction = INST_UNKNOWN;
	
	return node;
}


queue_node* parseNumber(rpn_container* container, queue_node* node, _bool negative)
{
	int i;
	_bool dot = false;
	
	for (i = 0; i < node->lengthRaw; i++)
	{
		if (PARSE_MAP[(int)node->raw[i]] == PARSE_NUMBER)
		{
			
		}
		else if (PARSE_MAP[(int)node->raw[i]] == PARSE_PERIOD)
		{
			if (dot)
				break;
			dot = true;
		}
		else
			break;
	}
	
	//If i doesn't equal the length then there are more things to parse.  Make a new token.
	if (i != node->lengthRaw)
		splitToken(container, node, i, node->lengthRaw);

	node->instruction = INST_DATA;
	node->number = atof(node->raw);
	if (negative)
		node->number = node->number * -1.0f;
	
	return node;
}

unsigned int PARSE_MAP[128] = { 
	PARSE_UNUSED,//PARSE_FINISH,		// 0
	PARSE_UNUSED,		// 1
	PARSE_UNUSED,		// 2
	PARSE_UNUSED,		// 3
	PARSE_UNUSED,		// 4
	PARSE_UNUSED,		// 5
	PARSE_UNUSED,		// 6
	PARSE_UNUSED,		// 7
	PARSE_UNUSED,		// 8
	PARSE_UNUSED,		// 9
	PARSE_UNUSED,		// 10
	PARSE_UNUSED,		// 11
	PARSE_UNUSED,		// 12
	PARSE_UNUSED,		// 13
	PARSE_UNUSED,		// 14
	PARSE_UNUSED,		// 15
	PARSE_UNUSED,		// 16
	PARSE_UNUSED,		// 17
	PARSE_UNUSED,		// 18
	PARSE_UNUSED,		// 19
	PARSE_UNUSED,		// 20
	PARSE_UNUSED,		// 21
	PARSE_UNUSED,		// 22
	PARSE_UNUSED,		// 23
	PARSE_UNUSED,		// 24
	PARSE_UNUSED,		// 25
	PARSE_UNUSED,		// 26
	PARSE_UNUSED,		// 27
	PARSE_UNUSED,		// 28
	PARSE_UNUSED,		// 29
	PARSE_UNUSED,		// 30
	PARSE_UNUSED,		// 31
	
	PARSE_SPACE,		// 32
	PARSE_UNUSED,		// 33
	PARSE_UNUSED,		// 34
	PARSE_POUND,		// 35
	PARSE_DOLLAR,		// 36
	PARSE_PERCENT,		// 37
	PARSE_UNUSED,		// 38
	PARSE_UNUSED,		// 39
	PARSE_UNUSED,		// 40
	PARSE_UNUSED,		// 41
	PARSE_MULTIPLY,		// 42
	PARSE_PLUS,			// 43
	PARSE_UNUSED,		// 44
	PARSE_DASH,			// 45
	PARSE_PERIOD,		// 46
	PARSE_DIVIDE,		// 47
	PARSE_NUMBER,		// 48
	PARSE_NUMBER,		// 49
	PARSE_NUMBER,		// 50
	PARSE_NUMBER,		// 51
	PARSE_NUMBER,		// 52
	PARSE_NUMBER,		// 53
	PARSE_NUMBER,		// 54
	PARSE_NUMBER,		// 55
	PARSE_NUMBER,		// 56
	PARSE_NUMBER,		// 57
	PARSE_COLON,		// 58
	PARSE_SEMICOLON,	// 59
	PARSE_LESS,			// 60
	PARSE_EQUALS,		// 61
	PARSE_GREATER,		// 62
	PARSE_QUESTION,		// 63
	
	PARSE_AT,			// 64
	PARSE_LETTER,		// 65
	PARSE_LETTER,		// 66
	PARSE_LETTER,		// 67
	PARSE_LETTER,		// 68
	PARSE_LETTER,		// 69
	PARSE_LETTER,		// 70
	PARSE_LETTER,		// 71
	PARSE_LETTER,		// 72
	PARSE_LETTER,		// 73
	PARSE_LETTER,		// 74
	PARSE_LETTER,		// 75
	PARSE_LETTER,		// 76
	PARSE_LETTER,		// 77
	PARSE_LETTER,		// 78
	PARSE_LETTER,		// 79
	PARSE_LETTER,		// 80
	PARSE_LETTER,		// 81
	PARSE_LETTER,		// 82
	PARSE_LETTER,		// 83
	PARSE_LETTER,		// 84
	PARSE_LETTER,		// 85
	PARSE_LETTER,		// 86
	PARSE_LETTER,		// 87
	PARSE_LETTER,		// 88
	PARSE_LETTER,		// 89
	PARSE_LETTER,		// 90
	PARSE_BRACKET_L,	// 91
	PARSE_UNUSED,		// 92
	PARSE_BRACKET_R,	// 93
	PARSE_CARROT,		// 94
	PARSE_UNDERSCORE,	// 95
	
	PARSE_UNUSED,		// 96
	PARSE_LETTER,		// 97
	PARSE_LETTER,		// 98
	PARSE_LETTER,		// 99
	PARSE_LETTER,		// 100
	PARSE_LETTER,		// 101
	PARSE_LETTER,		// 102
	PARSE_LETTER,		// 103
	PARSE_LETTER,		// 104
	PARSE_LETTER,		// 105
	PARSE_LETTER,		// 106
	PARSE_LETTER,		// 107
	PARSE_LETTER,		// 108
	PARSE_LETTER,		// 109
	PARSE_LETTER,		// 110
	PARSE_LETTER,		// 111
	PARSE_LETTER,		// 112
	PARSE_LETTER,		// 113
	PARSE_LETTER,		// 114
	PARSE_LETTER,		// 115
	PARSE_LETTER,		// 116
	PARSE_LETTER,		// 117
	PARSE_LETTER,		// 118
	PARSE_LETTER,		// 119
	PARSE_LETTER,		// 120
	PARSE_LETTER,		// 121
	PARSE_LETTER,		// 122
	PARSE_UNUSED,		// 123
	PARSE_UNUSED,		// 124
	PARSE_UNUSED,		// 125
	PARSE_UNUSED,		// 126
	PARSE_UNUSED		// 127
};


