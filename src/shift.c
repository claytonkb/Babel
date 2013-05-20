// shift.c
//

#include "babel.h"
#include "shift.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "array.h"
#include "tptr.h"
 
/* shift operator
**cushl/shl**  
> C-style unsigned shift-left  
> `{a} {b}| -> {a<<b}|`  
*/
bvm_cache *cushl(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) << (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* shift operator
**cushr/shr**  
> C-style unsigned shift-right
> `{a} {b}| -> {a>>b}|`  
*/
bvm_cache *cushr(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) >> (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* shift operator
**cashr**  
> C-style arithmetic shift-right
> `{a} {b}| -> {a>>b}|`  
> Performs an arithmetic shift-right which is suitable for dividing
> a signed (integer) value by a power of 2  
*/
bvm_cache *cashr(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) >> (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* shift operator
**curol/rol**  
> C-style unsigned rotate-left
> `{a} {b}| -> {a rol b}|`  
*/
bvm_cache *curol(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    mword tempA = (mword)car(TOS_1(this_bvm)) << (mword)car(TOS_0(this_bvm));
    mword tempB = (mword)car(TOS_1(this_bvm)) >> (MWORD_BIT_SIZE - (mword)car(TOS_0(this_bvm)));
    *result = tempA | tempB;

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* shift operator
**curor/ror**  
> C-style unsigned rotate-left
> `{a} {b}| -> {a ror b}|`  
*/
bvm_cache *curor(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    mword tempA = (mword)car(TOS_1(this_bvm)) >> (mword)car(TOS_0(this_bvm));
    mword tempB = (mword)car(TOS_1(this_bvm)) << (MWORD_BIT_SIZE - (mword)car(TOS_0(this_bvm)));
    *result = tempA | tempB;

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}


// Clayton Bauman 2011

