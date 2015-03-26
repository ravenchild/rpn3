#include "error.h"

int error_msg(const char *format, ...)
{
	va_list args;
	int res;
	
	va_start (args, format);
	res = vfprintf (stderr, format, args);
	va_end (args);
	return res;
}