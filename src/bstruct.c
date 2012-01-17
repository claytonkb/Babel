// bstruct.c
//

#include "babel.h"
#include "bstruct.h"
#include "load.h"
#include "bvm_opcodes.h"
#include "stack.h"
#include "bvm.h"

void cp(void){

    mword *result = _unload((mword*)TOS_0);
    push_alloc(result, CP);

    load();

}

// Clayton Bauman 2012

