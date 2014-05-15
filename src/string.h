// string.h
// 

#ifndef STRING_H
#define STRING_H

//void b2c(void);
mword *_b2c(bvm_cache *this_bvm, mword *string);
mword *_c2b(bvm_cache *this_bvm, char *string, mword max_safe_length);

bvm_cache *ar2str(bvm_cache *this_bvm);
bvm_cache *str2ar(bvm_cache *this_bvm);
bvm_cache *catoi(bvm_cache *this_bvm);
bvm_cache *dec2ci(bvm_cache *this_bvm);
bvm_cache *cu2dec(bvm_cache *this_bvm);
bvm_cache *ci2dec(bvm_cache *this_bvm);
bvm_cache *hex2cu(bvm_cache *this_bvm);
bvm_cache *cu2hex(bvm_cache *this_bvm);
bvm_cache *ordop(bvm_cache *this_bvm);
bvm_cache *cr(bvm_cache *this_bvm);
bvm_cache *charop(bvm_cache *this_bvm);
mword *_ar2str(bvm_cache *this_bvm, mword *array);
bvm_cache *sexpr(bvm_cache *this_bvm);
mword *_sexpr(bvm_cache *this_bvm, mword *string, mword *index, mword *sexpr_type);
mword *_pre_sexpr(bvm_cache *this_bvm, mword *string, mword *index);
bvm_cache *bin2cu(bvm_cache *this_bvm);
bvm_cache *oct2cu(bvm_cache *this_bvm);
mword *_radix2cu(bvm_cache *this_bvm, mword *string, mword radix);
mword *_unescape(bvm_cache *this_bvm, mword *string, mword escape_char);

//mword *SEXPR_LIST_SYMBOL;
//mword *SEXPR_CODE_SYMBOL;
//
//#define sexpr_list_symbol (SEXPR_LIST_SYMBOL+1)
//#define sexpr_code_symbol (SEXPR_CODE_SYMBOL+1)

////file.h
//extern char* myArray[];
//extern const size_t sizeof_myArray;


#endif //STRING_H

// Clayton Bauman 2011

