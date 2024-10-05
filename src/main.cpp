#include <stdio.h>
#include <stdlib.h>

#include "../include/const_define_struct.h"
#include "../include/stk_error.h"
#include "../include/operations_with_stack.h"

int main ()
{
	struct stk_t stk = {};

	if (stk_ctor (&stk, 1)) {return CTOR_ERROR;}

	if (stk_dump (&stk, __FILE__, __LINE__)) {return DUMP_WRITE_ERROR;}

	for (element_t element = 1; element < 67; element++)
	{
		if (stk_push (&stk, element)) {return PUSH_ERROR;}
	}

	if (stk_dump (&stk, __FILE__, __LINE__)) {return DUMP_WRITE_ERROR;}
	
	element_t use = 0;

	for (size_t quantity_print = 1; quantity_print < 51; quantity_print++)
	{
		if (stk_pop (&stk, &use)) {return POP_ERROR;}

		if (stk_dump (&stk, __FILE__, __LINE__)) {return DUMP_WRITE_ERROR;}
	}

	if (stk_dump (&stk, __FILE__, __LINE__)) {return DUMP_WRITE_ERROR;}

	if (stk_dtor (&stk)) {return DTOR_ERROR;}

	return NOT_ERROR;
}
