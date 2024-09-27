#include <stdio.h>
#include <assert.h>

#include "errors_stack.h"
#include "struct_stack.h"

#include "assert_stack.h"
#include "stack_realloc.h"
#include "mode_stack_realloc.h"
#include "stack_push.h"

errors_stack_t stack_push (stack_t* ptr_stack, stack_element_t element)
{
	assert (ptr_stack);

	errors_stack_t error_status = assert_stack (ptr_stack, __FILE__, __LINE__);

	if (error_status != NOT_ERROR) {return error_status;}

	*((*ptr_stack).data + (*ptr_stack).size) = element;
	(*ptr_stack).size          += 1;

	if ((*ptr_stack).size == (*ptr_stack).copacity) 
	{
		error_status = stack_realloc (ptr_stack, REVERSE_FALSE);

		if (error_status != NOT_ERROR) {return error_status;}
	}

	return assert_stack (ptr_stack, __FILE__, __LINE__);
}
