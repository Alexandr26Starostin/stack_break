#include <stdio.h>
#include <assert.h>

#include "errors_stack.h"
#include "struct_stack.h"

#include "assert_stack.h"
#include "stack_dump.h"

errors_stack_t stack_dump (stack_t* ptr_stack)
{
	assert (ptr_stack);

	errors_stack_t error_status = assert_stack (ptr_stack, __FILE__, __LINE__);

	if (error_status != NOT_ERROR) {return error_status;}

	printf ("ptr_stack           = %p\n"
	        "ptr_stack.data      = %p\n"
			"len type of element = %ld\n"
			"stack.size          = %ld\n"
			"stack.copacity      = %ld\n",
			 ptr_stack, (*ptr_stack).data, sizeof (stack_element_t), (*ptr_stack).size, (*ptr_stack).copacity);
	
	printf ("\n");

	return error_status;
}
