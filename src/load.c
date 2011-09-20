// load.c
//

#include "babel.h"
#include "load.h"
#include "except.h"
#include "stack.h"
#include "count.h"
#include "array.h"
#include "bvm_opcodes.h"

//
//
//void load(void){
//
//    mword *tree = global_VM-1;
//    load_tree(tree, 1*MWORD_SIZE);
//    clean_tree(global_VM);
//
//}

void load(void){

    _load((mword*)TOS_0);
//    d(TOS_0)
//    die
    TOS_0 = TOS_0 + MWORD_SIZE;

//    zap();
//    push_alloc(result, LOAD);

}

void _load(mword *tree){//, mword offset){

//    mword *tree = global_VM-1;
    load_tree(tree, 1*MWORD_SIZE);
    clean_tree(tree+1);

}

//load_tree
//
void load_tree(mword *tree, mword offset){
//void _load(mword *tree, mword offset){

    int i;
    #define tree_base (mword)tree;
    offset /= MWORD_SIZE ;

    if(offset > global_machine_page_size){
        except("load_tree: offset out of bounds", __FILE__, __LINE__);
    }

    if( s(tree+offset) & (MWORD_SIZE-1) ){ //Already loaded
        return;
    }

    int num_elem = size(tree+offset);

    if( 
            offset+num_elem > global_machine_page_size 
            ||
            num_elem == 0
      ){
        except("load_tree: array size is incorrect", __FILE__, __LINE__);
    }
    
    s(tree+offset) |= 0x1; //Mark visited

    if( is_inte(tree+offset) ){

        for(i=0; i<num_elem; i++){
            load_tree(tree, (mword)*(tree+offset+i));
            *(tree+offset+i) += tree_base;
        }

    }

}

void clean_tree(mword *tree){

    int i;

    if( !(s(tree) & (MWORD_SIZE-1)) ){ //Already cleaned
        return;
    }

    s(tree) = s(tree) & ~(MWORD_SIZE-1); //Mark clean

    if( is_inte(tree) ){

        for(i=0; i<size(tree); i++){
            clean_tree((mword *)*(tree+i));
        }

    }

}


void dump(void){

    dump_tree(global_VM);
    clean_tree(global_VM);

}

void dump_tree(mword *tree){

    int i;

    if( s(tree) & (MWORD_SIZE-1) ){ //Already dumped
        return;
    }

    int num_elem = size(tree);
    s(tree) |= 0x1; //Mark dumped

//    printf("num_elem %08x\n", num_elem);

    for(i=0; i<num_elem; i++){
        printf("%08x %08x\n", (mword)(tree+i), (mword *)*(tree+i));
        if(is_inte(tree)){
            dump_tree((mword *)*(tree+i));
        }
    }

}

void arser(void){

    arser_tree(global_VM);
    clean_tree(global_VM);

}


void arser_tree(mword *tree){

    int i;

    if( s(tree) & (MWORD_SIZE-1) ){ //Already dumped
        return;
    }

    int num_elem = size(tree);
    printf("-------- %08x\n", (mword)s(tree));
    s(tree) |= 0x1; //Mark dumped

//    printf("num_elem %08x\n", num_elem);

    for(i=0; i<num_elem; i++){
        if(is_inte(tree)){
            printf("%08x %08x\n", (mword)(tree+i), (mword)((mword *)*(tree+i)));
        }
        else{
            printf("%08x %08x\n", (mword)(tree+i), (mword *)*(tree+i));
        }
    }

    for(i=0; i<num_elem; i++){
        if(is_inte(tree)){
            arser_tree((mword *)*(tree+i));
        }
    }

}

// FIXME TOTALLY BROKEN!!!!!!!!!! FIXME
//
// unload() is going to be slightly more complex than load().
// we're going to need two arrays big enough to fit the babel structure
// being unloaded - one array will be a translation table between memory
// addresses and the destination array offsets and the second array will
// be the destination array.
void unload(void){

    mword* result = _newlf(_mu((mword*)TOS_0));
    clean_tree((mword*)TOS_0);

    unload_tree(TOS_0, (mword*)TOS_0, result, 0);
    clean_tree((mword*)TOS_0);

//    unload_tree((mword*)TOS_0);
    push_alloc(result, UNLOAD);

}

void unload_tree(mword base, mword* tree, mword* dest, mword offset){

    int i;


    if( s(tree) & (MWORD_SIZE-1) ){ //Already dumped
        return;
    }

    int num_elem = size(tree);

//    printf("-------- %08x\n", (mword)s(tree));
    *(dest+offset) = s(tree);
    offset++;

    s(tree) |= 0x1; //Mark dumped

//    printf("num_elem %08x\n", num_elem);

    for(i=0; i<num_elem; i++){
        if(is_inte(tree)){
//            printf("%08x %08x\n", (mword)(tree+i), (mword)((mword *)*(tree+i)));
            *(dest+offset) = (mword)((mword *)*(tree+i)) - base;
        }
        else{
//            printf("%08x %08x\n", (mword)(tree+i), (mword *)*(tree+i));
            *(dest+offset) = (mword)((mword *)*(tree+i));
        }
        offset++;
    }

    for(i=0; i<num_elem; i++){
        if(is_inte(tree)){
            unload_tree(base, (mword *)*(tree+i), dest, offset);
        }
    }

}




// Clayton Bauman 2011

