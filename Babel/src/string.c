// XXX STACK FIX DONE
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

void b2c(void){

    mword *result    = new_atom();

    result = _b2c((mword*)TOS_0);

    zap();
//    push_alloc(result, B2C);

}

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

void c2b(void){

}

mword *_c2b(char *string, mword max_safe_length){

    mword length, char_length, last_mword;

    //strlen,strcpy are unsafe, use memchr instead:
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
void ar2str(void){

    #define MAX_UTF8_CHAR_SIZE 4

    mword arsize = size((mword*)TOS_0);
    int temp_buffer_size = MAX_UTF8_CHAR_SIZE * (arsize);
    char *temp_buffer = malloc( temp_buffer_size );
    //FIXME: Check malloc
    
    //    int u8_toutf8(char *dest, int sz, uint32_t *src, int srcsz)
    mword utf8_length = (mword)u8_toutf8(temp_buffer, temp_buffer_size, (uint32_t *)TOS_0, arsize) - 1;
//    d(utf8_length)
//    d(arsize)

    mword arlength = (utf8_length / 4) + 1;

    if(utf8_length % 4){
        arlength++;
    }

    mword *result = _newlf(arlength);
    memcpy(result, temp_buffer, utf8_length);
    free(temp_buffer);

    c(result,arlength-1) = alignment_word8(utf8_length);

    zap();
    push_alloc(result, AR2STR);

}

void str2ar(void){

    //int u8_strlen(char *s, int max_safe_length)
    mword length8 = _arlen8((mword*)TOS_0);
    mword u8_length = (mword)u8_strlen((char *)TOS_0, length8);

    mword *result = _newlf(u8_length+1);

    //int u8_toucs(u_int32_t *dest, int sz, char *src, int srcsz)
    mword length = u8_toucs((uint32_t *)result, u8_length+1, (char *)TOS_0, length8);
//    d(length)

    _trunc(result, length);

    zap();
    push_alloc(result, STR2AR);

}

void catoi(void){

    mword *cstr = _b2c((mword*)TOS_0);
    mword *result = _newlf(1);
    *result = (mword)atoi((char*)cstr);

    zap();
    push_alloc(result, CATOI);

}

void dec2ci(void){

    mword *cstr = _b2c((mword*)TOS_0);
    mword *result = _newlf(1);
    *result = (mword)atoi((char*)cstr);

    zap();
    push_alloc(result, DEC2CI);

}


void ci2dec(void){

    char buffer[MWORD_BIT_SIZE/2];

    int size = sprintf(buffer, "%d", (int)car(TOS_0));

    mword arlength = (size / 4) + 1;

    if(size % 4){
        arlength++;
    }

    mword *result = _newlf(arlength);

    memcpy(result, buffer, size);
    c(result,arlength-1) = alignment_word8(size);

    zap();
    push_alloc(result, CI2DEC);

}

void cu2dec(void){

    char buffer[MWORD_BIT_SIZE/2];

    int size = sprintf(buffer, "%u", car(TOS_0));

    mword arlength = (size / 4) + 1;

    if(size % 4){
        arlength++;
    }

    mword *result = _newlf(arlength);

    memcpy(result, buffer, size);
    c(result,arlength-1) = alignment_word8(size);

    zap();
    push_alloc(result, CU2DEC);

}

void hex2cu(void){

    mword *cstr = _b2c((mword*)TOS_0);
    unsigned long *result = (unsigned long *)_newlf( sizeof(unsigned long) / sizeof(mword) ); //XXX Hmmmmmmm
    *result = strtoul ((char*)cstr,NULL,16);
//    *result = (mword)atoi((char*)cstr);

    zap();
    push_alloc((mword*)result, HEX2CU);

}

void cu2hex(void){

    char buffer[(MWORD_BIT_SIZE/4) + 1];

    int size = sprintf(buffer, "%x", car(TOS_0));

    mword arlength = (size / 4) + 1;

    if(size % 4){
        arlength++;
    }

    mword *result = _newlf(arlength);

    memcpy(result, buffer, size);
    c(result,arlength-1) = alignment_word8(size);

    zap();
    push_alloc(result, CU2HEX);

}


// Clayton Bauman 2011

