// cmp.c
//

#include "babel.h"
#include "cmp.h"
#include "stack.h"
#include "list.h"
//#include "bvm_opcodes.h"
#include "array.h"
#include "tptr.h"

#define babel_compare_operator(expr)            \
                                                \
    mword opA = c( op1, 0 );                    \
    mword opB = c( op0, 0 );                    \
                                                \
    result = _newva(this_bvm,  expr );


#define babel_signed_compare_operator(expr)     \
                                                \
    int opA = (int)c( op1, 0 );                 \
    int opB = (int)c( op0, 0 );                 \
                                                \
    int signed_result = (int)(expr);            \
                                                \
    result = _newva(this_bvm,  (mword)signed_result );


/* comparison operator
**cne** (~=)  
> C-style not-equal  
> `{a} {b}| -> {a != b}|`  
*/
bvm_cache *cne(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_compare_operator( opA != opB ) );

}


/* comparison operator
**ceq** (=)  
> C-style equal  
> `{a} {b}| -> {a == b}|`  
*/
bvm_cache *ceq(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_compare_operator( opA == opB ) );
    
}


/* comparison operator
**cult** (u<)  
> C-style unsigned less-than  
> `{a} {b}| -> {a < b}|`  
*/
bvm_cache *cult(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_compare_operator( opA < opB ) );

}


/* comparison operator
**cule** (u<=)  
> C-style unsigned less-than-or-equal  
> `{a} {b}| -> {a <= b}|`  
*/
bvm_cache *cule(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_compare_operator( opA <= opB ) );

}


/* comparison operator
**cugt** (u>)  
> C-style unsigned greater-than  
> `{a} {b}| -> {a > b}|`  
*/
bvm_cache *cugt(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_compare_operator( opA > opB ) );

}


/* comparison operator
**cuge** (u>=)  
> C-style unsigned greater-than-or-equal  
> `{a} {b}| -> {a >= b}|`  
*/
bvm_cache *cuge(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_compare_operator( opA >= opB ) );

}


/* comparison operator
**cilt** (<)  
> C-style integer less-than  
> `{a} {b}| -> {a < b}|`  
*/
bvm_cache *cilt(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_compare_operator( opA < opB ) );

}


/* comparison operator
**cile** (<=)  
> C-style integer less-than-or-equal  
> `{a} {b}| -> {a <= b}|`  
*/
bvm_cache *cile(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_compare_operator( opA <= opB ) );

}


/* comparison operator
**cigt** (>)  
> C-style integer greater-than  
> `{a} {b}| -> {a > b}|`  
*/
bvm_cache *cigt(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_compare_operator( opA > opB ) );

}


/* comparison operator
**cige** (>=)  
> C-style integer greater-than-or-equal  
> `{a} {b}| -> {a >= b}|`  
*/
bvm_cache *cige(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_compare_operator( opA >= opB ) );

}

// Clayton Bauman 2011

