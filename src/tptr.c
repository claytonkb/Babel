// tptr.c
//

#include "tptr.h"
#include "alloc.h"
#include "pearson16.h"

//tag-checking (equal, etc.)

//
//
mword *new_tptr(const mword *hash, mword *bs){ // new_tptr#

//    mword *ptr = balloc( MWORDS( TPTR_SIZE ) ); 
    mword *ptr = malloc( MWORDS( TPTR_SIZE ) ); 
    int i;

    ptr[0] = 0; // X.s = 0 -> tagged-ptr
    
    for(i=0; i<HASH_SIZE; i++){
        ptr[i+1] = hash[i];
    }

    ptr[HASH_SIZE+1] = -1*MWORD_SIZE;
    ptr[HASH_SIZE+2] = (mword)bs;

    return ptr+1;

}


//
//
mword *tptr_extract_hash(mword *tptr){ // tptr_extract_hash#

    mword *ptr = new_hash();
    int i;

    for(i=0; i<HASH_SIZE; i++){
        ptr[i] = c(tptr,i);
    }

    return ptr;

}


//
//
void tptr_update_hash(mword *tptr, mword *hash){ // tptr_update_hash#

    int i;

    for(i=0; i<HASH_SIZE; i++){
        tptr[i] = c(hash,i);
    }

}


//
//
mword *tptr_extract_ptr(mword *tptr){ // tptr_extract_ptr#

    mword* temp = (mword*)(tptr+TPTR_PTR);

    if(is_tptr(temp)){
        return tptr_extract_ptr(temp);
    }
    else{
        return temp;
    }

}



// Safely, recursively de-references a tag
mword *detag(mword *tptr){

    if(is_tptr(tptr)){
        return detag(get_tptr(tptr));
    }
    else{
        return tptr;
    }

}

// Hard de-references a tag (recursive)
mword *hard_detag(mword *tptr){

    mword *temp = get_tptr(tptr);

    if(is_tptr(temp)){
        return hard_detag(temp);
    }
    else{
        return temp;
    }

}

// Clayton Bauman 2013

