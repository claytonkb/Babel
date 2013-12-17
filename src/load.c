// load.c
//

#include "babel.h"
#include "load.h"
#include "except.h"
#include "stack.h"
#include "bstruct.h"
#include "array.h"
//#include "bvm_opcodes.h"
#include "list.h"
#include "alloc.h"
#include "tptr.h"

//
// babel_operator
bvm_cache *load(bvm_cache *this_bvm){ // load#

    mword *op0 = dstack_get(this_bvm,0);//, size(dstack_get(this_bvm,0)));
    popd(this_bvm);
    mword *result   = _load(op0, size(op0));

    //zapd(this_bvm,0);
    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


mword *_load(mword *tree, mword tree_size){ // _load#

    int i;

    mword *LUT_abs   = _newin(tree_size);
    mword *LUT_rel   = _newlfi(tree_size,-1);

//        for(i=0; i<tree_size; i++){
//            c(LUT_rel,i) = (mword)-1;
//        }

    mword offset     = MWORD_SIZE;
    mword LUT_offset = 0;

    mword *result = rload(tree, offset, LUT_abs, LUT_rel, &LUT_offset);

    bfree(LUT_abs);
    bfree(LUT_rel);

    return result;

//    return rload(tree, offset, LUT_abs, LUT_rel, &LUT_offset);

//    mword *tree = global_VM-1;
//    load_tree_reset(tree, 1*MWORD_SIZE);
//    clean_tree(tree+1);

}

//rload
//
mword *rload(
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
            c(new_arr,i) = (mword)rload(
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
    else{ //is_tptr ... FIXME: 32-bit specific and UGLY
        s(tree+offset) |= 0x1; //Mark dumped

        new_arr = _newtptr();

        c(LUT_rel,*LUT_offset) = offset*MWORD_SIZE;
        c(LUT_abs,*LUT_offset) = (mword)new_arr;
        *LUT_offset = *LUT_offset+1;

        c(new_arr,0) = c(tree,offset+0);
        c(new_arr,1) = c(tree,offset+1);
        c(new_arr,2) = c(tree,offset+2);
        c(new_arr,3) = c(tree,offset+3);
        c(new_arr,4) = (mword)(-1*MWORD_SIZE);
//        c(new_arr,5) = (mword)nil;
//        c(new_arr,6) = (mword)nil;
        c(new_arr,5) = (mword)rload(
                tree,
                c(tree,offset+5),
                LUT_abs, 
                LUT_rel, 
                LUT_offset);
//        c(new_arr,6) = (mword)rload(
//                tree,
//                c(tree,offset+6),
//                LUT_abs, 
//                LUT_rel, 
//                LUT_offset);


    }

    s(tree+offset) |= 0x1; //Mark dumped

    return new_arr;

}


mword *get_abs_offset(mword *LUT_rel, mword *LUT_abs, mword elem){

    int i=0;
    int LUT_size = size(LUT_rel);
    mword curr_elem;

    for(;i<LUT_size;i++){
        curr_elem = c(LUT_rel,i);
        if(curr_elem == elem){
            return (mword*)c(LUT_abs,i);
        }
        else if(curr_elem == (mword)-1){
            printf("%x\n", elem);
            fatal("Something went wrong");
            die; //FIXME: Is this ever a valid case?
            return (mword*)-1;
        }
    }

    return (mword*)-1;

}

///////////////////////////////////////////////////////////////////
//                  XXX MC ENHANCEMENT XXX
///////////////////////////////////////////////////////////////////

void mc_load(mword *bs){

    mc_rload(bs, 1);

    rclean(bs+1);

//    _dump(bs);
//    die;

}

//mc_rload
//
void mc_rload(mword *tree, mword offset){

    int i;

    mword *this_elem = tree+offset;

    if( TRAVERSED((mword*)(this_elem)) ){ 
        return;
    }

//    printf("\n");
//    _mem(this_elem);

    int num_elem = size(this_elem);

//    d(num_elem);

    mword *new_arr;

    if(is_inte(this_elem)){

//trace;

        MARK_TRAVERSED(this_elem);

        for(i=0; i<num_elem; i++){
            mc_rload(tree, c(this_elem,i)/MWORD_SIZE );
            c(this_elem,i) = (mword)(c(this_elem,i) + tree); // add base offset
        }

    }
    else if(is_leaf(this_elem)){

//trace;

        MARK_TRAVERSED(this_elem);

    }
    else{

//trace;

        MARK_TRAVERSED(this_elem);

        mc_rload(tree, c(this_elem,TPTR_PTR)/MWORD_SIZE );
        c(this_elem,TPTR_PTR) = (mword)(c(this_elem,TPTR_PTR) + tree);

    }

//    MARK_TRAVERSED(this_elem);
//
//    s(tree+*offset) |= 0x1; //Mark dumped
//
//    return new_arr;

}



///////////////////////////////////////////////////////////////////
//                  XXX END OF MC ENHANCEMENT XXX
///////////////////////////////////////////////////////////////////


//
// babel_operator
bvm_cache *unload(bvm_cache *this_bvm){ // unload#

    mword *op0 = dstack_get(this_bvm,0);//, size(dstack_get(this_bvm,0)));
    popd(this_bvm);
    mword *result   = _unload(op0);

    //zapd(this_bvm,0);
    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;
    
}

// XXX: (perf) the LUT method is highly non-optimal with regard to
// space, there is no reason unload() should require 2N space to
// unload an object of size N. Future perf enhancement will implement
// a binary tree to store the address translations.
mword *_unload(mword *bs){//, mword offset){

    mword bs_size     = _mu   (bs);
    mword num_arrays  = _nin  (bs);
          num_arrays += _nlf  (bs);
          num_arrays += _ntag (bs);

    mword *dest      = _newlf(bs_size);
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

    c(LUT_abs,*LUT_offset) = (mword)bs;
    c(LUT_rel,*LUT_offset) = *offset;
    *LUT_offset = *LUT_offset+1;

    mword local_offset = *offset;

    if(is_inte(bs)){
//        printf("A ");
        MARK_TRAVERSED(bs);
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
    else if(is_leaf(bs)){
//        printf("B ");
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            c(dest,(*offset)) = c(bs,i);
            *offset = *offset+1;
        }
    }
    else{ //tptr
//        printf("C ");
        MARK_TRAVERSED(bs);
        for(i=0; i<HASH_SIZE; i++){
            c(dest,(*offset)) = c(bs,i);
            *offset = *offset+1;
        }
        _runload((mword*)bs+HASH_SIZE+1, 
                    LUT_abs, 
                    LUT_rel, 
                    dest, 
                    offset, 
                    LUT_offset);
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

