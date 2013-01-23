// load.c
//

#include "babel.h"
#include "load.h"
#include "except.h"
#include "stack.h"
#include "bstruct.h"
#include "array.h"
#include "bvm_opcodes.h"
#include "list.h"

////
////
////void load(void){
////
////    mword *tree = global_VM-1;
////    load_tree(tree, 1*MWORD_SIZE);
////    clean_tree(global_VM);
////
////}

//
// babel_operator
bvm_cache *load(bvm_cache *this_bvm){

    mword *result    = new_atom;
    (mword *)*result = _load(TOS_0(this_bvm), size(TOS_0(this_bvm)));

    hard_zap(this_bvm);

    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

//    _load((mword*)TOS_0);
//    TOS_0 = TOS_0 + MWORD_SIZE;

}

////void _load(mword *tree){//, mword offset){
////
//////    mword *tree = global_VM-1;
////    load_tree(tree, 1*MWORD_SIZE);
////    clean_tree(tree+1);
////
////}
////
//////load_tree
//////
////void load_tree(mword *tree, mword offset){
//////void _load(mword *tree, mword offset){
////
////    int i;
////    #define tree_base (mword)tree;
////    offset /= MWORD_SIZE ;
////
//////    if(offset > global_machine_page_size){
//////        except("load_tree: offset out of bounds", __FILE__, __LINE__);
//////    }
////
////    if( s(tree+offset) & (MWORD_SIZE-1) ){ //Already loaded
////        return;
////    }
////
////    int num_elem = size(tree+offset);
////
//////    if( 
//////            offset+num_elem > global_machine_page_size 
//////            ||
//////            num_elem == 0
//////      ){
//////        except("load_tree: array size is incorrect", __FILE__, __LINE__);
//////    }
////    
////    s(tree+offset) |= 0x1; //Mark visited
////
////    if( is_inte(tree+offset) ){
////
////        for(i=0; i<num_elem; i++){
////            load_tree(tree, (mword)*(tree+offset+i));
////            *(tree+offset+i) += tree_base;
////        }
////
////    }
////
////}

mword *_load(mword *tree, mword tree_size){//, mword offset){

    int i;

//    mword tree_size  = size(tree);
//    mword *dest      = 0;
    mword *LUT_abs   = _newin(tree_size);
    mword *LUT_rel   = _newlf(tree_size);

        for(i=0; i<tree_size; i++){
            c(LUT_rel,i) = (mword)-1;
        }

    mword offset     = MWORD_SIZE;
    mword LUT_offset = 0;

    return load_tree(tree, offset, LUT_abs, LUT_rel, &LUT_offset);

//    mword *tree = global_VM-1;
//    load_tree_reset(tree, 1*MWORD_SIZE);
//    clean_tree(tree+1);

}

//load_tree_reset
//
mword *load_tree(
        mword *tree, 
        mword offset,
        mword *LUT_abs, 
        mword *LUT_rel, 
        mword *LUT_offset){

    offset /= MWORD_SIZE;

    int i;

    if( s((mword*)(tree+offset)) & (MWORD_SIZE-1) ){ //Already dumped
        return get_abs_offset(LUT_rel, LUT_abs, offset*MWORD_SIZE);
    }

    int num_elem = size(tree+offset);

    mword *new_arr;

    if(is_inte(tree+offset)){
        s(tree+offset) |= 0x1; //Mark dumped
        new_arr = _newin(num_elem);

        c(LUT_rel,*LUT_offset) = offset*MWORD_SIZE;
        c(LUT_abs,*LUT_offset) = (mword)new_arr;
        *LUT_offset = *LUT_offset+1;        

        for(i=0; i<num_elem; i++){
            c(new_arr,i) = (mword)load_tree(
                tree,
                c(tree,offset+i),
                LUT_abs, 
                LUT_rel, 
                LUT_offset);
        }

    }
    else if(is_leaf(tree+offset)){
        new_arr = _newlf(num_elem);

        c(LUT_rel,*LUT_offset) = offset*MWORD_SIZE;
        c(LUT_abs,*LUT_offset) = (mword)new_arr;
        *LUT_offset = *LUT_offset+1;

        for(i=0; i<num_elem; i++){
            c(new_arr,i) = c(tree,offset+i);
        }
    }
    else{ //FIXME: href
//        new_arr = _newref((mword*)(tree+offset));
//
////        d(num_elem)
////        for(i=0; i<num_elem; i++){
////            d(c(new_arr,i))// = c(tree,offset+i);
////        }
//
//        c(LUT_rel,*LUT_offset) = offset*MWORD_SIZE;
//        c(LUT_abs,*LUT_offset) = (mword)new_arr;
//        *LUT_offset = *LUT_offset+1;
//
//        for(i=0; i<num_elem; i++){
//            c(new_arr,i) = c(tree,offset+i);
//        }
    }

    s(tree+offset) |= 0x1; //Mark dumped

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

//
//void dump(void){
//
//    dump_tree(global_VM);
//    clean_tree(global_VM);
//
//}
//
//void dump_tree(mword *tree){
//
//    int i;
//
//    if( s(tree) & (MWORD_SIZE-1) ){ //Already dumped
//        return;
//    }
//
//    int num_elem = size(tree);
//    s(tree) |= 0x1; //Mark dumped
//
////    printf("num_elem %08x\n", num_elem);
//
//    for(i=0; i<num_elem; i++){
//        printf("%08x %08x\n", (mword)(tree+i), (mword *)*(tree+i));
//        if(is_inte(tree)){
//            dump_tree((mword *)*(tree+i));
//        }
//    }
//
//}
//
//void arser(void){
//
//    arser_tree(global_VM);
//    clean_tree(global_VM);
//
//}
//
//
//void arser_tree(mword *tree){
//
//    int i;
//
//    if( s(tree) & (MWORD_SIZE-1) ){ //Already dumped
//        return;
//    }
//
//    int num_elem = size(tree);
//    printf("-------- %08x\n", (mword)s(tree));
//    s(tree) |= 0x1; //Mark dumped
//
////    printf("num_elem %08x\n", num_elem);
//
//    for(i=0; i<num_elem; i++){
//        if(is_inte(tree)){
//            printf("%08x %08x\n", (mword)(tree+i), (mword)((mword *)*(tree+i)));
//        }
//        else{
//            printf("%08x %08x\n", (mword)(tree+i), (mword *)*(tree+i));
//        }
//    }
//
//    for(i=0; i<num_elem; i++){
//        if(is_inte(tree)){
//            arser_tree((mword *)*(tree+i));
//        }
//    }
//
//}

//
// babel_operator
bvm_cache *unload(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _unload(TOS_0(this_bvm));
//    d(TOS_0)
//    die
//    TOS_0 = TOS_0 + MWORD_SIZE;

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

}

// XXX: (perf) the LUT method is highly non-optimal with regard to
// space, there is no reason unload() should require 2N space to
// unload an object of size N. Future perf enhancement will implement
// a binary tree to store the address translations.
mword *_unload(mword *bs){//, mword offset){

    mword bs_size     = _mu   (bs);
    mword num_arrays  = _nin  (bs);
          num_arrays += _nlf  (bs);
          num_arrays += _nhref(bs);

    mword *dest      = _newlf(bs_size); // Error: returns wrong size due to hash-refs
    mword *LUT_abs   = _newin(num_arrays);
    mword *LUT_rel   = _newin(num_arrays);
    mword offset     = 0;
    mword LUT_offset = 0;

    _runload(bs, LUT_abs, LUT_rel, dest, &offset, &LUT_offset);
    rclean(bs);

    return dest;

}

//
mword _runload(
        mword *bs, 
        mword *LUT_abs, 
        mword *LUT_rel, 
        mword *dest, 
        mword *offset,
        mword *LUT_offset){

    int i;
    mword rel_offset;

    if( TRAVERSED(bs) ){ //& (MWORD_SIZE-1) ){ //Already dumped
        return get_rel_offset(LUT_abs, LUT_rel, bs);
    }

    int num_elem = size(bs);

//    printf("-------- %08x\n", (mword)s(bs));
    *(dest+(*offset)) = s(bs);
    *offset = *offset+1;
    MARK_TRAVERSED(bs); //s(bs) |= 0x1; //Mark dumped

    c(LUT_abs,*LUT_offset) = (mword)bs;
    c(LUT_rel,*LUT_offset) = *offset;
    *LUT_offset = *LUT_offset+1;

    mword local_offset = *offset;

    if(is_inte(bs)){
        *offset = *offset + num_elem;
        for(i=0; i<num_elem; i++){
            c(dest,local_offset+i) = _runload(
                                        (mword*)c(bs,i), 
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
            c(dest,(*offset)) = c(bs,i);
            *offset = *offset+1;
        }
    }

    return local_offset;

}

//
mword get_rel_offset(mword *LUT_abs, mword *LUT_rel, mword *entry){

    int i=0;
    int LUT_size = size(LUT_abs);

    for(;i<LUT_size;i++){
        if(c(LUT_abs,i) == (mword)entry) 
            return c(LUT_rel,i);
    }
    return (mword)nil;

}

// Clayton Bauman 2011

