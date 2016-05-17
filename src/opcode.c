// opcode.c
//

#include "babel.h"
#include "opcode.h"
#include "interp.h"

const babel_op opcode_array[] = {

#define X(a, b, c, d) a,
OPCODE_LIST
#undef X

};

// Clayton Bauman 2014

