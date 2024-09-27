#include <stdio.h>
#include <assert.h>

#include "errors_stack.h"
#include "struct_stack.h"

#include "stack_error.h"

errors_stack_t stack_error (stack_t* ptr_stack)
{
	assert (ptr_stack);
	
	errors_stack_t status_stack = NOT_ERROR;

	if ((*ptr_stack).data == NULL)
	{
		printf ("ptr_data == NULL\n");
		printf ("ptr_data == %p\n", (*ptr_stack).data);

		status_stack = HAVE_ERROR;
	}

	if ((*ptr_stack).size > (*ptr_stack).copacity)
	{
		printf ("size > copacity\n");
		printf ("size     == %ld\n"
				"copacity == %ld\n", 
				(*ptr_stack).size, (*ptr_stack).copacity);

		status_stack = HAVE_ERROR;
	}

	return status_stack;
}
