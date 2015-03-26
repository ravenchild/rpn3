#ifndef _RPN3_H_
#define _RPN3_H_

#include "rpn_container.h"
#include "instructions.h"
#include "tokenize.h"
#include "execute.h"
#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
typedef int _bool;
#define false 0
#define true 1
#endif

// Functions
void handleArgs(int argc, char* argv[], rpn_container* container);
void promptLine(rpn_container* container);

#endif
