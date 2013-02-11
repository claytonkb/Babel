// alloc.c
//

#include "babel.h"
#include "alloc.h"
#include "stack.h"
#include "array.h"
#include "load.h"
#include "bstruct.h"

// "shallow del"
// babel_operator
bvm_cache *free_op(bvm_cache *this_bvm){

    fatal("stack fix not done");
    bfree(TOS_0(this_bvm));
    hard_zap(this_bvm);

    return this_bvm;

}

//
// babel_operator
bvm_cache *del(bvm_cache *this_bvm){

    fatal("stack fix not done");
    _del(TOS_0(this_bvm));
    hard_zap(this_bvm);

    return this_bvm;

}

//
void _del(mword *bs){

    int i;

//    _dump(bs)
//        die
//
    mword *del_list = _bs2ar(bs);

    for(i=0;i<size(del_list);i++){
        if(!is_nil((mword*)c(del_list,i))){
            //FIXME: It's ok to delete non-interp nil's
            bfree((mword*)c(del_list,i));
        }
    }

}

//_balloc() 
// checks for NULL pointer
//
mword *_balloc(mword size){

    mword *ptr = malloc( MWORDS( size ) ); // 3 = s-field + car + cdr

    if(ptr == NULL){
        fatal("malloc returned NULL");
        //FIXME: In future, we will free all memory associated with
        //the running-bvm and return to the parent-bvm...
    }

    return ptr;

}

////
//bvm_cache *destroy(bvm_cache *this_bvm){
//
//    _del((mword*)TOS_0(this_bvm));
//    hard_zap(this_bvm);
//
//    return this_bvm;
//
//}

// Clayton Bauman 2012

