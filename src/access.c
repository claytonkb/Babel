// access.c
//

#include "babel.h"
#include "access.h"
#include "bstruct.h"
#include "introspect.h"
#include "operator.h"


//
//
mword *_get(bvm_cache *this_bvm, mword *bs, mword *pbp){ // _get#

    trav_return t;

    if(is_nil(pbp)){
        return bs;
    }

    t = _trav(this_bvm, bs, pbp); // XXX WAIVER(_trav) XXX //

    if(is_leaf(t.bs)){
        return _val(this_bvm, *t.unsafe_ptr);
    }
    else if(is_inte(t.bs)){
        return _ptr(this_bvm, (mword*)*t.unsafe_ptr);
    }
    else{ // is_tptr(t.bs)
        return t.bs;
    }


    return t.bs; // Silence compiler warning

}


//
//
mword _set(bvm_cache *this_bvm, mword *bs, mword *pbp, mword *payload){ // _set#

    trav_return t;

    if(is_nil(pbp)){
        *(mword**)bs = payload;
        return 1;
    }

    t = _trav(this_bvm, bs, pbp); // XXX WAIVER(_trav) XXX //

    if(is_leaf(t.bs)
            && is_leaf(payload)){
        *t.unsafe_ptr = rcl(payload,0);
        return 1;
    }
    else if(is_inte(t.bs)
            && is_inte(payload)){
        *t.unsafe_ptr = rcl(payload,0);
        return 1;
    }

    return 0;

}


/*****************************************************************************
 *                                                                           *
 *                           ACCESS OPERATORS                                *
 *                                                                           *
 ****************************************************************************/

// get (R2_W1)
// <bs> <pbp> get
#define GET_OPERATIONS \
    result0 = _get(this_bvm, oi1.data, oi0.data);

OPERATORA_R2_W1_D(get, GET_OPERATIONS, 
        nil, OI_MASK_LEAF, 0, 0,
        nil, OI_MASK_ANY, 0, 0)

// set -> R3_W0
// <bs> <pbp> <payload> set
#define SET_OPERATIONS \
    _set(this_bvm, oi2.data_overwrite_ptr, oi1.data, oi0.data);

OPERATORA_R3_W0_D(set, SET_OPERATIONS, 
        nil, OI_MASK_ANY, 0, 0,
        nil, OI_MASK_LEAF, 0, 0,
        nil, OI_MASK_ANY, 0, 0)

// get2
#define GET2_OPERATIONS \
    result0 = _get(this_bvm, oi0.data, nil);

OPERATORA_R1_W1_D(get2, GET2_OPERATIONS, 
        nil, OI_MASK_ANY, 0, 0)

// set2
#define SET2_OPERATIONS \
    _set(this_bvm, oi1.data_overwrite_ptr, nil, oi0.data);

OPERATORA_R2_W0_D(set2, SET2_OPERATIONS, 
        nil, OI_MASK_ANY, 0, 0,
        nil, OI_MASK_ANY, 0, 0)

// Clayton Bauman 2014

