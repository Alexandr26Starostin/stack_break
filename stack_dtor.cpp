#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "struct_stack.h"
#include "errors_stack.h"

#include "assert_stack.h"
#include "stack_dtor.h"

errors_stack_t stack_dtor (stack_t* ptr_stack)
{
	assert (ptr_stack);

	errors_stack_t error_status = assert_stack (ptr_stack, __FILE__, __LINE__);

	if (error_status != NOT_ERROR) {return error_status;}

	free ((*ptr_stack).data);
	(*ptr_stack).size     = 0;
	(*ptr_stack).copacity = 0;

	return error_status;
}
