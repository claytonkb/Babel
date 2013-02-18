// array.c
//

#include "babel.h"
#include "array.h"
#include "list.h"
#include "stack.h"
#include "bstruct.h"
#include "bvm_opcodes.h"
#include "except.h"
#include "bvm.h"
#include "load.h"

//
// babel_operator
bvm_cache *sfield(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;
    
    *result = s(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *arlen(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;
    
    *result = size(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *islf(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;
    
    *result = is_leaf(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *isinte(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;
    
    *result = is_inte(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    

    return this_bvm;

}

// _newlf
// New Leaf-array
//
mword *_newlf(mword size){

    mword *ptr = malloc( MWORDS(size+1) );
    if(ptr == NULL){
        error("_newlf: malloc returned NULL");
    }

    ptr[0] = size * MWORD_SIZE;

    return ptr+1;

}

// _newlfi
// New Leaf-array with Initialization
//
mword *_newlfi(mword size, mword init){

    mword *ptr = malloc( MWORDS(size+1) );
    if(ptr == NULL){
        error("_newlfi: malloc returned NULL");
    }

    memset((char*)ptr,init,MWORDS(size+1));
    ptr[0] = size * MWORD_SIZE;

    return ptr+1;

}


// indexes a leaf-array and creates a leaf-array to hold its result
//
inline mword* val(mword *leaf, mword index){ // val#

    mword *temp = new_atom;
    c(temp,0) = c(leaf,index);
    return temp;

} //val


// Accepts a data value and returns a leaf-array
// of size 1 containing that data value
//
mword *_newva(mword value){

    mword *ptr = malloc( MWORDS(1) );
    if(ptr == NULL){
        error("_newva: malloc returned NULL");
    }

    ptr[0] = MWORD_SIZE;
    ptr[1] = value;

    return ptr+1;

}


//
mword *_newin(mword size){

    mword *ptr = malloc( MWORDS(size+1) );
    if(ptr == NULL){
        error("_newin: malloc returned NULL");
    }

    ptr[0] = -1 * size * MWORD_SIZE;

    int i;
    for(i = 1; i<=size; i++){ // All pointers must be valid - initialize to nil
        ptr[i] = (mword)nil;
    }

    return ptr+1;

}

//
mword *_newtlist(void){ // _newtlist# DEPRECATE

    mword *ptr = malloc( MWORDS(4 + HASH_SIZE) ); // 4 = s-field + s-field + car + cdr

    if(ptr == NULL){
        error("_newtlist: malloc returned NULL");
    }

    //FIXME: 32-bit specific and UGLY
    ptr[0] = 0;
    ptr[1] = 0xdeadbeef;
    ptr[2] = 0xdeadbeef;
    ptr[3] = 0xdeadbeef;
    ptr[4] = 0xdeadbeef;
    ptr[5] = (mword)(-2*MWORD_SIZE);
    ptr[6] = (mword)nil;
    ptr[7] = (mword)nil;

    return ptr+1;

}


// utility function used for rstack
mword *_mkin(mword *entries, mword size){

    mword *arr = _newin(size);

    int i;
    for(i=0;i<size;i++){
        c(arr,i) = entries[i];
    }

    return arr;

}

// XXX DEPRECATED:
//// Does the same thing as _newin but doesn't initialize the array entries
//// to nil.
//inline mword *_newin_blank(mword size){
//
//    mword *ptr = malloc( MWORDS(size+1) );
//    if(ptr == NULL){
//        error("_newin: malloc returned NULL");
//    }
//
//    ptr[0] = -1 * size * MWORD_SIZE;
//
////    int i;
////    for(i = 1; i<=size; i++){ // All pointers must be valid - initialize to nil
////        ptr[i] = nil;
////    }
//
//    return ptr+1;
//
//}

// TOS_0 to
// TOS_1 from
// TOS_2 operand
// babel_operator
bvm_cache *slice(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result;

    if(car(TOS_0(this_bvm))<=car(TOS_1(this_bvm))){
        result = nil;
    }
    else{
        if(is_leaf(TOS_2(this_bvm))){
            result = _newlf(car(TOS_0(this_bvm))-car(TOS_1(this_bvm)));
        }
        else{
            result = _newin(car(TOS_0(this_bvm))-car(TOS_1(this_bvm)));
        }
    }

    mword i,j;
    for(    i=(mword)car(TOS_1(this_bvm)), j=0;
            i<(mword)car(TOS_0(this_bvm));
            i++,j++
        ){

        c(result,j) = c(TOS_2(this_bvm),i);

    }    

    zap(this_bvm);
    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends on inputs

    return this_bvm;

}

//
// babel_operator
bvm_cache *cxr(bvm_cache *this_bvm){

//    if(is_leaf(array)){
//        temp = new_atom;
//        c(temp,0) = c((mword*)TOS_1(this_bvm), car(TOS_0(this_bvm)));
//    }
//    else{
//        temp = (mword*)c((mword*)TOS_1(this_bvm), car(TOS_0(this_bvm)));
//    }

    mword *result = new_atom;
    (mword*)*result = _cxr(TOS_1(this_bvm), car(TOS_0(this_bvm)));

    hard_zap(this_bvm);
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: May depend on input

    return this_bvm;

}

//
mword *_cxr(mword *array, mword offset){

    mword *temp;

    if(is_leaf(array)){
        temp = new_atom;
        c(temp,0) = c(array, offset);
    }
    else{
        temp = (mword*)c(array, offset);
    }

    return temp;

}

//
//mword _cxr8(mword *val, mword bit){
//}

//
mword _cxr1(mword *val, mword bit){
    mword mword_select = bit/(MWORD_BIT_SIZE);
    mword bit_offset = bit % MWORD_BIT_SIZE;
    if(mword_select > size(val)-1) error("_cxr1 error");
    return (c(val,mword_select) & (1<<bit_offset)) >> bit_offset;
}

// XXX DEPRECATED
////w
////
//void w(void){
//
//    // TOS_0 offset
//    // TOS_1 dest_operand
//    // TOS_2 src_operand
//
////    //FIXME: There is no checking...
////
////    c((mword*)TOS_1,car(TOS_0)) = (mword)car(TOS_2);
////
////    zap();
////    zap();
////    zap();
//
//    mword offset    = (mword)car(TOS_0);
//    mword dest_size = size((mword*)TOS_1)-offset;
//    mword src_size  = size((mword*)TOS_2);
//
//    mword iter = (src_size < dest_size) ? src_size : dest_size;
//
//    mword i;
//    for(i=0;i<iter;i++){
//        c((mword*)TOS_1,i+offset) = c((mword*)TOS_2,i);
//    }    
//
//    zap();
////    zap();
//    swap();
//    zap();
//    //TODO: leave dest on the stack?
//
//}
//
//// save
//// A kind of "staggered-w"
//void save(void){
//
//    // TOS_0 offset
//    // TOS_1 dest_operand
//    // TOS_2 src_operand
//
////    //FIXME: There is no checking...
////
////    c((mword*)TOS_1,car(TOS_0)) = (mword)car(TOS_2);
////
////    zap();
////    zap();
////    zap();
//
//    mword offset    = (mword)car(TOS_0);
//    mword dest_size = size((mword*)car(TOS_1))-offset;
//    mword src_size  = size((mword*)TOS_2);
//
//    mword iter = (src_size < dest_size) ? src_size : dest_size;
//
//    mword i;
//    for(i=0;i<iter;i++){
//        c((mword*)car(TOS_1),i+offset) = c((mword*)TOS_2,i);
//    }    
//
//    zap();
////    zap();
//    swap();
//    zap();
//    //TODO: leave dest on the stack?
//
//}
//
//// Combination of the old w() and save() operators
//void paste(void){
//
////    mword offset    = car(TOS_0);
////    mword dest_size = size((mword*)TOS_1)-offset;
////    mword src_size  = size((mword*)TOS_2);
//
//    mword offset;//    = car(TOS_0);
//    mword dest_size;// = size((mword*)TOS_1)-offset;
//    mword src_size;//  = size((mword*)TOS_2);
//    mword i;
//    mword iter;
//
//    if(is_leaf((mword*)TOS_1) == is_leaf((mword*)TOS_2)){ //Both src and dest are leaves or both are inte's
//        offset    = (mword)car(TOS_0);
//        dest_size = size((mword*)TOS_1)-offset;
//        src_size  = size((mword*)TOS_2);
//
//        iter = (src_size < dest_size) ? src_size : dest_size;
//
//        for(i=0;i<iter;i++){
//            c((mword*)TOS_1,i+offset) = c((mword*)car(cdr(cdr(stack_ptr))),i);
//        }    
//
//    }
//    else{
//        //FIXME: Detect is_leaf(src) && is_inte(dest) and throw exception!
//        //FIXME: Detect !is_leaf(dest) and throw exception!
//        offset    = (mword)car(TOS_0);
//        dest_size = size((mword*)car(TOS_1))-offset;
//        src_size  = size((mword*)TOS_2);
//
//        iter = (src_size < dest_size) ? src_size : dest_size;
//
//        for(i=0;i<iter;i++){
//            c((mword*)car(TOS_1),i+offset) = c((mword*)TOS_2,i);
//        }
//    }
//
//    zap();
//    //    zap();
//    swap();
//    zap();
//
//}

//FIXME: I think this operator is broken
// babel_operator
bvm_cache *cut(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result_pre;
    mword *result_post;
    mword i;

    mword cut_point = (mword)car(TOS_0(this_bvm));
    mword *src      = TOS_1(this_bvm);

    hard_zap(this_bvm);

    if(cut_point == 0){
//        result = new_atom;
//        *result = nil;
        push_alloc(this_bvm, nil, IMMORTAL); //FIXME: depends on inputs
        swap(this_bvm);
    }
    else if(cut_point >= size(src)){
        push_alloc(this_bvm, nil, IMMORTAL); //FIXME: depends on inputs
    }
    else{
        hard_zap(this_bvm);
        if(is_leaf(src)){
            result_pre  = _newlf(cut_point);
            result_post = _newlf(size(src)-cut_point);
        }
        else{
            result_pre = _newin(cut_point);
            result_post = _newin(size(src)-cut_point);
        }

        mword num_entries = size(src);

        //TODO: memcpy!
        for(    i=0;
                i<num_entries;
                i++
            ){

            if(i<cut_point){
                c(result_pre,i) = c(src,i);
            }
            else{
                c(result_post,i-cut_point) = c(src,i);
            }

        }    

        push_alloc(this_bvm, result_pre,  IMMORTAL); //FIXME: depends on inputs
        push_alloc(this_bvm, result_post, IMMORTAL); //FIXME: depends on inputs
    }

    return this_bvm;

}

//
// babel_operator
bvm_cache *arlen8(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = _arlen8(TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;
 
}

// 
mword _arlen8(mword *string){

    mword strsize = size(string) - 1;
    mword last_mword = c(string, strsize);
    mword alignment = dec_alignment_word8(last_mword);

    if(last_mword){
        return  (strsize * MWORD_SIZE) - (MWORD_SIZE - alignment);
    }
    else{
        return (strsize * MWORD_SIZE);
    }

}

//
// babel_operator
bvm_cache *newin(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _newin((mword)car(TOS_0(this_bvm))); //FIXME: There is no checking...

    zap(this_bvm);
    push_alloc(this_bvm, result, NEWIN);

    return this_bvm;

}

//
// babel_operator
bvm_cache *newlf(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _newlf((mword)car(TOS_0(this_bvm))); //FIXME: There is no checking...

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *trunc(bvm_cache *this_bvm){

    fatal("stack fix not done");
    if((mword)car(TOS_0(this_bvm)) > size(TOS_1(this_bvm))){
        error("trunc: cannot truncate to larger size");
    }

    _trunc(TOS_1(this_bvm), (mword)car(TOS_0(this_bvm)));

    zap(this_bvm);

    return this_bvm;

}

//
void _trunc(mword *operand, mword new_size){

    if(is_leaf(operand)){
        s(operand) = (new_size*MWORD_SIZE);
    }
    else if(is_inte(operand)){ //is_inte
        s(operand) = (int)-1*(new_size*MWORD_SIZE);
    }
    //hash-refs can't be trunc'd

}

////FIXME: The array-8 alignment word is completely broken and needs to be
////fixed. In the current form, converting from a Babel-string to C-style
////string requires a lot of work but if the alignment word def'n is
////changed, this conversion would be unnecessary:
////
////align0 : 0x00000000
////align1 : 0x000000ff
////align2 : 0x0000ffff
////align3 : 0x00ffffff
////
////All Babel-strings would automatically be null-terminated. Changing the
////Perl-script and the below functions results in bugs. Not sure what is
////going wrong.

//Returns an alignment word based on size8
//
mword alignment_word8(mword size8){

    if(size8 % MWORD_SIZE == 0)
        return 0;

    mword alignment = (mword)-1;

    return alignment << ((size8 % MWORD_SIZE) * BITS_PER_BYTE);

}


//// Decodes the alignment word
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
// babel_operator
bvm_cache *arcat(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result;

    if      ( is_leaf(TOS_0(this_bvm))  &&  is_leaf(TOS_1(this_bvm)) ){
        result = _newlf( size(TOS_0(this_bvm)) + size(TOS_1(this_bvm)) );
    }
    else if ( is_inte(TOS_0(this_bvm))  &&  is_inte(TOS_1(this_bvm)) ){
        result = _newin( size(TOS_0(this_bvm)) + size(TOS_1(this_bvm)) );
    }
    else{ //Throw an errorion
        error("arcat: cannot concatenate leaf array and interior array");
    }

    mword i,j;
    for(    i=0;
            i<size(TOS_1(this_bvm));
            i++
        ){

        c((mword*)result,i) = c(TOS_1(this_bvm),i);

    }    

    for(    i=0,j=size(TOS_1(this_bvm));
            i<size(TOS_0(this_bvm));
            i++,j++
        ){

        c(result,j) = c(TOS_0(this_bvm),i);

    }    

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends on inputs

    return this_bvm;

}

mword array8_size(mword size8){

    mword size = size8 / MWORD_SIZE;

    if(size8 % MWORD_SIZE != 0){ //XXX Assumes the int div rounds to floor
        size++;
    }

    size++; //for the alignment_word

}

// TOS_1(this_bvm) . TOS_0(this_bvm)
//
// babel_operator
bvm_cache *arcat8(bvm_cache *this_bvm){

    fatal("stack fix not done");
    char *result;
    mword size8;

    if      ( is_leaf(TOS_0(this_bvm))  &&  is_leaf(TOS_1(this_bvm)) ){
        size8 = _arlen8(TOS_0(this_bvm)) + _arlen8(TOS_1(this_bvm));
        result = (char*)_newlf( array8_size(size8) );
    }
    else{ //Throw an errorion
        error("arcat8: cannot concatenate non-leaf arrays");
    }

    mword i,j;
    for(    i=0;
            i<_arlen8(TOS_1(this_bvm));
            i++
        ){

        result[i] = *((char*)TOS_1(this_bvm)+i);

    }

    for(    i=0,j=_arlen8(TOS_1(this_bvm));
            i<_arlen8(TOS_0(this_bvm));
            i++,j++
        ){

        result[j] = *((char*)TOS_0(this_bvm)+i);

    }

    c((mword*)result,array8_size(size8)-1) = alignment_word8(size8);

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, (mword*)result, IMMORTAL); //FIXME: Depends on inputs

    return this_bvm;

}


// TOS_0(this_bvm) to
// TOS_1(this_bvm) from
// TOS_2(this_bvm) operand
// babel_operator
bvm_cache *slice8(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword size8 = car(TOS_0(this_bvm))-car(TOS_1(this_bvm));

    char *result;
    if(is_leaf(TOS_2(this_bvm))){
        result = (char*)_newlf(array8_size(size8));
    }
    else{
        error("slice8: cannot slice8 a non-leaf array");
    }

    mword i,j;
    for(    i=(mword)car(TOS_1(this_bvm)), j=0;
            i<(mword)car(TOS_0(this_bvm));
            i++,j++
        ){

        result[j] = ((char*)TOS_2(this_bvm))[i];

    }    

    c((mword*)result,array8_size(size8)-1) = alignment_word8(size8);

    zap(this_bvm);
    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, (mword*)result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *arcmp(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    mword *left  = TOS_1(this_bvm);
    mword *right = TOS_0(this_bvm);

    if(is_leaf(left) && is_leaf(right)){
        *result = (mword)_arcmp(left, right);
    }
    else if(is_inte(left) && is_inte(right)){
        mword *unloaded_left  = _unload(left);
        mword *unloaded_right = _unload(right);
        *result = (mword)_arcmp(unloaded_left, unloaded_right);
    }
    else{
        error("Non-matching operands");
    }

    zap(this_bvm);
    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
int _arcmp(mword *left, mword *right){

    if(size(left) > size(right)){
        return 1;
    }
    else if(size(left) < size(right)){
        return -1;
    }

    return memcmp(left, right, size(left)*MWORD_SIZE);

}

//
// babel_operator
bvm_cache *ar2ls(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _ar2ls(TOS_0(this_bvm));

    zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends on inputs

    return this_bvm;

}

//
mword *_ar2ls(mword *arr){

    mword *last_cons = (mword*)nil;
    int i;
    mword *entry;

    if(is_inte(arr)){
        for(i=size(arr)-1;i>=0;i--){
            last_cons = _consls((mword*)c(arr,i),last_cons);
        }
    }
    else{
        for(i=size(arr)-1;i>=0;i--){
            entry = _newlf(1);
            *entry = c(arr,i);
            last_cons = _consls(entry,last_cons);
        }
    }

    return last_cons;

}

//TOS_0 perm_matrix
//TOS_1 src
// babel_operator
bvm_cache *perm(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result;

    if(is_inte(TOS_1(this_bvm))){
        result = _newin(size(TOS_1(this_bvm)));
    }
    else if(is_leaf(TOS_1(this_bvm))){
        result = _newlf(size(TOS_1(this_bvm)));
    }
    else{
        error("perm: !is_leaf && !is_inte");
        return;
    }

    _perm(TOS_1(this_bvm), result, TOS_0(this_bvm));

    zap(this_bvm);
    zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends on input

    return this_bvm;

}

//
void _perm(mword *src, mword *dest, mword *perm_matrix){

    mword array_size = size(src);

    if(array_size != size(dest) || array_size != size(perm_matrix)){
        error("_perm: array_size != size(dest) || array_size != size(perm_matrix)");
        return;
    }

    int i;
    for(i=0; i<array_size; i++){
        c((mword*)dest,i) = c((mword*)src, c((mword*)perm_matrix,i) % array_size );
    }

}

// Clayton Bauman 2011

