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
#include "alloc.h"


#define babel_arithmetic(y)                     \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    result = _newva( y );


#define babel_signed_arithmetic(y)              \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    int signed_result = (int)(y);               \
                                                \
    result = _newva( (mword)signed_result);


//
//
bvm_cache *cuadd(bvm_cache *this_bvm){ // cuadd#

    babel_operator_typeB( 
            this_bvm, 
            babel_arithmetic( opA + opB ) );

}


//
//
bvm_cache *cusub(bvm_cache *this_bvm){ // cusub#

    babel_operator_typeB( 
            this_bvm, 
            babel_arithmetic( opB - opA ) );
}


//
//
bvm_cache* ciadd(bvm_cache *this_bvm){ // ciadd#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_arithmetic( opA + opB ) );

}


//
//
bvm_cache* cisub(bvm_cache *this_bvm){ // cisub#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_arithmetic( opB - opA ) );

}


//
//
bvm_cache* cumul(bvm_cache *this_bvm){ // cumul#

    babel_operator_typeB( 
            this_bvm, 
            babel_arithmetic( opA * opB ) );

}


//
//
bvm_cache* cimul(bvm_cache *this_bvm){ // cimul#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_arithmetic( opA * opB ) );

}


#define babel_division(y)                       \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    if( opA == 0 ){                             \
       fatal("zero divisor or modulus");        \
    }                                           \
                                                \
    result = _newva( y );


#define babel_signed_division(y)                \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    if( opA == 0 ){                             \
       fatal("zero divisor or modulus");        \
    }                                           \
                                                \
    int signed_result = (int)(y);               \
                                                \
    result = _newva( (mword)signed_result );


//
//
bvm_cache* cudiv(bvm_cache *this_bvm){ // cudiv#

    babel_operator_typeB( 
            this_bvm, 
            babel_division( opB / opA ) );

}


//
//
bvm_cache* curem(bvm_cache *this_bvm){ // curem#

    babel_operator_typeB( 
            this_bvm, 
            babel_division( opB % opA ) );

}


//
//
bvm_cache* cidiv(bvm_cache *this_bvm){ // cidiv#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_division( opB / opA ) );

}


//
//
bvm_cache* cirem(bvm_cache *this_bvm){ // cirem#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_division( opB % opA ) );

}


//
//
bvm_cache* ciabs(bvm_cache *this_bvm){ // ciabs#

#define ciabs_operator                          \
    mword opA = c( op0, 0 );                    \
    result = _newva( abs( (int)opA ) );

    babel_operator_typeA( this_bvm, ciabs_operator );

}


//
//
bvm_cache *ciadd_assign(bvm_cache *this_bvm){ // ciadd_assign#

#define ciadd_assign_operator                   \
    int result = (int)*op0 + (int)*op1;         \
    *op1 = (mword)result;                       \

    babel_operator_typeE(
            this_bvm,
            ciadd_assign_operator);

}


//
//
bvm_cache *cisub_assign(bvm_cache *this_bvm){ // cisub_assign#

#define cisub_assign_operator                   \
    int result = (int)*op0 - (int)*op1;         \
    *op1 = (mword)result;                       \

    babel_operator_typeE(
            this_bvm,
            cisub_assign_operator);

}

// Clayton Bauman 2011

