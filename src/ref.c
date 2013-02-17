// ref.c
//

#include "ref.h"
#include "array.h"

// _deref
// Dereferences a reference-list
//
mword *_deref(mword *bs, mword *ref_list){

    if (is_nil(ref_list)) return bs;

    if (is_tlist(bs) || is_leaf(bs)){
        die;
    }
    else{
        return _deref(_ref_cxr(bs,(mword)tcar(tcar(ref_list))),(mword*)cdr(ref_list));
    }

}

//
mword *_ref_cxr(mword *array, mword offset){

    mword *temp;

    if(is_leaf(array)){
        temp = new_atom;
        c(temp,0) = c(array, offset);
    }
    else{
        temp = (mword*)c(array, offset);
    }

    return temp;

}

// Clayton Bauman 2013

