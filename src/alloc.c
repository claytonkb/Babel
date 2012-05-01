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

    mword *del_list = _bs2ar(bs);

    for(i=0;i<size(del_list);i++){
        if(!is_nil((mword*)c(del_list,i))){
            //FIXME: It's ok to delete non-interp nil's
            bfree((mword*)c(del_list,i));
        }
    }

}

// A "stack block" consists of 8 mwords in the present implementation
// This can change if the format of the stack entry is changed
#define STACK_BLOCK_SIZE 8

mword *alloc_stack_page(bvm_cache *this_bvm){

    mword *new_page = malloc(MWORDS(STACK_BLOCK_SIZE*ALLOC_PAGE_SIZE));

    init_stack_page(new_page);

    if(is_nil(this_bvm->machine.alloc_stack)){

        this_bvm->machine.alloc_stack              = new_page+1;
        (mword*)car(this_bvm->machine.alloc_stack) = new_page+1+STACK_BLOCK_SIZE;
        (mword*)cdr(this_bvm->machine.alloc_stack) = nil;
    
    }
    else{

        //find the end of the page list
        mword *page_list = (mword*)cdr(this_bvm->machine.alloc_stack);

        this_bvm->machine.alloc_stack              = new_page+1;
        (mword*)car(this_bvm->machine.alloc_stack) = new_page+1+STACK_BLOCK_SIZE;
        (mword*)cdr(this_bvm->machine.alloc_stack) = page_list;

    }

}

//
void init_stack_page(mword *stack_page){

    int i;

    mword init[8] = {   -2*MWORD_SIZE, (mword)nil, (mword)nil, 
                        -2*MWORD_SIZE, (mword)nil, (mword)nil, 
                           MWORD_SIZE, 0 };

    for(i=0; i<ALLOC_PAGE_SIZE; i+=STACK_BLOCK_SIZE){
        memcpy(stack_page+i,init,sizeof(init));
    }

    for(i=2; i<ALLOC_PAGE_SIZE; i+=STACK_BLOCK_SIZE){
        stack_page[i] = (mword)(stack_page+i+2);
    }

    for(i=4; i<ALLOC_PAGE_SIZE; i+=STACK_BLOCK_SIZE){
        stack_page[i] = (mword)(stack_page+i+3);
    }

}

// Clayton Bauman 2012

