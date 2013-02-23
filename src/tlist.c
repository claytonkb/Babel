// tlist.c
//

#include "tlist.h"
#include "alloc.h"

//tag-checking (equal, etc.)
//new tagged-list

//
//
mword *new_tlist(mword *hash, mword *list){ // new_tlist#

    mword *ptr = balloc( MWORDS( 3 + HASH_SIZE + 1 ) ); // 3 = s-field + car + cdr
    int i;

    ptr[0] = 0; // X.s = 0 -> tagged-list
    
    for(i=0; i<HASH_SIZE; i++){
        ptr[i+1] = hash[i];
    }

    ptr[HASH_SIZE+1] = -2*MWORD_SIZE;
    ptr[HASH_SIZE+2] = c(list,0);
    ptr[HASH_SIZE+3] = c(list,1);

    return ptr+1;

}

// Clayton Bauman 2013

