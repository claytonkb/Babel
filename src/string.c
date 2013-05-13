// string.c
//

#include "babel.h"
#include "string.h"
#include "array.h"
#include "except.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "stack.h"
#include "utf8.h"
#include "debug.h"
#include "array.h"
#include "alloc.h"
#include "bstruct.h"

////These operate on array-8:
////chomp
////  remove a newline
////chop
////  remove final character of string (usually null-terminator)
////pad
////  add a null-terminator
////cr
////  add a newline
////
////m2b -> convert array to array-8 (always adds alignment_word = 0)
////b2m -> convert array-8 to array (simply truncates by 1)


////void b2m(void){
////
////    if(size(TOS_0) < 1)
////        return;
////
////    _trunc((mword*)TOS_0, size(TOS_0)-1);
////
////}
//
//void b2c(void){
//
//    mword *result    = new_atom();
//
//    result = _b2c((mword*)TOS_0);
//
//    zap();
////    push_alloc(result, B2C);
//
//}

//TODO: Move these def'ns to string.h:
#ifdef WINDOWS
#define NEWLINE0 0xd
#define NEWLINE1 0xa
#define NEWLINE_SIZE 2
#endif

#ifdef LINUX
#define NEWLINE1 0xa
#define NEWLINE_SIZE 1
#endif

// TOS_0(this_bvm) . "\n"
// TODO: Optimize for cases when we have slack-space
// babel_operator
bvm_cache *cr(bvm_cache *this_bvm){

//    die;
//    mword *operand = get_from_stack( this_bvm, TOS_0( this_bvm ) ) ;
    mword *operand = dstack_get(this_bvm, 0);//(mword*)TOS_0( this_bvm );

    char *result;
    mword size8;

    if      ( is_leaf(operand)  ){
        size8 = _arlen8(operand) + NEWLINE_SIZE;
        result = (char*)_newlf( array8_size(size8) );
    }
    else{ //Throw an exception
        error("cr: cannot concatenate to a non-leaf array");
    }

    mword *temp = _newin(1);
    (mword*)*temp = (mword*)result;

    mword i,j;
    for(    i=0;
            i<_arlen8(operand);
            i++
        ){

        result[i] = *((char*)operand+i);

    }

#ifdef WINDOWS
    result[size8-2] = NEWLINE0;
    result[size8-1] = NEWLINE1;
#endif

    c((mword*)result,array8_size(size8)-1) = alignment_word8(size8);

//    _mem(result);
//    die;

    //zap(this_bvm);
    //push_alloc(this_bvm, consa((mword*)temp, nil), IMMORTAL);

    zapd(this_bvm,0);
    pushd(this_bvm, (mword*)result, IMMORTAL);

    return this_bvm;

}


//
//
mword *_b2c(mword *string){

    mword strsize = size(string);
    mword last_mword = c(string, strsize-1);
    mword char_length = _arlen8(string);

    last_mword = alignment_word8(dec_alignment_word8(last_mword)+1);

    mword *cstr = _newlf(strsize+1); //Just allocate an extra space in case we need it...

    memcpy(cstr, string, char_length);

    *((char*)cstr+char_length) = (char)0;

    if(~last_mword){ // IOW: last_mword != (mword)-1
        c(cstr, strsize) = last_mword;
    }
    else{
        c(cstr, strsize-1) = last_mword;
    }

    return cstr;
    
}

////
//mword *_b2c(mword *string){
//
//    mword strsize = size(string);
//    mword last_mword = c(string, strsize-1);
//    mword char_length = _arlen8(string);
//
//    last_mword = alignment_word8(dec_alignment_word8(last_mword)+1);
//
//    mword *cstr = _newlf(strsize+1); //Just allocate an extra space in case we need it...
//
//    memcpy(cstr, string, char_length);
//
//    *((char*)cstr+char_length) = (char)0;
//
//    if(last_mword == 0){
//        c(cstr, strsize) = last_mword;
//    }
//    else{
//        c(cstr, strsize-1) = last_mword;
//    }
//
//    return cstr;
//    
//}
//
//
//void c2b(void){
//
//}

//
mword *_c2b(char *string, mword max_safe_length){

    mword length, char_length, last_mword;

    //strlen is unsafe, use memchr instead:
    void *null_term = memchr(string, 0, (size_t)max_safe_length);

    if(null_term == NULL){
        char_length = max_safe_length;
    }
    else{
        char_length = (mword)(null_term - string);
    }

    mword last_mword = alignment_word8(char_length);

    length = (char_length / MWORD_SIZE) + 1;

    if(char_length % MWORD_SIZE != 0){
        length++;
    }

//    d(char_length)
//    d(length)

    mword *result = _newlf(length);

    memcpy(result, string, char_length);

    c(result,length-1) = last_mword;

    return result;

}

// XXX ar2str & str2ar definitely have undiscovered bugs
// ... may need to rewrite the utf-8 stuff
// babel_operator
bvm_cache *ar2str(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result;
    #define MAX_UTF8_CHAR_SIZE 4

    if(is_nil(TOS_0(this_bvm))){
        result = new_atom;
        *result = 0;
        zap(this_bvm);
        push_alloc(this_bvm, result, MORTAL);
        return this_bvm;
    }

    mword arsize = size(TOS_0(this_bvm));
    int temp_buffer_size = MAX_UTF8_CHAR_SIZE * (arsize);
    char *temp_buffer = malloc( temp_buffer_size );
    //FIXME: Check malloc
    
    //    int u8_toutf8(char *dest, int sz, uint32_t *src, int srcsz)
    mword utf8_length = (mword)u8_toutf8(temp_buffer, temp_buffer_size, (uint32_t *)TOS_0(this_bvm), arsize) - 1;
//    d(utf8_length)
//    d(arsize)

    mword arlength = (utf8_length / 4) + 1;

    if(utf8_length % 4){
        arlength++;
    }

    result = _newlf(arlength);
    memcpy(result, temp_buffer, utf8_length);
    free(temp_buffer);

    c(result,arlength-1) = alignment_word8(utf8_length);

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

// babel_operator
bvm_cache *str2ar(bvm_cache *this_bvm){

    fatal("stack fix not done");
    //int u8_strlen(char *s, int max_safe_length)
    mword length8 = _arlen8(TOS_0(this_bvm));
    mword u8_length = (mword)u8_strlen((char *)TOS_0(this_bvm), length8);

    mword *result = _newlf(u8_length+1);

    //int u8_toucs(u_int32_t *dest, int sz, char *src, int srcsz)
    mword length = u8_toucs((uint32_t *)result, u8_length+1, (char *)TOS_0(this_bvm), length8);
//    d(length)

    _trunc(result, length);

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

}

//
// babel_operator
bvm_cache *catoi(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *cstr = _b2c(TOS_0(this_bvm));
    mword *result = _newlf(1);
    *result = (mword)atoi((char*)cstr);

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

}

//
// babel_operator
bvm_cache *dec2ci(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *cstr = _b2c(TOS_0(this_bvm));
    mword *result = _newlf(1);
    *result = (mword)atoi((char*)cstr);

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

}

//
// babel_operator
bvm_cache *hex2cu(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *cstr = _b2c(TOS_0(this_bvm));
    unsigned long *result = (unsigned long *)_newlf( sizeof(unsigned long) / sizeof(mword) ); //XXX Hmmmmmmm
    *result = strtoul ((char*)cstr,NULL,16);
//    *result = (mword)atoi((char*)cstr);

    zap(this_bvm);
    push_alloc(this_bvm, (mword*)result, MORTAL);

}


#define numeric_to_string_operator(x)                   \
    char buffer[MWORD_BIT_SIZE/2];                      \
                                                        \
    int size = sprintf(                                 \
                    buffer, x,                          \
                    icar( dstack_get(this_bvm, 0) ) );  \
                                                        \
    mword arlength = (size / 4) + 1;                    \
                                                        \
    if(size % 4){                                       \
        arlength++;                                     \
    }                                                   \
                                                        \
    mword *result = _newlf(arlength);                   \
                                                        \
    memcpy(result, buffer, size);                       \
    c(result,arlength-1) = alignment_word8(size);       \
                                                        \
    zapd(this_bvm,0);                                   \
    pushd( this_bvm, result, IMMORTAL );                \

//
//
bvm_cache *ci2dec(bvm_cache *this_bvm){ // ci2dec#

    numeric_to_string_operator( "%i" );

}


//
//
bvm_cache *cu2dec(bvm_cache *this_bvm){ // cu2dec#

    numeric_to_string_operator( "%u" );

}

//
// babel_operator
bvm_cache *cu2hex(bvm_cache *this_bvm){ // cu2hex#

    numeric_to_string_operator( "%x" );

}

//
// babel_operator
bvm_cache *ordop(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result  = get_from_stack( this_bvm, TOS_0(this_bvm) );
    hard_zap(this_bvm);

    char ord_value = (char)car(result);

    result = new_atom;
    *result = ord_value;

    push_alloc(this_bvm, result, IMMORTAL);



}

// Clayton Bauman 2011

