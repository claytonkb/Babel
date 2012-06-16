// arith.c
//

#include "babel.h"
#include "arith.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"
#include "bvm_opcodes.h"
#include "array.h"
#include "bvm.h"

// babel_operator
bvm_cache *cuadd(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

//    *result = (mword)car(TOS_0(this_bvm)) + (mword)car(TOS_1(this_bvm));

    mword opA = car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );
    mword opB = car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );

    *result = opA + opB;

    // Detect overflow
//    if((mword)*result < (mword)car(TOS_0(this_bvm))){
//        error("cuadd: overflow");
//    }

    // FIXME Overflow error

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache *cusub(bvm_cache *this_bvm){

    // Detect underflow
//    if((mword)car(TOS_1(this_bvm)) < (mword)car(TOS_0(this_bvm))){
//        error("cusub: underflow");
//    }

    fatal("stack fix not done");
    mword *result    = new_atom;

    mword opA = car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );
    mword opB = car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );

    *result = opA - opB;

//    *result = (mword)car(TOS_1(this_bvm)) - (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache* ciadd(bvm_cache *this_bvm){

    mword *temp   = new_atom;
    mword *result = _newin(1);
    (mword*)*result = temp;
 
    int opA = (int)car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );
    int opB = (int)car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );

    (int)*temp = opA + opB;

//    // Detect underflow/overflow
//    if(        ((int)*result < 0) 
//            && (opA          > 0) 
//            && (opB          > 0) ){
//        error("ciadd: overflow");
//    }
//    if(        ((int)*result >= 0) 
//            && opA           <  0 
//            && opB           <  0 ){
//        error("ciadd: underflow");
//    }

    hard_zap(this_bvm);
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache* cisub(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

//    (int)*result = (int)car(TOS_1(this_bvm)) - (int)car(TOS_0(this_bvm));

    int opA = (int)car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );
    int opB = (int)car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );

    (int)*result = opA - opB;

    // Detect underflow/overflow
//    if( ((int)*result <  0) && ((int)car(TOS_0(this_bvm)) < 0) && ((int)car(TOS_1(this_bvm)) > 0) ){
//        error("cisub: overflow");
//    }
//    if( ((int)*result >= 0) && ((int)car(TOS_0(this_bvm)) > 0) && ((int)car(TOS_1(this_bvm)) < 0) ){
//        error("cisub: underflow");
//    }

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache* ciabs(bvm_cache *this_bvm){

    fatal("stack fix not done");
    // The most negative number in 2's complement cannot be abs()'d
    if( ((int)car(TOS_0(this_bvm)) - 1) > 0 ){
        error("ciabs: overflow");
    }

    mword *result    = new_atom;

//    (int)*result = abs((int)car(TOS_0(this_bvm)));

    int opA = (int)car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );
//    int opB = (int)car( get_from_stack( this_bvm, (mword*)TOS_0( this_bvm ) ) );

    (int)*result = abs(opA);

    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache* cumul(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = new_atom;

    mword opA = car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );
    mword opB = car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );

    *result = opA * opB;

//    *result = (mword)car(TOS_0(this_bvm)) * (mword)car(TOS_1(this_bvm));

    // Argh... There must be a better way!
//    if( *result / (mword)car(TOS_0(this_bvm)) != (mword)car(TOS_1(this_bvm)) ){
//        error("cumul: overflow");
//    }

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache* cudiv(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = new_atom;

    mword opA = car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );
    mword opB = car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );

    if( opB == 0 ){
        fatal("zero divisor");
    }

    *result = opA / opB;

//    *result = (mword)car(TOS_1(this_bvm)) / (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache* curem(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = new_atom;

    mword opA = car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );
    mword opB = car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );

    if( opB == 0 ){
        fatal("zero modulus");
    }

    *result = opA % opB;

//    *result = (mword)car(TOS_1(this_bvm)) % (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache* cimul(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = new_atom;

//    (int)*result = (int)car(TOS_0(this_bvm)) * (int)car(TOS_1(this_bvm));

    int opA = (int)car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );
    int opB = (int)car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );

    (int)*result = opA * opB;

    // Argh... There must be a better way!
//    if( (int)*result / (int)car(TOS_0(this_bvm)) != (int)car(TOS_1(this_bvm)) ){
//        error("cimul: overflow");
//    }

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}


// babel_operator
bvm_cache* cidiv(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = new_atom;

//    (int)*result = (int)car(TOS_1(this_bvm)) / (int)car(TOS_0(this_bvm));

    mword *result = new_atom;

    int opA = (int)car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );
    int opB = (int)car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );

    if( opB == 0 ){
        fatal("zero divisor");
    }

    (int)*result = opA / opB;

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache* cirem(bvm_cache *this_bvm){

//    if( car(TOS_0(this_bvm)) == 0 ){
//        error("cirem: zero modulus");
//    }
//
//    mword *result = new_atom;
//
//    (int)*result = (int)car(TOS_1(this_bvm)) % (int)car(TOS_0(this_bvm));

    fatal("stack fix not done");
    mword *result = new_atom;

    int opA = (int)car( get_from_stack( this_bvm, TOS_1( this_bvm ) ) );
    int opB = (int)car( get_from_stack( this_bvm, TOS_0( this_bvm ) ) );

    if( opB == 0 ){
        fatal("zero modulus");
    }

    (int)*result = opA % opB;

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache *ciadd_assign(bvm_cache *this_bvm){

//    (int)car(TOS_0(this_bvm)) = (int)car(TOS_0(this_bvm)) + (int)car(TOS_1(this_bvm));

//    // Detect underflow/overflow
//    if( ((int)*result <  0) && ((int)car(TOS_0(this_bvm)) > 0) && ((int)car(TOS_1(this_bvm)) > 0) ){
//        error("ciadd: overflow");
//    }
//    if( ((int)*result >= 0) && ((int)car(TOS_0(this_bvm)) < 0) && ((int)car(TOS_1(this_bvm)) < 0) ){
//        error("ciadd: underflow");
//    }

    int *opA = (int*)get_from_stack( this_bvm, TOS_1( this_bvm ) );
    int *opB = (int*)get_from_stack( this_bvm, TOS_0( this_bvm ) );

    *opA = *opA + *opB;

    // FIXME Overflow error

    zap(this_bvm);
    zap(this_bvm);

    return this_bvm;

}

// babel_operator
bvm_cache *cisub_assign(bvm_cache *this_bvm){

//    (int)car(TOS_0(this_bvm)) = (int)car(TOS_0(this_bvm)) - (int)car(TOS_1(this_bvm));

//    // Detect underflow/overflow
//    if( ((int)*result <  0) && ((int)car(TOS_0(this_bvm)) > 0) && ((int)car(TOS_1(this_bvm)) > 0) ){
//        error("ciadd: overflow");
//    }
//    if( ((int)*result >= 0) && ((int)car(TOS_0(this_bvm)) < 0) && ((int)car(TOS_1(this_bvm)) < 0) ){
//        error("ciadd: underflow");
//    }

    int *opA = (int*)get_from_stack( this_bvm, TOS_1( this_bvm ) );
    int *opB = (int*)get_from_stack( this_bvm, TOS_0( this_bvm ) );

    *opA = *opA - *opB;

    // FIXME Overflow error

    zap(this_bvm);
    zap(this_bvm);

    return this_bvm;

}

// Clayton Bauman 2011

