#ifndef CONST_DEFINE_STRUCT_H
#define CONST_DEFINE_STRUCT_H

#define DEBUG_ASSERT
#define CANARY_STK
#define CANARY_STK_DATA

//----------------------------------------------------------------------

typedef int element_t;

#ifdef CANARY_STK 

	typedef int canary_t;

	const canary_t canary = 0xC0FFEE;

	struct stk_t 
	{
		canary_t   canary_1;
		size_t     size;
		size_t     capacity;
		element_t* data;
		canary_t   canary_2;
	};

#else
	struct stk_t 
	{
		size_t     size;
		size_t     capacity;
		element_t* data;
	};

#endif

//-----------------------------------------------------------------------

#ifdef CANARY_STK_DATA 
    
	#ifndef CANARY_STK

		typedef int canary_t;
		const canary_t canary = 0xC0FFEE;

	#endif

#endif

//-----------------------------------------------------------------------

enum errors_t 
{
	NOT_ERROR             = 0,
	FALL_PTR_DATA         = 1,
	SIZE_MORE_CAPACITY    = 2,

	CTOR_ERROR            = 101,    
	DUMP_WRITE_ERROR      = 102,
	DTOR_ERROR            = 103,
	REALLOC_ERROR_BACK    = 104,
	REALLOC_ERROR_FORWARD = 105,
	PUSH_ERROR            = 106,
	POP_ERROR             = 107,

	CANARY_STK_ERROR      = 1001,
	CANARY_STK_DATA_ERROR = 1002
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
			                                                      \
			if ((stk_error (ptr_stk)) != NOT_ERROR)               \
			{                                                     \
				stk_dump (ptr_stk, __FILE__, __LINE__);           \ 
				return (stk_error (ptr_stk));                     \
			}                                
	#else 
		#define STK_ASSERT (ptr_stk, __FILE__, __LINE__);         \
			if ((stk_error (ptr_stk)) != NOT_ERROR)               \
			{                                                     \
				stk_dump (ptr_stk, __FILE__, __LINE__);           \
				assert (NULL);                                    \
			}   
	#endif

#endif

//--------------------------------------------------------------------------

#endif
