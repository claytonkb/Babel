// hash.c
//

#include "babel.h"
#include "hash.h"
#include "list.h"
#include "except.h"

//void push_alloc(mword *operand, mword alloc_type){

// make hash-entry
mword *new_hash_entry(mword *val, mword *hash){

    mword *temp_consA = new_cons();
    mword *temp_consB = new_cons();

    cons(temp_consA,  hash,  nil);
    cons(temp_consB,  val,   temp_consA);

    return temp_consB;

}

// make hash-entry with key
mword *new_hash_key_entry(mword *val, mword *hash, mword *key){
}

void _insha(mword *hash_table, mword *val){

    if(hash_table == (mword*)nil){ //nil
    }
    else if( is_inte(hash_table) && (size(hash_table) == 2) ){ //list
    }
    else if( is_inte(hash_table) && (size(hash_table) == HASH_LEVEL_SIZE) ){ //list
    }
    else{
        except("_insha: Not a hash", __FILE__, __LINE__);
    }

}

void _inskha(mword *hash_table, mword *val, mword *key){
}

// Clayton Bauman 2011

