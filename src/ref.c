// ref.c
//

#include "ref.h"
#include "array.h"
#include "bstruct.h"
#include "tptr.h"
#include "hash.h"
#include "stack.h"
#include "pearson16.h"
#include "list.h"


bvm_cache *deref(bvm_cache *this_bvm){ // deref#

//    mword *ref = (mword*)icar(tptr_extract_ptr(this_bvm, dstack_get(this_bvm,0)));
//    mword *result = rderef(get_tptr(this_bvm->sym_table),ref);
//
//    pushd(this_bvm, result, IMMORTAL);
//    return this_bvm;

    mword *result = _deref(this_bvm, dstack_get(this_bvm,0));

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


// Dereferences a reference-list
//
mword *_deref(bvm_cache *this_bvm, mword *ref_list){ // _deref#

    mword *ref = (mword*)icar(tptr_extract_ptr(this_bvm, ref_list));
    mword *result;

    //Determine if sym_table-relative
    if(is_nil((mword*)icar(ref))){
        result = rderef(this_bvm, get_tptr(this_bvm->sym_table),(mword*)icdr(ref));
    }
    else{
        result = rderef(this_bvm, (mword*)icar(ref),(mword*)icdr(ref));
    }

    return result;

}


//
//
static mword *rderef(bvm_cache *this_bvm, mword *bs, mword *ref_list){ // rderef#

    if(is_nil(ref_list))
        return bs;

    mword *tag = (mword*)icar(ref_list);

deref_restart:
    if(tageq(tag,BABEL_TAG_REF_HASH,TAG_SIZE)){
        mword *hash = get_tptr(tag);
        return rderef(this_bvm, _luha(this_bvm,  bs, hash ), (mword*)icdr(ref_list));
    }
    else if(tageq(tag,BABEL_TAG_REF_LIST,TAG_SIZE)){
        mword index = icar(get_tptr(tag));
        return rderef(this_bvm, (mword*)_ith(this_bvm, bs,index), (mword*)icdr(ref_list));
    }
    else if(tageq(tag,BABEL_TAG_REF_ARRAY,TAG_SIZE)){
        mword index = icar(get_tptr(tag));
        return rderef(this_bvm, (mword*)c(bs,index), (mword*)icdr(ref_list));
    }
    else if(tageq(tag,BABEL_TAG_REF_STRING,TAG_SIZE)){
        mword *hash = _hash8(this_bvm, get_tptr(tag));
        return rderef(this_bvm, _luha(this_bvm,  bs, hash ), (mword*)icdr(ref_list));
    }
    else if(tageq(tag,BABEL_TAG_REF,TAG_SIZE)){
        tag = _deref(this_bvm, tag);
        goto deref_restart;
    }
    else{
        _mem(tag);
        fatal("unrecognized tag");
    }

}


//
//
mword *_chain_deref(bvm_cache *this_bvm, mword *ref_list){ // _chain_deref#

    mword *result = ref_list;

    while(tageq(result,BABEL_TAG_REF,TAG_SIZE)){ 
        result = _deref(this_bvm, ref_list);
    }

    return result;

}

// Clayton Bauman 2013

