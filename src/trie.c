// trie.c
//

#include "babel.h"
#include "trie.h"
#include "bstruct.h"
#include "tptr.h"
#include "pearson16.h"
#include "string.h"
#include "list.h"
#include "array.h"
#include "introspect.h"
#include "mem.h"
#include "cache.h"


// FIXME: 32-bit only
static mword *trie_template_trie_entry = (mword []){ // trie_template_trie_entry#
    0x0000005c, 0x00000000, 0x591589e0, 0x1a9ec87a, 0x078adc82, 0xdbae9012, 0xfffffffc, 0x00000020, 
    0xfffffff8, 0x0000002c, 0x00000048, 0x00000000, 0x3023f4e7, 0x8c2f644d, 0x71cf647b, 0xe974b23a, 
    0xfffffffc, 0x0000002c, 0xfffffff8, 0x0000002c, 0x00000054, 0xfffffff8, 0x0000002c, 0x0000002c};

//
//
mword *trie_new_cell(bvm_cache *this_bvm, mword *key, mword *secondary_key, mword *payload){  // trie_new_cell#

    // XXX PERF shows 20+% improvement over old trie_new_cell XXX
    mword entry_size = size(trie_template_trie_entry+1);
    mword *entry     = _newlfcp(this_bvm, trie_template_trie_entry+1, entry_size);

    mword *result = bstruct_load_fast(this_bvm, entry, entry_size);

    mword *rel_offsets = (mword []){ 0x0c, 0x08, 0x12, 0x15 };
    mword *abs_offsets = (mword []){ 0x0c, 0x00, 0x00, 0x00 };

    rel_offsets++;
    abs_offsets++;

    // 8, 18, 21
    lcl(abs_offsets, 0) = (mword)key;
    lcl(abs_offsets, 1) = (mword)secondary_key;
    lcl(abs_offsets, 2) = (mword)payload;

    bstruct_load_template(this_bvm, entry, rel_offsets, abs_offsets);

    return result;

}


//
//
mword *trie_new(bvm_cache *this_bvm){ // trie_new#

    return tptr_new(this_bvm, 
                    HASH8(this_bvm, "/babel/tag/trie"), 
                    _mkin(this_bvm, 2, nil, nil));

}


////
////
//void trie_insert(bvm_cache *this_bvm, mword *trie, mword *key, mword *secondary_key, mword *payload){
//
//    if(is_tptr(trie)){ 
//        trie = get_tptr(trie);  // XXX We don't check the tag
//    }
//
//    if( (is_nil(key) && is_nil(secondary_key))
//            || !is_inte(trie))
//        return; // do nothing
//
//    if(is_nil(key)){
//
//        if(is_nil(secondary_key)){
//            return; // do nothing
//        }
//        else{
//            key = _hash8(this_bvm, secondary_key);
//        }
//
//    }
//    else{
//
//        key = _cp(this_bvm, key);
//
//    }
//
//    if(!is_nil(secondary_key))
//        secondary_key = _cp(this_bvm, secondary_key);
//
//// XXX !!!!!!!!!BROKEN!!!!!!!! XXX
//    rtrie_insert(this_bvm, trie, key, trie_new_cell(this_bvm, key, secondary_key, payload), 0);
//
//}


//
//
void rtrie_insert(bvm_cache *this_bvm, mword *trie, mword *key, mword *payload, mword level){ // rtrie_insert#

    mword cons_side   = _cxr1(this_bvm, key, level);
    mword *next_level = rci(trie, cons_side);

    // 1. cons_side = nil
    //      insert
    // 2. cons_side is inte AND size = 2
    //      recurse
    // 3. cons_side is inte AND size = HASH_ENTRY_SIZE
    //      split and insert

    if(is_inte(next_level)){
        rtrie_insert(this_bvm, next_level, key, payload, level+1);
    }
    else if(is_nil(next_level)){
        lci(trie,cons_side) = payload;
    }
    else if(is_tptr(next_level)){ //XXX: We are ASSUMING it's a trie_entry...

        mword *entry     = tptr_detag(this_bvm, next_level);

        mword *entry_key = trie_entry_get_key(this_bvm, entry);

        if( tageq(entry_key, key, TAG_SIZE) ){ //already exists...  
            trie_entry_set_payload2(this_bvm, entry, payload); 
        }
        else{

            //mword *existing_key = trie_entry_get_key(this_bvm, entry);
            if( _cxr1(this_bvm, entry_key, level+1) ){
                // note that lci(trie, cons_side) is equivalent to next_level
                lci(trie, cons_side) = _cons(this_bvm,  nil,         next_level  );
            }
            else{
                lci(trie, cons_side) = _cons(this_bvm,  next_level,  nil         );
            }

            rtrie_insert(this_bvm, lci(trie, cons_side), key, payload, level+1);

        }
    }
    else{ // is_leaf(next_level)
        return; // FIXME: Should throw an exception here due to mal-formed trie...
    }

}


//
//
void trie_insert(bvm_cache *this_bvm, mword *trie, mword *key, mword *secondary_key, mword *payload){ // trie_insert#

    if(is_tptr(trie)){ 
        trie = get_tptr(trie);  // XXX We don't check the tag
    }

    if( (is_nil(key) && is_nil(secondary_key))
            || !is_inte(trie))
        return; // do nothing

    if(is_nil(key)){

        if(is_nil(secondary_key)){
            return; // do nothing
        }
        else{
            key = _hash8(this_bvm, secondary_key);
        }

    }
    else{

        if(is_tptr(key)){ // FIXME: kludge due to single-tptr-copy bug in _unload_fast()
            key = tptr_extract_hash(this_bvm, key);
        }
        else{
            key = _cp(this_bvm, key);
        }

    }

//    if(is_nil(key)){
//
//        if(is_nil(secondary_key)){
//            return; // do nothing
//        }
//        else{
//            key = tptr_new(this_bvm, _hash8(this_bvm, secondary_key), nil);
//        }
//
//    }
//    else{
//
//        if(!is_tptr(key)){
//            key = tptr_new(this_bvm, _cp(this_bvm, key), nil);
//        }
//
//    }

    if(!is_nil(secondary_key))
        secondary_key = _cp(this_bvm, secondary_key);

    rtrie_insert2(this_bvm, trie, key, secondary_key, payload, 0);

}


//
//
void rtrie_insert2(bvm_cache *this_bvm, mword *trie, mword *key, mword *secondary_key, mword *payload, mword level){ // rtrie_insert#

    mword cons_side   = _cxr1(this_bvm, key, level);
    mword *next_level = rci(trie, cons_side);

    // 1. cons_side = nil
    //      insert
    // 2. cons_side is inte AND size = 2
    //      recurse
    // 3. cons_side is inte AND size = HASH_ENTRY_SIZE
    //      split and insert

    if(is_inte(next_level)){
        rtrie_insert2(this_bvm, next_level, key, secondary_key, payload, level+1);
    }
    else if(is_nil(next_level)){
//trie_new_cell(this_bvm, key, secondary_key, payload)
//        lci(trie,cons_side) = payload;
        lci(trie,cons_side) = trie_new_cell(this_bvm, key, secondary_key, payload);
    }
    else if(is_tptr(next_level)){ //XXX: We are ASSUMING it's a trie_entry...

        mword *entry     = tptr_detag(this_bvm, next_level);

        mword *entry_key = trie_entry_get_key(this_bvm, entry);

        if( tageq(entry_key, key, TAG_SIZE) ){ //already exists...  
            trie_entry_set_payload2(this_bvm, entry, payload); 
        }
        else{

            //mword *existing_key = trie_entry_get_key(this_bvm, entry);
            if( _cxr1(this_bvm, entry_key, level+1) ){
                // note that lci(trie, cons_side) is equivalent to next_level
                lci(trie, cons_side) = _cons(this_bvm,  nil,         next_level  );
            }
            else{
                lci(trie, cons_side) = _cons(this_bvm,  next_level,  nil         );
            }

            rtrie_insert2(this_bvm, lci(trie, cons_side), key, secondary_key, payload, level+1);

        }
    }
    else{ // is_leaf(next_level)
        return; // FIXME: Should throw an exception here due to mal-formed trie...
    }

}


//
//
mword *trie_lookup_binary(bvm_cache *this_bvm, mword *trie, mword *binary_key){ // trie_lookup_binary#

    return nil;

}


//
//
mword *trie_lookup_hash(bvm_cache *this_bvm, mword *trie, mword *hash_key, mword *string_key){ // trie_lookup_hash#

//static mword count=0;
//count++;
//
////_d(count);
//
//if(count > 0x23a){
//    _trace;
//cache_dump(this_bvm);
//    _d((mword)trie);
//    _d(*trie);
//    _trace;
//}

    if(is_tptr(trie)){ 
        trie = get_tptr(trie);  // XXX We don't check the tag
    }

    if(!is_inte(trie))
        return nil; // return nothing

    if(is_nil(hash_key)){

        if(is_nil(string_key)){
            return nil;
        }
        else{
            hash_key = _hash8(this_bvm, string_key);
        }

    }

    return rtrie_lookup(this_bvm, trie, hash_key, 0);

}


//
//
mword *rtrie_lookup(bvm_cache *this_bvm, mword *trie, mword *key, mword level){ // rtrie_lookup#

    mword cons_side   = _cxr1(this_bvm, key, level);
    mword *next_level = rci(trie, cons_side);

    if(is_inte(next_level)){
        return rtrie_lookup(this_bvm, next_level, key, level+1);
    }
    if(is_nil(next_level)){ //dead-end
        return nil;
    }
    else if(is_tptr(next_level)){

        mword *trie_entry = tptr_detag(this_bvm, next_level); // XXX assumes well-formedness
        mword *entry_key = trie_entry_get_key(this_bvm, trie_entry);

        if(tageq(entry_key, key, TAG_SIZE)){ 
            return trie_entry; // found it!
        }
        else{
            return nil; //dead-end
        }

    }
    // else is_leaf(next_level)... FIXME should raise an exception

    return nil;

}


//
//
mword trie_remove(bvm_cache *this_bvm, mword *trie, mword *key, mword *secondary_key){ // trie_remove#

    if(is_tptr(trie)){ 
        trie = get_tptr(trie);  // XXX We don't check the tag
    }

    if(!is_inte(trie))
        return 0; // nothing removed

    if(is_nil(key)){

        if(is_nil(secondary_key)){
            return 0;
        }
        else{
            key = _hash8(this_bvm, secondary_key); 
        }

    }

    return rtrie_remove(this_bvm, trie, key, 0);

}


//
//
mword rtrie_remove(bvm_cache *this_bvm, mword *trie, mword *key, mword level){ // rtrie_remove#

    mword cons_side   = _cxr1(this_bvm, key, level);
    mword *next_level = rci(trie, cons_side);

    if(is_inte(next_level)){
        if(rtrie_remove(this_bvm, rci(trie,cons_side), key, level+1)){
            if(    is_nil( rci(next_level,  cons_side) ) 
                && is_nil( rci(next_level,1-cons_side) )){
                lci(trie,cons_side) = nil; 
            }            
//            else if(    is_nil( rci(next_level,  cons_side) ) 
//                && is_tptr( rci(next_level,1-cons_side) )){
//                lci(trie, ????) = rci(next_level,1-cons_side);
//            }            
            return 1;
        }
        return 0;
    }
    else if(is_nil(next_level)){ //dead-end
        return 0;
    }
    else if(is_tptr(next_level)){ // xxx assumes well-formed key-entry

        if(tageq(car(next_level),key,TAG_SIZE)){ //match
            lci(trie,cons_side) = nil;
            return 1;
        }
        else{
            return 0;
        }

    }

    return 0; // silence compiler warning

}


// FIXME: This function is BUSTED
//
mword *trie_entries(bvm_cache *this_bvm, mword *trie){ // trie_entries#

    if(is_nil(trie)){
        return nil;
    }
    else{
        return rtrie_entries(this_bvm, trie, 0);
    }

}


//
//
mword *rtrie_entries(bvm_cache *this_bvm, mword *trie, mword level){ // rtrie_entries#

    //FIXME: Check for level > HASH_BIT_SIZE

    if(is_nil(trie)){ //dead-end
        return nil;
    }
    else if(is_conslike(trie)){

        mword *list0 = rtrie_entries(this_bvm, (mword*)car(trie), level+1);
        mword *list1 = rtrie_entries(this_bvm, (mword*)cdr(trie), level+1);

        if(is_nil(list0)){
            if(is_nil(list1)){
                return nil;
            }
            else{
                return list1;
            }
        }
        else{
            if(is_nil(list1)){
                return list0;
            }
            else{
                return _append_direct(this_bvm, list0, list1);
            }
        }

    }
    else if(is_tptr(trie)){
        return _cons(this_bvm, tptr_hard_detag(this_bvm, trie), nil);
    }
    else{
        _fatal("unexpected element in hash-table"); //FIXME: except, not fatal
    }

}





// Clayton Bauman 2014

