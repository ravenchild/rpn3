#include "tokenize.h"

char* readLine(FILE* input, char* buffer, size_t buffer_length)
{
	char *line;

	if (feof(input))
		return NULL;
	
	line = fgets(buffer, (int) buffer_length, input);
	
	if (line != NULL)
	{
		size_t last = strlen(buffer);
		if (last > 0)
			if (buffer[last-1] == '\n' || buffer[last-1] == '\r')
				buffer[last-1] = '\0';
	}
	return line;
}

queue_node* tokenizeLine(rpn_container* container, char* line, queue_node* node)
{
	char* line_backup;
	char* inst_raw;
	char* inst_line;
	char* token = NULL;
	char token_stop[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,32,59,0};
	queue_node* after_node = node;
	size_t i = 0, commentStart = -1;
	size_t token_length = 0, length = 0;

	if (line == NULL)
		return NULL;
	
	length = strlen(line);
	
	if (line == NULL)
		return node;
	if (length < 1)
		return node;
	
	if (line[length - 1] == '\n')
		line[length - 1] = '\0';
	
	line_backup = (char*) malloc(sizeof(char) * (length + 1));
	memcpy(line_backup, line, sizeof(char) * (length + 1));
	
	for (;i<length; i++)
	{
		if (PARSE_MAP[(int)line[i]] == PARSE_POUND)
		{
			line[i] = '\0';
			commentStart = i + 1;
		}
	}
	
	token = strtok(line, token_stop);
	while (token != NULL)
	{
		token_length = strlen(token);
		inst_raw = (char*) malloc(sizeof(char) * (token_length + 1));
		memcpy(inst_raw, token, sizeof(char) * (token_length + 1));
		inst_line = (char*) malloc(sizeof(char) * (length + 1));
		memcpy(inst_line, line_backup, sizeof(char) * (length + 1));
		
		if (node == NULL)
			putInst(container, inst_raw, inst_line, 0.0f, INST_NONE, true);
		else
			after_node = insertAfter(container, after_node, inst_raw, inst_line, 0.0f, INST_NONE, true);
		
		token = strtok(NULL, token_stop);
	}
	
	if (commentStart != (size_t)(-1))
	{
		token_length = strlen(line + commentStart);
		inst_raw = (char*) malloc(sizeof(char) * (token_length + 1));
		memcpy(inst_raw, line + commentStart, sizeof(char) * (token_length + 1));
		inst_line = (char*) malloc(sizeof(char) * (length + 1));
		memcpy(inst_line, line_backup, sizeof(char) * (length + 1));
		
		if (node == NULL)
			putInst(container, inst_raw, inst_line, 0.0f, INST_COMMENT, true);
		else
			after_node = insertAfter(container, after_node, inst_raw, inst_line, 0.0f, INST_COMMENT, true);
	}
	
	if (line_backup != NULL)
		free(line_backup);
	line_backup = NULL;
	
	return after_node;
}