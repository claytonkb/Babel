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
        size8 = _arlen8(this_bvm, operand) + NEWLINE_SIZE;
        result = (char*)_newlfi(this_bvm,  array8_size(this_bvm, size8), 0);
    }
    else{ //Throw an exception
        error("cr: cannot concatenate to a non-leaf array");
    }

    mword *temp = _newin(this_bvm, 1);
    (mword*)*temp = (mword*)result;

    mword i,j;
    for(    i=0;
            i<_arlen8(this_bvm, operand);
            i++
        ){

        result[i] = *((char*)operand+i);

    }

#ifdef WINDOWS
    result[size8-2] = NEWLINE0;
    result[size8-1] = NEWLINE1;
#endif

    c((mword*)result,array8_size(this_bvm, size8)-1) = alignment_word8(this_bvm, size8);

    //zapd(this_bvm,0);
    pushd(this_bvm, (mword*)result, IMMORTAL);

    return this_bvm;

}


//FIXME: Probably broken, doesn't seem to work
// --> Rename and back-propagate... UGH!!!!
mword *_b2c(bvm_cache *this_bvm, mword *string){ // *_b2c#

    mword strsize = size(string);
    mword last_mword = c(string, strsize-1);
    mword char_length = _arlen8(this_bvm, string);

    last_mword = alignment_word8(this_bvm, dec_alignment_word8(this_bvm, last_mword)+1);

//Just allocate an extra space in case we need it...
    mword *cstr = _newlfi(this_bvm, strsize+1,0);

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


// rename and back-propagate
//
mword *_c2b(bvm_cache *this_bvm, char *string, mword max_safe_length){ // _c2b#

    mword length, char_length, last_mword;
    //strlen is unsafe, use memchr instead:
    void *null_term = memchr(string, 0, (size_t)max_safe_length);

    if(null_term == NULL){
        char_length = max_safe_length;
    }
    else{
        char_length = (mword)(null_term - string);
    }

    mword last_mword = alignment_word8(this_bvm, char_length);

    length = (char_length / MWORD_SIZE) + 1;

    if(char_length % MWORD_SIZE != 0){
        length++;
    }

//    d(char_length)
//    d(length)

    mword *result = _newlfi(this_bvm, length,0);

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

//    mword *result;
//    #define MAX_UTF8_CHAR_SIZE 4
//
//    if(is_nil(op0)){
//        result = new_atom;
//        *result = 0;
//        zap(this_bvm);
//        push_alloc(this_bvm, result, MORTAL);
//        return this_bvm;
//    }
//
//    mword arsize = size(op0);
//    int temp_buffer_size = MAX_UTF8_CHAR_SIZE * (arsize);
//
//    //free'd below
//    char *temp_buffer = malloc( temp_buffer_size ); // XXX WAIVER XXX
//    //FIXME: Check malloc
//    
//    mword utf8_length = (mword)u8_toutf8(temp_buffer, temp_buffer_size, (uint32_t *)op0, arsize) - 1;
//
//    mword arlength = (utf8_length / 4) + 1;
//
//    if(utf8_length % 4){
//        arlength++;
//    }
//
//    result = _newlfi(this_bvm, arlength,0);
//    memcpy(result, temp_buffer, utf8_length);
//    free(temp_buffer);
//
//    c(result,arlength-1) = alignment_word8(this_bvm, utf8_length);

    mword *result = _ar2str(this_bvm, op0);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
mword *_ar2str(bvm_cache *this_bvm, mword *array){

    if(is_nil(array)){
        return _newlfi(this_bvm, 1, 0);
    }

    mword *result;
    #define MAX_UTF8_CHAR_SIZE 4

    mword arsize = size(array);
    int temp_buffer_size = MAX_UTF8_CHAR_SIZE * (arsize);

    //free'd below
    char *temp_buffer = malloc( temp_buffer_size ); // XXX WAIVER XXX
    //FIXME: Check malloc
    
    mword utf8_length = (mword)u8_toutf8(temp_buffer, temp_buffer_size, (uint32_t *)array, arsize) - 1;

    mword arlength = (utf8_length / 4) + 1;

    if(utf8_length % 4){
        arlength++;
    }

    result = _newlfi(this_bvm, arlength,0);
    memcpy(result, temp_buffer, utf8_length);
    free(temp_buffer);

    c(result,arlength-1) = alignment_word8(this_bvm, utf8_length);

    return result;

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

    mword length8 = _arlen8(this_bvm, op0);
    mword u8_length = (mword)u8_strlen((char *)op0, length8);

    mword *result = _newlfi(this_bvm, u8_length+1, 0);

    mword length = u8_toucs((uint32_t *)result, u8_length+1, (char *)op0, length8);

    _trunc(this_bvm, result, length);

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

    mword *cstr = _b2c(this_bvm, op0);
    mword *result = _newlfi(this_bvm, 1,0);
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

    mword *cstr = _b2c(this_bvm, op0);
    mword *result = _newlfi(this_bvm, 1, 0);
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

    mword *result = _radix2cu(this_bvm, op0, 16);

//    mword *cstr = _b2c(this_bvm, op0);
//
//    unsigned long *result = 
//        (unsigned long *)
//            _newlfi(
//                    this_bvm, 
//                    (sizeof(unsigned long) / sizeof(mword)),
//                    0);
//    *result = strtoul ((char*)cstr,NULL,16);
//
    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
mword *_radix2cu(bvm_cache *this_bvm, mword *string, mword radix){ // _radix2cu#

    mword *cstr = _b2c(this_bvm, string);

    unsigned long *result = 
        (unsigned long *)
            _newlfi(
                    this_bvm, 
                    (sizeof(unsigned long) / sizeof(mword)),
                    0);
    *result = strtoul ((char*)cstr,NULL,radix);

    return (mword *)result;

}


/* string operator
**bin2cu** ($b)  
*/
bvm_cache *bin2cu(bvm_cache *this_bvm){ // bin2cu#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *cstr = _b2c(this_bvm, op0);

    unsigned long *result = 
        (unsigned long *)
            _newlfi(
                    this_bvm, 
                    (sizeof(unsigned long) / sizeof(mword)),
                    0);
    *result = strtoul ((char*)cstr,NULL,2);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}



/* string operator
**oct2cu** ($o)  
*/
bvm_cache *oct2cu(bvm_cache *this_bvm){ // oct2cu#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *cstr = _b2c(this_bvm, op0);

    unsigned long *result = 
        (unsigned long *)
            _newlfi(
                    this_bvm, 
                    (sizeof(unsigned long) / sizeof(mword)),
                    0);
    *result = strtoul ((char*)cstr,NULL,8);

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
    mword *result = _newlfi(this_bvm, arlength,0);        \
                                                        \
    memcpy(result, buffer, size);                       \
    c(result,arlength-1) = alignment_word8(this_bvm, size);       \
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

    result = _newva( this_bvm, ord_value);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* string operator
**char**   
> Creates a character from its UTF-8 encoding
*/
bvm_cache *charop(bvm_cache *this_bvm){ // charop# char#

//    mword *result = dstack_get(this_bvm,0);
//    popd(this_bvm);
//
//    char ord_value = (char)icar(result);
//
//    result = _newva( this_bvm, ord_value);
//
//    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* string operator
**sexpr**   
> Parses an s-expression string into a Babel structure
*/
bvm_cache *sexpr(bvm_cache *this_bvm){ // sexpr#

    str2ar(this_bvm);

    mword *string = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword index=0;
    mword *result = _pre_sexpr(this_bvm, string, &index);

//_dump(result);
//die;

    pushd(this_bvm, result, IMMORTAL);

}


#define append_sexpr(x) \
    if(is_nil(curr_sexpr)){         \
        curr_sexpr = consa(this_bvm, x, nil);\
    }\
    else{\
        _unshift(this_bvm,                              \
            curr_sexpr,                                 \
            x);                            \
    }


#define capture_token                                       \
    capture_length = j-i;                                   \
    if(capture_length){                                     \
        captured_token = _slice(this_bvm, string, i, j);    \
        append_sexpr(captured_token);                       \
    }                                                       \
    else{                                                   \
        parse_error;                                        \
    }                                                       \


#define capture_dquote                                       \
    capture_length = j-i;                                   \
    if(capture_length){                                     \
        captured_token = _unescape( this_bvm, _slice(this_bvm, string, i, j), '\\');    \
        append_sexpr(captured_token);                       \
    }                                                       \
    else{                                                   \
        parse_error;                                        \
    }                                                       \


#define capture_squote                                       \
    capture_length = j-i;                                   \
    if(capture_length){                                     \
        captured_token = _unescape( this_bvm, _slice(this_bvm, string, i, j), '/');    \
        append_sexpr(captured_token);                       \
    }                                                       \
    else{                                                   \
        parse_error;                                        \
    }                                                       \


#define  adv(x) j++;  goto x; //advance
#define  ret(x) j--;  goto x; //retard
#define  esc(x) j+=2; goto x; //escape
#define advi(x) i=j; j++; goto x; //advance, set i
#define parse_error return nil;

#define chkdone    if(!(j<length)){goto done;}

mword *_pre_sexpr(bvm_cache *this_bvm, mword *string, mword *index){


    mword i,j;
    i=j=*index;

    mword length = size(string);

    if(!length){ goto done; }

//    mword SEXPR_LIST_SYMBOL[] = { 0x10, 0x6c, 0x69, 0x73, 0x74 };
//    mword SEXPR_CODE_SYMBOL[] = { 0x10, 0x63, 0x6f, 0x64, 0x65 };

//// file.c
//char* myArray[] = { "str1", "str2", ... "str100" };
//const size_t sizeof_myArray = sizeof myArray;

null_context:
//trace;
    chkdone;
    switch(string[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: adv(null_context);
        case '{' : 
                *index = j+1;
                return _sexpr(this_bvm, string, index, SEXPR_CODE_SYMBOL);
        case '(' : 
                *index = j+1;
                return _sexpr(this_bvm, string, index, SEXPR_LIST_SYMBOL);
                //return _sexpr(this_bvm, string, index, SEXPR_LIST_SYMBOL);
        case '[' : 
                *index = j+1;
                return _sexpr(this_bvm, string, index, nil);
        case '-' : adv(comment_required);
        default  : parse_error;
    }


comment_required:
//trace;
    chkdone;
    switch(string[j]){
        case '-': adv(comment);
        default : parse_error;
    }


comment:
//trace;
    chkdone;
    switch(string[j]){
        case 0x0d: 
        case 0x0a: adv(null_context);
        default  : adv(comment);
    }    

done:
    return nil;

}

//
//
mword *_sexpr(bvm_cache *this_bvm, mword *string, mword *index, mword *sexpr_type){

    mword i,j;
    i=j=*index;

    mword length = size(string);

    if(!length){ goto done; }

    mword *curr_sexpr     = nil;
    mword *captured_token = nil;
    mword capture_length  = 0;

    if(!is_nil(sexpr_type)){
        append_sexpr(sexpr_type);
    }

list_context:
//trace;
    chkdone;
    switch(string[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: adv(list_context);
        case '{' : 
                *index = ++j;
                append_sexpr(_sexpr(this_bvm, string, index, SEXPR_CODE_SYMBOL));
                j = *index;
                goto list_context;
        case '(' : 
                *index = ++j;
                append_sexpr(_sexpr(this_bvm, string, index, SEXPR_LIST_SYMBOL));
                j = *index;
                goto list_context;
        case '[' : 
                *index = ++j;
                append_sexpr(_sexpr(this_bvm, string, index, nil));
                j = *index;
                goto list_context;
        case '-' : adv(comment_or_token);
        case '}' : 
        case ')' : 
        case ']' : adv(done);
        default  : goto token;
    }


token:
//trace;
    switch(string[j]){
        case '"' : advi(dquote);
        case '\'': advi(squote);
        default  : advi(non_quote);
    }


dquote:
//trace;
    chkdone;
    switch(string[j]){
        case '"' : j++;
                   capture_dquote;
                   goto list_context;
        case '\\': esc(dquote);
        default  : adv(dquote);
    }


squote:        
//trace;
    chkdone;
    switch(string[j]){
        case '\'': j++;
                   capture_squote;
                   goto list_context;
        case '/' : esc(squote);
        default  : adv(squote);
    }


non_quote:
//trace;
    chkdone;
    switch(string[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: 
        case '{' :
        case '}' :
        case '(' :
        case ')' :
        case '[' :
        case ']' :
            capture_token;
            goto list_context;
        case '\\': esc(non_quote);
        default  : adv(non_quote);
    }


comment_or_token:
//trace;
    chkdone;
    switch(string[j]){
        case '-': adv(comment);
        default : ret(token);
    }


comment:
//trace;
    chkdone;
    switch(string[j]){
        case 0x0d: 
        case 0x0a: adv(list_context);
        default  : adv(comment);
    }


done:
//trace;
    *index = j;

    return curr_sexpr;

}


// input string: array-8 string ... INCLUDING THE QUOTES
// returns: standard Babel-string
//
mword *_unescape(bvm_cache *this_bvm, mword *string, mword escape_char){

    int i,j;
    mword *temp_string;
    mword *final_string;
    mword string_length = size(string);

    if(!string_length){ return _newlfi(this_bvm, 1, 0); } //return the empty string

    temp_string = _newlfi(this_bvm, string_length, 0);

    for(i=0,j=0;i<string_length;i++,j++){

        if(c(string,i) == escape_char){
            mword character = c(string,i+1);
            if(character == 'n'){
                c(temp_string,j) = 0x0a;
                i++;
            }
            else if(character == 'r'){
                c(temp_string,j) = 0x0d;
                i++;
            }
            else if(character == 't'){
                c(temp_string,j) = 0x09;
                i++;
            }
            else if(character == '0'){

                #define ASCII_CODE_CHAR_LENGTH 2
                mword *ascii_code = _newlfi(this_bvm, ASCII_CODE_CHAR_LENGTH, 0);

                c(ascii_code,0) = c(string,i+2);
                c(ascii_code,1) = c(string,i+3);

                mword *ascii_value = _radix2cu(this_bvm, _ar2str(this_bvm, ascii_code), 16);

                c(temp_string,j) = c(ascii_value,0);

                i+=3;

            }
            else if(character == 'u'){

                #define UNICODE_CHAR_LENGTH 4
                mword *unicode = _newlfi(this_bvm, UNICODE_CHAR_LENGTH, 0);

                c(unicode,0) = c(string,i+2);
                c(unicode,1) = c(string,i+3);
                c(unicode,2) = c(string,i+4);
                c(unicode,3) = c(string,i+5);

                mword *unicode_value = _radix2cu(this_bvm, _ar2str(this_bvm, unicode), 16);

                c(temp_string,j) = c(unicode_value,0);

                i+=5;

            }
            else{
                c(temp_string,j) = character;
                i++;
            }

        }
        else{ //Not an escape sequence

            c(temp_string,j) = c(string,i);

        }

    }

    if(j != string_length){
        final_string = _slice(this_bvm, temp_string, 0, j);
    }
    else{
        final_string = temp_string;
    }

    return final_string;

}


// Clayton Bauman 2011

