#ifndef _RPN3_EXECUTE_H_
#define _RPN3_EXECUTE_H_

#include "error.h"
#include "rpn_container.h"
#include "instructions.h"
#include "display.h"
#include "tokenize.h"
#include "functions.h"

#include <math.h>

#define RPN_BUFFER_LENGTH 8192

void execute(rpn_container* container);
void cleanup(rpn_container* container);

#endif