// sexpr.h
// 

#ifndef SEXPR_H
#define SEXPR_H

#include "operator.h"

mword *_pre_sexpr(bvm_cache *this_bvm, mword *string, mword *index);
mword *_sexpr(bvm_cache *this_bvm, mword *string, mword *index, mword *sexpr_type);
mword *_unescape(bvm_cache *this_bvm, mword *string, mword escape_char);
bvm_cache *sexpr(bvm_cache *this_bvm);
mword *sexpr_op(bvm_cache *this_bvm, oinfo *oi);
mword *sexpr_op2(bvm_cache *this_bvm, mword *bstring);

#endif //SEXPR_H

// Clayton Bauman 2014

