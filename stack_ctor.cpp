#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "struct_stack.h"
#include "errors_stack.h"

#include "assert_stack.h"
#include "stack_ctor.h"

errors_stack_t stack_ctor (stack_t* ptr_stack, size_t start_copacity)
{
	assert (ptr_stack);

	(*ptr_stack).data     = (stack_element_t*) calloc (start_copacity, sizeof (stack_element_t));
	(*ptr_stack).size     = 0;
	(*ptr_stack).copacity = start_copacity;

	return assert_stack (ptr_stack, __FILE__, __LINE__);
}
