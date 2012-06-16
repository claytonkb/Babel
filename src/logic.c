// logic.c
//

#include "babel.h"
#include "logic.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "array.h"

//
// babel_operator
bvm_cache *logicF0(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = 0;

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF1(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) & (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF2(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) & ~((mword)car(TOS_0(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF3(bvm_cache *this_bvm){

     fatal("stack fix not done");
   mword *result    = new_atom;

    *result = (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF4(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ~((mword)car(TOS_1(this_bvm))) & (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF5(bvm_cache *this_bvm){

     fatal("stack fix not done");
   mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF6(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) ^ (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF7(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) | (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF8(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ~((mword)car(TOS_1(this_bvm)) | (mword)car(TOS_0(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicF9(bvm_cache *this_bvm){

     fatal("stack fix not done");
   mword *result    = new_atom;

    *result = ~((mword)car(TOS_1(this_bvm)) ^ (mword)car(TOS_0(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicFA(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ~((mword)car(TOS_1(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicFB(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ~( ~(mword)car(TOS_1(this_bvm)) & (mword)car(TOS_0(this_bvm)) );

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicFC(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ~(mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *cnot(bvm_cache *this_bvm){

     fatal("stack fix not done");
   mword *result    = new_atom;

    *result = ~(mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}


//
// babel_operator
bvm_cache *logicFD(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ~( (mword)car(TOS_1(this_bvm)) & ~(mword)car(TOS_0(this_bvm)) );

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicFE(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ~( (mword)car(TOS_1(this_bvm)) & (mword)car(TOS_0(this_bvm)) );

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *logicFF(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)(-1);

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *andop(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ( !is_false(TOS_1(this_bvm)) && !is_false(TOS_0(this_bvm)) );
    
    zap(this_bvm);
    zap(this_bvm);

    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *orop(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = ( !is_false(TOS_1(this_bvm)) || !is_false(TOS_0(this_bvm)) );
    
    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *notop(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = is_false(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

// Clayton Bauman 2011

