#include <stdio.h>
#include <assert.h>

#include "../include/const_define_struct.h"
#include "../include/stk_error.h"

 //--------------------------------------------------------------------------------------------

errors_t stk_error (stk_t* ptr_stk)
{
	assert (ptr_stk);

	if ((*ptr_stk).data == NULL)
	{
		return FALL_PTR_DATA;
	}

	if ((*ptr_stk).size > (*ptr_stk).capacity)
	{
		return SIZE_MORE_CAPACITY;
	}

	return NOT_ERROR;
}
