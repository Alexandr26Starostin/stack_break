#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "errors_stack.h"
#include "struct_stack.h"

#include "assert_stack.h"
#include "mode_stack_realloc.h"
#include "stack_realloc.h"

errors_stack_t stack_realloc (stack_t* ptr_stack, mode_realloc_t reverse)
{
	assert (ptr_stack);

	errors_stack_t error_status = assert_stack (ptr_stack, __FILE__, __LINE__);

	if (error_status != NOT_ERROR) {return error_status;}

	if (reverse == REVERSE_FALSE)
	{
		size_t new_copacity = (*ptr_stack).copacity * 2;

		(*ptr_stack).data = (stack_element_t*) realloc ((*ptr_stack).data, new_copacity * sizeof (stack_element_t));

		(*ptr_stack).copacity = new_copacity;

	}

	else
	{
		size_t new_copacity = (*ptr_stack).copacity / 4;

		(*ptr_stack).data = (stack_element_t*) realloc ((*ptr_stack).data, new_copacity * sizeof (stack_element_t));

		(*ptr_stack).copacity = new_copacity;
	}

	return assert_stack (ptr_stack, __FILE__, __LINE__);
}
