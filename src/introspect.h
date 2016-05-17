// introspect.h
// 

#ifndef INTROSPECT_H
#define INTROSPECT_H

mword *_bs2gv(bvm_cache *this_bvm, mword *bs);
mword  _rbs2gv(bvm_cache *this_bvm, mword *bs, char *buffer, int initial);

mword *_bs2gv2(bvm_cache *this_bvm, mword *bs);
//mword _rbs2gv2(bvm_cache *this_bvm, mword *bs, mword *result, mword *offset, int initial);
void _rbs2gv2(bvm_cache *this_bvm, mword *bs, mword *result, mword *offset, int initial);

//mword *_bbl2str(bvm_cache *this_bvm, mword *operand);
//mword  _rbbl2str(bvm_cache *this_bvm, mword *bs, char *buffer);

bvm_cache *bs2gv_d(bvm_cache *this_bvm);
bvm_cache *bs2str_d(bvm_cache *this_bvm);
bvm_cache *bs2str2_d(bvm_cache *this_bvm);

mword *_bs2str(bvm_cache *this_bvm, mword *bs);
//mword _rbs2str(bvm_cache *this_bvm, mword *bs, char *buffer);
void _rbs2str(bvm_cache *this_bvm, mword *bs, mword *result, mword *offset);

mword *_bs2str2(bvm_cache *this_bvm, mword *bs);

mword introspect_calc_buf_length( bvm_cache *this_bvm, mword *bs, mword const_padding, mword nlf_scaling, mword nva_scaling, mword nin_scaling, mword nptr_scaling, mword ntag_scaling);

void _stack_dump(bvm_cache *this_bvm, mword *stack);
mword is_nil_leaf_based(mword *bs);

#endif //INTROSPECT_H

// Clayton Bauman 2014

