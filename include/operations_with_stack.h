#ifndef OPERATIONS_WITH_STACK_H
#define OPERATIONS_WITH_STACK_H

#include "const_define_struct.h"

errors_t stk_ctor    (stk_t* ptr_stk, size_t start_capacity);
errors_t stk_dtor    (stk_t* ptr_stk                                  );
errors_t stk_dump    (stk_t* ptr_stk, const char* file, const int line);
errors_t stk_pop     (stk_t* ptr_stk, element_t*     ptr_element      );
errors_t stk_push    (stk_t* ptr_stk, element_t      element          );
errors_t stk_realloc (stk_t* ptr_stk, mode_realloc_t reverse          );

#endif
