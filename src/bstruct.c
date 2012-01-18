// bstruct.c
//

#include "babel.h"
#include "bstruct.h"
#include "load.h"
#include "bvm_opcodes.h"
#include "stack.h"
#include "bvm.h"
#include "array.h"
#include "count.h"

void cp(void){

    mword *result = _unload((mword*)TOS_0);
    push_alloc(result, CP);

    load();

}

//Creates interior array with one pointer to each array
//in a bstruct
//TODO: Look into re-writing load/unload to utilize bs2ar...
mword *_bs2ar(mword *tree){

    mword num_arrays = _nin(tree);
    clean_tree(tree);
    num_arrays += _nlf(tree);
    clean_tree(tree);

    mword *arr_list = _newin(num_arrays);
    mword offset = 0;

    bs2ar_tree(tree, arr_list, &offset);
    clean_tree(tree);

    return arr_list;

}

void bs2ar_tree(mword *tree, mword *arr_list, mword *offset){

    int i;

    if( s(tree) & (MWORD_SIZE-1) ){ //Already dumped
        return;
    }

    int num_elem = size(tree);
    
    s(tree) |= 0x1; //Mark dumped

    c(arr_list,*offset) = (mword)tree;
    *offset = *offset+1;

    if(is_inte(tree)){
        for(i=0; i<num_elem; i++){
            bs2ar_tree((mword*)c(tree,i),arr_list,offset);
        }
    }

}

// Clayton Bauman 2012

