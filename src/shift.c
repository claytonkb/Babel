// shift.c
//

#include "babel.h"
#include "shift.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "array.h"

//
bvm_cache *cushl(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) << (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
bvm_cache *cushr(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) >> (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
bvm_cache *cashr(bvm_cache *this_bvm){

    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) >> (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
bvm_cache *curol(bvm_cache *this_bvm){

    mword *result    = new_atom;

    mword tempA = (mword)car(TOS_1(this_bvm)) << (mword)car(TOS_0(this_bvm));
    mword tempB = (mword)car(TOS_1(this_bvm)) >> (MWORD_BIT_SIZE - (mword)car(TOS_0(this_bvm)));
    *result = tempA | tempB;

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
bvm_cache *curor(bvm_cache *this_bvm){

    mword *result    = new_atom;

    mword tempA = (mword)car(TOS_1(this_bvm)) >> (mword)car(TOS_0(this_bvm));
    mword tempB = (mword)car(TOS_1(this_bvm)) << (MWORD_BIT_SIZE - (mword)car(TOS_0(this_bvm)));
    *result = tempA | tempB;

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}


// Clayton Bauman 2011

