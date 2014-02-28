// tptr.c
//

#include "tptr.h"
#include "alloc.h"
#include "pearson16.h"
#include "mem.h"
#include "stack.h"


//
//
bvm_cache *get_tag(bvm_cache *this_bvm){ // get_tag#

    mword *tptr = dstack_get(this_bvm,0);

    popd(this_bvm);

    pushd(this_bvm, tptr_extract_hash(this_bvm, tptr), IMMORTAL);

    return this_bvm;    

}


//
//
bvm_cache *newtptrop(bvm_cache *this_bvm){ // newtptrop#

    mword *hash = dstack_get(this_bvm,1);
    mword *ptr  = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

    pushd(this_bvm, new_tptr(this_bvm, hash, ptr), IMMORTAL);

    return this_bvm;    

}


//
//
mword *new_tptr(bvm_cache *this_bvm, const mword *hash, mword *bs){ // new_tptr#

//    mword *ptr = malloc( MWORDS( TPTR_SIZE ) ); 
//    int i;
//
//    ptr[0] = 0; // X.s = 0 -> tagged-ptr
//    
//    for(i=0; i<HASH_SIZE; i++){
//        ptr[i+1] = hash[i];
//    }
//
//    ptr[HASH_SIZE+1] = -1*MWORD_SIZE;
//    ptr[HASH_SIZE+2] = (mword)bs;
//
//    return ptr+1;

    mword *ptr = mc_alloc( this_bvm, 0 ); //FIXME: Naked constant

//    mword *ptr = malloc( MWORDS(TPTR_SIZE) );
//    ptr[0] = 0;
//    ptr = ptr + 1;

    int i;
    for(i=0; i<HASH_SIZE; i++){
        ptr[i] = hash[i];
    }

    ptr[HASH_SIZE  ] = -1*MWORD_SIZE;
    ptr[HASH_SIZE+1] = (mword)bs;

    return ptr;

}


//
//
mword *tptr_extract_hash(bvm_cache *this_bvm, mword *tptr){ // tptr_extract_hash#

    //mword *ptr = new_hash(this_bvm);
    mword *ptr = _newlfi(this_bvm, HASH_SIZE, 0);
    int i;

    for(i=0; i<HASH_SIZE; i++){
        ptr[i] = c(tptr,i);
    }

    return ptr;

}


//
//
void tptr_update_hash(bvm_cache *this_bvm, mword *tptr, mword *hash){ // tptr_update_hash#

    int i;

    for(i=0; i<HASH_SIZE; i++){
        tptr[i] = c(hash,i);
    }

}


//
//
mword *tptr_extract_ptr(bvm_cache *this_bvm, mword *tptr){ // tptr_extract_ptr#

    mword* temp = (mword*)(tptr+TPTR_PTR);

    if(is_tptr(temp)){
        return tptr_extract_ptr(this_bvm, temp);
    }
    else{
        return temp;
    }

}


//
//
bvm_cache *detag_op(bvm_cache *this_bvm){ // detag_op#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *result = get_tptr(op0);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

#define MAX_DETAG_DEPTH 1024

// Safely, recursively de-references a tag
mword *detag(bvm_cache *this_bvm, mword *tptr){ // *detag#

    static int livelock_detect=0;

    if(is_tptr(tptr)){
        if(livelock_detect++ > MAX_DETAG_DEPTH){
            cat_except(this_bvm);
        }
        return detag(this_bvm, get_tptr(tptr));
    }
    else{
        livelock_detect=0;
        return tptr;
    }

}

// Hard de-references a tag (recursive)
mword *hard_detag(bvm_cache *this_bvm, mword *tptr){ // *hard_detag#

    mword *temp = get_tptr(tptr);

    if(is_tptr(temp)){
        return hard_detag(this_bvm, temp);
    }
    else{
        return temp;
    }

}

// Clayton Bauman 2013

