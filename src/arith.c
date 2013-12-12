// arith.c
//

#include "babel.h"
#include "arith.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"
//#include "bvm_opcodes.h"
#include "array.h"
#include "bvm.h"
#include "alloc.h"


#define babel_arithmetic(expr)                  \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    result = _newva( expr );


#define babel_signed_arithmetic(expr)           \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    int signed_result = (int)(expr);            \
                                                \
    result = _newva( (mword)signed_result);

/* arithmetic operator
_Note: The arithmetic operators are all prefixed with 'c' because
Babel 2.0 will have its own native number format and arbitrary-precision
arithmetic._
*/

/* arithmetic operator
**cuadd** (u+)  
> C-style unsigned addition  
> Adds two mwords on TOS (Top-Of-Stack)  
> 
> Example:  
>     `2 3 cuadd`  
> 
> Leaves 5 on TOS
> 
> Stack effect:  
> `{a} {b}| -> {a+b}|`  
*/
bvm_cache *cuadd(bvm_cache *this_bvm){ // cuadd#

    babel_operator_typeB( 
            this_bvm, 
            babel_arithmetic( opA + opB ) );

}


/* arithmetic operator
**cusub** (u-)  
> C-style unsigned subtraction  
> `{a} {b}| -> {a-b}|`  
*/
bvm_cache *cusub(bvm_cache *this_bvm){ // cusub#

    babel_operator_typeB( 
            this_bvm, 
            babel_arithmetic( opB - opA ) );
}


/* arithmetic operator
**ciadd** (+)  
> C-style integer addition  
> `{a} {b}| -> {a+b}|`  
*/
bvm_cache* ciadd(bvm_cache *this_bvm){ // ciadd#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_arithmetic( opA + opB ) );

}


/* arithmetic operator
**cisub** (-)  
> C-style integer subtraction  
> `{a} {b}| -> {a-b}|`  
*/
bvm_cache* cisub(bvm_cache *this_bvm){ // cisub#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_arithmetic( opB - opA ) );

}


/* arithmetic operator
**cumul** (u*)  
> C-style unsigned multiplication  
> `{a} {b}| -> {lo(a*b) hi(a*b)}|`  
>   
> Note: The result of multiplication is two mwords, so it is stored
> in a leaf array of size 2.  
*/
bvm_cache* cumul(bvm_cache *this_bvm){ // cumul#

    babel_operator_typeB( 
            this_bvm, 
            babel_arithmetic( opA * opB ) );

}


/* arithmetic operator
**cimul** (*)  
> C-style integer multiplication  
> `{a} {b}| -> {lo(a*b) hi(a*b)}|`  
>   
> See also: cumul  
*/
bvm_cache* cimul(bvm_cache *this_bvm){ // cimul#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_arithmetic( opA * opB ) );

}

//FIXME: Fatals wrong; should be exceptions...

#define babel_division(expr)                    \
                                                \
    mword opA = c( op0, 0 );                    \
    mword opB = c( op1, 0 );                    \
                                                \
    if( opA == 0 ){                             \
       fatal("zero divisor or modulus");        \
    }                                           \
                                                \
    result = _newva( expr );


//#define babel_signed_division(expr)             \
//                                                \
//    mword opA = c( op0, 0 );                    \
//    mword opB = c( op1, 0 );                    \
//                                                \
//    if( opA == 0 ){                             \
//       fatal("zero divisor or modulus");        \
//    }                                           \
//                                                \
//    int signed_result = (int)(expr);            \
//                                                \
//    result = _newva( (mword)signed_result );
//

/* arithmetic operator
**cudiv** (u/)  
> C-style unsigned division  
> `{a} {b}| -> {a/b}|`  
*/
bvm_cache* cudiv(bvm_cache *this_bvm){ // cudiv#

    babel_operator_typeB( 
            this_bvm, 
            babel_division( opB / opA ) );

}


/* arithmetic operator
**curem** (%)  
> C-style unsigned remainder (usually termed "mod")  
> `{a} {b}| -> {a%b}|`  
*/
bvm_cache* curem(bvm_cache *this_bvm){ // curem#

    babel_operator_typeB( 
            this_bvm, 
            babel_division( opB % opA ) );

}


#define babel_signed_division(expr)             \
                                                \
    int opA = (int)c( op0, 0 );                    \
    int opB = (int)c( op1, 0 );                    \
                                                \
    if( opA == 0 ){                             \
       fatal("zero divisor or modulus");        \
    }                                           \
                                                \
    int signed_result = (int)(expr);            \
                                                \
    result = _newva( (mword)signed_result );\


/* arithmetic operator
**cidiv** (/)  
> C-style integer division  
> `{a} {b}| -> {a/b}|`  
*/
bvm_cache* cidiv(bvm_cache *this_bvm){ // cidiv#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_division(opB / opA) );

}


/* arithmetic operator
**cirem**  
> C-style integer remainder (usually termed "mod")  
> `{a} {b}| -> {a%b}|`  
*/
bvm_cache* cirem(bvm_cache *this_bvm){ // cirem#

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_division( opB % opA ) );

}


/* arithmetic operator
**ciabs**  
> C-style absolute-value  
> `{a}| -> {abs(a)}|`  
*/
bvm_cache* ciabs(bvm_cache *this_bvm){ // ciabs#

#define ciabs_operator                          \
    mword opA = c( op0, 0 );                    \
    result = _newva( abs( (int)opA ) );

    babel_operator_typeA( this_bvm, ciabs_operator );

}


/* arithmetic operator
**ciadd_assign** (+=)  
> C-style add-and-assign  
> `{a} {b}| -> |   --> a = a+b`  
*/
bvm_cache *ciadd_assign(bvm_cache *this_bvm){ // ciadd_assign#

    mword *op0 = dstack_get(this_bvm,0);
    mword *op1 = dstack_get(this_bvm,1);

    popd(this_bvm);

    int result = (int)*op0 + (int)*op1;
    *op1 = (mword)result;

    return this_bvm;

}


/* arithmetic operator
**cisub_assign** (-=)  
> C-style subtract-and-assign  
> `{a} {b}| -> |   --> a = a-b`  
*/
bvm_cache *cisub_assign(bvm_cache *this_bvm){ // cisub_assign#

    mword *op0 = dstack_get(this_bvm,0);
    mword *op1 = dstack_get(this_bvm,1);

    popd(this_bvm);

    int result = (int)*op0 - (int)*op1;
    *op1 = (mword)result;

    //remove_from_udr_stack(this_bvm->dstack_ptr,1);

    return this_bvm;

}

/* arithmetic operator
**cimul_assign** (*=)  
> C-style multiply-and-assign  
> `{a} {b}| -> |   --> a = a*b`  
*/

/* arithmetic operator
**cidiv_assign** (/=)  
> C-style divide-and-assign  
> `{a} {b}| -> |   --> a = a/b`  
*/


// Clayton Bauman 2011

