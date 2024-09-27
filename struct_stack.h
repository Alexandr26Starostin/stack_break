#ifndef STRUCT_STACK_H
#define STRUCT_STACK_H

typedef double stack_element_t;

struct stack_t {stack_element_t* data;
			    size_t           size;
				size_t           copacity;
			   };

#endif
