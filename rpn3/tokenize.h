#ifndef _RPN3_TOKENIZE_H_
#define _RPN3_TOKENIZE_H_

#include "rpn_container.h"
#include "instructions.h"

char* readLine(FILE* input, char* buffer, size_t buffer_length);
queue_node* tokenizeLine(rpn_container* container, char* line, queue_node* node);

#endif