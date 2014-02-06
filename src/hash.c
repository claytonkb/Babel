// hash.c
//
//

#include "babel.h"
#include "hash.h"
#include "list.h"
#include "except.h"
#include "array.h"
#include "stack.h"
#include "pearson16.h"
#include "bvm.h"
//#include "bvm_opcodes.h"
#include "bstruct.h"
#include "string.h"
#include "tptr.h"
#include "mem.h"

// Updates the payload of a hash-entry
//
void sym_update(bvm_cache *this_bvm, mword *hash, mword *new_payload){

    mword *entry = _luha(this_bvm, get_tptr(this_bvm->sym_table), hash);
    mword *old_payload = _cdri(this_bvm, entry, 2); //FIXME: Naked constant
    (mword*)icar(old_payload) = new_payload;

}


/* hash operator
**newha**
*/
bvm_cache *newha(bvm_cache *this_bvm){ // newha#

    mword *result = new_hash_table(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* hash operator
**keysha**  
> Extracts all keys from the hash. 
*/
bvm_cache *keysha(bvm_cache *this_bvm){ // newha#

    mword *hash_table = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *result = _entha(this_bvm, hard_detag(this_bvm, hash_table));

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// Returns a list of all entries in the hash
//
mword *_entha(bvm_cache *this_bvm, mword *hash_table){ // _entha#

    if(is_nil(hash_table)){
        return nil;
    }
    else{
        return rentha(this_bvm, hash_table, 0);
    }

}


//
//
mword *rentha(bvm_cache *this_bvm, mword *hash_table, mword level){ // rentha#

//    mword *temp;
//    mword cons_side   = _cxr1(this_bvm, hash,level);
//    mword *next_level = (mword*)cxr(hash_table,cons_side);

    //FIXME: Check level == HASH_BIT_SIZE

    if(is_nil(hash_table)){ //dead-end

        return nil;
    }
    else if(is_conslike(hash_table)){

        mword *list0 = rentha(this_bvm, (mword*)car(hash_table), level+1);
        mword *list1 = rentha(this_bvm, (mword*)cdr(hash_table), level+1);

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
//                _dump(list0);
//                die;
                return _append_direct(this_bvm, list0, list1);
            }
        }

    }
    else if(is_tptr(hash_table)){ // XXX ASSUMES well-formed hash-entry

        //append it to the keys_list
//        _dump(_ith(this_bvm, hard_detag(this_bvm, hash_table),1));
//        die;
        return consa(this_bvm, hard_detag(this_bvm, hash_table),nil);
    }
    else{
        fatal("unexpected element in hash-table"); //FIXME: except, not fatal
    }

}


/* hash operator
**enref/unref**  
**deref**  
**ls2ha**  
**ha2ls**  
**lukha**  
> Look up a hash-entry's key  
**lurha  
> Look up a hash-entry's ref-count  
*/

/* hash operator
**diff**  
> Diffs the hashes on TOS  
> L R| -> ((L n !R) (!L n R) (L n R))|  
>  
> To test for sameness, check if TOS-1 and TOS-2 are both   
> nil or both not nil.  
*/

/* hash operator
**merge**  
> Merges a list of hashes together, key-by-key:  
>  
> (A)| -> B|  
>  
> Where (A) is a list of hashes and B is the resulting merged  
> hash.  
*/

//
//
mword *new_hash_table(bvm_cache *this_bvm){ // new_hash_table#

//    return new_tptr(this_bvm,   _hash8(this_bvm, C2B("/babel/tag/hash_table")), 
//                        _consls(this_bvm,  _consls(this_bvm, nil, nil), nil ) );

    return new_tptr(this_bvm,   _hash8(this_bvm, C2B("/babel/tag/hash_table")), 
                        consa(this_bvm,  consa(this_bvm, nil, nil), nil ) );

}


//
//
mword *new_hash_table_entry(bvm_cache *this_bvm, mword *hash, mword *key, mword *payload){  // new_hash_table_entry#

    if(is_nil(hash)){
        hash = _hash8(this_bvm, key); //FIXME: detect key == nil
    }
    else{
        hash = _cp(this_bvm, hash);
    }

    if(!is_nil(key)){
        key = _cp(this_bvm, key);
    }

    mword *temp = new_tptr(this_bvm, 
        _hash8(this_bvm, C2B("/babel/tag/hash_table_entry")), 
        consa(this_bvm,  hash,
            consa(this_bvm,  key,
                consa(this_bvm,  payload, nil )))
    );

//    _dump(temp);
//    die;

    return temp;

}


/* hash operator
**inskha**  
> Insert into hash:  
>  
> my_hash `"foo" 42 inskha`  
>  
> ... inserts the value 42 into my_hash with the key "foo". Does 
> store the key in the hash.  
*/
bvm_cache *inskha(bvm_cache *this_bvm){ // insha#

    mword *hash_table = dstack_get(this_bvm,2);
    mword *key        = dstack_get(this_bvm,1);
    mword *payload    = dstack_get(this_bvm,0);

    mword *hash = _hash8(this_bvm, key);

    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);

    _insha(this_bvm, hash_table, hash, nil, new_hash_table_entry(this_bvm,  hash, key, payload ));

//    pushd(this_bvm, hash_table, IMMORTAL);

    return this_bvm;

}



/* hash operator
**insha**  
> Insert into hash:  
>  
> my_hash `"foo" hash8 42 insha`  
>  
> ... inserts the value 42 into my_hash with the key "foo". Does not  
> store the key in the hash.  
*/
bvm_cache *insha(bvm_cache *this_bvm){ // insha#

    mword *hash_table = dstack_get(this_bvm,2);
    mword *hash       = dstack_get(this_bvm,1);
    mword *payload    = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);

    _insha(this_bvm, hash_table, hash, nil, new_hash_table_entry(this_bvm,  hash, nil, payload ));

//    pushd(this_bvm, hash_table, IMMORTAL);

    return this_bvm;

}

// entry is generated by _new_hash_table_entry, e.g:
//  _insha(this_bvm, my_hash_table, my_hash, my_key, _new_hash_table_entry(my_hash, my_key, my_data));
//
void _insha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword *key, mword *entry){ // _insha#

    if(is_nil(hash)){
        hash = _hash8(this_bvm, key); //FIXME: detect key == nil
    }
    else{
        hash = _cp(this_bvm, hash);
    }

    if(!is_nil(key)){
        key = _cp(this_bvm, key);
    }


    rinsha(this_bvm, (mword*)get_tptr(hash_table), hash, key, entry, 0); //XXX: was using tcar

}


//
//
static void rinsha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword *key, mword *entry, mword level){ // rinsha#

    mword cons_side   = _cxr1(this_bvm, hash,level);
    mword *next_level;
    mword *payload;

    if(cons_side){
        next_level = scdr(hash_table);
    }
    else{
        next_level = scar(hash_table);
    }

    // 1. cons_side = nil
    //      insert
    // 2. cons_side is inte AND size = 2
    //      recurse
    // 3. cons_side is inte AND size = HASH_ENTRY_SIZE
    //      split and insert

    if(is_nil(next_level)){
        (mword*)c(hash_table,cons_side) = entry;
    }
    else if(is_conslike(next_level)){
        rinsha(this_bvm, (mword*)c(hash_table,cons_side), hash, key, entry, level+1);
    }
    else if(is_tptr(next_level)){ //XXX: We are ASSUMING it's a hash_table_entry...

        if(tageq(car(next_level),hash,TAG_SIZE)){ //already exists...

            payload = (mword*)cdr(cdr(next_level));
            c(payload,0) = car(cdr(cdr(entry)));

        }
        else{

            if( _cxr1(this_bvm,  (mword*)car(next_level), level+1 ) ){ //XXX: was using tcar
                (mword*)c(hash_table,cons_side) = consa(this_bvm,  nil, next_level );
            }
            else{
                (mword*)c(hash_table,cons_side) = consa(this_bvm,  next_level, nil );
            }

            rinsha(this_bvm, (mword*)c(hash_table,cons_side), hash, key, entry, level+1);

        }

    }
    else{
        _dump(next_level);
        die;
        enhance("Exception due to error inserting into hash");
    }

}

/* hash operator
**exha**  
> Test existence in hash:  
>  
> my_hash `[`{0 0 0 0}] ["foo"] hash8 exha  
>  
> Leaves 1 on TOS if the associated entry is found in the hash,   
> otherwise, leaves 0 on TOS.  
*/
bvm_cache *exha(bvm_cache *this_bvm){ // exha#

    mword *hash_table = dstack_get(this_bvm,1);
    mword *hash       = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

    mword *result = _newva( this_bvm, _exha(this_bvm, get_tptr(hash_table), hash));

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

//
//
mword _exha(bvm_cache *this_bvm, mword *hash_table, mword *hash){ // _exha#

    if(is_nil(hash_table)){
        return 0;
    }
    else{
        return rexha(this_bvm, hash_table, hash, 0);
    }

}


//
//
static mword rexha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level){ // rexha#

    mword *temp;
    mword cons_side   = _cxr1(this_bvm, hash,level);
    mword *next_level = (mword*)cxr(hash_table,cons_side);

    //FIXME: Check level == HASH_BIT_SIZE

    if(is_nil(next_level)){ //dead-end
        return 0;
    }
    else if(is_conslike(next_level)){
        rexha(this_bvm, (mword*)c(hash_table,cons_side), hash, level+1);
    }
    else if(is_tptr(next_level)){ // XXX ASSUMES well-formed hash-entry
        return (tageq(car(next_level),hash,TAG_SIZE));
    }

}

/* hash operator
**luha**  
> Look up a hash-entry  
*/
bvm_cache *luha(bvm_cache *this_bvm){ // luha#

//    mword *hash_table = getd(this_bvm,2);
//    mword *hash       = getd(this_bvm,1);
//    mword *payload    = getd(this_bvm,0);

    mword *hash_table = dstack_get(this_bvm,1);
    mword *hash       = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

//    mword *result = HASH_ENTRY_PAY( 
//                        this_bvm,
//                        _luha(this_bvm, get_tptr(hash_table), hash) );

    mword *result = _luha(this_bvm, get_tptr(hash_table), hash);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
mword *_luha(bvm_cache *this_bvm, mword *hash_table, mword *hash){ // _luha#

    mword *result;

    if(is_nil(hash_table)){
        return nil;
    }
    else{
        return rluha(this_bvm, hash_table, hash, 0);
    }

    return result;

}


//
//
static mword *rluha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level){ // rluha#

    mword *temp;
    mword cons_side   = _cxr1(this_bvm, hash,level);
    mword *next_level = (mword*)c(hash_table,cons_side);

    //FIXME: Check level == HASH_BIT_SIZE
    //FIXME: do full tag check on the entry before returning it
    //FIXME: don't crash when failing to find a hash entry

    if(is_nil(next_level)){ //dead-end
        return nil;
    }
    else if(is_conslike(next_level)){
        return rluha(this_bvm, (mword*)c(hash_table,cons_side), hash, level+1);
    }
    else if(is_tptr(next_level)){ // XXX ASSUMES well-formed hash-entry
        //_dump(detag(this_bvm, next_level));
        //return (mword*)car(cdr(cdr(next_level)));
        //return _ith(this_bvm, detag(this_bvm, next_level), 2);
        //return HASH_ENTRY_PAY(this_bvm, next_level);
        return next_level;
    }

}

/* hash operator
**rmha**  
> Remove from hash:  
>  
> my_hash `[`{0 0 0 0}] ["foo"] hash8 rmha  
>  
> ... removes the the entry from my_hash with the key "foo" if it   
> exists.  
> 
> returns: 1 if an entry was removed, 0 otherwise  
*/
bvm_cache *rmha(bvm_cache *this_bvm){ // rmha#

    mword *hash_table = dstack_get(this_bvm,1);
    mword *hash       = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

    mword *result = _newva( this_bvm, _rmha(this_bvm, get_tptr(hash_table), hash));

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


// returns: 1 if an entry was removed, 0 otherwise
//
mword _rmha(bvm_cache *this_bvm, mword *hash_table, mword *hash){ // _rmha#

    if(is_nil(hash_table)){
        return 0;
    }
    else{
        return rrmha(this_bvm, hash_table, hash, 0);
    }

}


//
//
static mword rrmha(bvm_cache *this_bvm, mword *hash_table, mword *hash, mword level){ // rrmha#

    mword *temp;
    mword cons_side   = _cxr1(this_bvm, hash,level);
    mword *next_level = (mword*)cxr(hash_table,cons_side);

    //FIXME: Check level == HASH_BIT_SIZE

    //FIXME: Fully re-collapse the tree after removal of an entry...

    if(is_nil(next_level)){ //dead-end
        return 0;
    }
    else if(is_conslike(next_level)){
        if(rrmha(this_bvm, (mword*)c(hash_table,cons_side), hash, level+1)){
            if(    is_nil( (mword*)c(next_level,  cons_side) ) 
                && is_nil( (mword*)c(next_level,1-cons_side) )){ //FIXME: performs a check on every level... redundant
                (mword*)c(hash_table,cons_side) = nil; //FIXME: De-allocation
            }            
            return 1;
        }
        return 0;
    }
    else if(is_tptr(next_level)){ // XXX ASSUMES well-formed hash-entry

        if(tageq(car(next_level),hash,TAG_SIZE)){ //match
            (mword*)c(hash_table,cons_side) = nil; //FIXME: De-allocation
            return 1;
        }
        else{
            return 0;
        }

    }

}


// Clayton Bauman 2011

