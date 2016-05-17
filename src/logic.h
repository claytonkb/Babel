// logic.h
// 

#ifndef LOGIC_H
#define LOGIC_H

mword is_false(mword *bs);
mword is_false_val(mword *bs);
mword is_false_leaf(mword *bs);
mword is_false_ptr(mword *bs);
mword is_false_inte(mword *bs);
mword is_false_tptr(mword *bs);

//bvm_cache *and_od(bvm_cache *this_bvm);
bvm_cache *cnot_od(bvm_cache *this_bvm);

bvm_cache *not_d(bvm_cache *this_bvm);
bvm_cache *and_d(bvm_cache *this_bvm);
bvm_cache *or_d(bvm_cache *this_bvm);
bvm_cache *xor_d(bvm_cache *this_bvm);

bvm_cache *logicF0_od(bvm_cache *this_bvm);
bvm_cache *logicF1_od(bvm_cache *this_bvm);
bvm_cache *logicF2_od(bvm_cache *this_bvm);
bvm_cache *logicF3_od(bvm_cache *this_bvm);
bvm_cache *logicF4_od(bvm_cache *this_bvm);
bvm_cache *logicF5_od(bvm_cache *this_bvm);
bvm_cache *logicF6_od(bvm_cache *this_bvm);
bvm_cache *logicF7_od(bvm_cache *this_bvm);
bvm_cache *logicF8_od(bvm_cache *this_bvm);
bvm_cache *logicF9_od(bvm_cache *this_bvm);
bvm_cache *logicFa_od(bvm_cache *this_bvm);
bvm_cache *logicFb_od(bvm_cache *this_bvm);
bvm_cache *logicFc_od(bvm_cache *this_bvm);
bvm_cache *logicFd_od(bvm_cache *this_bvm);
bvm_cache *logicFe_od(bvm_cache *this_bvm);
bvm_cache *logicFf_od(bvm_cache *this_bvm);


#endif //LOGIC_H

// Clayton Bauman 2014

