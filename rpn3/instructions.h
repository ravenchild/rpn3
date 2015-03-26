#ifndef _RPN3_INSTRUCTIONS_H_
#define _RPN3_INSTRUCTIONS_H_

#include "rpn_container.h"
#include "functions.h"
#include "error.h"
#include <stdio.h>
#include <string.h>

#ifdef WIN32
typedef int _bool;
#define false 0
#define true 1
#endif

// Instructions

#define INST_NONE		0
#define INST_UNKNOWN	1
#define INST_VAR		2
#define INST_DATA		3
#define INST_ADD		4
#define INST_SUBTRACT	5
#define INST_MULTIPLY	6
#define INST_DIVIDE		7
#define INST_SWAP		8
#define INST_POP		9
#define INST_DISPLAY	10
#define INST_CLEAR		11
#define	INST_HELP		12
#define INST_FUNC_POW	13
#define INST_FUNC_MOD	14
#define INST_FUNC_SIN	15
#define INST_FUNC_COS	16
#define INST_FUNC_TAN	17
#define INST_FUNC_SINH	18
#define INST_FUNC_COSH	19
#define INST_FUNC_TANH	20
#define INST_FUNC_LN	21
#define INST_FUNC_LOG	22
#define INST_FUNC_EXP	23
#define INST_FUNC_ABS	24
#define INST_FUNC_FLOOR	25
#define INST_FUNC_CEIL	26
#define INST_FUNC_SQRT	27
#define INST_CONST_E	28
#define INST_CONST_PI	29
#define INST_QUIT		30
#define INST_EXECUTE	31
#define INST_DISPLAY_ALL 32
#define INST_FILE		33
#define INST_GT			34
#define INST_LT			35
#define INST_GTE		36
#define INST_LTE		37
#define INST_EQUAL		38
#define INST_QUESTION	39
#define INST_COPY		40
#define INST_COMMENT	41
#define INST_SETVAR		42
#define INST_USERFUNC	43
#define INST_USERFUNC_B	44
#define INST_USERFUNC_E	45
#define INST_STACK_LEN	46
#define INST_PRINT		47

//#define PARSE_FINISH		0
//#define PARSE_NEXT			1
#define PARSE_NUMBER		2
#define PARSE_LETTER		3
#define PARSE_DASH			4
#define PARSE_PLUS			5
#define PARSE_DIVIDE		6
#define PARSE_MULTIPLY		7
#define PARSE_DOLLAR		8
#define PARSE_COLON			9
#define PARSE_PERIOD		10
#define PARSE_SPACE			11
#define PARSE_UNUSED		12
#define PARSE_SEMICOLON		13
#define PARSE_EQUALS		14
#define PARSE_QUESTION		15
#define PARSE_UNDERSCORE	16
#define PARSE_CARROT		17
#define PARSE_PERCENT		18
#define PARSE_GREATER		19
#define PARSE_LESS			20
#define PARSE_POUND			21
#define PARSE_AT			22
#define PARSE_BRACKET_L		23
#define PARSE_BRACKET_R		24


extern unsigned int PARSE_MAP[128];

void parseTokens(rpn_container* container);
void splitToken(rpn_container* container, queue_node* node, size_t i, size_t length);
void splitSpecials(rpn_container* container, queue_node* node);

queue_node* parseHelp(rpn_container* container, queue_node* node);
queue_node* parseDash(rpn_container* container, queue_node* node);
queue_node* parseMultiply(rpn_container* container, queue_node* node);
queue_node* parseOperator(rpn_container* container, queue_node* node);
queue_node* parseFile(rpn_container* container, queue_node* node);
queue_node* parseVariable(rpn_container* container, queue_node* node);
queue_node* parseUserFunction(rpn_container* container, queue_node* node);
queue_node* parseFunction(rpn_container* container, queue_node* node);
queue_node* parseNumber(rpn_container* container, queue_node* node, _bool negative);

#endif
