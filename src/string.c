// string.c
//

#include "babel.h"
#include "string.h"
#include "array.h"
#include "bstruct.h"
#include "utf8.h"
#include "operator.h"
#include "mem.h"

// FIXME: This function is probably unnecessary - just do a leaf-copy,
// since every Babel string is already a valid C-string
//
mword *string_b2c(bvm_cache *this_bvm, mword *string){ // string_b2c#

    mword strsize = size(string);
    mword last_mword = rcl(string, strsize-1);
    mword char_length = _arlen8(this_bvm, string);

    last_mword = _alignment_word8(this_bvm, _dec_alignment_word8(this_bvm, last_mword)+1);

    //Just allocate an extra space in case we need it...
    mword *cstr = _newlfi(this_bvm, strsize+1,0);

//mword cstr_bounds = mem_bounds_check(this_bvm, cstr);
//mword string_bounds = mem_bounds_check(this_bvm, string);
//_d(cstr_bounds);
//_d(string_bounds); // fails bounds-check
//_d(char_length); //absurd value

    memcpy(cstr, string, char_length); // XXX WAIVER(memcpy) XXX //

    *((char*)cstr+char_length) = (char)0;

    if(~last_mword){ // IOW: last_mword != (mword)-1
        lcl(cstr, strsize) = last_mword;
    }
    else{
        lcl(cstr, strsize-1) = last_mword;
    }

    return cstr;
    
}


//
//
mword *string_c2b(bvm_cache *this_bvm, char *string, mword max_safe_length){ // string_c2b#

    mword char_length;

    //strlen is unsafe, use memchr instead:
    void *null_term = memchr(string, 0, (size_t)max_safe_length);

    if(null_term == NULL){
        char_length = max_safe_length;
    }
    else{
        char_length = (mword)((char*)null_term - string);
    }

    mword *result = _newstr(this_bvm, char_length, ' ');

    memcpy(result, string, char_length); // XXX WAIVER(memcpy) XXX //

    return result;

}


////
////
//mword *string_c2b(bvm_cache *this_bvm, char *string, mword max_safe_length){ // string_c2b#
//
//    mword length, char_length, last_mword;
//
//    //strlen is unsafe, use memchr instead:
//    void *null_term = memchr(string, 0, (size_t)max_safe_length);
//
//    if(null_term == NULL){
//        char_length = max_safe_length;
//    }
//    else{
//        char_length = (mword)((char*)null_term - string);
//    }
//
//    last_mword = _alignment_word8(this_bvm, char_length);
//
//    length = (char_length / MWORD_SIZE) + 1;
//
//    if(char_length % MWORD_SIZE != 0){
//        length++;
//    }
//
////    d(char_length)
////    d(length)
//
//    mword *result = _newlfi(this_bvm, length,0);
//
//    memcpy(result, string, char_length); // XXX WAIVER(memcpy) XXX //
//
//    lcl(result,length-1) = last_mword;
//
//    return result;
//
//}


//
//
mword *_ar2str(bvm_cache *this_bvm, mword *array){ // _ar2str#

    if(is_nil(array)){
        return _newlfi(this_bvm, 1, 0);
    }

    mword *result;
    #define MAX_UTF8_CHAR_SIZE 4

    mword arsize = size(array);
    int temp_buffer_size = MAX_UTF8_CHAR_SIZE * (arsize);

    //free'd below
    char *temp_buffer = malloc( temp_buffer_size ); // XXX FIXME XXX SHOULD NOT USE MALLOC!!!!
    //FIXME: Check malloc; any reason not to use mem_sys_alloc?
    
    mword utf8_length = (mword)u8_toutf8(temp_buffer, temp_buffer_size, (uint32_t *)array, arsize) - 1;

    mword arlength = (utf8_length / 4) + 1;

    if(utf8_length % 4){
        arlength++;
    }

    result = _newlfi(this_bvm, arlength,0);
    memcpy(result, temp_buffer, utf8_length); // XXX FIXME XXX This is only needed because malloc above
    free(temp_buffer);

    lcl(result,arlength-1) = _alignment_word8(this_bvm, utf8_length);

    return result;

}


// Intended for internal-use... 
//
mword *_newstr(bvm_cache *this_bvm, mword size8, char set_char){ // _newstr#

    if(!set_char)
        set_char = ' ';

    mword arlength = _array8_size(this_bvm, size8);

    mword *result = _newlfi(this_bvm, arlength, set_char);

    mword alignment_word = _alignment_word8(this_bvm, size8);

    lcl(result, arlength-1) = alignment_word;
    lcl(result, arlength-2) = (rcl(result, arlength-2)) & ~alignment_word;

    return result;

}

//
//
mword *_str2ar(bvm_cache *this_bvm, mword *string){ // _str2ar#

    mword length8 = _arlen8(this_bvm, string);
    mword u8_length = (mword)u8_strlen((char *)string, length8);

    mword *result = _newlfi(this_bvm, u8_length+1, 0);

    mword length = u8_toucs((uint32_t *)result, u8_length+1, (char *)string, length8);

    _trunc(this_bvm, result, length); //FIXME: Change to array_resize

    return result;

}


//
//
mword *_dec2ci(bvm_cache *this_bvm, mword *op0){ // _dec2ci#

    mword *cstr = string_b2c(this_bvm, op0);
    mword *result = _newlfi(this_bvm, 1, 0);
    *result = (mword)atoi((char*)cstr);

    return result;

}


//
//
mword *_hex2cu(bvm_cache *this_bvm, mword *op0){ // _hex2cu#

    return _radix2cu(this_bvm, op0, 16);

}


//
//
mword *_ci2str(bvm_cache *this_bvm, mword val, mword radix){

    char *result = (char*)_newstr(this_bvm, MWORD_BIT_SIZE, ' '); //MWORD_BIT_SIZE is longest possible (binary)

    itoa(val, result, radix);

    wstrim(this_bvm, (mword*)result);

    return (mword*)result;

}


//
//
mword *_cf2str(bvm_cache *this_bvm, mword *fp_val){

    char *result = (char*)_newstr(this_bvm, (FP_ARITH_SIZE*APPROX_DIGITS_PER_BYTE), ' '); //MWORD_BIT_SIZE is longest possible (binary)

    sprintf(result, "%f", *(double*)fp_val); // FIXME: Doesn't work right on MinGW due to bug in MSVCRT.dll

    wstrim(this_bvm, (mword*)result);

    return (mword*)result;

}


//
//
mword *_radix2cu(bvm_cache *this_bvm, mword *string, mword radix){ // _radix2cu#

    mword *cstr = string_b2c(this_bvm, string);

    unsigned long *result = 
        (unsigned long *)
            _newlfi(
                    this_bvm, 
                    (sizeof(unsigned long) / sizeof(mword)),
                    0);

    *result = strtoul((char*)cstr,NULL,radix);

    return (mword*)result;

}


//
//
mword *_str2cf(bvm_cache *this_bvm, mword *string){ // _str2cf#

//    mword *cstr = string_b2c(this_bvm, string);

    double *result = (double *)_newlfi(this_bvm, FP_ARITH_SIZE, 0);

    *result = strtod((char*)string,NULL);

    return (mword*)result;

}


//
//
mword *_bin2cu(bvm_cache *this_bvm, mword *op0){ // _bin2cu#

    mword *cstr = string_b2c(this_bvm, op0);

    unsigned long *result = 
        (unsigned long *)
            _newlfi(
                    this_bvm, 
                    (sizeof(unsigned long) / sizeof(mword)),
                    0);
    *result = strtoul ((char*)cstr,NULL,2);

    return (mword*)result;

}


//
//
mword *_oct2cu(bvm_cache *this_bvm, mword *op0){ // _oct2cu#

    mword *cstr = string_b2c(this_bvm, op0);

    unsigned long *result = 
        (unsigned long *)
            _newlfi(
                    this_bvm, 
                    (sizeof(unsigned long) / sizeof(mword)),
                    0);
    *result = strtoul ((char*)cstr,NULL,8);

    return (mword*)result;

}

// FIXME Naked constants
// XXX WAIVER(memcpy) XXX // (below)
#define numeric_to_string_operator(x)                          \
    char buffer[MWORD_BIT_SIZE/2];                             \
                                                               \
    int size = sprintf(                                        \
                    buffer, x,                                 \
                    icar( op0 ) );                             \
                                                               \
    mword arlength = (size / 4) + 1;                           \
                                                               \
    if(size % 4){                                              \
        arlength++;                                            \
    }                                                          \
                                                               \
    mword *result = _newlfi(this_bvm, arlength,0);             \
                                                               \
    memcpy(result, buffer, size);                              \
    lcl(result,arlength-1) = _alignment_word8(this_bvm, size); \
    return result;


//
//
mword *_ci2dec(bvm_cache *this_bvm, mword *op0){ // _ci2dec#

    numeric_to_string_operator( "%i" );

}


//
//
mword *_cu2dec(bvm_cache *this_bvm, mword *op0){ // _cu2dec#

    numeric_to_string_operator( "%u" );

}


//
//
mword *_cu2hex(bvm_cache *this_bvm, mword *op0){ // _cu2hex#

    numeric_to_string_operator( "%x" );

}


//
//
mword *_cu2bin(bvm_cache *this_bvm, mword *op0){ // _cu2bin#

    char *result = (char*)_newstr(this_bvm, MWORD_BIT_SIZE, ' ');

    itoa(*op0, result, 2); // FIXME: Naked constant

    wstrim(this_bvm, (mword*)result);

    return (mword*)result;

}


//
//
mword *_cu2oct(bvm_cache *this_bvm, mword *op0){ // _cu2oct#

    char *result = (char*)_newstr(this_bvm, 11, ' '); // FIXME: Naked constant

    itoa(*op0, result, 8); // FIXME: Naked constant

    wstrim(this_bvm, (mword*)result);

    return (mword*)result;

}


// babel-ized sprintf()
//
void bsprintf( bvm_cache *this_bvm, mword *buf, mword *offset, const char *format, ... ){ // bsprintf#

    int printed_length;

    int buf_length = _arlen8(this_bvm, buf) 
                            - *offset 
                            + 1; // +1 because vsnprintf() discounts for the null-terminator

    if(buf_length < 0)
        return;

    va_list args;
    va_start(args,format);

    printed_length = vsnprintf((char*)buf+*offset, buf_length, format, args);

    if(printed_length < 0) // encoding error
        return;

    *offset = *offset + printed_length;

    va_end(args);

}


// Trims trailing whitespace in a leaf8, similar to trunc() but different ;)
// Intended for internal-use ...
void wstrim(bvm_cache *this_bvm, mword *bs){ // wstrim#

    #define WS_CHAR ' '

    mword size8 = _arlen8(this_bvm, bs);

    //find last white-space character
    int i = size8-1;
    char *char_bs = (char*)bs;

    while((i >= 0) && (char_bs[i] == ' ')){
        i--;
    }

    //get new mword-size
    mword msize = _array8_size(this_bvm, i);

    //calculate, set new alignment-word
    mword alignment_word = _alignment_word8(this_bvm, i);
    lcl(bs,msize-1) = alignment_word;

    //clean-up next-to-last mword
    lcl(bs,msize-2) = rcl(bs,msize-2) & ~alignment_word;

    //trunc the s-field
    s(bs) = BYTE_SIZE(msize);

}


//string_ascii_char_match (fast scan for a character)
//string_ascii_char_match_all (fast-scan for all chars, e.g. "/", " ", "\n", etc.)
//string_ascii_str_match (first match from an unsafe-ptr)
//string_ascii_str_match_all
//string_array_match      --> general-purpose matching, i.e. UTF8 strings
//string_array_match_all

//
//
mword string_ascii_char_match(bvm_cache *this_bvm, mword *string, mword character){ // string_ascii_char_match#

    mword *result = memchr(string, character, _arlen8(this_bvm, string));

    if(result == NULL){
        return (mword)-1; //FIXME naked constant
    }
    else{
        return (mword)(result-string);
    }

}


//
//
mword *string_ascii_char_match_all(bvm_cache *this_bvm, mword *string, mword character){ // string_ascii_char_match_all#

    mword *result = nil;
    mword *result_tail;
    char  *last_match = (char*)string;
    char  *curr_match;
    mword length = _arlen8(this_bvm, string);
    mword curr_offset = 0;

    do{

        curr_match = memchr(last_match, character, length-curr_offset);

        if(curr_match == NULL){
            break;
        }
        else{
            curr_offset = (mword)((mword)curr_match-(mword)string);
            if(is_nil(result)){
                result = _cons(this_bvm, _val(this_bvm, curr_offset), nil);
                result_tail = result;
            }
            else{
                lci(result_tail,1) = _cons(this_bvm, _val(this_bvm, curr_offset), nil);
                result_tail = rci(result_tail,1);
            }
        }

        last_match = curr_match+1;

    } while(1);

    return result;

}


//
//
mword *string_ascii_str_match(bvm_cache *this_bvm, char *target_str, char *search_str, mword max_match_count){ // string_ascii_str_match#

    mword search_length     = _arlen8(this_bvm, (mword*)search_str);
    mword target_length     = _arlen8(this_bvm, (mword*)target_str);
    mword *result;

    if(   (search_length > target_length) 
       || (search_length   == 0) 
       || (target_length   == 0)
       || (max_match_count == 0) )
        return nil;

    if( (search_length == target_length) ){
        if(memcmp(target_str, search_str, search_length) == 0){
            return _cons(this_bvm, _val(this_bvm, 0), nil);
        }
        else{
            return nil;
        }
    }

    if(   search_length < STRING_LOW_THRESH_SEARCH_LEN
       || target_length < STRING_LOW_THRESH_TARGET_LEN){
        result = string_ascii_str_match_naive(this_bvm, target_str, target_length, search_str, search_length, max_match_count);
    }
    else{
        result = string_ascii_str_horspool(this_bvm, target_str, target_length, search_str, search_length, max_match_count);
    }

    return result;


}


//
//
mword *string_ascii_str_match_naive(bvm_cache *this_bvm, char *target_str, mword target_length, char *search_str, mword search_length, mword max_match_count){ // string_ascii_str_match_naive#

    mword *result = nil;
    mword *result_tail;

    char  *curr_char_match;

    mword curr_char_offset = search_length-1;
    mword last_search_char = curr_char_offset;
    mword str_match_found  = 1; // match is positive when str_match_found==0
    mword match_count      = 0;

    mword search_str_last_index = search_length-1;
    mword target_baseline = (mword)(target_str+search_str_last_index);
    mword result_index;

    while( (max_match_count > match_count)
            && (curr_char_offset < target_length)){

        curr_char_match = memchr((target_str+curr_char_offset), search_str[last_search_char], target_length-curr_char_offset);

        if(curr_char_match == NULL){ // no more matches
            break;
        }
        else{

            str_match_found = memcmp((curr_char_match-search_str_last_index), search_str, search_length);

            if(str_match_found != 0){
                curr_char_offset = (curr_char_match-target_str) + 1;
                continue;
            }

            result_index = (mword)(curr_char_match-target_baseline);

            // add matching sub-string offset to result list
            if(is_nil(result)){
                result = _cons(this_bvm, _val(this_bvm, result_index), nil);
                result_tail = result;
            }
            else{
                lci(result_tail,1) = _cons(this_bvm, _val(this_bvm, result_index), nil);
                result_tail = rci(result_tail,1);
            }

            match_count++;
            curr_char_offset = (mword)(curr_char_match-target_str)+search_length; // non-overlapping matches

        }

    }

    return result;

}


// Boyer-Moore Horspool
//
mword *string_ascii_str_horspool(bvm_cache *this_bvm, char *target_str, mword target_length, char *search_str, mword search_length, mword max_match_count){ // string_ascii_str_horspool#

    mword *horspool_shift   = _newlfi(this_bvm, EXTENDED_ASCII_ALPHA_SIZE, 0);

    int i;
    for(i=0; i<EXTENDED_ASCII_ALPHA_SIZE; i++){
        lcl(horspool_shift, i) = search_length;
    }

    for(i=0; i<search_length; i++){
        lcl(horspool_shift, search_str[i]) = search_length-i-1;
    }

    mword *result = nil;
    mword *result_tail;

    mword  curr_char_offset = search_length-1;
    mword  str_match_found  = 1; // match is positive when str_match_found==0
    mword  match_count      = 0;

    mword  search_str_last_index = search_length-1;
    char   last_search_char = search_str[search_length-1];
    char   curr_char;
    mword result_index;

    while( (max_match_count > match_count)
            && (curr_char_offset < target_length)){

        curr_char = target_str[curr_char_offset];

        if(curr_char != last_search_char){
            curr_char_offset += horspool_shift[(mword)curr_char];
            continue;
        }

        result_index = curr_char_offset-search_str_last_index;

        str_match_found = memcmp((target_str+result_index), search_str, search_length);

        if(str_match_found != 0){
            curr_char_offset++;
            continue;
        }

        // add matching sub-string offset to result list
        if(is_nil(result)){
            result = _cons(this_bvm, _val(this_bvm, result_index), nil);
            result_tail = result;
        }
        else{
            lci(result_tail,1) = _cons(this_bvm, _val(this_bvm, result_index), nil);
            result_tail = rci(result_tail,1);
        }

        match_count++;
        curr_char_offset += search_length; // non-overlapping matches

    }

    return result;

}

// base64-e (e stands for 'ECC-compatible')
// This is a base64 encoding designed to allow bit-flip errors to "show through"
// the encoding. This simplifies the assumptions of coupling with an ECC algorithm.
//
//  3 2 1 0
//0 y x x x ==> ==YY XXXX 
//
//0 y x x x ==> =ZZZ YYYY XXXX
//1 z z y y
//
//0 y x x x ==> WWWW ZZZZ YYYY XXXX
//1 z z y y
//2 w w w z
//
//            76543210 76543210 76543210 76543210
//-----------------------------------------------
//    x   ==> 00111101 00111101 ..0000xx ..xxxxxx
//  y x   ==> 00111101 ..00yyyy ..yyyyxx ..xxxxxx
//z y x   ==> ..zzzzzz ..zzyyyy ..yyyyxx ..xxxxxx
//-----------------------------------------------
//2 1 0              3        2        1        0


// array_size must be in bytes
//
mword *string_base64e_encode(bvm_cache *this_bvm, mword *array, int array_size){ // string_base64e_encode#

    mword *result;
    char *src_array = (char*)array;
    mword result_size;

    if(array_size == 0){
        return nil;
    }

    result_size = ((array_size+2)/3)*4;

    result = _newstr(this_bvm, result_size, ' ');

    char *work_result = (char*)result;

    while(array_size > 0){
        if(array_size == 1){
            base64e_encode_block((unsigned char*)src_array, (unsigned char*)work_result, 1);
            break;
        }
        else if(array_size == 2){
            base64e_encode_block((unsigned char*)src_array, (unsigned char*)work_result, 2);
            break;
        }
        else{
            base64e_encode_block((unsigned char*)src_array, (unsigned char*)work_result, 3);
        }
        array_size  -= 3;
        src_array   += 3;
        work_result += 4;
    }

    return result;

}


// src_length must be 1, 2  or 3
// dest is always written with 4 characters
//
void base64e_encode_block(unsigned char *src, unsigned char *dest, mword src_length){ // base64e_encode_block#

    unsigned char base64e_alpha[64] = 
        { '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
          'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
          '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
          'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', '?' };

    switch(src_length){
        case 3:
            //33333322 22221111 11000000 --> 3 bytes : 4 chars
            dest[0] = base64e_alpha[ (src[0] & 0x3f) ];
            dest[1] = base64e_alpha[ (((src[1] & 0x0f) << 2) | ((src[0] & 0xc0) >> 6)) ];
            dest[2] = base64e_alpha[ (((src[2] & 0x03) << 4) | ((src[1] & 0xf0) >> 4)) ];
            dest[3] = base64e_alpha[ ( (src[2] & 0xfc) >> 2) ];
            break;
        case 2:
            //XXXXXX-- 22221111 11000000 --> 3 bytes : 3 chars + 1 padding char
            dest[0] = base64e_alpha[ (src[0] & 0x3f) ];
            dest[1] = base64e_alpha[ (((src[1] & 0x0f) << 2) | ((src[0] & 0xc0) >> 6)) ];
            dest[2] = base64e_alpha[ (                         ((src[1] & 0xf0) >> 4)) ];
            dest[3] = '=';
            break;
        case 1:
            //XXXXXXYY YYYY---- 11000000 --> 1 byte  : 2 chars + 2 padding chars
            dest[0] = base64e_alpha[ (src[0] & 0x3f) ];
            dest[1] = base64e_alpha[ (                         ((src[0] & 0xc0) >> 6)) ];
            dest[2] = '=';
            dest[3] = '=';
            break;
        default:
            _fatal("Bad src_length");
    }

}


//
//
mword *string_base64e_decode(bvm_cache *this_bvm, mword *string, mword access_size){ // string_base64e_decode#

    mword string_size = _arlen8(this_bvm, string);
    char *string_array = (char*)string;
    mword result_size;
    mword *result;

    if(string_size % 4) // bad string
        return nil;

    if(string_size>4){
        result_size = ((string_size-4) / 4) * 3;
    }
    else{
        result_size = 0;
    }

    if(     string_array[string_size-2] == '='){ // 2 padding chars
        result_size += 1;
    }
    else if(string_array[string_size-1] == '='){ // 1 padding char
        result_size += 2;
    }
    else{ // 0 padding chars
        result_size += 3;
    }

    if(access_size == ACCESS_SIZE_MWORD){
        if(result_size % MWORD_SIZE)
            return nil;
        result = _newlfi(this_bvm, result_size/MWORD_SIZE, 0);
    }
    else{
        result = _newstr(this_bvm, result_size, ' ');
    }

    char *work_result = (char*)result;

    while(string_size > 0){
        base64e_decode_block(string_array, work_result);
        string_size  -= 4;
        string_array += 4;
        work_result  += 3;
    }

    return result;

}


// src must be a 4-byte block
//
void base64e_decode_block(char *src, char *dest){ // base64e_decode_block#

    if(src[3] == '='){ // Check for end-block

        dest[0] =  ((src[0] & 0x3f) | ((src[1] & 0x03) << 6));

        if(src[2] != '='){
            dest[1] = (((src[1] & 0x3c) >> 2) | ((src[2] & 0x0f) << 4));
        }

    }
    else{
        //33333322 22221111 11000000 --> 3 bytes : 4 chars
        dest[0] =  ((src[0] & 0x3f)       | ((src[1] & 0x03) << 6));
        dest[1] = (((src[1] & 0x3c) >> 2) | ((src[2] & 0x0f) << 4));
        dest[2] = (((src[2] & 0x30) >> 4) | ((src[3] & 0x3f) << 2));

    }

}



/*****************************************************************************
 *                                                                           *
 *                           STRING OPERATORS                                *
 *                                                                           *
 ****************************************************************************/


//mword *string_ascii_str_match(bvm_cache *this_bvm, char *target_str, char *search_str, mword max_match_count);
#define STRMATCH_8D_OPERATIONS \
    result0 = string_ascii_str_match(this_bvm, (char*)oi1.data, (char*)oi0.data, MWORD_MAX_VAL);

OPERATORA_R2_W1_D(strmatch_8d, 
        STRMATCH_8D_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 0)


//mword *_newstr(bvm_cache *this_bvm, mword size8, char set_char){ // _newstr#

#define NEWSTR_D_OPERATIONS \
    result0 = _newstr( this_bvm, *oi1.data, *(char*)oi0.data);

OPERATORA_R2_W1_D(newstr_d, 
        NEWSTR_D_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 0)


// "      " 0 "%x" 23189 sprint
// Rules: Operator can only use a single printf %-code in format arg
//
#define SPRINT_OPERATIONS \
    bsprintf( this_bvm, oi3.data, oi2.data, (char*)oi1.data, *oi0.data );

//OPERATOR_R4_W0_D(sprint, 
//        SPRINT_OPERATIONS, 
//        0, OI_MASK_LEAF, 0, 0,
//        0, OI_MASK_LEAF, 0, 0,
//        0, OI_MASK_LEAF, 0, 0,
//        0, OI_MASK_LEAF, 0, 0)

OPERATORA_R4_W0_D(sprint, 
        SPRINT_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 0)


#define STR2AR_OPERATIONS \
    result0 = _str2ar( this_bvm, oi0.data );

//OPERATOR_R1_W1_ID(str2ar_d, 
//        STR2AR_OPERATIONS , 
//        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)

OPERATORA_R1_W1_D(str2ar_d, 
        STR2AR_OPERATIONS , 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define AR2STR_OPERATIONS \
    result0 = _ar2str( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(ar2str_d, 
        AR2STR_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define NUM2STR_D_OPERATIONS \
    result0 = _ci2str( this_bvm, *oi1.data, *oi0.data );

OPERATORA_R2_W1_D(num2str_d, 
        NUM2STR_D_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 1)


#define STR2NUM_D_OPERATIONS \
    result0 = _radix2cu( this_bvm, oi1.data, *oi0.data );

OPERATORA_R2_W1_D(str2num_d, 
        STR2NUM_D_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 1)


#define NUM2STR_FD_OPERATIONS \
    result0 = _cf2str( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(num2str_fd, 
        NUM2STR_FD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0)


#define STR2NUM_FD_OPERATIONS \
    result0 = _str2cf( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(str2num_fd, 
        STR2NUM_FD_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0)


#define NUM2STR_ID_OPERATIONS \
    result0 = _ci2dec( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(num2str_id, 
        NUM2STR_ID_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define NUM2STR_XD_OPERATIONS \
    result0 = _cu2hex( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(num2str_xd, 
        NUM2STR_XD_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define NUM2STR_BD_OPERATIONS \
    result0 = _cu2bin( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(num2str_bd, 
        NUM2STR_BD_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define STR2NUM_ID_OPERATIONS \
    result0 = _dec2ci( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(str2num_id, 
        STR2NUM_ID_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define NUM2STR_OD_OPERATIONS \
    result0 = _cu2oct( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(num2str_od, 
        NUM2STR_OD_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define STR2NUM_OD_OPERATIONS \
    result0 = _oct2cu( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(str2num_od, 
        STR2NUM_OD_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define STR2NUM_XD_OPERATIONS \
    result0 = _hex2cu( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(str2num_xd, 
        STR2NUM_XD_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define STR2NUM_BD_OPERATIONS \
    result0 = _bin2cu( this_bvm, oi0.data );

OPERATORA_R1_W1_ID(str2num_bd, 
        STR2NUM_BD_OPERATIONS, 
        0, OI_MASK_LEAF|OI_MASK_NONE, 0, 0)


#define ORD_D_OPERATIONS \
    char ord_value = (char)*oi0.data; \
    result0 = _val( this_bvm, ord_value );

OPERATORA_R1_W1_ID(ord_d, 
        ORD_D_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0)


//XXX BROKEN XXX
#define C2B_D_OPERATIONS \
    result0 = string_c2b( this_bvm, (char*)*oi0.data, strlen((char*)oi0.data) ); //XXX UNSAFE XXX

OPERATORA_R1_W1_ID(c2b_d, 
        C2B_D_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0)


// Clayton Bauman 2014

