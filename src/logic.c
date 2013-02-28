// logic.c
//

#include "babel.h"
#include "logic.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "array.h"
#include "alloc.h"

#define babel_logic_operator(x)                 \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    result = _newva( x );


//
//
bvm_cache *logicF0(bvm_cache *this_bvm){ // logicF0 

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( 0 ) );

}


//
//
bvm_cache *logicF1(bvm_cache *this_bvm){ // logicF1#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA & opB ) );

}


//
//
bvm_cache *logicF2(bvm_cache *this_bvm){ // logicF2#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA & ~opB ) );

}


//
//
bvm_cache *logicF3(bvm_cache *this_bvm){ // logicF3#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA ) );

}


//
//
bvm_cache *logicF4(bvm_cache *this_bvm){ // logicF4#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opA & opB ) );

}


//
//
bvm_cache *logicF5(bvm_cache *this_bvm){ // logicF5#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opB ) );

}


//
//
bvm_cache *logicF6(bvm_cache *this_bvm){ // logicF6#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA ^ opB ) );

}


//
//
bvm_cache *logicF7(bvm_cache *this_bvm){ // logicF7#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA | opB ) );

}


//
//
bvm_cache *logicF8(bvm_cache *this_bvm){ // logicF8#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opA | opB ) );

}


//
//
bvm_cache *logicF9(bvm_cache *this_bvm){ // logicF9#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opA ^ opB ) );

}


//
//
bvm_cache *logicFA(bvm_cache *this_bvm){ // logicFA#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opA ) );

}


//
//
bvm_cache *logicFB(bvm_cache *this_bvm){ // logicFB#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~( ~opA & opB ) ) );

}


//
//
bvm_cache *logicFC(bvm_cache *this_bvm){ // logicFC#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opA ) );

}


//
//
bvm_cache *logicFD(bvm_cache *this_bvm){ // logicFD#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~( opA & ~opB ) ) );

}


//
//
bvm_cache *logicFE(bvm_cache *this_bvm){ // logicFE#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~( opA & opB ) ) );

}


//
//
bvm_cache *logicFF(bvm_cache *this_bvm){ // logicFF#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( -1 ) );

}


//
//
bvm_cache *cnot(bvm_cache *this_bvm){ // cnot#

#define babel_cnot_operator(x)                  \
    mword opA = c( op0, 0 );                    \
    result = _newva( x );

    babel_operator_typeA( 
            this_bvm, 
            babel_cnot_operator( ~opA ) );

}


//
//
bvm_cache *andop(bvm_cache *this_bvm){ // andop#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( !is_false(opA) && !is_false(opB) ) );

}


//
//
bvm_cache *orop(bvm_cache *this_bvm){ // orop#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( !is_false(opA) || !is_false(opB) ) );

}


//
//
bvm_cache *notop(bvm_cache *this_bvm){ // notop#

#define babel_notop_operator(x)                 \
    mword opA = c( op0, 0 );                    \
    result = _newva( x );

    babel_operator_typeA( 
            this_bvm, 
            babel_notop_operator( is_false(opA) ) );

}


// Clayton Bauman 2011

