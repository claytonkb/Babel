// cmp.c
//

#include "babel.h"
#include "cmp.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "array.h"

/* comparison operator
**cne** (~=)  
> C-style not-equal  
> `{a} {b}| -> {a != b}|`  
*/
bvm_cache *cne(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_0(this_bvm)) != (mword)car(TOS_1(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**ceq** (=)  
> C-style equal  
> `{a} {b}| -> {a == b}|`  
*/
bvm_cache *ceq(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_0(this_bvm)) == (mword)car(TOS_1(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**cult** (u<)  
> C-style unsigned less-than  
> `{a} {b}| -> {a < b}|`  
*/
bvm_cache *cult(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) < (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**cule** (u<=)  
> C-style unsigned less-than-or-equal  
> `{a} {b}| -> {a <= b}|`  
*/
bvm_cache *cule(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) <= (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**cugt** (u>)  
> C-style unsigned greater-than  
> `{a} {b}| -> {a > b}|`  
*/
bvm_cache *cugt(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) > (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**cuge** (u>=)  
> C-style unsigned greater-than-or-equal  
> `{a} {b}| -> {a >= b}|`  
*/
bvm_cache *cuge(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = (mword)car(TOS_1(this_bvm)) >= (mword)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**cilt** (<)  
> C-style integer less-than  
> `{a} {b}| -> {a < b}|`  
*/
bvm_cache *cilt(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) < (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**cile** (<=)  
> C-style integer less-than-or-equal  
> `{a} {b}| -> {a <= b}|`  
*/
bvm_cache *cile(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) <= (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**cigt** (>)  
> C-style integer greater-than  
> `{a} {b}| -> {a > b}|`  
*/
bvm_cache *cigt(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) > (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* comparison operator
**cige** (>=)  
> C-style integer greater-than-or-equal  
> `{a} {b}| -> {a >= b}|`  
*/
bvm_cache *cige(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    (int)*result = (int)car(TOS_1(this_bvm)) >= (int)car(TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

// Clayton Bauman 2011

