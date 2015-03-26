#include "main.h"

int main(int argc, char* argv[])
{
	// Initiate status
	// Make the instruction stack and the data stack.
	rpn_container* RPNStack = (rpn_container*) malloc(sizeof(rpn_container));
	RPNStack->inst_queue = NULL;
	RPNStack->data_stack = NULL;
	RPNStack->variable_list = NULL;
	RPNStack->function_list = NULL;
	RPNStack->running = true;
	RPNStack->execute = true;
	RPNStack->lengthStack = 0;
	
	handleArgs(argc, argv, RPNStack);
	execute(RPNStack);
	
	while (RPNStack->running)
	{	// Run main loop.
		// Get new input line.
		promptLine(RPNStack);
		execute(RPNStack);
	}
	
	freeFunctions(RPNStack->function_list);
	cleanup(RPNStack);
	RPNStack = NULL;
	return 0;
}

void handleArgs(int argc, char* argv[], rpn_container* container)
{
	char* commandBuffer;
	size_t* allLengths;
	
	size_t commandLength = 0;
	int currentArg = 1;
	size_t bufferPos = 0;
	
	allLengths = (size_t*) malloc(sizeof(size_t) * (argc - 1));
	if (allLengths == NULL)
	{
		error_msg("Memory error, could not allocate buffer.");
		container->running = false;
		return;
	}
	
	for(currentArg = 1; currentArg < argc; currentArg++)
		commandLength += (allLengths[currentArg - 1] = strlen(argv[currentArg])) + 1; // Add one for the separating space.
	
	commandBuffer = (char*) malloc(sizeof(char) * (commandLength + 1));
	if (commandBuffer == NULL)
	{
		error_msg("Memory error, could not allocate buffer.");
		container->running = false;
		free(allLengths);
		allLengths = NULL;
		return;
	}
	
	for(currentArg = 1; currentArg < argc; currentArg++)
	{
		memcpy(commandBuffer + bufferPos, argv[currentArg], allLengths[currentArg - 1]);
		bufferPos += allLengths[currentArg - 1];
		commandBuffer[bufferPos++] = (currentArg == argc - 1) ? '\n' : ' ';
	}

	tokenizeLine(container, commandBuffer, NULL);
	
	free(allLengths);
	allLengths = NULL;
	free(commandBuffer);
	commandBuffer = NULL;
	
}

void promptLine(rpn_container* container)
{
	char* buffer = (char*) malloc(sizeof(char) * (RPN_BUFFER_LENGTH + 1));
	char* line = readLine(stdin, buffer, RPN_BUFFER_LENGTH);
	tokenizeLine(container, line, NULL);
	
	free(buffer);
	buffer = NULL;
}
