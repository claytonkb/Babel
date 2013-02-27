// ref.c
//

#include "ref.h"
#include "array.h"
#include "bstruct.h"
#include "tlist.h"
#include "hash.h"

// Dereferences a reference-list
//
mword *_deref(mword *bs, mword *ref_list){ // _deref#

    //return rderef(bs,(mword*)car(ref_list));
    return rderef(bs,(mword*)car(ref_list));

}


//
//
static mword *rderef(mword *bs, mword *ref_list){ // rderef#

    mword *rl = (mword*)car(ref_list);

    if (is_nil(rl)){
        return bs;
    }
    else if (is_ref(rl)){
        enhance("Nested references not supported");
        die;
    }
    else if (is_tlist(rl)){
        //_dump(extract_hash(rl));
        bs = _luha( bs, extract_hash(rl) );
        //die;
    }
    else if(is_leaf(rl)){
        if(is_leaf(bs)){
            return val(bs,car(rl));
        }
        else if(is_tlist(bs)){
            if(car(rl)){
                bs = (mword*)tcar(bs);
            }
            else{
                bs = (mword*)tcdr(bs);
            }
        }
        else{
            bs = (mword*)c(bs,car(rl) % size(bs));
        }
    }
    else{ // is_inte(rl)
        fatal("Bad reference");
    }

    return rderef(bs,(mword*)cdr(ref_list));

}


//
//
mword *_chain_deref(mword *bs, mword *ref_list){ // _chain_deref#

    mword *result = ref_list;

    while(is_ref(result)){ 
        result = _deref(bs, result);
    }

    return result;

}

// Clayton Bauman 2013

