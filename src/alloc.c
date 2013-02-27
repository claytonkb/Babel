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
//
void _del(mword *bs){ // _del#

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
//
mword *_balloc(mword size){

    // FIXME: Check allocation limit; except if exceeded

    mword *ptr = malloc( MWORDS( size ) ); // 3 = s-field + car + cdr

    if(ptr == NULL){
        fatal("malloc returned NULL");
        //FIXME: Exception
    }

    //FIXME: Put ptr into the bvm's alloc table

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

