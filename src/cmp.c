// cmp.c
//

#include "babel.h"
#include "cmp.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "array.h"

//
// babel_operator
bvm_cache *cne(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_0(this_bvm)) != (mword)car(TOS_1(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *ceq(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_0(this_bvm)) == (mword)car(TOS_1(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cult(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) < (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cule(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) <= (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cugt(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) > (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cuge(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) >= (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cilt(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) < (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cile(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) <= (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cigt(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) > (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cige(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) >= (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

// Clayton Bauman 2011

