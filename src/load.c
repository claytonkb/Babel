// load.c
//

#include "babel.h"
#include "load.h"
#include "except.h"
#include "stack.h"
#include "count.h"
#include "array.h"
#include "bvm_opcodes.h"
#include "list.h"

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

//    if(offset > global_machine_page_size){
//        except("load_tree: offset out of bounds", __FILE__, __LINE__);
//    }

    if( s(tree+offset) & (MWORD_SIZE-1) ){ //Already loaded
        return;
    }

    int num_elem = size(tree+offset);

//    if( 
//            offset+num_elem > global_machine_page_size 
//            ||
//            num_elem == 0
//      ){
//        except("load_tree: array size is incorrect", __FILE__, __LINE__);
//    }
    
    s(tree+offset) |= 0x1; //Mark visited

    if( is_inte(tree+offset) ){

        for(i=0; i<num_elem; i++){
            load_tree(tree, (mword)*(tree+offset+i));
            *(tree+offset+i) += tree_base;
        }

    }

}


mword *_load_at_reset(mword *tree, mword tree_size){//, mword offset){

    int i;

//    mword tree_size  = size(tree);
//    mword *dest      = 0;
    mword *LUT_abs   = _newin_blank(tree_size);
    mword *LUT_rel   = _newlf(tree_size);

        for(i=0; i<tree_size; i++){
            c(LUT_rel,i) = (mword)-1;
        }

    mword offset     = MWORD_SIZE;
    mword LUT_offset = 0;

    return load_tree_reset(tree, offset, LUT_abs, LUT_rel, &LUT_offset);

//    mword *tree = global_VM-1;
//    load_tree_reset(tree, 1*MWORD_SIZE);
//    clean_tree(tree+1);

}

//load_tree_reset
//
mword *load_tree_reset(
        mword *tree, 
        mword offset,
        mword *LUT_abs, 
        mword *LUT_rel, 
        mword *LUT_offset){

    offset = offset / MWORD_SIZE;

    int i;

    if( s((mword*)(tree+offset)) & (MWORD_SIZE-1) ){ //Already dumped
        return get_abs_offset(LUT_rel, LUT_abs, offset);
    }

    int num_elem = size(tree+offset);
    
////    printf("-------- %08x\n", (mword)s(tree));
//    *(dest+(*offset)) = s(tree);
//    *offset = *offset+1;
    s(tree+offset) |= 0x1; //Mark dumped

    mword *new_arr;

    if(is_inte(tree+offset)){ //<--- FIXME DOESN'T WORK
//        printf("inte\n");
        new_arr = _newin_blank(num_elem);

        c(LUT_rel,*LUT_offset) = offset*MWORD_SIZE;
        c(LUT_abs,*LUT_offset) = (mword)new_arr;
        *LUT_offset = *LUT_offset+1;

        for(i=0; i<num_elem; i++){
//            d(c(tree,offset+i))
            c(new_arr,i) = (mword)load_tree_reset(
                tree,
                c(tree,offset+i),
                LUT_abs, 
                LUT_rel, 
                LUT_offset);
        }

        // foreach
        //     recurse
        //     copy returned value into allocated slot
    }
    else{
//        printf("leaf\n");
        new_arr = _newlf(num_elem);

        c(LUT_rel,*LUT_offset) = offset*MWORD_SIZE;
        c(LUT_abs,*LUT_offset) = (mword)new_arr;
        *LUT_offset = *LUT_offset+1;

        for(i=0; i<num_elem; i++){
            c(new_arr,i) = c(tree,offset+i);
        }
    }

    return new_arr;

}


mword *get_abs_offset(mword *LUT_rel, mword *LUT_abs, mword *elem){

    int i=0;
    int LUT_size = size(LUT_rel);
    mword curr_elem;

    for(;i<LUT_size;i++){
        curr_elem = c(LUT_rel,i);
        if(curr_elem == (mword)elem){
            return (mword*)c(LUT_abs,i);
        }
        else if(curr_elem == (mword)-1){
            return (mword*)-1;
        }
    }

    return (mword*)-1;

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

void unload(void){

    mword *result = _unload((mword*)TOS_0);
//    d(TOS_0)
//    die
//    TOS_0 = TOS_0 + MWORD_SIZE;

    zap();
    push_alloc(result, UNLOAD);

}

// XXX: (perf) the LUT method is highly non-optimal with regard to
// space, there is no reason unload() should require 2N space to
// unload an object of size N. Future perf enhancement will implement
// a binary tree to store the address translations.
mword *_unload(mword *tree){//, mword offset){

    mword tree_size =  _mu(tree);
    clean_tree(tree);

    mword num_arrays = _nin(tree);
    clean_tree(tree);
   
    num_arrays += _nlf(tree);
    clean_tree(tree);

//    d(tree_size)
//        die

    mword *dest      = _newlf(tree_size);
    mword *LUT_abs   = _newin(num_arrays);
    mword *LUT_rel   = _newin(num_arrays);
    mword offset     = 0;
    mword LUT_offset = 0;

    unload_tree(tree, LUT_abs, LUT_rel, dest, &offset, &LUT_offset);
    clean_tree(tree);

    return dest;

}


mword unload_tree(
        mword *tree, 
        mword *LUT_abs, 
        mword *LUT_rel, 
        mword *dest, 
        mword *offset,
        mword *LUT_offset){

    int i;
    mword rel_offset;

    if( s(tree) & (MWORD_SIZE-1) ){ //Already dumped
        return get_rel_offset(LUT_abs, LUT_rel, tree);
    }

    int num_elem = size(tree);

//    printf("-------- %08x\n", (mword)s(tree));
    *(dest+(*offset)) = s(tree);
    *offset = *offset+1;
    s(tree) |= 0x1; //Mark dumped

    c(LUT_abs,*LUT_offset) = (mword)tree;
    c(LUT_rel,*LUT_offset) = *offset;
    *LUT_offset = *LUT_offset+1;

    mword local_offset = *offset;

    if(is_inte(tree)){
        *offset = *offset + num_elem;
        for(i=0; i<num_elem; i++){
            c(dest,local_offset+i) = unload_tree(
                                        (mword*)c(tree,i), 
                                        LUT_abs, 
                                        LUT_rel, 
                                        dest, 
                                        offset, 
                                        LUT_offset)
                                    * MWORD_SIZE
                ;
        }
    }
    else{
        for(i=0; i<num_elem; i++){
            c(dest,(*offset)) = c(tree,i);
            *offset = *offset+1;
        }
    }

    return local_offset;

}

mword get_rel_offset(mword *LUT_abs, mword *LUT_rel, mword *elem){

    int i=0;
    int LUT_size = size(LUT_abs);

    for(;i<LUT_size;i++){
        if(c(LUT_abs,i) == (mword)elem) 
            return c(LUT_rel,i);
    }
    return nil;

}



// Clayton Bauman 2011

