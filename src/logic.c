// logic.c
//

#include "babel.h"
#include "logic.h"
#include "operator.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"


//
//
mword is_false(mword *bs){ // is_false#

    if(      is_leaf(bs)){
        return is_false_leaf(bs);
    }
    else if( is_inte(bs)){
        return is_false_inte(bs);
    }
    else{ // is_tptr(bs)
        return is_false_tptr(bs);
    }

}


//
//
mword is_false_val(mword *bs){ // is_false_val#

    return (rcl(bs,0) == 0);

}


//
//
mword is_false_leaf(mword *bs){ // is_false_leaf#

    int i;

    for(i=0;i<size(bs);i++){
        if(rcl(bs,i) != 0){
            return 0;
        }
    }

    return 1;

}


//
//
mword is_false_ptr(mword *bs){ // is_false_ptr#

    return is_nil(bs);

}


//
//
mword is_false_inte(mword *bs){ // is_false_inte#

    int i;
    for(i=0;i<size(bs);i++){
        if(!is_nil(rci(bs,i))){
            return 0;
        }
    }

    return 1;

}


//
//
mword is_false_tptr(mword *bs){ // is_false_tptr#

    return 
           (tageq(bs, BABEL_TAG_FALSE     , TAG_SIZE)
        ||  tageq(bs, BABEL_TAG_REJECT    , TAG_SIZE)
        ||  tageq(bs, BABEL_TAG_UNEXIST   , TAG_SIZE)
        ||  tageq(bs, BABEL_TAG_INTERP_NIL, TAG_SIZE));

}


//#define CAND_UOD_OPERATIONS 
//    result0 = _val( this_bvm, *(oi0.data) & *(oi1.data) );
//
//OPERATOR_R2_W1_UOD(and_od, 
//        CAND_UOD_OPERATIONS, 
//        0, OI_MASK_ANY, 0, 0,
//        0, OI_MASK_ANY, 0, 1)


#define NOT_D_OPERATIONS \
    result0 = _val( this_bvm, is_false(oi0.data) );

OPERATORA_R1_W1_D(not_d, 
        NOT_D_OPERATIONS, 
        0, OI_MASK_ANY, 0, 1)


#define AND_D_OPERATIONS \
    result0 = _val( this_bvm, (!is_false(oi0.data) && !is_false(oi1.data)) );

OPERATORA_R2_W1_D(and_d, 
        AND_D_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define OR_D_OPERATIONS \
    result0 = _val( this_bvm, (!is_false(oi0.data) || !is_false(oi1.data)) );

OPERATORA_R2_W1_D(or_d, 
        OR_D_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define XOR_D_OPERATIONS \
    result0 = _val( this_bvm, (!is_false(oi0.data) &&  is_false(oi1.data)) \
                           || ( is_false(oi0.data) && !is_false(oi1.data)));

OPERATORA_R2_W1_D(xor_d, 
        XOR_D_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICF0_OD_OPERATIONS \
    result0 = _val( this_bvm, 0 );

OPERATORA_R2_W1_UOD(logicF0_od, 
        LOGICF0_OD_OPERATIONS,
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


#define LOGICF1_OD_OPERATIONS \
    result0 = _val( this_bvm, (*oi0.data & *oi1.data) );

OPERATORA_R2_W1_UOD(logicF1_od, 
        LOGICF1_OD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


#define LOGICF2_OD_OPERATIONS \
    result0 = _val( this_bvm, (*oi0.data & ~(*oi1.data)) );

OPERATORA_R2_W1_UOD(logicF2_od, 
        LOGICF2_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICF3_OD_OPERATIONS \
    result0 = _val( this_bvm, *oi0.data );

OPERATORA_R2_W1_UOD(logicF3_od, 
        LOGICF3_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICF4_OD_OPERATIONS \
    result0 = _val( this_bvm, (~(*oi0.data) & *oi1.data) );

OPERATORA_R2_W1_UOD(logicF4_od, 
        LOGICF4_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICF5_OD_OPERATIONS \
    result0 = _val( this_bvm, *oi1.data );

OPERATORA_R2_W1_UOD(logicF5_od, 
        LOGICF5_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICF6_OD_OPERATIONS \
    result0 = _val( this_bvm, (*oi0.data ^ *oi1.data) );

OPERATORA_R2_W1_UOD(logicF6_od, 
        LOGICF6_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICF7_OD_OPERATIONS \
    result0 = _val( this_bvm, (*oi0.data | *oi1.data) );

OPERATORA_R2_W1_UOD(logicF7_od, 
        LOGICF7_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICF8_OD_OPERATIONS \
    result0 = _val( this_bvm, (~(*oi0.data) | *oi1.data) );

OPERATORA_R2_W1_UOD(logicF8_od, 
        LOGICF8_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICF9_OD_OPERATIONS \
    result0 = _val( this_bvm, ~(*oi0.data ^ *oi1.data) );

OPERATORA_R2_W1_UOD(logicF9_od, 
        LOGICF9_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICFA_OD_OPERATIONS \
    result0 = _val( this_bvm, ~(*oi1.data) );

OPERATORA_R2_W1_UOD(logicFa_od, 
        LOGICFA_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICFB_OD_OPERATIONS \
    result0 = _val( this_bvm, ~(~(*oi0.data) & *oi1.data) );

OPERATORA_R2_W1_UOD(logicFb_od, 
        LOGICFB_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICFC_OD_OPERATIONS \
    result0 = _val( this_bvm, ~(*oi0.data) );

OPERATORA_R2_W1_UOD(logicFc_od, 
        LOGICFC_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICFD_OD_OPERATIONS \
    result0 = _val( this_bvm, ~(*oi0.data & ~(*oi1.data)) );

OPERATORA_R2_W1_UOD(logicFd_od, 
        LOGICFD_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICFE_OD_OPERATIONS \
    result0 = _val( this_bvm, ~(*oi0.data & *oi1.data) );

OPERATORA_R2_W1_UOD(logicFe_od, 
        LOGICFE_OD_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define LOGICFF_OD_OPERATIONS \
    result0 = _val( this_bvm, -1 );

OPERATORA_R2_W1_UOD(logicFf_od,
        LOGICFF_OD_OPERATIONS,
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 1)


#define CNOT_OD_OPERATIONS \
    result0 = _val( this_bvm, ~(*oi0.data) );

OPERATORA_R1_W1_D(cnot_od, 
        CNOT_OD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0)


// Clayton Bauman 2014

