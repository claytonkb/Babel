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

//cuadd
//#define CUADD      0x030
bvm_cache *cuadd(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    mword *result    = new_atom;

    *result = (mword)car(TOS_0(this_bvm)) + (mword)car(TOS_1(this_bvm));

    // Detect overflow
    if((mword)*result < (mword)car(TOS_0(this_bvm))){
        error("cuadd: overflow");
    }

    // FIXME Overflow error

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//cusub
//#define CUSUB      0x031
bvm_cache *cusub(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    // Detect underflow
    if((mword)car(TOS_1(this_bvm)) < (mword)car(TOS_0(this_bvm))){
        error("cusub: underflow");
    }

    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) - (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//ciadd
//#define CIADD      0x038
bvm_cache* ciadd(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    mword *result    = new_atom;

    (int)*result = (int)car(TOS_0(this_bvm)) + (int)car(TOS_1(this_bvm));

    // Detect underflow/overflow
    if( ((int)*result <  0) && ((int)car(TOS_0(this_bvm)) > 0) && ((int)car(TOS_1(this_bvm)) > 0) ){
        error("ciadd: overflow");
    }
    if( ((int)*result >= 0) && ((int)car(TOS_0(this_bvm)) < 0) && ((int)car(TOS_1(this_bvm)) < 0) ){
        error("ciadd: underflow");
    }

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//cisub
//#define CISUB      0x039
bvm_cache* cisub(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) - (int)car(TOS_0(this_bvm));

    // Detect underflow/overflow
    if( ((int)*result <  0) && ((int)car(TOS_0(this_bvm)) < 0) && ((int)car(TOS_1(this_bvm)) > 0) ){
        error("cisub: overflow");
    }
    if( ((int)*result >= 0) && ((int)car(TOS_0(this_bvm)) > 0) && ((int)car(TOS_1(this_bvm)) < 0) ){
        error("cisub: underflow");
    }

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//ciabs
//#define CIABS      0x03C
bvm_cache* ciabs(bvm_cache *this_bvm){

    REQUIRE_LEAF(this_bvm);

    // The most negative number in 2's complement cannot be abs()'d
    if( ((int)car(TOS_0(this_bvm)) - 1) > 0 ){
        error("ciabs: overflow");
    }

    mword *result    = new_atom;

    (int)*result = abs((int)car(TOS_0(this_bvm)));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//cumul
//#define CUMUL      0x032
bvm_cache* cumul(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    mword *result = new_atom;

    *result = (mword)car(TOS_0(this_bvm)) * (mword)car(TOS_1(this_bvm));

    // Argh... There must be a better way!
    if( *result / (mword)car(TOS_0(this_bvm)) != (mword)car(TOS_1(this_bvm)) ){
        error("cumul: overflow");
    }

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//cudiv 
//#define CUDIV      0x033
bvm_cache* cudiv(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    if( car(TOS_0(this_bvm)) == 0 ){
        error("cudiv: zero divisor");
    }

    mword *result = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) / (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//curem
//#define CUREM      0x035
bvm_cache* curem(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    if( car(TOS_0(this_bvm)) == 0 ){
        error("curem: zero modulus");
    }

    mword *result = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) % (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//cimul
//#define CIMUL      0x03A
bvm_cache* cimul(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    mword *result = new_atom;

    (int)*result = (int)car(TOS_0(this_bvm)) * (int)car(TOS_1(this_bvm));

    // Argh... There must be a better way!
    if( (int)*result / (int)car(TOS_0(this_bvm)) != (int)car(TOS_1(this_bvm)) ){
        error("cimul: overflow");
    }

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}


//cidiv 
//#define CIDIV      0x03B
bvm_cache* cidiv(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    if( car(TOS_0(this_bvm)) == 0 ){
        error("cidiv: zero divisor");
    }

    mword *result = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) / (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//curem
//#define CIREM      0x03D
bvm_cache* cirem(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    if( car(TOS_0(this_bvm)) == 0 ){
        error("cirem: zero modulus");
    }

    mword *result = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) % (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
bvm_cache *ciadd_assign(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    (int)car(TOS_0(this_bvm)) = (int)car(TOS_0(this_bvm)) + (int)car(TOS_1(this_bvm));

//    // Detect underflow/overflow
//    if( ((int)*result <  0) && ((int)car(TOS_0(this_bvm)) > 0) && ((int)car(TOS_1(this_bvm)) > 0) ){
//        error("ciadd: overflow");
//    }
//    if( ((int)*result >= 0) && ((int)car(TOS_0(this_bvm)) < 0) && ((int)car(TOS_1(this_bvm)) < 0) ){
//        error("ciadd: underflow");
//    }

    // FIXME Overflow error

    zap(this_bvm);
    zap(this_bvm);

    return this_bvm;

}

//
bvm_cache *cisub_assign(bvm_cache *this_bvm){

    REQUIRE_LEAF_LEAF(this_bvm);

    (int)car(TOS_0(this_bvm)) = (int)car(TOS_0(this_bvm)) - (int)car(TOS_1(this_bvm));

//    // Detect underflow/overflow
//    if( ((int)*result <  0) && ((int)car(TOS_0(this_bvm)) > 0) && ((int)car(TOS_1(this_bvm)) > 0) ){
//        error("ciadd: overflow");
//    }
//    if( ((int)*result >= 0) && ((int)car(TOS_0(this_bvm)) < 0) && ((int)car(TOS_1(this_bvm)) < 0) ){
//        error("ciadd: underflow");
//    }

    // FIXME Overflow error

    zap(this_bvm);
    zap(this_bvm);

    return this_bvm;

}


// Clayton Bauman 2011

