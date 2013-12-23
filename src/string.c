// string.c
//

#include "babel.h"
#include "string.h"
#include "array.h"
#include "except.h"
#include "list.h"
//#include "bvm_opcodes.h"
#include "stack.h"
#include "utf8.h"
//#include "debug.h"
#include "array.h"
#include "alloc.h"
#include "bstruct.h"
#include "tptr.h"
#include "mem.h"

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

// TODO: Optimize for cases when we have slack-space
/* string operator
**cr**
> Appends a newline to TOS  
> `TOS\ . "\n"`
*/
bvm_cache *cr(bvm_cache *this_bvm){ // cr#

    mword *operand = dstack_get(this_bvm, 0);//(mword*)TOS_0( this_bvm );
    popd(this_bvm);

    char *result;
    mword size8;

    if      ( is_leaf(operand)  ){
        size8 = _arlen8(operand) + NEWLINE_SIZE;
        result = (char*)_new2lf(this_bvm,  array8_size(size8) );
    }
    else{ //Throw an exception
        error("cr: cannot concatenate to a non-leaf array");
    }

    mword *temp = _new2in(this_bvm, 1);
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

    //zapd(this_bvm,0);
    pushd(this_bvm, (mword*)result, IMMORTAL);

    return this_bvm;

}


//FIXME: Probably broken, doesn't seem to work
// --> Rename and back-propagate... UGH!!!!
mword *_b2c(mword *string){

    mword strsize = size(string);
    mword last_mword = c(string, strsize-1);
    mword char_length = _arlen8(string);

    last_mword = alignment_word8(dec_alignment_word8(last_mword)+1);

//Just allocate an extra space in case we need it...
    mword *cstr = _newlf(strsize+1); //FIXME DEPRECATED _newlf (see above)

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

// rename and back-propagate
//
mword *_c2b(char *string, mword max_safe_length){ // _c2b#

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

    mword *result = _newlf(length); //FIXME DEPRECATED _newlf (see above)

    memcpy(result, string, char_length);

    c(result,length-1) = last_mword;

    return result;

}

// XXX ar2str & str2ar need to be cleaned up...
/* string operator
**ar2str**
> "Convert array to string"  
> Undoes str2ar, see above.  
>  
> `{X}| -> {"string"}|`  
*/
bvm_cache *ar2str(bvm_cache *this_bvm){ // ar2str#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *result;
    #define MAX_UTF8_CHAR_SIZE 4

    if(is_nil(op0)){
        result = new_atom;
        *result = 0;
        zap(this_bvm);
        push_alloc(this_bvm, result, MORTAL);
        return this_bvm;
    }

    mword arsize = size(op0);
    int temp_buffer_size = MAX_UTF8_CHAR_SIZE * (arsize);
    char *temp_buffer = malloc( temp_buffer_size );
    //FIXME: Check malloc
    
    mword utf8_length = (mword)u8_toutf8(temp_buffer, temp_buffer_size, (uint32_t *)op0, arsize) - 1;

    mword arlength = (utf8_length / 4) + 1;

    if(utf8_length % 4){
        arlength++;
    }

    result = _new2lf(this_bvm, arlength);
    memcpy(result, temp_buffer, utf8_length);
    free(temp_buffer);

    c(result,arlength-1) = alignment_word8(utf8_length);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* string operator
**str2ar**
> "Convert string to array"  
> 
> "Expands" a string so that each character is one value of a leaf-
> array. The input string must be valid UTF-8 (UTF-8 is a superset
> of ASCII) and each value of the resulting array will be the
> Unicode codepoint corresponding to each UTF-8 character.  
>
> `{"string"}| -> {X}|`  
>  
> Where X is a leaf-array of Unicode code-points.  
*/
bvm_cache *str2ar(bvm_cache *this_bvm){ // str2ar#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword length8 = _arlen8(op0);
    mword u8_length = (mword)u8_strlen((char *)op0, length8);

    mword *result = _new2lf(this_bvm, u8_length+1);

    mword length = u8_toucs((uint32_t *)result, u8_length+1, (char *)op0, length8);

    _trunc(result, length);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* string operator
**catoi** 
> C-style atoi() (ASCII-to-integer)  
*/
bvm_cache *catoi(bvm_cache *this_bvm){ // catoi#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *cstr = _b2c(op0);
    mword *result = _new2lf(this_bvm, 1);
    *result = (mword)atoi((char*)cstr);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* string operator
**dec2ci** ($d)  
> Undoes ci2dec
*/
bvm_cache *dec2ci(bvm_cache *this_bvm){ // dec2ci#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *cstr = _b2c(op0);
    mword *result = _new2lf(this_bvm, 1);
    *result = (mword)atoi((char*)cstr);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* string operator
**hex2cu** ($x)  
> Undoes cu2hex  
*/
bvm_cache *hex2cu(bvm_cache *this_bvm){ // hex2cu#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *cstr = _b2c(op0);
    unsigned long *result = (unsigned long *)_new2lf(this_bvm, sizeof(unsigned long) / sizeof(mword) ); //XXX Hmmmmmmm
    *result = strtoul ((char*)cstr,NULL,16);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

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
    mword *result = _new2lf(this_bvm, arlength);        \
                                                        \
    memcpy(result, buffer, size);                       \
    c(result,arlength-1) = alignment_word8(size);       \
                                                        \
    popd(this_bvm);                                     \
    pushd( this_bvm, result, IMMORTAL );                \

/* string operator
**ci2dec** (%d)  
> Converts an C-style integer value to its ASCII decimal
> representation  
>  
> `{x}| -> {"string"}|`  
*/
bvm_cache *ci2dec(bvm_cache *this_bvm){ // ci2dec#

    numeric_to_string_operator( "%i" );

}


/* string operator
**cu2dec** (%u)  
> Converts an C-style unsigned value to its ASCII decimal
> representation  
>  
> `{x}| -> {"string"}|`  
*/
bvm_cache *cu2dec(bvm_cache *this_bvm){ // cu2dec#

    numeric_to_string_operator( "%u" );

}

/* string operator
**cu2hex** (%x)  
> Converts an C-style unsigned value to its ASCII hexadecimal
> representation  
>  
> `{x}| -> {"string"}|`  
*/
bvm_cache *cu2hex(bvm_cache *this_bvm){ // cu2hex#

    numeric_to_string_operator( "%x" );

}

/* string operator
**ord**   
> Returns the numeric value of a character  
*/
bvm_cache *ordop(bvm_cache *this_bvm){ // ordop# ord#

    mword *result = dstack_get(this_bvm,0);
    popd(this_bvm);

    char ord_value = (char)icar(result);

    result = _new2va( this_bvm, ord_value);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// Clayton Bauman 2011

