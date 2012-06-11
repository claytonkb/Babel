// alloc.c
//

#include "babel.h"
#include "alloc.h"
#include "stack.h"
#include "array.h"
#include "load.h"
#include "bstruct.h"

// "shallow del"
bvm_cache *free_op(bvm_cache *this_bvm){

    bfree(TOS_0(this_bvm));
    hard_zap(this_bvm);

    return this_bvm;

}

//
bvm_cache *del(bvm_cache *this_bvm){

    _del((mword*)TOS_0(this_bvm));
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

