// logic.c
//

#include "babel.h"
#include "logic.h"
#include "stack.h"
#include "list.h"
//#include "bvm_opcodes.h"
#include "array.h"
#include "alloc.h"
#include "tptr.h"
#include "mem.h"

#define babel_logic_operator(x)                 \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    result = _newva( this_bvm,  x );

/* logic operator
_Note: Babel has 16 logic operators F0-Ff, corresponding to each 
two-bit LUT-based logic table._
*/

/* logic operator
**F0**  
> `{a} {b}| -> {0}|`  
*/
bvm_cache *logicF0(bvm_cache *this_bvm){ // logicF0  // *logicF0#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( 0 ) );

}


/* logic operator
**F1** or **cand** (&)  
> C-style bitwise AND  
> `{a} {b}| -> {a & b}|`  
*/
bvm_cache *logicF1(bvm_cache *this_bvm){ // logicF1#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA & opB ) );

}


/* logic operator
**F2**  
> `{a} {b}| -> {a & !b}|`  
*/
bvm_cache *logicF2(bvm_cache *this_bvm){ // logicF2#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA & ~opB ) );

}


/* logic operator
**F3**  
> `{a} {b}| -> {a}|`  
*/
bvm_cache *logicF3(bvm_cache *this_bvm){ // logicF3#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA ) );

}


/* logic operator
**F4**  
> `{a} {b}| -> {!a & b}|`  
*/
bvm_cache *logicF4(bvm_cache *this_bvm){ // logicF4#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opA & opB ) );

}


/* logic operator
**F5**  
> `{a} {b}| -> {b}|`  
*/
bvm_cache *logicF5(bvm_cache *this_bvm){ // logicF5#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opB ) );

}


/* logic operator
**F6** or **cxor** (^)  
> C-style bitwise XOR  
> `{a} {b}| -> {a xor b}|`  
*/
bvm_cache *logicF6(bvm_cache *this_bvm){ // logicF6#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA ^ opB ) );

}


/* logic operator
**F7** or **cor** (|)  
> C-style bitwise XOR  
> `{a} {b}| -> {a | b}|`  
*/
bvm_cache *logicF7(bvm_cache *this_bvm){ // logicF7#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( opA | opB ) );

}


/* logic operator
**F5**  
> `{a} {b}| -> {!a | b}|`  
*/
bvm_cache *logicF8(bvm_cache *this_bvm){ // logicF8#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opA | opB ) );

}


/* logic operator
**F9** or **cxnor** (~^)   
> C-style bitwise XNOR  
> `{a} {b}| -> {!(a xor b)}|`  
*/
bvm_cache *logicF9(bvm_cache *this_bvm){ // logicF9#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~(opA ^ opB) ) );

}


/* logic operator
**FA**  
> `{a} {b}| -> {!b}|`  
*/
bvm_cache *logicFA(bvm_cache *this_bvm){ // logicFA#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opB ) );

}


/* logic operator
**FB**  
> `{a} {b}| -> {!(!a & b)}|`  
*/
bvm_cache *logicFB(bvm_cache *this_bvm){ // logicFB#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~( ~opA & opB ) ) );

}


/* logic operator
**FC**  
> `{a} {b}| -> {!a}|`  
*/
bvm_cache *logicFC(bvm_cache *this_bvm){ // logicFC#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~opA ) );

}


/* logic operator
**FD**  
> `{a} {b}| -> {!(a & !b)}|`  
*/
bvm_cache *logicFD(bvm_cache *this_bvm){ // logicFD#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~( opA & ~opB ) ) );

}


/* logic operator
**FE** or **cnand** (~&)  
> C-style bitwise NAND  
> `{a} {b}| -> !{a & b}|`  
*/
bvm_cache *logicFE(bvm_cache *this_bvm){ // logicFE#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( ~( opA & opB ) ) );

}


/* logic operator
**FF**  
> `{a} {b}| -> {-1}|`  
*/
bvm_cache *logicFF(bvm_cache *this_bvm){ // logicFF#

    babel_operator_typeB( 
            this_bvm, 
            babel_logic_operator( -1 ) );

}


/* logic operator
**cnot** (~)  
> C-style bitwise complement  
> `{a}| -> {~a}|`  
*/
bvm_cache *cnot(bvm_cache *this_bvm){ // cnot#

#define babel_cnot_operator(x)                  \
    mword opA = c( op0, 0 );                    \
    result = _newva( this_bvm,  x );

    babel_operator_typeA( 
            this_bvm, 
            babel_cnot_operator( ~opA ) );

}


/* logic operator
**and**  
> C-style logical AND  
> `{a} {b}| -> {a && b}|`  
*/
bvm_cache *andop(bvm_cache *this_bvm){ // andop#

#define babel_andop_operator(x)                 \
    mword *opA = op0;                    \
    mword *opB = op1;                    \
    result = _newva( this_bvm,  x );

    babel_operator_typeB( 
            this_bvm, 
            babel_andop_operator( !is_false(opA) && !is_false(opB) ) );

}


/* logic operator
**or**  
> C-style logical OR  
> `{a} {b}| -> {a || b}|`  
*/
bvm_cache *orop(bvm_cache *this_bvm){ // orop#

#define babel_orop_operator(x)                 \
    mword *opA = op0;                    \
    mword *opB = op1;                    \
    result = _newva( this_bvm,  x );

    babel_operator_typeB( 
            this_bvm, 
            babel_orop_operator( !is_false(opA) || !is_false(opB) ) );

}


/* logic operator
**not**  
> C-style logical NOT  
> `{a} | -> {!a}|`  
*/
bvm_cache *notop(bvm_cache *this_bvm){ // notop#

//#define babel_notop_operator(x)                 \
// z  mword opA = c( op0, 0 );                    \
//    result = _newva(this_bvm,  x );

#define babel_notop_operator(x)                 \
    mword *opA = op0;                    \
    result = _newva( this_bvm,  x );

    babel_operator_typeA( 
            this_bvm, 
            babel_notop_operator( is_false(opA) ) );

}


// Clayton Bauman 2011

