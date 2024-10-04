#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/const_define_struct.h"
#include "../include/stk_error.h"
#include "../include/operations_with_stack.h"

//---------------------------------------------------------------------------------------------------------

errors_t stk_ctor (stk_t* ptr_stk, size_t start_capacity)
{
	assert (ptr_stk);

	printf ("\nBegin Ctor\n");

	#ifdef CANARY_STK 
		(*ptr_stk).canary_1 = canary;
		(*ptr_stk).canary_2 = canary;
	#endif

	#ifdef CANARY_STK_DATA
		size_t place_canary_data = 2;
		size_t size_memory       = start_capacity * sizeof (element_t) + place_canary_data * sizeof (canary_t) + (8 - start_capacity % 8) * sizeof (char);
	#else
		size_t place_canary_data = 0;
		size_t size_memory       = start_capacity * sizeof (element_t);
	#endif

	(*ptr_stk).size     = 0;
	(*ptr_stk).capacity = start_capacity;

	char* ptr_memory   = (char*) calloc (1, size_memory);
	if (ptr_memory == NULL) {printf ("PTR_MEMORY_ERROR in file %s, in line %d \n\tptr_memory == NULL", __FILE__, __LINE__); return PTR_MEMORY_ERROR;}

	memset (ptr_memory, poison, size_memory);

	#ifdef CANARY_STK_DATA 
		(*ptr_stk).data = (element_t*) (ptr_memory + sizeof (canary_t));
		
		*((canary_t*) ptr_memory)                                                                                                   = canary;
		*((canary_t*) ((char*) ((*ptr_stk).data) + start_capacity * sizeof (element_t) + (8 - start_capacity % 8) * sizeof (char))) = canary;
	#else 
		(*ptr_stk).data = (element_t*) (ptr_memory);
	#endif

	#ifdef HASH_STK
		count_hash (ptr_stk);
	#endif

	errors_t error_status = stk_error (ptr_stk);

	if (error_status != NOT_ERROR)
	{
		printf ("We can not create your stack. Please, try again.\n");
		printf ("This information can help your in find error:\n\n");

		printf ("start_capacity == %ld\n", start_capacity);
		stk_dump (ptr_stk, __FILE__, __LINE__);

		return error_status;
	}

	printf ("End Ctor\n\n");

	return error_status;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_dtor (stk_t* ptr_stk)
{
	printf ("\nBegin Dtor\n");

	assert (ptr_stk);

	(*ptr_stk).size     = 0;
	(*ptr_stk).capacity = 0;

	#ifdef CANARY_STK_DATA
		free ((canary_t*) ((char*) (*ptr_stk).data - sizeof (canary_t)));
	#else
		free ((*ptr_stk).data);
	#endif

	printf ("End Dtor\n\n");
	
	return NOT_ERROR;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_pop (stk_t* ptr_stk, element_t* ptr_element)
{
	assert (ptr_stk);
	assert (ptr_element);

	printf ("\nBegin pop\n");

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	(*ptr_stk).size                     -= 1;
	*ptr_element                         = *((*ptr_stk).data + (*ptr_stk).size);
	*((*ptr_stk).data + (*ptr_stk).size) = poison;

	#ifdef HASH_STK
		count_hash (ptr_stk);
	#endif

	if ((*ptr_stk).size < ((*ptr_stk).capacity / 4))  
	{
		errors_t error_status = stk_realloc (ptr_stk, REVERSE_TRUE);

		if (error_status != NOT_ERROR) {return REALLOC_ERROR_BACK;}
	}

	#ifdef HASH_STK
		count_hash (ptr_stk);
	#endif

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	printf ("End pop\n\n");

	return NOT_ERROR;
}
//-------------------------------------------------------------------------------------------------------

errors_t stk_push (stk_t* ptr_stk, element_t element)
{
	assert (ptr_stk);

	printf ("\nBegin push\n");

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);
	
	*((*ptr_stk).data + (*ptr_stk).size) = element;
	(  *ptr_stk).size                   += 1;

	#ifdef HASH_STK
		count_hash (ptr_stk);
	#endif

	if ((*ptr_stk).size == (*ptr_stk).capacity) 
	{
		errors_t error_status = stk_realloc (ptr_stk, REVERSE_FALSE);

		if (error_status != NOT_ERROR) {return REALLOC_ERROR_FORWARD;}
	}

	#ifdef HASH_STK
		count_hash (ptr_stk);
	#endif

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	printf ("End push\n\n");

	return NOT_ERROR;
}

//-------------------------------------------------------------------------------------------------------------

errors_t stk_realloc (stk_t* ptr_stk, mode_realloc_t reverse)
{
	assert (ptr_stk);

	printf ("\nBegin realloc\n");

    STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	if (reverse == REVERSE_FALSE)
	{
		size_t new_capacity = (*ptr_stk).capacity * 2;
		(*ptr_stk).capacity = new_capacity;

		#ifdef CANARY_STK_DATA
			size_t place_canary_data = 2;
			size_t size_memory       = new_capacity * sizeof (element_t) + place_canary_data * sizeof (canary_t) + (8 - 8 % new_capacity) * sizeof (char); 
			char*  ptr_realloc       = (char*) (*ptr_stk).data - sizeof (canary_t);
		#else
			size_t place_canary_data = 0;
			size_t size_memory       = new_capacity * sizeof (element_t);
			char*  ptr_realloc       = (char*) (*ptr_stk).data;
		#endif

		char* ptr_memory = (char*) realloc (ptr_realloc, size_memory);
		if (ptr_memory == NULL) {printf ("PTR_MEMORY_ERROR in file %s, in line %d \n\t ptr_memory == NULL", __FILE__, __LINE__); return PTR_MEMORY_ERROR;}

		#ifdef CANARY_STK_DATA
			(*ptr_stk).data = (element_t*) (ptr_memory + sizeof (canary_t));

			*((canary_t*) ptr_memory)                                                                                               = canary;
			*((canary_t*) ((char*) ((*ptr_stk).data) + new_capacity * sizeof (element_t) + (8 - new_capacity % 8) * sizeof (char))) = canary;

			size_t len_poison = ((*ptr_stk).capacity - (*ptr_stk).size) * sizeof (element_t) + (8 - new_capacity % 8) * sizeof (char);
			memset ((*ptr_stk).data + (*ptr_stk).size, poison, len_poison);
		#else 
			(*ptr_stk).data = (element_t*) ptr_memory;

			size_t len_poison = ((*ptr_stk).capacity - (*ptr_stk).size) * sizeof (element_t);
			memset ((*ptr_stk).data + (*ptr_stk).size, poison, len_poison);
		#endif	
	}

	else
	{
		size_t new_capacity = (*ptr_stk).capacity / 4;
		(*ptr_stk).capacity = new_capacity;

		#ifdef CANARY_STK_DATA
			size_t place_canary_data = 2;
			size_t size_memory       = new_capacity * sizeof (element_t) + place_canary_data * sizeof (canary_t) + (8 - 8 % new_capacity) * sizeof (char);
			char*  ptr_realloc       = (char*) (*ptr_stk).data - sizeof (canary_t);
		#else
			size_t place_canary_data = 0;
			size_t size_memory       = new_capacity * sizeof (element_t);
			char*  ptr_realloc       = (char*) (*ptr_stk).data;
		#endif

		char* ptr_memory = (char*) realloc (ptr_realloc, size_memory);
		if (ptr_memory == NULL) {printf ("PTR_MEMORY_ERROR in file %s, in line %d \n\t ptr_memory == NULL", __FILE__, __LINE__); return PTR_MEMORY_ERROR;}

		#ifdef CANARY_STK_DATA
			(*ptr_stk).data = (element_t*) (ptr_memory + sizeof (canary_t));

			*((canary_t*) ptr_memory)                                                                                               = canary;
			*((canary_t*) ((char*) ((*ptr_stk).data) + new_capacity * sizeof (element_t) + (8 - new_capacity % 8) * sizeof (char))) = canary;

			size_t len_poison = ((*ptr_stk).capacity - (*ptr_stk).size) * sizeof (element_t) + (8 - new_capacity % 8) * sizeof (char);
			memset ((*ptr_stk).data + (*ptr_stk).size, poison, len_poison);
		#else 
			(*ptr_stk).data = (element_t*) ptr_memory;

			size_t len_poison = ((*ptr_stk).capacity - (*ptr_stk).size) * sizeof (element_t);
			memset ((*ptr_stk).data + (*ptr_stk).size, poison, len_poison);
		#endif	
	}

	#ifdef HASH_STK
		count_hash (ptr_stk);
	#endif

	STK_ASSERT (ptr_stk, __FILE__, __LINE__);

	printf ("End realloc\n\n");

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

		#ifdef CANARY_STK_DATA

			canary_t canary_3 = *((canary_t*) ((char*) (*ptr_stk).data - sizeof (canary_t)));
			canary_t canary_4 = *((canary_t*) ((char*) ((*ptr_stk).data) + (*ptr_stk).capacity * sizeof (element_t) + (8 - (*ptr_stk).capacity % 8) * sizeof (char)));

			printf ("\t canary_3 == %x\n", canary_3);
			printf ("\t canary_4 == %x\n", canary_4);

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

	//-----------------------------------------------------------------------------------------------------------------

	#ifdef CANARY_STK_DATA

		if (error_status == CANARY_STK_DATA_ERROR)
		{
			printf ("ERROR   CANARY_STK_DATA_ERROR  in file: %s, in line: %d\n", file, line);
			printf ("\t ptr_data == %p          \n", (*ptr_stk).data);
			printf ("\t size     == %ld         \n", (*ptr_stk).size);
			printf ("\t capacity == %ld         \n", (*ptr_stk).capacity);

			printf ("\n");

			canary_t canary_3 = *((canary_t*) ((char*) (*ptr_stk).data - sizeof (canary_t)));
			canary_t canary_4 = *((canary_t*) ((char*) (*ptr_stk).data + (*ptr_stk).capacity * sizeof (element_t) + (8 - (*ptr_stk).capacity % 8) * sizeof (char)));

			printf ("\t canary_true == %x\n", canary);
			printf ("\t canary_3 == %x\n", canary_3);
			printf ("\t canary_4 == %x\n", canary_4);

			printf ("\t canary_3 or canary_4 != canary_true\n\n");
		}
	#endif

	//-----------------------------------------------------------------------------------------------------------------

	#ifdef HASH_STK
		if (error_status == HASH_ERROR)
		{
			printf ("ERROR   HASH_ERROR  in file: %s, in line: %d\n", file, line);
			printf ("\t ptr_data == %p          \n", (*ptr_stk).data);
			printf ("\t size     == %ld         \n", (*ptr_stk).size);
			printf ("\t capacity == %ld         \n", (*ptr_stk).capacity);

			hash_t hash_1 = hash ((char*) ptr_stk, sizeof (stk_t) - sizeof (hash_t) * 2);

			#ifdef CANARY_STK_DATA
				char* ptr_memory = (char*) (*ptr_stk).data - sizeof (canary_t);
				size_t len_data  = sizeof (canary_t) * 2 + (*ptr_stk).capacity * sizeof (element_t) + (8 - (*ptr_stk).capacity % 8) * sizeof (char);

			#else
				char* ptr_memory = (char*) (*ptr_stk).data;
				size_t len_data  = (*ptr_stk).capacity * sizeof (element_t);
			#endif

			hash_t hash_2 = hash (ptr_memory, len_data);

			printf ("\t hash_stk_true == %ld\n", (*ptr_stk).hash_stk);
			printf ("\t our_hash_stk  == %ld\n", hash_1);

			printf ("\n");

			printf ("\t hash_stk_data_true == %ld\n", (*ptr_stk).hash_stk_data);
			printf ("\t our_hash_stk_data  == %ld\n", hash_2);
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

			canary_t canary_3 = *((canary_t*) ((char*) (*ptr_stk).data - sizeof (canary_t)));
			canary_t canary_4 = *((canary_t*) ((char*) ((*ptr_stk).data) + (*ptr_stk).capacity * sizeof (element_t) + (8 - (*ptr_stk).capacity % 8) * sizeof (char)));

			printf ("\t canary_3 == %x\n", canary_3);
			printf ("\t canary_4 == %x\n", canary_4);

		#endif

		#ifdef HASH_STK
			printf ("\t hash_stk      == %ld\n",   (*ptr_stk).hash_stk);
			printf ("\t hash_stk_data == %ld\n\n", (*ptr_stk).hash_stk_data);
		#endif 

		printf ("\t elements of stack:\n");

		for (size_t index_element = 0; index_element < (*ptr_stk).size; index_element++)
		{
			printf ("\t \t [%ld] == %d\n", index_element, *((*ptr_stk).data + index_element));
		}

		if ((*ptr_stk).size != 0 && (*ptr_stk).capacity != 1)
		{
			for (size_t index_element = (*ptr_stk).size; index_element < (*ptr_stk).capacity; index_element++)
			{
				printf ("\t \t [%ld] == %d  (poison)\n", index_element, *((*ptr_stk).data + index_element));	
			}
		}

		printf ("\n\n");

		return NOT_ERROR;
	}

	return error_status;
}
