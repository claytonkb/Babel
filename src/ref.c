// ref.c
//

#include "babel.h"
#include "ref.h"
#include "pearson16.h"
#include "except.h"
#include "bvm_opcodes.h"
#include "stack.h"
#include "bvm.h"
#include "list.h"
#include "array.h"

//void newref(void){
//
//    mword *result = _newref((mword*)TOS_0);
//    zap();
//    push_alloc(result, NEWREF);
//
//}

//
mword *_newref(mword *hash){

    if(size(hash) != HASH_SIZE)
        error("_newref: size(hash) != HASH_SIZE");

    mword *ptr = malloc( MWORDS(HASH_SIZE+1) );
    if(ptr == NULL){
        error("_newref: malloc returned NULL");
    }

    ptr[0] = 0;
    mword *result = (ptr+1);

    memcpy( result, hash, MWORDS(HASH_SIZE) );

    return result;

}

// babel_operator
bvm_cache *ishref(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;
    
    *result = is_href(TOS_0(this_bvm));

    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL);

}

//
// babel_operator
bvm_cache *deref(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = get_from_stack( this_bvm, TOS_0(this_bvm));
    
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL);

}


// Clayton Bauman 2012

