// bstruct.c
//

#include "babel.h"
#include "bstruct.h"
#include "load.h"
#include "bvm_opcodes.h"
#include "stack.h"
#include "bvm.h"
#include "array.h"
#include "list.h"

void mu(void) {


    mword *result    = new_atom();

    *result = _mu((mword*)TOS_0);
       clean_tree((mword*)TOS_0);


    zap();
    push_alloc(result, MU);
    
//    printf("size %d\n", size_tree(global_VM));

}

// Returns the size of a tree in mwords
mword _mu(mword *tree){

    int i;
    mword count = 0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
    count = num_elem + 1;

    s(tree) |= 0x1;

    if(is_inte(tree)){
        for(i=0; i<num_elem; i++){
            count += _mu((mword *)*(tree+i));
        }
    }

    return count;

}

void nlf(void) {

    mword *result    = new_atom();

    *result = _nlf((mword*)TOS_0);
        clean_tree((mword*)TOS_0);


    zap();
    push_alloc(result, NLF);


//    printf("leafs %d\n", leafs(global_VM));
//    clean_tree(global_VM);

}

// Returns 
mword _nlf(mword *tree){

    int i;
    mword count = 0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    if(is_inte(tree)){
        s(tree) |= 0x1;
        for(i=0; i<num_elem; i++){
            count += _nlf((mword *)*(tree+i));
        }
    }
    else if(is_href(tree)){
        count = 0;
    }
    else if(is_leaf(tree)){ // is_leaf
        count = 1;
    }

    s(tree) |= 0x1;

    return count;

}

void nhref(void) {

    mword *result    = new_atom();

    *result = _nhref((mword*)TOS_0);
        clean_tree((mword*)TOS_0);

    zap();
    push_alloc(result, NHREF);


//    printf("leafs %d\n", leafs(global_VM));
//    clean_tree(global_VM);

}

// Returns 
mword _nhref(mword *tree){

    int i;
    mword count = 0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    if(is_inte(tree)){
        s(tree) |= 0x1;
        for(i=0; i<num_elem; i++){
            count += _nhref((mword *)*(tree+i));
        }
    }
    else if(is_href(tree)){ // is_leaf
        count = 1;
    }
    else if(is_leaf(tree)){ // is_leaf
        count = 0;
    }

    s(tree) |= 0x1;

    return count;

}


void nin(void) {


    mword *result    = new_atom();

    *result = _nin((mword*)TOS_0);
        clean_tree((mword*)TOS_0);


    zap();
    push_alloc(result, NIN);

//    printf("intes %d\n", intes(global_VM));
//    clean_tree(global_VM);

}

// Returns 
mword _nin(mword *tree){

    int i;
    mword count = 0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    s(tree) |= 0x1;

    if(is_leaf(tree) || is_href(tree)){
        count = 0;
    }
    else{
        for(i=0; i<num_elem; i++){
            count += _nin((mword *)*(tree+i));
        }
        count++;
    }

    return count;

}


void nva(void) {


    mword *result    = new_atom();

    *result = _nva((mword*)TOS_0);
        clean_tree((mword*)TOS_0);


    zap();
    push_alloc(result, NIN);

//    printf("vals %d\n", vals(global_VM));
//    clean_tree(global_VM);

}

// Returns 
mword _nva(mword *tree){

    int i;
    mword count = 0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    s(tree) |= 0x1;

    if(is_inte(tree)){
        for(i=0; i<num_elem; i++){
            count += _nva((mword *)*(tree+i));
        }
    }
    else{ // is_leaf
        count = num_elem;
    }

    return count;

}

void npt(void) {


    mword *result    = new_atom();

    *result = _npt((mword*)TOS_0);
        clean_tree((mword*)TOS_0);


    zap();
    push_alloc(result, NIN);

//    printf("ptrs %d\n", ptrs(global_VM));
//    clean_tree(global_VM);

}

// Returns 
mword _npt(mword *tree){

    int i;
    mword count = 0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    s(tree) |= 0x1;

    if(is_inte(tree)){
        for(i=0; i<num_elem; i++){
            count += _npt((mword *)*(tree+i));
        }
        count += num_elem;
    }

    return count;

}

void cp(void){

    mword *result = _unload((mword*)TOS_0);
    push_alloc(result, CP);

    load();

}

void span(void){

    mword *result = _bs2ar((mword*)TOS_0);

    zap();

    push_alloc(result, SPAN);

}

//Creates interior array with one pointer to each array
//in a bstruct
//TODO: Look into re-writing load/unload to utilize bs2ar...
mword *_bs2ar(mword *tree){

    mword num_arrays = _nin(tree);
    clean_tree(tree);
    num_arrays += _nlf(tree);
    clean_tree(tree);
    num_arrays += _nhref(tree);
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

