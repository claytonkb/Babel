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
#include "mem.h"

//
// babel_operator
bvm_cache *load(bvm_cache *this_bvm){ // load#

    mword *op0 = dstack_get(this_bvm,0);//, size(dstack_get(this_bvm,0)));
    popd(this_bvm);

    mword *result   = _load(op0, alloc_size(op0));

//    _mem(result);
//
//    die;
//
    //zapd(this_bvm,0);
    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


///////////////////////////////////////////////////////////////////
//                  XXX MC ENHANCEMENT XXX
///////////////////////////////////////////////////////////////////

mword *_load(mword *bs, mword size){

//    size = MWORDS(size);
//
//    mword *load_area = malloc(size); //FIXME
//    memcpy(load_area, bs, size);
//
//    _rload(load_area, 1);
//    rclean(load_area+1);
//
//    return load_area+1;

//trace;
//_mem(bs+1);
//

    _rload(bs, 1);

    rclean(bs+1);

//    int i;
//    for(i=0;i<size;i++){
//        //printf("%08x\n", c(bbl,i));
//        printf("%08x\n", c(bs,i));
//    }
//
//    die;

    return (bs+1);

}

//_rload
//
void _rload(mword *tree, mword offset){

    int i;

    mword *this_elem = tree+offset;

    if( TRAVERSED((mword*)(this_elem)) ){ 
        return;
    }

    int num_elem = size(this_elem);

    mword *new_arr;

    if(is_inte(this_elem)){

        MARK_TRAVERSED(this_elem);

        for(i=0; i<num_elem; i++){
            _rload(tree, c(this_elem,i)/MWORD_SIZE );
            c(this_elem,i) = (mword)(c(this_elem,i)/MWORD_SIZE + tree); // add base offset
        }

    }
    else if(is_leaf(this_elem)){

        MARK_TRAVERSED(this_elem);

    }
    else{

        MARK_TRAVERSED(this_elem);
        MARK_TRAVERSED(this_elem+TPTR_PTR);

        _rload(tree, c(this_elem,TPTR_PTR)/MWORD_SIZE );
        c(this_elem,TPTR_PTR) = (mword)(c(this_elem,TPTR_PTR)/MWORD_SIZE + tree);

    }

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

//    mword *dest      = _newlf(bs_size);
//    mword *LUT_abs   = _newin(num_arrays);
//    mword *LUT_rel   = _newin(num_arrays);

    mword *dest      = newleaf(bs_size);

//    mword *LUT_abs   = newinte(num_arrays);
//    mword *LUT_rel   = newinte(num_arrays);

    mword *LUT_abs   = malloc(MWORDS(num_arrays));
    mword *LUT_rel   = malloc(MWORDS(num_arrays));

    mword offset     = 0;
    mword LUT_offset = 0;

    _runload(bs, LUT_abs, LUT_rel, dest, &offset, &LUT_offset);
    rclean(bs);

    free(LUT_abs);
    free(LUT_rel);

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

