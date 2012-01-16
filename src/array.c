// XXX STACK FIX DONE
// array.c
//

#include "babel.h"
#include "array.h"
#include "list.h"
#include "stack.h"
#include "count.h"
#include "bvm_opcodes.h"
#include "except.h"

void sfield(void){

    mword *result    = new_atom();
    
    *result = s((mword*)TOS_0);

    zap();
    push_alloc(result, SFIELD);

}

void arlen(void){

    mword *result    = new_atom();
    
    *result = size((mword*)TOS_0);

    zap();
    push_alloc(result, ARLEN);

}

void islf(void){

    mword *result    = new_atom();
    
    *result = is_leaf((mword*)TOS_0);

    zap();
    push_alloc(result, ISLF);

}

void isinte(void){

    mword *result    = new_atom();
    
    *result = is_inte((mword*)TOS_0);

    zap();
    push_alloc(result, ISINTE);

}

//
//
inline mword *_newlf(mword size){

    mword *ptr = malloc( MWORDS(size+1) );
    if(ptr == NULL){
        except("_newlf: malloc returned NULL", __FILE__, __LINE__);
    }

    ptr[0] = size * MWORD_SIZE;
    return ptr+1;

}

//
//
inline mword *_newin(mword size){

    mword *ptr = malloc( MWORDS(size+1) );
    if(ptr == NULL){
        except("_newin: malloc returned NULL", __FILE__, __LINE__);
    }

    ptr[0] = -1 * size * MWORD_SIZE;

    int i;
    for(i = 1; i<=size; i++){ // All pointers must be valid - initialize to nil
        ptr[i] = nil;
    }

    return ptr+1;

}

// Does the same thing as _newin but doesn't initialize the array entries
// to nil.
inline mword *_newin_blank(mword size){

    mword *ptr = malloc( MWORDS(size+1) );
    if(ptr == NULL){
        except("_newin: malloc returned NULL", __FILE__, __LINE__);
    }

    ptr[0] = -1 * size * MWORD_SIZE;

//    int i;
//    for(i = 1; i<=size; i++){ // All pointers must be valid - initialize to nil
//        ptr[i] = nil;
//    }

    return ptr+1;

}

// TOS_0 to
// TOS_1 from
// TOS_2 operand
//
void cindex(void){

    mword *result;
    if(is_leaf((mword*)TOS_2)){
        result = _newlf(car(TOS_0)-car(TOS_1)+1);
    }
    else{
        result = _newin(car(TOS_0)-car(TOS_1)+1);
    }

    mword i,j;
    for(    i=car(TOS_1), j=0;
            i<=car(TOS_0);
            i++,j++
        ){

        c(result,j) = c((mword*)TOS_2,i);

    }    

    zap();
    zap();
    zap();
    push_alloc(result, SLICE);

}

void cxr(void){

//    mword *car_TOS_0 = (mword*)car(TOS_0);
    mword *temp = (mword*)c((mword*)TOS_1, car(TOS_0));

    zap();
    zap();
    push_alloc(temp, CXR);

}

//w
//
void w(void){

    // TOS_0 offset
    // TOS_1 dest_operand
    // TOS_2 src_operand

//    //FIXME: There is no checking...
//
//    c((mword*)TOS_1,car(TOS_0)) = (mword)car(TOS_2);
//
//    zap();
//    zap();
//    zap();

    mword offset    = car(TOS_0);
    mword dest_size = size((mword*)TOS_1)-offset;
    mword src_size  = size((mword*)TOS_2);

    mword iter = (src_size < dest_size) ? src_size : dest_size;

    mword i;
    for(i=0;i<iter;i++){
        c((mword*)TOS_1,i+offset) = c((mword*)TOS_2,i);
    }    

    zap();
//    zap();
    swap();
    zap();
    //TODO: leave dest on the stack?

}

// save
// A kind of "staggered-w"
void save(void){

    // TOS_0 offset
    // TOS_1 dest_operand
    // TOS_2 src_operand

//    //FIXME: There is no checking...
//
//    c((mword*)TOS_1,car(TOS_0)) = (mword)car(TOS_2);
//
//    zap();
//    zap();
//    zap();

    mword offset    = car(TOS_0);
    mword dest_size = size((mword*)car(TOS_1))-offset;
    mword src_size  = size((mword*)TOS_2);

    mword iter = (src_size < dest_size) ? src_size : dest_size;

    mword i;
    for(i=0;i<iter;i++){
        c((mword*)car(TOS_1),i+offset) = c((mword*)TOS_2,i);
    }    

    zap();
//    zap();
    swap();
    zap();
    //TODO: leave dest on the stack?

}

// Combination of the old w() and save() operators
void paste(void){

//    mword offset    = car(TOS_0);
//    mword dest_size = size((mword*)TOS_1)-offset;
//    mword src_size  = size((mword*)TOS_2);

    mword offset;//    = car(TOS_0);
    mword dest_size;// = size((mword*)TOS_1)-offset;
    mword src_size;//  = size((mword*)TOS_2);
    mword i;
    mword iter;

    if(is_leaf((mword*)TOS_1) == is_leaf((mword*)TOS_2)){ //Both src and dest are leaves or both are inte's
        offset    = car(TOS_0);
        dest_size = size((mword*)TOS_1)-offset;
        src_size  = size((mword*)TOS_2);

        iter = (src_size < dest_size) ? src_size : dest_size;

        for(i=0;i<iter;i++){
            c((mword*)TOS_1,i+offset) = c((mword*)TOS_2,i);
        }    

    }
    else{
        //FIXME: Detect is_leaf(src) && is_inte(dest) and throw exception!
        //FIXME: Detect !is_leaf(dest) and throw exception!
        offset    = car(TOS_0);
        dest_size = size((mword*)car(TOS_1))-offset;
        src_size  = size((mword*)TOS_2);

        iter = (src_size < dest_size) ? src_size : dest_size;

        for(i=0;i<iter;i++){
            c((mword*)car(TOS_1),i+offset) = c((mword*)TOS_2,i);
        }
    }

    zap();
    //    zap();
    swap();
    zap();

}


void cut(void){

    mword *result_pre;
    mword *result_post;
    mword i;

    mword cut_point = car(TOS_0);
    mword *src      = (mword*)TOS_1;

    zap();
    zap();

    if(cut_point == 0){
//        result = new_atom();
//        *result = nil;
        push_alloc((mword*)nil, CUT);
        swap();
    }
    else if(cut_point >= size(src)){
        push_alloc((mword*)nil, CUT);
    }
    else{
        if(is_leaf(src)){
            result_pre  = _newlf(cut_point);
            result_post = _newlf(size(src)-cut_point);
        }
        else{
            result_pre = _newin(cut_point);
            result_post = _newin(size(src)-cut_point);
        }    

        //TODO: memcpy!
        for(    i=0;
                i<size(src);
                i++
            ){

            if(i<cut_point){
                c(result_pre,i) = c(src,i);
            }
            else{
                c(result_post,i-cut_point) = c(src,i);
            }

        }    

        push_alloc(result_pre,  CUT);
        push_alloc(result_post, CUT);
    }

}

void arlen8(void){

    mword *result    = new_atom();

    *result = _arlen8((mword*)TOS_0);

    zap();
    push_alloc(result, ARLEN8);
    
}

inline mword _arlen8(mword *string){

    mword strsize = size(string) - 1;
    mword last_mword = c(string, strsize);
    mword alignment = dec_alignment_word8(last_mword);

    if(~last_mword){ //FIXME: 64-bit
        return  (strsize * MWORD_SIZE) - (MWORD_SIZE - alignment);
    }
    else{
        return (strsize * MWORD_SIZE);
    }

}

// cp means DEEP copy as oppposed to w* which are shallow copy operators
void cp(void){



}

// FIXME This function is incorrectly named...
void _cp(mword offset, mword *dest, mword *src){ //FIXME: This function is full of landmines...

    mword dest_size = size((mword*)dest)-offset;
    mword src_size  = size((mword*)src);

    mword iter = src_size < dest_size ? src_size : dest_size;

    mword i;
    for(i=0;i<iter;i++){
        c((mword*)dest,i+offset) = c((mword*)src,i);
    }    

}

void newin(void){

    mword *result = _newin(car(TOS_0)); //FIXME: There is no checking...

    zap();
    push_alloc(result, NEWIN);

}

void newlf(void){

    mword *result = _newlf(car(TOS_0)); //FIXME: There is no checking...

    zap();
    push_alloc(result, NEWLF);

}

//void del(void){
//
////    free((mword*)(TOS_0 - 1)); //FIXME: Extremely dangerous!!!
//    _del((mword*)TOS_0);
//    zap();
//
//}
//
//void _del(mword *mem){
//
//    free(mem-1);
//
//}
//

void trunc(void){

    if(car(TOS_0) > size((mword*)TOS_1)){
        except("trunc: cannot truncate to larger size", __FILE__, __LINE__);
    }

    _trunc((mword*)TOS_1, car(TOS_0));

    zap();

}

// 
//
void _trunc(mword *operand, mword new_size){

//    mword *result    = new_atom();

    if(is_leaf(operand)){
        s(operand) = (new_size*MWORD_SIZE);
    }
    else{ //is_inte
        s(operand) = (int)-1*(new_size*MWORD_SIZE);
    }

//    push_alloc(result, SFIELD);

}

//FIXME: The array-8 alignment word is completely broken and needs to be
//fixed. In the current form, converting from a Babel-string to C-style
//string requires a lot of work but if the alignment word def'n is
//changed, this conversion would be unnecessary:
//
//align0 : 0x00000000
//align1 : 0x000000ff
//align2 : 0x0000ffff
//align3 : 0x00ffffff
//
//All Babel-strings would automatically be null-terminated. Changing the
//Perl-script and the below functions results in bugs. Not sure what is
//going wrong.

//Returns an alignment word based on size8
//
mword alignment_word8(mword size8){

    if(size8 == 0)
        return 0;

    mword alignment = (mword)-1;

    return alignment << ((size8 % MWORD_SIZE) * BITS_PER_BYTE);

}

// Decodes the alignment word
mword dec_alignment_word8(mword alignment_word){

    if(alignment_word == 0){
        return 0;
    }

    alignment_word = ~alignment_word;
    mword alignment = 0;

    while(alignment_word != 0){
        alignment_word = alignment_word >> BITS_PER_BYTE;
        alignment++;
    }

    return alignment;

}

// TOS_1 . TOS_0
//
void arcat(void){

    mword *result;

    if      ( is_leaf((mword*)TOS_0)  &&  is_leaf((mword*)TOS_1) ){
        result = _newlf( size((mword*)TOS_0) + size((mword*)TOS_1) );
    }
    else if ( is_inte((mword*)TOS_0)  &&  is_inte((mword*)TOS_1) ){
        result = _newin( size((mword*)TOS_0) + size((mword*)TOS_1) );
    }
    else{ //Throw an exception
        except("arcat: cannot concatenate leaf array and interior array", __FILE__, __LINE__);
    }

    mword i,j;
    for(    i=0;
            i<size((mword*)TOS_1);
            i++
        ){

        c(result,i) = c((mword*)TOS_1,i);

    }    

    for(    i=0,j=size((mword*)TOS_1);
            i<size((mword*)TOS_0);
            i++,j++
        ){

        c(result,j) = c((mword*)TOS_0,i);

    }    

    zap();
    zap();
    push_alloc(result, ARCAT);

}

void arcmp(void){

    mword *result    = new_atom();

    *result = (mword)_arcmp((mword*)TOS_1, (mword*)TOS_0);

    zap();
    zap();
    push_alloc(result, ARCMP);

}

int _arcmp(mword *left, mword *right){

    if(size(left) > size(right)){
        return 1;
    }
    else if(size(left) < size(right)){
        return -1;
    }

    return memcmp(left, right, size(left)*MWORD_SIZE);

}

// Clayton Bauman 2011

