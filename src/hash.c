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
#include "string.h"
#include "tptr.h"


/* hash operator
**newha**
*/
bvm_cache *newha(bvm_cache *this_bvm){ // newha#

    mword *result = new_hash_table();

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* hash operator
**keysha**  
> Extracts all "keys" from the hash. Note that the extracted keys  
> are actually hash-references, not symbolic keys. The symbolic  
> keys can be recovered with the lukha operator.  
*/


//
//
mword _keysha(mword *hash_table, mword *hash){ // _keysha#

    if(is_nil(hash_table)){
        return 0;
    }
    else{
        return rkeysha(hash_table, hash, 0);
    }

}


//
//
static mword rkeysha(mword *hash_table, mword *hash, mword level){ // rkeysha#

    mword *temp;
    mword cons_side   = _cxr1(hash,level);
    mword *next_level = (mword*)cxr(hash_table,cons_side);

    //FIXME: Check level == HASH_BIT_SIZE

    if(is_nil(next_level)){ //dead-end
        return 0;
    }
    else if(is_conslike(next_level)){
        rkeysha((mword*)c(hash_table,cons_side), hash, level+1);
    }
    else if(is_tptr(next_level)){ // XXX ASSUMES well-formed hash-entry
        //append it to the keys_list
        //return (tageq(car(next_level),hash,TAG_SIZE));
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
mword *new_hash_table(void){ // new_hash_table#

//    return new_tptr(  _hash8(C2B("/babel/tag/hash_table")), 
//                        _consls( _consls(nil, nil), nil ) );

    return new_tptr(  _hash8(C2B("/babel/tag/hash_table")), 
                        consa( consa(nil, nil), nil ) );

}


//
//
mword *new_hash_table_entry(mword *hash, mword *key, mword *payload){  // new_hash_table_entry#

    if(is_nil(hash)){
        hash = _hash8(key); //FIXME: detect key == nil
    }
    else{
        hash = _cp(hash);
    }

    if(!is_nil(key)){
        key = _cp(key);
    }

    mword *temp = new_tptr(
        _hash8(C2B("/babel/tag/hash_table_entry")), 
        consa( hash,
            consa( key,
                consa( payload, nil )))
    );

//    _dump(temp);
//    die;

    return temp;

}

/* hash operator
**insha**  
> Insert into hash:  
>  
> my_hash `[{0 0 0 0}] ["foo"] hash8 [42] insha`  
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

    _insha(hash_table, hash, nil, new_hash_table_entry( hash, nil, payload ));

//    pushd(this_bvm, hash_table, IMMORTAL);

    return this_bvm;

}

// entry is generated by _new_hash_table_entry, e.g:
//  _insha(my_hash_table, my_hash, my_key, _new_hash_table_entry(my_hash, my_key, my_data));
//
void _insha(mword *hash_table, mword *hash, mword *key, mword *entry){ // _insha#

    if(is_nil(hash)){
        hash = _hash8(key); //FIXME: detect key == nil
    }
    else{
        hash = _cp(hash);
    }

    if(!is_nil(key)){
        key = _cp(key);
    }


    rinsha((mword*)get_tptr(hash_table), hash, key, entry, 0); //XXX: was using tcar

}


//
//
static void rinsha(mword *hash_table, mword *hash, mword *key, mword *entry, mword level){ // rinsha#

    mword cons_side   = _cxr1(hash,level);
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
        rinsha((mword*)c(hash_table,cons_side), hash, key, entry, level+1);
    }
    else if(is_tptr(next_level)){ //XXX: We are ASSUMING it's a hash_table_entry...

        if(tageq(car(next_level),hash,TAG_SIZE)){ //already exists...

            payload = (mword*)cdr(cdr(next_level));
            c(payload,0) = car(cdr(cdr(entry)));

        }
        else{

            if( _cxr1( (mword*)car(next_level), level+1 ) ){ //XXX: was using tcar
                (mword*)c(hash_table,cons_side) = consa( nil, next_level );
            }
            else{
                (mword*)c(hash_table,cons_side) = consa( next_level, nil );
            }

            rinsha((mword*)c(hash_table,cons_side), hash, key, entry, level+1);

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

    mword *result = _newva(_exha(get_tptr(hash_table), hash));

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

//
//
mword _exha(mword *hash_table, mword *hash){ // _exha#

    if(is_nil(hash_table)){
        return 0;
    }
    else{
        return rexha(hash_table, hash, 0);
    }

}


//
//
static mword rexha(mword *hash_table, mword *hash, mword level){ // rexha#

    mword *temp;
    mword cons_side   = _cxr1(hash,level);
    mword *next_level = (mword*)cxr(hash_table,cons_side);

    //FIXME: Check level == HASH_BIT_SIZE

    if(is_nil(next_level)){ //dead-end
        return 0;
    }
    else if(is_conslike(next_level)){
        rexha((mword*)c(hash_table,cons_side), hash, level+1);
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

    mword *result = _luha(get_tptr(hash_table), hash);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

//
//
mword *_luha(mword *hash_table, mword *hash){ // _luha#

    mword *result;

    if(is_nil(hash_table)){
        return nil;
    }
    else{
        return rluha(hash_table, hash, 0);
    }

    return result;

}


//
//
static mword *rluha(mword *hash_table, mword *hash, mword level){ // rluha#

    mword *temp;
    mword cons_side   = _cxr1(hash,level);
    mword *next_level = (mword*)c(hash_table,cons_side);

    //FIXME: Check level == HASH_BIT_SIZE
    //FIXME: do full tag check on the entry before returning it
    //FIXME: don't crash when failing to find a hash entry

    if(is_nil(next_level)){ //dead-end
        return nil;
    }
    else if(is_conslike(next_level)){
        return rluha((mword*)c(hash_table,cons_side), hash, level+1);
    }
    else if(is_tptr(next_level)){ // XXX ASSUMES well-formed hash-entry
        return (mword*)car(cdr(cdr(next_level)));
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

    mword *result = _newva(_rmha(get_tptr(hash_table), hash));

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


// returns: 1 if an entry was removed, 0 otherwise
//
mword _rmha(mword *hash_table, mword *hash){ // _rmha#

    if(is_nil(hash_table)){
        return 0;
    }
    else{
        return rrmha(hash_table, hash, 0);
    }

}


//
//
static mword rrmha(mword *hash_table, mword *hash, mword level){ // rrmha#

    mword *temp;
    mword cons_side   = _cxr1(hash,level);
    mword *next_level = (mword*)cxr(hash_table,cons_side);

    //FIXME: Check level == HASH_BIT_SIZE

    //FIXME: Fully re-collapse the tree after removal of an entry...

    if(is_nil(next_level)){ //dead-end
        return 0;
    }
    else if(is_conslike(next_level)){
        if(rrmha((mword*)c(hash_table,cons_side), hash, level+1)){
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

