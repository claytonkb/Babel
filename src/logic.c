// XXX STACK FIX DONE
// logic.c
//

#include "babel.h"
#include "logic.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "array.h"

bvm_cache *logicF0(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = 0;

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F000);

    

    return this_bvm;

}

bvm_cache *logicF1(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) & (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F001);

    

    return this_bvm;

}

bvm_cache *logicF2(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) & ~((mword)car(TOS_0(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F002);

    

    return this_bvm;

}

bvm_cache *logicF3(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F003);

    

    return this_bvm;

}

bvm_cache *logicF4(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~((mword)car(TOS_1(this_bvm))) & (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F004);

    

    return this_bvm;

}

bvm_cache *logicF5(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F005);

    

    return this_bvm;

}

bvm_cache *logicF6(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) ^ (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F006);

    

    return this_bvm;

}

bvm_cache *logicF7(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) | (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F007);

    

    return this_bvm;

}

bvm_cache *logicF8(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~((mword)car(TOS_1(this_bvm)) | (mword)car(TOS_0(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F008);

    

    return this_bvm;

}

bvm_cache *logicF9(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~((mword)car(TOS_1(this_bvm)) ^ (mword)car(TOS_0(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F009);

    

    return this_bvm;

}

bvm_cache *logicFA(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~((mword)car(TOS_1(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F00A);

    

    return this_bvm;

}

bvm_cache *logicFB(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~( ~(mword)car(TOS_1(this_bvm)) & (mword)car(TOS_0(this_bvm)) );

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F00B);

    

    return this_bvm;

}

bvm_cache *logicFC(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~(mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F00C);

    

    return this_bvm;

}

bvm_cache *cnot(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~(mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, CNOT);

    

    return this_bvm;

}


bvm_cache *logicFD(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~( (mword)car(TOS_1(this_bvm)) & ~(mword)car(TOS_0(this_bvm)) );

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F00D);

    

    return this_bvm;

}

bvm_cache *logicFE(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ~( (mword)car(TOS_1(this_bvm)) & (mword)car(TOS_0(this_bvm)) );

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F00E);

    

    return this_bvm;

}

bvm_cache *logicFF(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)(-1);

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, F00F);

    

    return this_bvm;

}

bvm_cache *andop(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ( !is_false((mword*)TOS_1(this_bvm)) && !is_false((mword*)TOS_0(this_bvm)) );
    
    zap(this_bvm);
    zap(this_bvm);

    push_alloc(this_bvm, result, ANDOP);

    

    return this_bvm;

}

bvm_cache *orop(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = ( !is_false((mword*)TOS_1(this_bvm)) || !is_false((mword*)TOS_0(this_bvm)) );
    
    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, OROP);

    

    return this_bvm;

}

bvm_cache *notop(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = is_false((mword*)TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, NOTOP);

    

    return this_bvm;

}

// Clayton Bauman 2011

