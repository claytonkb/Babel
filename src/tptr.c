// tptr.c
//

#include "tptr.h"
#include "alloc.h"
#include "pearson16.h"

//tag-checking (equal, etc.)
//new tagged-pointer

//
//
mword *new_tptr(mword *hash, mword *list){ // new_tptr#

    mword *ptr = balloc( MWORDS( 3 + HASH_SIZE ) ); // 3 = s-field + s-field + car
    int i;

    ptr[0] = 0; // X.s = 0 -> tagged-ptr
    
    for(i=0; i<HASH_SIZE; i++){
        ptr[i+1] = hash[i];
    }

    ptr[HASH_SIZE+1] = -1*MWORD_SIZE;
    ptr[HASH_SIZE+2] = c(list,0);
    //ptr[HASH_SIZE+3] = c(list,1);

    return ptr+1;

}


//
//
mword *extract_hash(mword *tptr){ // extract_hash#

    mword *ptr = new_hash();
    int i;

    for(i=0; i<HASH_SIZE; i++){
        ptr[i] = c(tptr,i);
    }

    return ptr;

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

