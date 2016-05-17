// sexpr.c
//

#include "babel.h"
#include "sexpr.h"
#include "string.h"
#include "array.h"
#include "list.h"
#include "stack.h"
#include "utf8.h"
#include "bstruct.h"
#include "tptr.h"
#include "mem.h"
#include "introspect.h"

#define append_sexpr(x) \
    if(is_nil(curr_sexpr)){         \
        curr_sexpr = _cons(this_bvm, x, nil);\
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


#define capture_dquote                                      \
    capture_length = j-i;                                   \
    if(capture_length){                                     \
        captured_token = _unescape( this_bvm, _slice(this_bvm, string, i, j), '\\');    \
        append_sexpr(captured_token);                       \
    }                                                       \
    else{                                                   \
        parse_error;                                        \
    }                                                       \


#define capture_squote                                      \
    capture_length = j-i;                                   \
    if(capture_length){                                     \
        captured_token = _unescape( this_bvm, _slice(this_bvm, string, i, j), '/'); \
        append_sexpr(captured_token);                       \
    }                                                       \
    else{                                                   \
        parse_error;                                        \
    }                                                       \


#define  adv(x) j++;  goto x;     //advance
#define  ret(x) j--;  goto x;     //retard
#define  esc(x) j+=2; goto x;     //escape
#define advi(x) i=j; j++; goto x; //advance, set i
#define parse_error return nil;

#define chkdone    if(!(j<length)){goto done;}


//
//
mword *_pre_sexpr(bvm_cache *this_bvm, mword *string, mword *index){ // _pre_sexpr#

    mword j;
    j=*index;

    mword length = size(string);

    if(!length){ goto done; }

//    mword SEXPR_LIST_SYMBOL[] = { 0x10, 0x6c, 0x69, 0x73, 0x74 };
//    mword SEXPR_CODE_SYMBOL[] = { 0x10, 0x63, 0x6f, 0x64, 0x65 };

//// file.c
//char* myArray[] = { "str1", "str2", ... "str100" };
//const size_t sizeof_myArray = sizeof myArray;

null_context:
//_trace;
    chkdone;
    switch(string[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: adv(null_context);
        case '{' : 
                *index = j+1;
// XXX XXX Was copying entire _sexpr... probably unnecessary XXX XXX
//                return _cp( this_bvm, _sexpr(this_bvm, string, index, SEXPR_CODE_SYMBOL) );
                return _sexpr(this_bvm, string, index, _cp(this_bvm, SEXPR_CODE_SYMBOL));
        case '(' : 
                *index = j+1;
                return _sexpr(this_bvm, string, index, _cp(this_bvm, SEXPR_LIST_SYMBOL) );
//                return _cp( this_bvm, _sexpr(this_bvm, string, index, SEXPR_LIST_SYMBOL) );
                //return _sexpr(this_bvm, string, index, SEXPR_LIST_SYMBOL);
        case '[' : 
                *index = j+1;
                return _sexpr(this_bvm, string, index, nil);
//                return _cp( this_bvm, _sexpr(this_bvm, string, index, nil) );
        case '-' : adv(comment_required);
        default  : parse_error;
    }


comment_required:
//_trace;
    chkdone;
    switch(string[j]){
        case '-': adv(comment);
        default : parse_error;
    }


comment:
//_trace;
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
mword *_sexpr(bvm_cache *this_bvm, mword *string, mword *index, mword *sexpr_type){ // _sexpr#

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
//_trace;
    chkdone;
    switch(string[j]){
        case ' ' :
        case 0x0a:
        case 0x0d: adv(list_context);
        case '{' : 
                *index = ++j;
                append_sexpr(_sexpr(this_bvm, string, index, _cp(this_bvm, SEXPR_CODE_SYMBOL)));
//                append_sexpr(_cp( this_bvm, _sexpr(this_bvm, string, index, SEXPR_CODE_SYMBOL)));
                j = *index;
                goto list_context;
        case '(' : 
                *index = ++j;
                append_sexpr(_sexpr(this_bvm, string, index, _cp(this_bvm, SEXPR_LIST_SYMBOL)));
//                append_sexpr(_cp( this_bvm, _sexpr(this_bvm, string, index, SEXPR_LIST_SYMBOL)));
                j = *index;
                goto list_context;
        case '[' :
                // add a state to check for nil braces []
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
//_trace;
    switch(string[j]){
        case '"' : advi(dquote);
        case '\'': advi(squote);
        default  : advi(non_quote);
    }


dquote:
//_trace;
    chkdone;
    switch(string[j]){
        case '"' : j++;
                   capture_dquote;
                   goto list_context;
        case '\\': esc(dquote);
        default  : adv(dquote);
    }


squote:        
//_trace;
    chkdone;
    switch(string[j]){
        case '\'': j++;
                   capture_squote;
                   goto list_context;
        case '/' : esc(squote);
        default  : adv(squote);
    }


non_quote:
//_trace;
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
//_trace;
    chkdone;
    switch(string[j]){
        case '-': adv(comment);
        default : ret(token);
    }


comment:
//_trace;
    chkdone;
    switch(string[j]){
        case 0x0d: 
        case 0x0a: adv(list_context);
        default  : adv(comment);
    }


done:
//_trace;
    *index = j;

    if((_len(this_bvm, curr_sexpr) == 1) && (_arcmp((mword*)icar(curr_sexpr), SEXPR_LIST_SYMBOL) == 0)){
        curr_sexpr = _ptr(this_bvm, _cp(this_bvm, SEXPR_NIL_SYMBOL));
    }

    return curr_sexpr;

}


// input string: array-8 string ... INCLUDING THE QUOTES
// returns: standard Babel-string
//
mword *_unescape(bvm_cache *this_bvm, mword *string, mword escape_char){ // _unescape#

    int i,j;
    mword *temp_string;
    mword *final_string;
    mword string_length = size(string);

    if(!string_length){ return _newlfi(this_bvm, 1, 0); } //return the empty string

    temp_string = _newlfi(this_bvm, string_length, 0);

    for(i=0,j=0;i<string_length;i++,j++){

        if(rcl(string,i) == escape_char){
            mword character = rcl(string,i+1);
            if(character == 'n'){
                rcl(temp_string,j) = 0x0a;
                i++;
            }
            else if(character == 'r'){
                rcl(temp_string,j) = 0x0d;
                i++;
            }
            else if(character == 't'){
                rcl(temp_string,j) = 0x09;
                i++;
            }
            else if(character == '0'){

                #define ASCII_CODE_CHAR_LENGTH 2
                mword *ascii_code = _newlfi(this_bvm, ASCII_CODE_CHAR_LENGTH, 0);

                lcl(ascii_code,0) = rcl(string,i+2);
                lcl(ascii_code,1) = rcl(string,i+3);

                mword *ascii_value = _radix2cu(this_bvm, _ar2str(this_bvm, ascii_code), 16);

                lcl(temp_string,j) = rcl(ascii_value,0);

                i+=3;

            }
            else if(character == 'u'){

                #define UNICODE_CHAR_LENGTH 4
                mword *unicode = _newlfi(this_bvm, UNICODE_CHAR_LENGTH, 0);

                lcl(unicode,0) = rcl(string,i+2);
                lcl(unicode,1) = rcl(string,i+3);
                lcl(unicode,2) = rcl(string,i+4);
                lcl(unicode,3) = rcl(string,i+5);

                mword *unicode_value = _radix2cu(this_bvm, _ar2str(this_bvm, unicode), 16);

                lcl(temp_string,j) = rcl(unicode_value,0);

                i+=5;

            }
            else{
                lcl(temp_string,j) = character;
                i++;
            }

        }
        else{ //Not an escape sequence

            lcl(temp_string,j) = rcl(string,i);

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


//
//
mword *sexpr_op(bvm_cache *this_bvm, oinfo *oi){ // sexpr_op#

    mword index=0;
    return _pre_sexpr(this_bvm, oi->data, &index);

}


//
//
mword *sexpr_op2(bvm_cache *this_bvm, mword *bstring){ // sexpr_op#

    mword index=0;
    return _pre_sexpr(this_bvm, bstring, &index);

}


/*****************************************************************************
 *                                                                           *
 *                            SEXPR OPERATORS                                *
 *                                                                           *
 ****************************************************************************/


//#define SEXPR_OPERATIONS                                 
//    result0 = sexpr_op(this_bvm, &oi0);
//
//OPERATORA_R1_W1_D(sexpr,
//        SEXPR_OPERATIONS,
//        0, OI_MASK_LEAF, 0, 0)

#define SEXPR_OPERATIONS                                 \
    mword *bstring = _str2ar(this_bvm, oi0.data);        \
    result0 = sexpr_op2(this_bvm, bstring);

OPERATORA_R1_W1_D(sexpr,
        SEXPR_OPERATIONS,
        0, OI_MASK_LEAF, 0, 0)


// Clayton Bauman 2014 

