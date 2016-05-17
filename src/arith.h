// arith.h
// 

#ifndef ARITH_H
#define ARITH_H

mword *_addc(bvm_cache *this_bvm, mword a, mword b, mword carry_in);

mword badd_term(bvm_cache *this_bvm, mword x, mword y, mword *result, mword carry_in);
mword *badd_array(bvm_cache *this_bvm, mword *x, mword *y);

bvm_cache *cadd_uod(bvm_cache *this_bvm);
bvm_cache *cadd_iod(bvm_cache *this_bvm);
bvm_cache *cadd_cod(bvm_cache *this_bvm);
bvm_cache *cadd_fod(bvm_cache *this_bvm);

bvm_cache *csub_uod(bvm_cache *this_bvm);
bvm_cache *csub_iod(bvm_cache *this_bvm);

bvm_cache *cmul_uod(bvm_cache *this_bvm);
bvm_cache *cmul_iod(bvm_cache *this_bvm);

bvm_cache *cdiv_uod(bvm_cache *this_bvm);
bvm_cache *cdiv_iod(bvm_cache *this_bvm);

bvm_cache *crem_uod(bvm_cache *this_bvm);
bvm_cache *crem_iod(bvm_cache *this_bvm);

//    X(rsvd,           "crem_iod",       OP_CREM_IOD,    0x038)   

bvm_cache *cabs_iod(bvm_cache *this_bvm);

bvm_cache *cshl_uod(bvm_cache *this_bvm);
bvm_cache *cshr_uod(bvm_cache *this_bvm);
bvm_cache *cshr_iod(bvm_cache *this_bvm);

bvm_cache *crol_uod(bvm_cache *this_bvm);

bvm_cache *clt_iod(bvm_cache *this_bvm);
bvm_cache *cle_iod(bvm_cache *this_bvm);
bvm_cache *cgt_iod(bvm_cache *this_bvm);
bvm_cache *cge_iod(bvm_cache *this_bvm);

bvm_cache *ceq_od(bvm_cache *this_bvm);
bvm_cache *cne_od(bvm_cache *this_bvm);

//    X(rsvd,           "cadd_iop",       OP_CADD_IOP,    0x011)   
//    X(rsvd,           "cadd_iad",       OP_CADD_IAD,    0x012)   
//    X(rsvd,           "cadd_iap",       OP_CADD_IAP,    0x013)   
//    X(rsvd,           "cadd_uop",       OP_CADD_UOP,    0x015)   
//    X(rsvd,           "cadd_uad",       OP_CADD_UAD,    0x016)   
//    X(rsvd,           "cadd_uap",       OP_CADD_UAP,    0x017)   
//
//    X(rsvd,           "csub_iop",       OP_CSUB_IOP,    0x019)   
//    X(rsvd,           "csub_iad",       OP_CSUB_IAD,    0x01a)   
//    X(rsvd,           "csub_iap",       OP_CSUB_IAP,    0x01b)   
//    X(rsvd,           "csub_uop",       OP_CSUB_UOP,    0x01d)   
//    X(rsvd,           "csub_uad",       OP_CSUB_UAD,    0x01e)   
//    X(rsvd,           "csub_uap",       OP_CSUB_UAP,    0x01f)   
//
//    X(rsvd,           "cmul_iop",       OP_CMUL_IOP,    0x021)   
//    X(rsvd,           "cmul_iad",       OP_CMUL_IAD,    0x022)   
//    X(rsvd,           "cmul_iap",       OP_CMUL_IAP,    0x023)   
//    X(rsvd,           "cmul_uop",       OP_CMUL_UOP,    0x025)   
//    X(rsvd,           "cmul_uad",       OP_CMUL_UAD,    0x026)   
//    X(rsvd,           "cmul_uap",       OP_CMUL_UAP,    0x027)   
//
//    X(rsvd,           "cdiv_iop",       OP_CDIV_IOP,    0x029)   
//    X(rsvd,           "cdiv_iad",       OP_CDIV_IAD,    0x02a)   
//    X(rsvd,           "cdiv_iap",       OP_CDIV_IAP,    0x02b)   
//    X(rsvd,           "cdiv_uop",       OP_CDIV_UOP,    0x02d)   
//    X(rsvd,           "cdiv_uad",       OP_CDIV_UAD,    0x02e)   
//    X(rsvd,           "cdiv_uap",       OP_CDIV_UAP,    0x02f)   
//
//    X(rsvd,           "cabs_iop",       OP_CABS_IOP,    0x031)   
//    X(rsvd,           "cabs_iad",       OP_CABS_IAD,    0x032)   
//    X(rsvd,           "cabs_iap",       OP_CABS_IAP,    0x033)   
//    X(rsvd,           "cabs_uop",       OP_CABS_UOP,    0x035)   
//    X(rsvd,           "cabs_uad",       OP_CABS_UAD,    0x036)   
//    X(rsvd,           "cabs_uap",       OP_CABS_UAP,    0x037)   
//
//    X(rsvd,           "crem_iop",       OP_CREM_IOP,    0x039)   
//    X(rsvd,           "crem_iad",       OP_CREM_IAD,    0x03a)   
//    X(rsvd,           "crem_iap",       OP_CREM_IAP,    0x03b)   
//    X(rsvd,           "crem_uod",       OP_CREM_UOD,    0x03c)   
//    X(rsvd,           "crem_uop",       OP_CREM_UOP,    0x03d)   
//    X(rsvd,           "crem_uad",       OP_CREM_UAD,    0x03e)   
//    X(rsvd,           "crem_uap",       OP_CREM_UAP,    0x03f)   
//
//    X(rsvd,           "cshl_uop",       OP_CSHL_UOP,    0x041)   
//    X(rsvd,           "cshl_uad",       OP_CSHL_UAD,    0x042)   
//    X(rsvd,           "cshl_uap",       OP_CSHL_UAP,    0x043)   
//
//    X(rsvd,           "crol_uop",       OP_CROL_UOP,    0x045)   
//    X(rsvd,           "crol_uad",       OP_CROL_UAD,    0x046)   
//    X(rsvd,           "crol_uap",       OP_CROL_UAP,    0x047)   
//
//    X(rsvd,           "cshr_uop",       OP_CSHR_UOP,    0x049)   
//    X(rsvd,           "cshr_uad",       OP_CSHR_UAD,    0x04a)   
//    X(rsvd,           "cshr_uap",       OP_CSHR_UAP,    0x04b)   
//
//    X(rsvd,           "cshr_iop",       OP_CSHR_IOP,    0x04d)   
//    X(rsvd,           "cshr_iad",       OP_CSHR_IAD,    0x04e)   
//    X(rsvd,           "cshr_iap",       OP_CSHR_IAP,    0x04f)   
//
//    X(rsvd,           "clt_uop",        OP_CLT_UOP,     0x0a1)   
//    X(rsvd,           "clt_uad",        OP_CLT_UAD,     0x0a2)   
//    X(rsvd,           "clt_uap",        OP_CLT_UAP,     0x0a3)   
//
//    X(rsvd,           "clt_iop",        OP_CLT_IOP,     0x0a5)   
//    X(rsvd,           "clt_iad",        OP_CLT_IAD,     0x0a6)   
//    X(rsvd,           "clt_iap",        OP_CLT_IAP,     0x0a7)   
//
//    X(rsvd,           "cle_uop",        OP_CLE_UOP,     0x0a9)   
//    X(rsvd,           "cle_uad",        OP_CLE_UAD,     0x0aa)   
//    X(rsvd,           "cle_uap",        OP_CLE_UAP,     0x0ab)   
//
//    X(rsvd,           "cle_iop",        OP_CLE_IOP,     0x0ad)   
//    X(rsvd,           "cle_iad",        OP_CLE_IAD,     0x0ae)   
//    X(rsvd,           "cle_iap",        OP_CLE_IAP,     0x0af)   
//
//    X(rsvd,           "cgt_uop",        OP_CGT_UOP,     0x0b1)   
//    X(rsvd,           "cgt_uad",        OP_CGT_UAD,     0x0b2)   
//    X(rsvd,           "cgt_uap",        OP_CGT_UAP,     0x0b3)   
//
//    X(rsvd,           "cgt_iop",        OP_CGT_IOP,     0x0b5)   
//    X(rsvd,           "cgt_iad",        OP_CGT_IAD,     0x0b6)   
//    X(rsvd,           "cgt_iap",        OP_CGT_IAP,     0x0b7)   
//
//    X(rsvd,           "cge_uop",        OP_CGE_UOP,     0x0b9)   
//    X(rsvd,           "cge_uad",        OP_CGE_UAD,     0x0ba)   
//    X(rsvd,           "cge_uap",        OP_CGE_UAP,     0x0bb)   
//
//    X(rsvd,           "cge_iop",        OP_CGE_IOP,     0x0bd)   
//    X(rsvd,           "cge_iad",        OP_CGE_IAD,     0x0be)   
//    X(rsvd,           "cge_iap",        OP_CGE_IAP,     0x0bf)   
//
//    X(rsvd,           "cne_op",         OP_CNE_OP,      0x0c1)   
//    X(rsvd,           "cne_ad",         OP_CNE_AD,      0x0c2)   
//    X(rsvd,           "cne_ap",         OP_CNE_AP,      0x0c3)   
//
//    X(rsvd,           "ceq_op",         OP_CEQ_OP,      0x0c5)   
//    X(rsvd,           "ceq_ad",         OP_CEQ_AD,      0x0c6)   
//    X(rsvd,           "ceq_ap",         OP_CEQ_AP,      0x0c7)   
//

#endif //ARITH_H

// Clayton Bauman 2014

