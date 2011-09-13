// XXX STACK FIX DONE
// count.c
//
// Utilities for counting things in the Babel tree

#include "babel.h"
#include "count.h"
#include "load.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "stack.h"

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

    s(tree) |= 0x1;

    if(is_inte(tree)){
        for(i=0; i<num_elem; i++){
            count += _nlf((mword *)*(tree+i));
        }
    }
    else{ // is_leaf
        count = 1;
    }

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

    if(is_leaf(tree)){
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

// Clayton Bauman 2011

