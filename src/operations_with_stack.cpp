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

	#ifdef CANARY_STK 
		(*ptr_stk).canary_1 = canary;
		(*ptr_stk).canary_2 = canary;
	#endif

	#ifdef CANARY_STK_DATA
		size_t place_canary_data = 2;

	#else
		size_t place_canary_data = 0;
	#endif

	element_t* ptr_memory   = (element_t*) calloc (start_capacity + place_canary_data, sizeof (element_t));
	(*ptr_stk).size     = 0;
	(*ptr_stk).capacity = start_capacity;

	#ifdef CANARY_STK_DATA
		(*ptr_stk).data = ptr_memory + 1;

		*ptr_memory                             = canary;
		*(ptr_memory + 1 + (*ptr_stk).capacity) = canary;

	#else 
		(*ptr_stk).data = ptr_memory;
	
	#endif

	printf ("End Ctor\n\n");

	errors_t error_status = stk_error (ptr_stk);

	if (error_status != NOT_ERROR)
	{
		printf ("We can not create your stack. Please, try again.\n");
		printf ("This information can help your in find error.\n\n");

		printf ("start_capacity == %ld\n", start_capacity);
		stk_dump (ptr_stk, __FILE__, __LINE__);

		return error_status;
	}

	return error_status;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_dtor (stk_t* ptr_stk)
{
	assert (ptr_stk);

	(*ptr_stk).size     = 0;
	(*ptr_stk).capacity = 0;
	free ((*ptr_stk).data - 1);
	
	return NOT_ERROR;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_dump (stk_t* ptr_stk, const char* file, const int line)
{
	assert (ptr_stk);
	assert (file);

	errors_t error_status = stk_error (ptr_stk);
    
	//--------------------------------------------------------------------------------------------------------------

	if (error_status == FALL_PTR_DATA)
	{
		printf ("ERROR   FALL_PTR_DATA   in file: %s, in line: %d\n", file, line);
		printf ("\t ptr_data == %p          \n", (*ptr_stk).data);
		printf ("\t size     == %x          \n", (*ptr_stk).size);
		printf ("\t capacity == %x          \n", (*ptr_stk).capacity);

		#ifdef CANARY_STK 
			printf ("\t canary_1 == %x\n", (*ptr_stk).canary_1);
			printf ("\t canary_2 == %x\n", (*ptr_stk).canary_2);
	    #endif

		printf ("\t PTR_DATA == NULL\n\n");

		return FALL_PTR_DATA;
	}

	//------------------------------------------------------------------------------------------------------------------

	if (error_status == SIZE_MORE_CAPACITY)
	{
		printf ("ERROR   SIZE_MORE_CAPACITY  in file: %s, in line: %d\n", file, line);
		printf ("\t ptr_data == %p          \n", (*ptr_stk).data);
		printf ("\t size     == %ld         \n", (*ptr_stk).size);
		printf ("\t capacity == %ld         \n", (*ptr_stk).capacity);

		#ifdef CANARY_STK 
			printf ("\t canary_1 == %x\n", (*ptr_stk).canary_1);
			printf ("\t canary_2 == %x\n", (*ptr_stk).canary_2);
	    #endif

		printf ("\t SIZE > CAPACITY\n\n");

		return SIZE_MORE_CAPACITY;
	}
    
	//--------------------------------------------------------------------------------------------------------------------

	#ifdef CANARY_STK 
		if (error_status == CANARY_STK_ERROR)
		{
			printf ("ERROR   CANARY_STK_ERROR  in file: %s, in line: %d\n", file, line);
			printf ("\t ptr_data == %p          \n", (*ptr_stk).data);
			printf ("\t size     == %ld         \n", (*ptr_stk).size);
			printf ("\t capacity == %ld         \n", (*ptr_stk).capacity);

			printf ("\n");
			
			printf ("\t canary_true == %x\n", canary);
			printf ("\t canary_1    == %x\n", (*ptr_stk).canary_1);
			printf ("\t canary_2    == %x\n", (*ptr_stk).canary_2);

			printf ("\t canary_1 or canary_2 != canary_true\n\n");

			return CANARY_STK_ERROR;
		}
	#endif

	//------------------------------------------------------------------------------------------------------------------

	if (error_status == NOT_ERROR)
	{
		printf ("STACK print in file: %s, line %d\n", file, line);
		printf ("\t ptr_stk             == %p \n", ptr_stk            );
		printf ("\t ptr_data            == %p \n", (*ptr_stk).data    );
		printf ("\t len type of element == %ld\n", sizeof (element_t) );
		printf ("\t size                == %ld\n", (*ptr_stk).size    ); 
		printf ("\t capacity            == %ld\n\n", (*ptr_stk).capacity);

		#ifdef CANARY_STK 
			printf ("\t canary_1 == %x\n",   (*ptr_stk).canary_1);
			printf ("\t canary_2 == %x\n\n", (*ptr_stk).canary_2);
	    #endif

		#ifdef CANARY_STK_DATA

			canary_t canary_3 = *((*ptr_stk).data - 1);
			canary_t canary_4 = *((*ptr_stk).data + (*ptr_stk).capacity);

			printf ("\t canary_3 == %x\n", canary_3);
			printf ("\t canary_4 == %x\n", canary_4);

		#endif

		printf ("\t elements of stack:\n");

		for (size_t index_element = 0; index_element < (*ptr_stk).size; index_element++)
		{
			printf ("\t \t [%ld] == %d\n", index_element, *((*ptr_stk).data + index_element));
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
//-------------------------------------------------------------------------------------------------------

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
		#ifdef CANARY_STK_DATA
			size_t place_canary_data = 2;

		#else
			size_t place_canary_data = 0;
		#endif

		size_t new_capacity = (*ptr_stk).capacity * 2;

		element_t* ptr_memory = (element_t*) realloc ((*ptr_stk).data - 1, (new_capacity + place_canary_data) * sizeof (element_t));

		(*ptr_stk).capacity = new_capacity;

		#ifdef CANARY_STK_DATA
			(*ptr_stk).data = ptr_memory + 1;

			*ptr_memory                             = canary;
			*(ptr_memory + 1 + (*ptr_stk).capacity) = canary;
		#else 
			(*ptr_stk).data = ptr_memory;
		#endif	

	}

	else
	{
		#ifdef CANARY_STK_DATA
			size_t place_canary_data = 2;

		#else
			size_t place_canary_data = 0;
		#endif

		size_t new_capacity = (*ptr_stk).capacity / 4;

		element_t* ptr_memory = (element_t*) realloc ((*ptr_stk).data - 1, (new_capacity +  place_canary_data) * sizeof (element_t));

		(*ptr_stk).capacity = new_capacity;

		#ifdef CANARY_STK_DATA
			(*ptr_stk).data = ptr_memory + 1;

			*ptr_memory                             = canary;
			*(ptr_memory + 1 + (*ptr_stk).capacity) = canary;
		#else 
			(*ptr_stk).data = ptr_memory;
		#endif	
	}

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	return NOT_ERROR;
}
