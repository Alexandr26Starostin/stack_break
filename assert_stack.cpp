#include <stdio.h>
#include <assert.h>

#include "errors_stack.h"
#include "struct_stack.h"

#include "stack_error.h"
#include "assert_stack.h"

errors_stack_t assert_stack (stack_t* ptr_stack, const char* file, const int line)
{
	assert (ptr_stack);
	assert (file);

	errors_stack_t error_status = stack_error (ptr_stack);

	if (error_status != NOT_ERROR)
	{
		printf ("Error in file: %s; in line: %d\n", file, line);
	}

	return error_status;
}
