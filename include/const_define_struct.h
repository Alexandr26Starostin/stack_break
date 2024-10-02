#ifndef CONST_DEFINE_STRUCT_H
#define CONST_DEFINE_STRUCT_H

//----------------------------------------------------------------------

typedef double element_t;

struct stk_t 
{
	element_t* data;
	size_t     size;
	size_t     capacity;
};

//-----------------------------------------------------------------------

enum errors_t 
{
	NOT_ERROR          = 0,
	FALL_PTR_DATA      = 1,
	SIZE_MORE_CAPACITY = 2,

	CTOR_ERROR            = 101,    
	DUMP_WRITE_ERROR      = 102,
	DTOR_ERROR            = 103,
	REALLOC_ERROR_BACK    = 104,
	REALLOC_ERROR_FORWARD = 105,
	PUSH_ERROR            = 106,
	POP_ERROR             = 107
};

//-----------------------------------------------------------------------

enum mode_realloc_t 
{
	REVERSE_TRUE  = 1,
	REVERSE_FALSE = 0, 
};

//------------------------------------------------------------------------

#include "operations_with_stack.h"
#include "stk_error.h"

#ifdef DEBUG_ALL
	#define STK_ASSERT (ptr_stk, __FILE__, __LINE__);

#else
	#ifdef DEBUG_ASSERT
		#define STK_ASSERT (ptr_stk, __FILE__, __LINE__);         \
			errors_t error_status = stk_error (ptr_stk); \
			                                             \
			if (error_status (ptr_stk) != NOT_ERROR)     \
			{                                            \
				stk_dump (ptr_stk, __FILE__, __LINE__);          \ 
				return error_status;                     \
			}                                
	#else 
		#define STK_ASSERT (ptr_stk, __FILE__, __LINE__);         \
			if (stk_error (ptr_stk) != 0)                \
			{                                            \
				stk_dump (ptr_stk, __FILE__, __LINE__);          \
				assert (NULL);                           \
			}   
	#endif

#endif

//--------------------------------------------------------------------------

#endif
