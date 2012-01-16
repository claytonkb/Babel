// alloc.c
//

#include "babel.h"
#include "alloc.h"
#include "stack.h"

void free_op(void){

    _free_op((mword*)TOS_0);
    zap();

}

void _free_op(mword *mem){

    free(mem-1);

}

void del(void){
}

// Clayton Bauman 2012

