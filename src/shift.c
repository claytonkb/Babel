// shift.c
//

#include "babel.h"
#include "shift.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "array.h"
#include "tptr.h"

#define babel_shift_operator(expr)              \
                                                \
    mword opA = c( op1, 0 );                    \
    mword opB = c( op0, 0 );                    \
                                                \
    result = _newva( expr );

#define babel_signed_shift_operator(expr)       \
                                                \
    int opA = (int)c( op1, 0 );                 \
    int opB = (int)c( op0, 0 );                 \
                                                \
    int signed_result = (int)(expr);            \
                                                \
    result = _newva( (mword)signed_result );

/* shift operator
**cushl/shl**  
> C-style unsigned shift-left  
> `{a} {b}| -> {a<<b}|`  
*/
bvm_cache *cushl(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_shift_operator( opA << opB ) );

}

/* shift operator
**cushr/shr**  
> C-style unsigned shift-right
> `{a} {b}| -> {a>>b}|`  
*/
bvm_cache *cushr(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_shift_operator( opA >> opB ) );

}

/* shift operator
**cashr**  
> C-style arithmetic shift-right
> `{a} {b}| -> {a>>b}|`  
> Performs an arithmetic shift-right which is suitable for dividing
> a signed (integer) value by a power of 2  
*/
bvm_cache *cashr(bvm_cache *this_bvm){

    babel_operator_typeB( 
            this_bvm, 
            babel_signed_shift_operator( opA >> opB ) );

}

/* shift operator
**curol/rol**  
> C-style unsigned rotate-left
> `{a} {b}| -> {a rol b}|`  
*/
bvm_cache *curol(bvm_cache *this_bvm){

    mword *op0 = dstack_get(this_bvm,0);
    mword *op1 = dstack_get(this_bvm,1);

    mword *result    = _newva(0);

    mword tempA = (mword)icar(op1) << (mword)icar(op0);
    mword tempB = (mword)icar(op1) >> (MWORD_BIT_SIZE - (mword)icar(op0));
    *result = tempA | tempB;

    popd(this_bvm);
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* shift operator
**curor/ror**  
> C-style unsigned rotate-right
> `{a} {b}| -> {a ror b}|`  
*/
bvm_cache *curor(bvm_cache *this_bvm){

    mword *op0 = dstack_get(this_bvm,0);
    mword *op1 = dstack_get(this_bvm,1);

    mword *result    = _newva(0);

    mword tempA = (mword)icar(op1) >> (mword)icar(op0);
    mword tempB = (mword)icar(op1) << (MWORD_BIT_SIZE - (mword)icar(op0));
    *result = tempA | tempB;

    popd(this_bvm);
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;


}


// Clayton Bauman 2011

