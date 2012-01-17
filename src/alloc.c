// alloc.c
//

#include "babel.h"
#include "alloc.h"
#include "stack.h"
#include "count.h"
#include "array.h"
#include "load.h"

void free_op(void){

    _free_op((mword*)TOS_0);
    zap();

}

void _free_op(mword *mem){

    free(mem-1);

}

void del(void){


    _del((mword*)TOS_0);
    zap();

}

void _del(mword *tree){

    mword num_arrays = _nin(tree);
    clean_tree(tree);
    num_arrays += _nlf(tree);
    clean_tree(tree);

    mword *arr_list = _newlf(num_arrays);

    mword offset = 0;

    del_tree(tree, arr_list, offset);

}

void del_tree(mword *tree, mword *arr_list, mword offset){

//    int i;
//
//    if( s((mword*)(tree+offset)) & (MWORD_SIZE-1) ){ //Already dumped
//        return get_abs_offset(LUT_rel, LUT_abs, offset*MWORD_SIZE);
//    }
//
//    int num_elem = size(tree+offset);
//    
//    s(tree+offset) |= 0x1; //Mark dumped
//
//    mword *new_arr;
//
//    if(is_inte(tree+offset)){
//        new_arr = _newin_blank(num_elem);
//
//        c(LUT_rel,*LUT_offset) = offset*MWORD_SIZE;
//        c(LUT_abs,*LUT_offset) = (mword)new_arr;
//        *LUT_offset = *LUT_offset+1;        
//
//        for(i=0; i<num_elem; i++){
//            c(new_arr,i) = (mword)load_tree(
//                tree,
//                c(tree,offset+i),
//                LUT_abs, 
//                LUT_rel, 
//                LUT_offset);
//        }
//
//    }
//    else{
//        new_arr = _newlf(num_elem);
//
//        c(LUT_rel,*LUT_offset) = offset*MWORD_SIZE;
//        c(LUT_abs,*LUT_offset) = (mword)new_arr;
//        *LUT_offset = *LUT_offset+1;
//
//        for(i=0; i<num_elem; i++){
//            c(new_arr,i) = c(tree,offset+i);
//        }
//    }
//
//    return new_arr;

}


mword check_address(mword *arr_list, mword *address){

//    int i=0;
//    int LUT_size = size(LUT_rel);
//    mword curr_elem;
//
//    for(;i<LUT_size;i++){
//        curr_elem = c(LUT_rel,i);
//        if(curr_elem == (mword)elem){
//            return (mword*)c(LUT_abs,i);
//        }
//        else if(curr_elem == (mword)-1){
//            return (mword*)-1;
//        }
//    }
//
//    return (mword*)-1;

    return 0;

}


// Clayton Bauman 2012

