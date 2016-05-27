// string.h
// 

#ifndef STRING_H
#define STRING_H

// Just use naive string-matching below these thresholds (profiling-dependent):
#define STRING_LOW_THRESH_SEARCH_LEN 8    // STRING_LOW_THRESH_SEARCH_LEN#
#define STRING_LOW_THRESH_TARGET_LEN 1024 // STRING_LOW_THRESH_TARGET_LEN#

//mword * string_b2c(bvm_cache *this_bvm, mword *string);
mword * string_c2b(bvm_cache *this_bvm, char *string, mword max_safe_length);
mword *_str2ar(bvm_cache *this_bvm, mword *string);
mword *_dec2ci(bvm_cache *this_bvm, mword *op0);
mword *_cu2dec(bvm_cache *this_bvm, mword *op0);
mword *_ci2dec(bvm_cache *this_bvm, mword *op0);
mword *_hex2cu(bvm_cache *this_bvm, mword *op0);
mword *_cu2hex(bvm_cache *this_bvm, mword *op0);
mword *_cu2bin(bvm_cache *this_bvm, mword *op0);
mword *_ar2str(bvm_cache *this_bvm, mword *array);
mword *_bin2cu(bvm_cache *this_bvm, mword *op0);
mword *_oct2cu(bvm_cache *this_bvm, mword *op0);
mword *_cu2oct(bvm_cache *this_bvm, mword *op0);
mword *_radix2cu(bvm_cache *this_bvm, mword *string, mword radix);
void   bsprintf( bvm_cache *this_bvm, mword *buf, mword *offset, const char *format, ... );
mword *_newstr(bvm_cache *this_bvm, mword size8, char set_char);
void  wstrim(bvm_cache *this_bvm, mword *bs);

mword string_ascii_char_match(bvm_cache *this_bvm, mword *string, mword character);
mword *string_ascii_char_match_all(bvm_cache *this_bvm, mword *string, mword character);

mword *string_ascii_str_match(bvm_cache *this_bvm, char *target_str, char *search_str, mword max_match_count);
mword *string_ascii_str_match_naive(bvm_cache *this_bvm, char *target_str, mword target_length, char *search_str, mword search_length, mword max_match_count);
mword *string_ascii_str_horspool(bvm_cache *this_bvm, char *target_str, mword target_length, char *search_str, mword search_length, mword max_match_count);

mword *string_base64e_encode(bvm_cache *this_bvm, mword *array, int array_size);
void base64e_encode_block(unsigned char *src, unsigned char *dest, mword src_length);

mword *string_base64e_decode(bvm_cache *this_bvm, mword *string, mword access_size);
void base64e_decode_block(char *src, char *dest);

bvm_cache *num2str_id(bvm_cache *this_bvm);
bvm_cache *str2num_id(bvm_cache *this_bvm);
bvm_cache *str2num_xd(bvm_cache *this_bvm);
bvm_cache *str2num_bd(bvm_cache *this_bvm);
bvm_cache *num2str_od(bvm_cache *this_bvm);
bvm_cache *str2num_od(bvm_cache *this_bvm);
bvm_cache *str2ar_d(  bvm_cache *this_bvm);
bvm_cache *ar2str_d(  bvm_cache *this_bvm);
bvm_cache *num2str_xd(bvm_cache *this_bvm);
bvm_cache *num2str_bd(bvm_cache *this_bvm);
bvm_cache *c2b_d(     bvm_cache *this_bvm);
bvm_cache *sprint(    bvm_cache *this_bvm);
bvm_cache *str2num_d( bvm_cache *this_bvm);
bvm_cache *num2str_d( bvm_cache *this_bvm);
bvm_cache *num2str_fd(bvm_cache *this_bvm);
bvm_cache *str2num_fd(bvm_cache *this_bvm);
bvm_cache *newstr_d(  bvm_cache *this_bvm);
bvm_cache *strmatch_8d(bvm_cache *this_bvm);

mword *_ci2str(bvm_cache *this_bvm, mword val, mword radix);
mword *_cf2str(bvm_cache *this_bvm, mword *fp_val);
mword *_str2cf(bvm_cache *this_bvm, mword *string);

#endif //STRING_H

// Clayton Bauman 2011

