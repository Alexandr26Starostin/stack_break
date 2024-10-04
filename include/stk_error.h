#ifndef STK_ERROR_H
#define STK_ERROR_H

errors_t stk_error  (stk_t* ptr_stk);

#include "const_define_struct.h"

#ifdef HASH_STK
	hash_t   hash       (char*  ptr_stk, size_t len);
	errors_t count_hash (stk_t* ptr_stk            );
#endif

#endif
