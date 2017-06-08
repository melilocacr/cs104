#ifndef __SYM_TABLE__
#define __SYM_TABLE__

#include <bitset>
#include <string.h>
#include <vector>
#include <unordered_map>

#include "lyutils.h"
#include "auxlib.h"


enum { ATTR_void, ATTR_int, ATTR_null, ATTR_string, 
	ATTR_struct, ATTR_array, ATTR_function, ATTR_variable, 
	ATTR_field, ATTR_typeid, ATTR_param, ATTR_lval, ATTR_const, 
	ATTR_vreg, ATTR_vaddr, ATTR_bitset_size,
};

