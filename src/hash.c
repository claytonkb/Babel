// hash.c
//

#include "babel.h"
#include "hash.h"
#include "list.h"
#include "except.h"
#include "array.h"
#include "stack.h"
#include "pearson16.h"
#include "bvm.h"
#include "bvm_opcodes.h"
#include "bstruct.h"
#include "tlist.h"
#include "string.h"

//
//
mword *_new_hash_table(void){

    return _new_tlist(_hash8(C2B("/babel/tag/hash_table")), _consls(nil, nil));

}

//
//
mword *_new_hash_table_entry(mword *hash, mword *key, mword *payload){

    if(is_nil(hash)){
        hash = _hash8(key);
    }

    return nil;

}

// FIXME: USES HREF
mword *new_hash_entry(mword *hash, mword *key, mword *val, mword ref_count, mword bounding){

//    mword *href = _newref(hash);
//
//    mword *ref_count_ptr = new_atom;
//    *ref_count_ptr       = ref_count;
//
//    mword *bounding_ptr  = new_atom;
//    *bounding_ptr        = bounding;
//
//    mword *hash_entry    = _newin(HASH_ENTRY_SIZE);
//
////    (mword*)c(hash_entry,0) = href;
////    (mword*)c(hash_entry,1) = val;
////    (mword*)c(hash_entry,2) = key;
////    (mword*)c(hash_entry,3) = ref_count_ptr;
////    (mword*)c(hash_entry,4) = bounding_ptr;
//
//    HASH_ENTRY_REF(hash_entry) = href;
//    HASH_ENTRY_VAL(hash_entry) = val;
//    HASH_ENTRY_KEY(hash_entry) = key;
//    HASH_ENTRY_CNT(hash_entry) = ref_count_ptr;
//    HASH_ENTRY_BND(hash_entry) = bounding_ptr;
//
//    return hash_entry;

}

//
mword *_insha(mword *hash_table, mword *hash, mword *val){

    mword *result;
    mword *temp;
    mword cons_side;

//    mword *hash = _hash8(key);
    mword *key = empty_string;

    if(is_nil(hash_table)){
        cons_side = _cxr1(hash,0);
        temp = new_hash_entry(hash, key, val, (mword)-1, 0);
        result = new_cons;
        c(result,cons_side) = (mword)temp;
//printf("%s",_bs2gv(temp));
    }
    else{
        _rinskha(hash_table, hash, key, val, 0);
        result = hash_table;
    }

    return result;

}

// inskha - insert with key into hash
// babel_operator
bvm_cache *inskha(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *hash_table = TOS_2(this_bvm);
    mword *key        = TOS_1(this_bvm);
    mword *val        = TOS_0(this_bvm);

    mword *result = _inskha(hash_table, key, val);

    zap(this_bvm);
    zap(this_bvm);
    zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Revisit

//printf("%s\n\n",_bs2gv(this_bvm->stack_ptr));

//    _dump(this_bvm->stack_ptr);
//    die;

    return this_bvm;

}

//
mword *_inskha(mword *hash_table, mword *key, mword *val){

    mword *result;
    mword *temp;
    mword cons_side;

    mword *hash = _hash8(key);

    if(is_nil(hash_table)){
        cons_side = _cxr1(hash,0);
        temp = new_hash_entry(hash, key, val, (mword)-1, 0);
        result = new_cons;
        c(result,cons_side) = (mword)temp;
//printf("%s",_bs2gv(temp));
    }
    else{
        _rinskha(hash_table, hash, key, val, 0);
        result = hash_table;
    }

    return result;

}

//
void _rinskha(mword *hash_table, mword *hash, mword *key, mword *val, mword level){

    mword *temp;
    mword cons_side   = _cxr1(hash,level);
    mword *next_level = (mword*)c(hash_table,cons_side);

    // 1. cons_side = nil
    //      insert
    // 2. cons_side is inte AND size = 2
    //      recurse
    // 3. cons_side is inte AND size = HASH_ENTRY_SIZE
    //      split and insert

//    printf("%x\n",s(next_level));

    if(is_nil(next_level)){

        (mword*)c(hash_table,cons_side) 
            = new_hash_entry(hash, key, val, (mword)-1, 0);

    }
    else if(is_inte(next_level) && size(next_level) == 2){

        _rinskha((mword*)c(hash_table,cons_side), hash, key, val, level+1);

    }
    else if(is_inte(next_level) && size(next_level) == HASH_ENTRY_SIZE){

        mword *colliding_hash_ref           = HASH_ENTRY_REF(next_level);
        mword colliding_next_cons_side      = _cxr1(colliding_hash_ref,level+1);

        temp = new_cons;
        (mword *)c(temp,colliding_next_cons_side) = next_level;

        (mword*)c(hash_table,cons_side)     = temp;

        _rinskha((mword*)c(hash_table,cons_side), hash, key, val, level+1);

    }
    else{
        //printf("%s",_bs2gv(next_level));
        //die
        error("_rinskha: Error inserting into hash");
    }

}

//
// babel_operator
bvm_cache *exha(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *hash_table   = TOS_1(this_bvm);
    mword *key          = TOS_0(this_bvm);
    mword *hash         = _hash8(key);

    mword *result = new_atom;

    if(is_nil(hash_table)){
        *result = 0;
    }
    else{
        *result = _exha(hash_table,hash);
    }

    zap(this_bvm);

    push_alloc(this_bvm, result, MORTAL);

}

//
mword _exha(mword *hash_table, mword *hash){

    if(is_nil(hash_table)){
        return 0;
    }
    else{
        return _rexha(hash_table, hash, 0);
    }

}

//
mword _rexha(mword *hash_table, mword *hash, mword level){

    mword *temp;
    mword cons_side   = _cxr1(hash,level);
    mword *next_level = (mword*)c(hash_table,cons_side);

    if(is_nil(next_level)){ //dead-end
        return 0;
    }
    else if(is_inte(next_level) && size(next_level) == 2){
        _rexha((mword*)c(hash_table,cons_side), hash, level+1);
    }
    else if(is_inte(next_level) && size(next_level) == HASH_ENTRY_SIZE){
        return !_arcmp((mword*)HASH_ENTRY_REF(next_level),hash);
    }

}

//
// babel_operator
bvm_cache *luha(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *hash_table   = TOS_1(this_bvm);
    mword *key          = TOS_0(this_bvm);
    mword *hash         = _hash8(key);

    mword *result = new_atom; //FIXME: Completely busted, need to check type returned by _luha
    *result = car(_luha(hash_table,hash));

    zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Revisit

}

//
mword *_luha(mword *hash_table, mword *hash){

    mword *result;

    if(is_nil(hash_table)){
        return nil;
    }
    else{
        return _rluha(hash_table, hash, 0);
    }

    return result;

}

//
mword *_rluha(mword *hash_table, mword *hash, mword level){

    mword *temp;
    mword cons_side   = _cxr1(hash,level);
    mword *next_level = (mword*)c(hash_table,cons_side);

    if(is_nil(next_level)){ //dead-end
        return nil;
    }
    else if(is_inte(next_level) && size(next_level) == 2){
        return _rluha((mword*)c(hash_table,cons_side), hash, level+1);
    }
    else if(is_inte(next_level) && size(next_level) == HASH_ENTRY_SIZE){
        return (mword*)HASH_ENTRY_VAL(next_level);
    }

}

// Clayton Bauman 2011

