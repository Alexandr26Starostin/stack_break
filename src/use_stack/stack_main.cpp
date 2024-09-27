#include <stdio.h>
#include <stdlib.h>

#include "struct_stack.h"
#include "errors_stack.h"

#include "stack_ctor.h"
#include "stack_push.h"
#include "stack_dump.h"
#include "stack_pop.h"
#include "stack_dtor.h"

int main ()
{
	struct stack_t stack = {};

	if (stack_ctor (&stack, 10)) {return HAVE_ERROR;}

	if (stack_dump (&stack)) {return HAVE_ERROR;}

	for (stack_element_t element = 1; element < 51; element++)
	{
		if (stack_push (&stack, element)) {return HAVE_ERROR;}
	}

	if (stack_dump (&stack)) {return HAVE_ERROR;}

	stack_element_t use = 0;

	for (size_t quantity_print = 1; quantity_print < 41; quantity_print++)
	{
		if (stack_pop (&stack, &use)) {return HAVE_ERROR;}

		printf ("element %ld:  %.0lf\n", quantity_print, use);
		if (stack_dump (&stack)) {return HAVE_ERROR;}
		printf ("\n");

	}

	if (stack_dump (&stack)) {return HAVE_ERROR;}

	if (stack_dtor (&stack)) {return HAVE_ERROR;}

	return NOT_ERROR;
}
