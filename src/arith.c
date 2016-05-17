// arith.c
//

#include "babel.h"
#include "arith.h"
#include "operator.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"

/* arithmetic operator
_Note: The arithmetic operators are all prefixed with 'c' because
Babel 2.0 will have its own native number format and arbitrary-precision
arithmetic._
*/


// Add with carry
//
mword *_addc(bvm_cache *this_bvm, mword a, mword b, mword carry_in){

    mword sum = a + b + carry_in;
    mword *result;

    if(sum < a){
        result = _newlfi(this_bvm, 2, 0);
        lcl(result,0) = (mword)sum;
        lcl(result,1) = 1; // carry bit
    }
    else{
        result = _val(this_bvm, (mword)sum);
    }

    return result;

}

// XXX DEPRECATED XXX >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define BMATH_SHIFT   (MWORD_BIT_SIZE/2)
#define BMATH_HI_MASK ((-1)<<(BMATH_SHIFT))
#define BMATH_LO_MASK ((-1)>>(BMATH_SHIFT))
#define BMATH_HI_WORD(x) ((x)&(BMATH_HI_MASK)>>BMATH_SHIFT)
#define BMATH_LO_WORD(x) ((x)&(BMATH_LO_MASK))

//
//
mword badd_term(bvm_cache *this_bvm, mword x, mword y, mword *result, mword carry_in){

    mword sum = (x + y + carry_in);

    *result = sum;

    if(sum < x){
        return 1;
    }

    return 0;

}


// precondition: size(x) >= size(y)
//
mword *badd_array(bvm_cache *this_bvm, mword *x, mword *y){

    mword max_size = MAX(size(x), size(y));
    mword min_size = MIN(size(x), size(y));

    mword *result = _newlfi(this_bvm, max_size+1, 0);

    int i;
    mword carry = 0;
    for(i=0; i<min_size; i++){
        carry = badd_term(this_bvm, rcl(x,i), rcl(y,i), (result+i), carry);
    }
    
    for(;i<max_size;i++){
        carry = badd_term(this_bvm, rcl(x,i), 0, (result+i), carry);
    }

    if(carry){
        lcl(result,max_size) = carry;
    }
    else{
        s(result) = (s(result)-MWORD_SIZE);
    }

    return result;

}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< XXX DEPRECATED XXX
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


/* arithmetic operator
**cadd_uod** **add**  
> C-style unsigned addition  
> `a b| -> (a+b)|`  
*/
#define CADD_UOD_OPERATIONS \
    result0 = _val( this_bvm, *(oi0.data) + *(oi1.data) );

OPERATORA_R2_W1_UOD(cadd_uod, 
        CADD_UOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**cadd_iod** (+)  
> C-style integer addition  
> `a b| -> (a+b)|`  
*/
#define CADD_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi0.data) + *(int*)(oi1.data) );

OPERATORA_R2_W1_IOD(cadd_iod, 
        CADD_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**cadd_iod** (+)  
> C-style integer addition  
> `a b| -> (a+b)|`  
*/
#define CADD_COD_OPERATIONS \
    result0 = _addc( this_bvm, *oi0.data, *oi1.data, 0 );

OPERATORA_R2_W1_IOD(cadd_cod, 
        CADD_COD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**csub_uod** **sub**  
> C-style unsigned subtraction  
> `a b| -> (a-b)|`  
*/
#define CSUB_UOD_OPERATIONS \
    result0 = _val( this_bvm, *(oi1.data) - *(oi0.data) );

OPERATORA_R2_W1_UOD(csub_uod, 
        CSUB_UOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**csub_iod** (-)  
> C-style integer subtraction  
> `a b| -> (a-b)|`  
*/
#define CSUB_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) - *(int*)(oi0.data) );

OPERATORA_R2_W1_IOD(csub_iod, 
        CSUB_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**cmul_uod** (mul)  
> C-style unsigned multiplication  
> `a b| -> [a*b]|`  
*/
#define CMUL_UOD_OPERATIONS \
    result0 = _val( this_bvm, *(oi1.data) * *(oi0.data) );

//    result0 = _newlfi(this_bvm, 2, 0);
//    *(long unsigned *)result0 = (long unsigned )(*(long unsigned *)(oi1.data) * *(long unsigned *)(oi0.data));

OPERATORA_R2_W1_UOD(cmul_uod, 
        CMUL_UOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**cmul_iod** (*)  
> C-style integer multiplication  
> `a b| -> [a*b]|`  
*/
#define CMUL_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) * *(int*)(oi0.data) );

OPERATORA_R2_W1_IOD(cmul_iod, 
        CMUL_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**cdiv_uod** (div)  
> C-style unsigned division  
> `a b| -> (a/b)|`  
*/
#define CDIV_UOD_OPERATIONS \
    result0 = _val( this_bvm, *(oi1.data) / *(oi0.data) );

OPERATORA_R2_W1_UOD(cdiv_uod, 
        CDIV_UOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**cdiv_iod** (/)  
> C-style integer division  
> `a b| -> (a/b)|`  
*/
#define CDIV_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) / *(int*)(oi0.data) );

OPERATORA_R2_W1_IOD(cdiv_iod, 
        CDIV_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**crem_iod** **rem**  
> C-style integer remainder (modulo division)  
> `a b| -> (a%b)|`  
*/
#define CREM_UOD_OPERATIONS \
    result0 = _val( this_bvm, *oi1.data % *oi0.data );

OPERATORA_R2_W1_UOD(crem_uod,
        CREM_UOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


#define CREM_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) % *(int*)(oi0.data) );

OPERATORA_R2_W1_IOD(crem_iod,
        CREM_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**cabs_iod** **abs**  
> C-style absolute-value  
> `a| -> abs(a)|`  
*/
#define CABS_IOD_OPERATIONS \
    result0 = _val(this_bvm, abs(rcl(oi0.data,0)));

OPERATORA_R1_W1_D(cabs_iod, 
        CABS_IOD_OPERATIONS, 
        nil, OI_MASK_LEAF, 0, 0)


/* shift operator
**cshl_uod** **shl**  
> C-style unsigned shift-left  
> `a b| -> (a<<b)|`  
*/
#define CSHL_UOD_OPERATIONS \
    result0 = _val( this_bvm, *(oi1.data) << *(oi0.data) );

OPERATORA_R2_W1_UOD(cshl_uod, 
        CSHL_UOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* shift operator
**cshr_uod** **shr**  
> C-style unsigned shift-right  
> `a b| -> (a>>b)|`  
*/
#define CSHR_UOD_OPERATIONS \
    result0 = _val( this_bvm, *(oi1.data) >> *(oi0.data) );

OPERATORA_R2_W1_UOD(cshr_uod, 
        CSHR_UOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* shift operator
**cshr_iod**  
> C-style arithmetic shift-right
> `a b| -> (a>>b)|`  
> Performs an arithmetic shift-right which is suitable for dividing
> a signed (integer) value by a power of 2  
*/
#define CSHR_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) >> *(oi0.data) );

OPERATORA_R2_W1_IOD(cshr_iod, 
        CSHR_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* shift operator
**crol_uod** **rol**  
> C-style unsigned rotate-left
> `a b| -> (a rol b)|`  
*/
#define CROL_UOD_OPERATIONS \
    mword tempA = rcl(oi1.data,0) << rcl(oi0.data,0); \
    mword tempB = rcl(oi1.data,0) >> (MWORD_BIT_SIZE - rcl(oi0.data,0)); \
    result0 = _val( this_bvm, tempA | tempB );

OPERATORA_R2_W1_UOD(crol_uod, 
        CROL_UOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* comparison operator
**clt_iod** **lt**  
> C-style unsigned less-than  
> `a b| -> (a < b)|`  
*/
#define CLT_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) < *(int*)(oi0.data) );

OPERATORA_R2_W1_IOD(clt_iod, 
        CLT_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* comparison operator
**cgt_iod** **gt**  
> C-style unsigned greater-than  
> `a b| -> (a > b)|`  
*/
#define CGT_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) > *(int*)(oi0.data) );

OPERATORA_R2_W1_IOD(cgt_iod, 
        CGT_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* comparison operator
**cle_iod** **le**  
> C-style unsigned less-than-or-equal  
> `a b| -> (a <= b)|`  
*/
#define CLE_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) <= *(int*)(oi0.data) );

OPERATORA_R2_W1_IOD(cle_iod, 
        CLE_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* comparison operator
**cge_iod** **ge**  
> C-style unsigned greater-than-or-equal  
> `a b| -> (a >= b)|`  
*/
#define CGE_IOD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) >= *(int*)(oi0.data) );

OPERATORA_R2_W1_IOD(cge_iod, 
        CGE_IOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* comparison operator
**ceq_od** **eq**  
> C-style equal  
> `a b| -> (a == b)|`  
*/
#define CEQ_OD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) == *(int*)(oi0.data) );

OPERATORA_R2_W1_D(ceq_od, 
        CEQ_OD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* comparison operator
**cne** **ne**  
> C-style not-equal  
> `a b| -> (a != b)|`  
*/
#define CNE_OD_OPERATIONS \
    result0 = _val( this_bvm, *(int*)(oi1.data) != *(int*)(oi0.data) );

OPERATORA_R2_W1_D(cne_od, 
        CNE_OD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


/* arithmetic operator
**cadd_fod** (+)  
> C-style floating-point addition  
> `a b| -> (a+b)|`  
*/
#define CADD_FOD_OPERATIONS \
    double result_num = *(double*)(oi1.data) + *(double*)(oi0.data); \
    mword *result = (mword*)&result_num; \
    result0 = _newlfi( this_bvm, FP_ARITH_SIZE, 0 ); \
    lcl(result0,0) = rcl(result,0); \
    lcl(result0,1) = rcl(result,1);

OPERATORA_R2_W1_FOD(cadd_fod, 
        CADD_FOD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 1)


// Clayton Bauman 2014

