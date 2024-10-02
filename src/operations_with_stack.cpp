#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../include/const_define_struct.h"
#include "../include/stk_error.h"
#include "../include/operations_with_stack.h"

//---------------------------------------------------------------------------------------------------------

errors_t stk_ctor (stk_t* ptr_stk, size_t start_capacity)
{
	assert (ptr_stk);

	printf ("Begin Ctor\n");

	(*ptr_stk).data     = (element_t*) calloc (start_capacity, sizeof (element_t));
	(*ptr_stk).size     = 0;
	(*ptr_stk).capacity = start_capacity;

	printf ("End Ctor\n");

	errors_t error_status = stk_error (ptr_stk);

	if (error_status  != NOT_ERROR)
	{
		printf ("We can create your stack. Please, try again.\n");
		printf ("This information can help your in find error.\n\n");

		printf ("start_capacity== %ld\n", start_capacity);
		stk_dump (ptr_stk, __FILE__, __LINE__);

		assert (NULL);

		return error_status;
	}

	return error_status;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_dtor (stk_t* ptr_stk)
{
	assert (ptr_stk);

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	free ((*ptr_stk).data);
	(*ptr_stk).size     = 0;
	(*ptr_stk).capacity = 0;

	return NOT_ERROR;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_dump (stk_t* ptr_stk, const char* file, const int line)
{
	assert (ptr_stk);
	assert (file);

	errors_t error_status = stk_error (ptr_stk);

	if (error_status == FALL_PTR_DATA)
	{
		printf ("ERROR   FALL_PTR_DATA   in file: %s, in line: %d\n", file, line);
		printf ("\t ptr_data == %p          \n", (*ptr_stk).data);
		printf ("\t size     == %ld         \n", (*ptr_stk).size);
		printf ("\t capacity == %ld         \n", (*ptr_stk).capacity);
		printf ("\t PTR_DATA == NULL\n\n");

		return FALL_PTR_DATA;
	}

	if (error_status == SIZE_MORE_CAPACITY)
	{
		printf ("ERROR   SIZE_MORE_CAPACITY  in file: %s, in line: %d\n", file, line);
		printf ("\t ptr_data == %p          \n", (*ptr_stk).data);
		printf ("\t size     == %ld         \n", (*ptr_stk).size);
		printf ("\t capacity == %ld         \n", (*ptr_stk).capacity);
		printf ("\t SIZE > CAPACITY\n\n");

		return SIZE_MORE_CAPACITY;
	}

	if (error_status == NOT_ERROR)
	{
		printf ("STACK print in file: %s, line %d\n", file, line);
		printf ("\t ptr_stk             == %p \n", ptr_stk            );
		printf ("\t ptr_data            == %p \n", (*ptr_stk).data    );
		printf ("\t len type of element == %ld\n", sizeof (element_t) );
		printf ("\t size                == %ld\n", (*ptr_stk).size    ); 
		printf ("\t capacity            == %ld\n", (*ptr_stk).capacity);
		printf ("\t elements of stack:\n");

		for (size_t index_element = 0; index_element < (*ptr_stk).size; index_element++)
		{
			printf ("\t \t [%ld] == %lg\n", index_element, *((*ptr_stk).data + index_element));
		}

		printf ("\n\n");

		return NOT_ERROR;
	}

	return error_status;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_pop (stk_t* ptr_stk, element_t* ptr_element)
{
	assert (ptr_stk);
	assert (ptr_element);

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	(*ptr_stk).size -= 1;
	*ptr_element     = *((*ptr_stk).data + (*ptr_stk).size);

	if ((*ptr_stk).size < ((*ptr_stk).capacity / 4))  
	{
		errors_t error_status = stk_realloc (ptr_stk, REVERSE_TRUE);

		if (error_status != NOT_ERROR) {return REALLOC_ERROR_BACK;}
	}

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	return NOT_ERROR;
}
//---------------------------------------------------------------------------------------------

errors_t stk_push (stk_t* ptr_stk, element_t element)
{
	assert (ptr_stk);

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	*((*ptr_stk).data + (*ptr_stk).size) = element;
	(  *ptr_stk).size                   += 1;

	if ((*ptr_stk).size == (*ptr_stk).capacity) 
	{
		errors_t error_status = stk_realloc (ptr_stk, REVERSE_FALSE);

		if (error_status != NOT_ERROR) {return REALLOC_ERROR_FORWARD;}
	}

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	return NOT_ERROR;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_realloc (stk_t* ptr_stk, mode_realloc_t reverse)
{
	assert (ptr_stk);

    STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	if (reverse == REVERSE_FALSE)
	{
		size_t new_capacity = (*ptr_stk).capacity * 2;

		(*ptr_stk).data = (element_t*) realloc ((*ptr_stk).data, new_capacity * sizeof (element_t));

		(*ptr_stk).capacity = new_capacity;

	}

	else
	{
		size_t new_capacity = (*ptr_stk).capacity / 4;

		(*ptr_stk).data = (element_t*) realloc ((*ptr_stk).data, new_capacity * sizeof (element_t));

		(*ptr_stk).capacity = new_capacity;
	}

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	return NOT_ERROR;
}
