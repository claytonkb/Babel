// array.c
//

#include "babel.h"
#include "array.h"
#include "bstruct.h"
#include "operator.h"
#include "pbp.h"
#include "introspect.h"
#include "string.h"
#include "lib_babel.h"
#include "logic.h"
#include "list.h"


/*****************************************************************************
 *                                                                           *
 *                                ARRAY-8                                    *
 *                                                                           *
 ****************************************************************************/


//
//
mword _arlen8(bvm_cache *this_bvm, mword *string){ // _arlen8#

    mword strsize = size(string) - 1;
    mword last_mword = rcl(string, strsize);
//    mword alignment = _dec_alignment_word8(this_bvm, last_mword);

    if(last_mword){
        return (strsize * MWORD_SIZE) - (MWORD_SIZE - _dec_alignment_word8(this_bvm, last_mword));
    }
    else{
        return (strsize * MWORD_SIZE);
    }

}


// decodes the alignment word
//
mword _dec_alignment_word8(bvm_cache *this_bvm, mword alignment_word){ // dec_alignment_word8#

    if(alignment_word == 0){
        return 0;
    }

    alignment_word = ~alignment_word;
    mword alignment = 0;

    while(alignment_word != 0){ //FIXME: PERF ... really inefficient
        alignment_word = alignment_word >> BITS_PER_BYTE;
        alignment++;
    }

    return alignment;

}


//Returns an alignment word based on size8
//
mword _alignment_word8(bvm_cache *this_bvm, mword size8){ // _alignment_word8#

    if(size8 % MWORD_SIZE == 0)
        return 0;

    mword alignment = (mword)-1;

    return alignment << ((size8 % MWORD_SIZE) * BITS_PER_BYTE);

}


//
//
mword _array8_size(bvm_cache *this_bvm, mword size8){ // _array8_size#

    mword size = (size8 / MWORD_SIZE);

    if(size8 % MWORD_SIZE != 0){ //XXX Assumes that int div rounds to floor
        size++;
    }

    return size+1; //for the alignment_word

}


//
//
mword *_arcat8(bvm_cache *this_bvm, mword *left, mword *right){ // _arcat8#

    char *result;

    mword size_left  = _arlen8(this_bvm, left);
    mword size_right = _arlen8(this_bvm, right);
    mword total_size = size_left + size_right;
    mword array8_total_size = _array8_size(this_bvm, total_size);

    if      ( is_leaf(right)  &&  is_leaf(left) ){
        result = (char*)_newlfi(this_bvm,  array8_total_size, 0 );
    }
    else{ //FIXME: Throw an exception
        _fatal("cannot concatenate interior array");
    }

    //FIXME: PERF use memcpy!
    mword i,j;
    for(    i=0;
            i<size_left;
            i++
        ){

        result[i] = *((char*)left+i);

    }

    for(    i=0,j=size_left;
            i<size_right;
            i++,j++
        ){

        result[j] = *((char*)right+i);

    }

    lcl((mword*)result,array8_total_size-1) = _alignment_word8(this_bvm, total_size);

    return (mword*)result;

}


////
////
//mword *_by2lf(bvm_cache *this_bvm, mword *array8){ // _by2lf#
//
//    unsigned char *cast_array = (unsigned char *)array8;
//    mword arr8_size  = _arlen8(this_bvm, array8);
//    mword *result = _newlfi(this_bvm, arr8_size, 0);
//
//    int i;
//
//    for(i=0; i<arr8_size; i++){
//
//        result[i] = (mword)cast_array[i];
//
//    }
//
//    return result;
//
//}
//


// FIXME: DOES NOT HANDLE UNICODE PROPERLY
//
mword *_lf2by(bvm_cache *this_bvm, mword *array){ // _lf2by#

    mword arr_size  = size(array);
    char *result = (char*)_newstr(this_bvm, arr_size, ' ');

    int i;

    for(i=0; i<arr_size; i++){
        result[i] = (char)(array[i] & 0xff);
    }

    return (mword*)result;

}


//
//
mword *_by2lf(bvm_cache *this_bvm, mword *array8){ // _by2lf#

    unsigned char *cast_array = (unsigned char *)array8;
    mword arr8_size  = _arlen8(this_bvm, array8);
    mword *result = _newlfi(this_bvm, arr8_size, 0);

    int i;

    for(i=0; i<arr8_size; i++){

        result[i] = (mword)cast_array[i];

    }

    return result;

}


// Only safe for ptr- or val-arrays; do not use on tags
//
int _arcmp8(bvm_cache *this_bvm, mword *left, mword *right){ // _arcmp8#

    mword left_size  = _arlen8(this_bvm, left);
    mword right_size = _arlen8(this_bvm, right);

    if(left_size > right_size){
        return 1;
    }
    else if(left_size < right_size){
        return -1;
    }

    return memcmp(left, right, left_size);

}


// Only for use with val-arrays
//
int array_cmp_alpha(bvm_cache *this_bvm, mword *left, mword *right, mword lex_alpha_select, mword access_size){ // array_cmp_alpha#

    mword left_size;
    mword right_size;
    mword result;

    if(access_size == ACCESS_SIZE_MWORD){
        left_size  = size(left);
        right_size = size(right);
    }
    else{ // access_size == ACCESS_SIZE_BYTE
        left_size  = _arlen8(this_bvm, left);
        right_size = _arlen8(this_bvm, right);
    }

    if(lex_alpha_select == ARRAY_CMP_LEX){
        if(left_size > right_size){
            return 1;
        }
        else if(left_size < right_size){
            return -1;
        }
        result = memcmp(left, right, left_size);
    }
    else{ // lex_alpha_select == ARRAY_CMP_ALPHA
        if(left_size == right_size){
            result = memcmp(left, right, left_size);
        }
        else{
            result = memcmp(left, right, MIN(left_size,right_size));
            if(result == 0){
                if(left_size > right_size){
                    result = 1;
                }
                else{
                    result = -1;
                }
            }
        }
    
    }

    return result;

}


//
//
mword *_slice8(bvm_cache *this_bvm, mword *array, mword start, mword end){ // _slice8#

    mword *result;

    mword size8 = _arlen8(this_bvm, array);
    end = (end > size8) ? size8 : end;

    if(end<=start){
        result = _val(this_bvm, 0); // Empty-string
    }
    else{
        result = _newstr(this_bvm, end-start, ' ');
        move(this_bvm, result, 0, array, start, end-start, ACCESS_SIZE_BYTE);
    }

    return result;

}


/*****************************************************************************
 *                                                                           *
 *                                ARRAY-1                                    *
 *                                                                           *
 ****************************************************************************/


//
//
mword _arlen1(bvm_cache *this_bvm, mword *string){ // _arlen1#

    mword strsize = size(string) - 1;
    mword last_mword = rcl(string, strsize);
    mword alignment = _dec_alignment_word1(this_bvm, last_mword);

    if(last_mword){
        return  (strsize * MWORD_BIT_SIZE) - (MWORD_BIT_SIZE - alignment);
    }
    else{
        return (strsize * MWORD_BIT_SIZE);
    }

}


// decodes the alignment word
//
mword _dec_alignment_word1(bvm_cache *this_bvm, mword alignment_word){ // dec_alignment_word1#

    if(alignment_word == 0){
        return 0;
    }

    alignment_word = ~alignment_word;
    mword alignment = 0;

    while(alignment_word != 0){ //FIXME: PERF ... really inefficient
        alignment_word = alignment_word >> 1;
        alignment++;
    }

    return alignment;

}


//Returns an alignment word based on size1
//
mword _alignment_word1(bvm_cache *this_bvm, mword size1){ // _alignment_word1#

    if((size1 % MWORD_BIT_SIZE) == 0)
        return 0;

    mword alignment = (mword)-1;

    return alignment << (size1 % MWORD_BIT_SIZE);

}


//
//
mword _array1_size(bvm_cache *this_bvm, mword size1){ // _array1_size#

    mword size = (size1 / MWORD_BIT_SIZE);

    if((size1 % MWORD_BIT_SIZE) != 0){ //XXX Assumes that int div rounds to floor
        size++;
    }

    return size+1; //for the alignment_word

}


//
//
mword *_lf2bi(bvm_cache *this_bvm, mword *array){ // _lf2bi#

    mword arr_size  = size(array);
    mword arr1_size = _array1_size(this_bvm, arr_size);
    mword *result = _newlfi(this_bvm, arr1_size, 0);

    int i;

    for(i=0; i<arr_size-1; i++){

         _wrcxr1(this_bvm, result, i, (array[i] & 0x1));

    }

    lcl(result, arr1_size-1) = _alignment_word1(this_bvm, arr1_size);

    return (mword*)result;

}


//
//
mword *_bi2lf(bvm_cache *this_bvm, mword *array1){ // _bi2lf#

    mword arr1_size  = _arlen1(this_bvm, array1);
    mword *result = _newlfi(this_bvm, arr1_size, 0);

    int i;

    for(i=0; i<arr1_size; i++){

        result[i] = _cxr1(this_bvm, array1, i);

    }

    return result;

}


//
//
void _wrcxr1(bvm_cache *this_bvm, mword *arr, mword off, mword val){ // _wrcxr1#

    mword mword_select = off / MWORD_BIT_SIZE;
    mword bit_offset   = off % MWORD_BIT_SIZE;

    if(mword_select > size(arr)-1){
        _fatal("error")
    };

    mword mask = (1<<bit_offset);

    rcl(arr,mword_select) = (rcl(arr,mword_select) & ~mask) | (val & mask);

}


//
//
mword _cxr1(bvm_cache *this_bvm, mword *arr, mword off){ // _cxr1#

    mword mword_select = off / MWORD_BIT_SIZE;
    mword bit_offset   = off % MWORD_BIT_SIZE;

    if(mword_select > (size(arr)-1)){
        _fatal("_cxr1 error");
    }

    mword result = (rcl(arr,mword_select) & (1<<bit_offset)) >> bit_offset;

//    return (rcl(arr,mword_select) & (1<<bit_offset)) >> bit_offset;
    return result;

}


// In-place mergesort of an array
//
void _msort(bvm_cache *this_bvm, mword *array){ // _msort#

    _rmsort(this_bvm, 0, size(array), array);

}


//
//
void _rmsort(bvm_cache *this_bvm, mword left, mword right, mword *array){ // _rmsort#

    // base case, array is already sorted
    if (right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    _rmsort(this_bvm, left_start, left_end, array);

    // sort right half
    _rmsort(this_bvm, right_start, right_end, array);

    // merge sorted halves back together
    _merge(this_bvm, array, left_start, left_end, right_start, right_end);

}


//
//
void _merge(bvm_cache *this_bvm, mword *array, mword left_start, mword left_end, mword right_start, mword right_end){ // _merge#

    // calculate temporary array sizes
    mword left_length  = left_end  - left_start;
    mword right_length = right_end - right_start;

    // declare temporary arrays
    mword *left_half  = _newlfi(this_bvm,left_length,0);
    mword *right_half = _newlfi(this_bvm,right_length,0);

    mword r = 0; // right_half index
    mword l = 0; // left_half index
    mword i = 0; // array index

    // copy left half of array into left_half
    for (i = left_start; i < left_end; i++, l++){
        left_half[l] = array[i];
    }

    // copy right half of array into right_half
    for (i = right_start; i < right_end; i++, r++){
        right_half[r] = array[i];
    }

    // merge left_half and right_half back into array
    for ( i = left_start, r = 0, l = 0; l < left_length && r < right_length; i++){

        if ( left_half[l] < right_half[r] ){
            array[i] = left_half[l++];
        }
        else {
            array[i] = right_half[r++];
        }
    }

    // copy over leftovers of whichever temporary array hasn't finished
    for ( ; l < left_length; i++, l++){
        array[i] = left_half[l];
    }

    for ( ; r < right_length; i++, r++){
        array[i] = right_half[r];
    }

}


// In-place mergesort of a ptr-array
//
void array_ptr_sort(bvm_cache *this_bvm, mword *array, mword *comparator){ // array_ptr_sort#

    mword *temp;

    if(is_leaf(array)){

        array_rptr_sort(this_bvm, 0, size(array), array, nil, ARRAY_SORT_TYPE_LEAF);

    }
    else{

        temp = _cp(this_bvm, comparator);
        _append_direct(this_bvm, temp, _cons(this_bvm, _cp(this_bvm, bpdl_return_opcode), nil));
        array_rptr_sort(this_bvm, 0, size(array), array, lib_code_to_bvm(this_bvm, temp, this_bvm->soft_root), ARRAY_SORT_TYPE_NON_LEAF);

    }

}


//
//
void array_rptr_sort(bvm_cache *this_bvm, mword left, mword right, mword *array, mword *comparator, mword sort_type){ // array_rptr_sort#

    // base case, array is already sorted
    if(right - left <= 1){
        return;
    }

    // set up bounds to slice array into
    mword left_start  = left;
    mword left_end    = (left+right)/2;
    mword right_start = left_end;
    mword right_end   = right;

    // sort left half
    array_rptr_sort(this_bvm, left_start, left_end, array, comparator, sort_type);

    // sort right half
    array_rptr_sort(this_bvm, right_start, right_end, array, comparator, sort_type);

    // merge sorted halves back together
    array_ptr_sort_merge(this_bvm, array, left_start, left_end, right_start, right_end, comparator, sort_type);

}


//
//
void array_ptr_sort_merge(bvm_cache *this_bvm, mword *array, mword left_start, mword left_end, mword right_start, mword right_end, mword *comparator, mword sort_type){ // array_ptr_sort_merge#

    // calculate temporary array sizes
    mword left_length  = left_end  - left_start;
    mword right_length = right_end - right_start;

    // declare temporary arrays
    mword *left_half  = _newin(this_bvm,left_length);
    mword *right_half = _newin(this_bvm,right_length);

    mword r = 0; // right_half index
    mword l = 0; // left_half index
    mword i = 0; // array index

    // copy left half of array into left_half
//    for (i = left_start; i < left_end; i++, l++){
//        left_half[l] = array[i];
//    }
//
//    // copy right half of array into right_half
//    for (i = right_start; i < right_end; i++, r++){
//        right_half[r] = array[i];
//    }

    memcpy(left_half,  (array+left_start),  BYTE_SIZE(left_length));  // XXX WAIVER(memcpy) XXX //
    memcpy(right_half, (array+right_start), BYTE_SIZE(right_length)); // XXX WAIVER(memcpy) XXX //

    mword *comparison_result;
    mword *arg_list;

    if(sort_type == ARRAY_SORT_TYPE_LEAF){

        // merge left_half and right_half back into array
        for( i = left_start, r = 0, l = 0; l < left_length && r < right_length; i++){

            if ( left_half[l] < right_half[r] ){
                array[i] = left_half[l++];
            }
            else {
                array[i] = right_half[r++];
            }

        }

    }
    else{ // if(sort_type == ARRAY_SORT_TYPE_NON_LEAF)

        // merge left_half and right_half back into array
        for( i = left_start, r = 0, l = 0; l < left_length && r < right_length; i++){

            arg_list = _mkls(this_bvm, 2, left_half[l], right_half[r]);
            comparison_result = lib_babelr(this_bvm, comparator, arg_list);

            if( is_false(comparison_result) ){
                array[i] = left_half[l++];
            }
            else {
                array[i] = right_half[r++];
            }

        }

    }

    // copy over leftovers of whichever temporary array hasn't finished
//    for ( ; l < left_length; i++, l++){
//        array[i] = left_half[l];
//    }
//
//    for ( ; r < right_length; i++, r++){
//        array[i] = right_half[r];
//    }

    if(l<left_length){
        memcpy((array+i), (left_half+l), BYTE_SIZE(left_length-l));   // XXX WAIVER(memcpy) XXX //
    }

    if(r<right_length){
        memcpy((array+i), (right_half+r), BYTE_SIZE(right_length-r)); // XXX WAIVER(memcpy) XXX //
    }

}






/*****************************************************************************
 *                                                                           *
 *                                ARRAY-M                                    *
 *                                                                           *
 ****************************************************************************/


// Only safe for ptr- or val-arrays; do not use on tags
//
int _arcmp(mword *left, mword *right){ // _arcmp#

    mword left_size  = size(left);
    mword right_size = size(right);

    if(left_size > right_size){
        return 1;
    }
    else if(left_size < right_size){
        return -1;
    }

    return memcmp(left, right, BYTE_SIZE(left_size));

}


//
//
mword *_slice(bvm_cache *this_bvm, mword *array, mword start, mword end){ // _slice#

    mword *result;

    end = (end > size(array)) ? size(array) : end;

    if(end<=start){
        result = nil;
    }
    else{

        if(is_leaf(array)){
            result = _newlfi(this_bvm, end-start, 0);
        }
        else{
            result = _newin(this_bvm, end-start);
        }

        move(this_bvm, result, 0, array, start, end-start, ACCESS_SIZE_MWORD);

    }

    return result;


//    mword *result;
//
//    end = (end > size(array)) ? size(array) : end;
//
//    if(end<=start){
//        result = nil;
//    }
//    else{
//
//        int i,j;
//
//        if(is_leaf(array)){
//
//            result = _newlfi(this_bvm, (mword)(end-start), 0);
//
//        }
//        else{
//
//            result = _newin(this_bvm, end-start);
//
//        }
//
//        for( i=start, j=0;
//             i<end;
//             i++,j++     ){
//
//            lci(result,j) = rci(array,i);
//
//        }
//
//    }
//
//    return result;

}


//
//
mword *_th(bvm_cache *this_bvm, mword *bs, mword entry){ // _th#

    if(is_leaf(bs)){
        return _val(this_bvm, rcl(bs,entry%size(bs)));
    }
    if(is_tptr(bs)){
        return nil;
    }

    return (mword*)rci(bs,entry%size(bs));

}


//
//
mword *_th8(bvm_cache *this_bvm, mword *bs, mword entry){ // _th8#

    if(is_leaf(bs)){
        mword *byte = _newstr(this_bvm, 1, ' ');
        mword index  = entry / MWORD_SIZE;
        mword offset = (entry % MWORD_SIZE) * BITS_PER_BYTE;
        lcl(byte, 0) = ((rcl(bs,index%size(bs)) & (0xff<<offset)) >> offset);
        return byte;
    }

    return nil;

}


// FIXME: cut --> reimplement as two _slice's (left and right)


//
//
mword *_arcat(bvm_cache *this_bvm, mword *left, mword *right){ // _arcat#

    mword *result;

    mword size_left  = size(left);
    mword size_right = size(right);

    if      ( is_leaf(right)  &&  is_leaf(left) ){
        result = _newlfi(this_bvm,  size_left + size_right, 0 );
    }
    else if ( is_inte(right)  &&  is_inte(left) ){
        result = _newin(this_bvm,  size_left + size_right );
    }
    else{ //FIXME: Throw an exception
        _fatal("cannot concatenate leaf array and interior array");
    }

    mword i,j;
    for(    i=0;
            i<size_left;
            i++
        ){

        lci((mword*)result,i) = rci(left,i);

    }

    for(    i=0,j=size_left;
            i<size_right;
            i++,j++
        ){

        lci(result,j) = rci(right,i);

    }

    return result;

}


//// Babelized wrapper around memmove()
////
//void move(mword *dest, mword dest_index, mword *src, mword src_index, mword size){  // move#
//
//    mword  src_size = size(src );
//    mword dest_size = size(dest);
//
//     src_index =  src_index %  src_size;
//    dest_index = dest_index % dest_size;
//
//    mword  src_headroom = ( src_size -  src_index);
//    mword dest_headroom = (dest_size - dest_index);
//
//    mword size_limit = (src_headroom < dest_headroom) ? src_headroom : dest_headroom;
//
//    size = (size < size_limit) ? size : size_limit;
//
//    //void * memmove( void * destination, const void * source, size_t num );
//    memmove( dest+dest_index, src+src_index, (size_t)BYTE_SIZE(size) );
//
//}


// Babelized wrapper around memmove()
//
void move(bvm_cache *this_bvm, mword *dest, mword dest_index, mword *src, mword src_index, mword size_arg, mword access_size){  // move#

    mword src_size;
    mword dest_size;
    mword src_headroom;
    mword dest_headroom;
    mword size_limit;
    mword final_size;

    if(access_size == ACCESS_SIZE_MWORD){

        src_size  = size(src );
        dest_size = size(dest);

    }
    else{ // access_size = ACCESS_SIZE_BYTE

        src_size  = _arlen8(this_bvm, src );
        dest_size = _arlen8(this_bvm, dest);

    }

    src_index  =  src_index %  src_size;
    dest_index = dest_index % dest_size;

    src_headroom  = ( src_size -  src_index);
    dest_headroom = (dest_size - dest_index);

    size_limit = (src_headroom < dest_headroom) ? src_headroom : dest_headroom;

    final_size = (size_arg < size_limit) ? size_arg : size_limit;

    //void * memmove( void * destination, const void * source, size_t num );
//    memmove( dest+dest_index, src+src_index, (size_t)size );
    if(access_size == ACCESS_SIZE_MWORD){

        memmove( dest+dest_index, src+src_index, (size_t)BYTE_SIZE(final_size) );

    }
    else{ // access_size = ACCESS_SIZE_BYTE

        memmove( ((char*)dest+dest_index), ((char*)src+src_index), (size_t)final_size );

    }

}


//
//
mword *_ar2bytes(bvm_cache *this_bvm, mword *array){ // _ar2bytes#

    if(is_nil(array)){
        return _newlfi(this_bvm, 1, 0);
    }

    mword arsize = size(array);

    char *result = (char*)_newstr(this_bvm, arsize, ' ');

    int i;
    for(i=0;i<arsize;i++){
        result[i] = (rcl(array,i) & 0xff);
    }

    return (mword *)result;

}


// XXX The return-value from this function contains unsafe pointers!!! XXX
// XXX internal interp use ONLY                                        XXX
// XXX If you pass tag=nil, returns ALL tags in bs                     XXX
// XXX PERF: A _tags2ar (like _bs2ar) would be more efficient          XXX
//
mword *array_find_val(bvm_cache *this_bvm, mword *bs, mword *val){ // array_find_val#

    mword *span_array     = _bs2ar(this_bvm, bs);
    mword size_span_array = size(span_array);
    mword size_inte;
    mword *val_list       = nil;
    mword *curr_span_elem;
    mword *curr_inte_elem;


//_dump(span_array);

    int i,j;
    for(i=0; i<size_span_array; i++){

        curr_span_elem = rci(span_array,i);

        if(is_inte(curr_span_elem)){ // check each element

            size_inte = size(curr_span_elem);

            for(j=0;j<size_inte;j++){

                curr_inte_elem = rci(curr_span_elem,j);

                if(is_nil(curr_inte_elem)){
                    continue;
                }

                if(is_leaf(curr_inte_elem)){

                    if( !_arcmp(curr_inte_elem, val) ){

                        // push onto val_list
                        if(is_nil(val_list)){
                            val_list = _cons(this_bvm, (curr_span_elem+j), nil);
                        }
                        else{
                            _unshift(this_bvm, val_list, (curr_span_elem+j));
                        }

                    }

                }

            }

        }

    }

    return val_list;

}


// XXX The return-value from this function contains unsafe pointers!!! XXX
// XXX internal interp use ONLY                                        XXX
// XXX If you pass tag=nil, returns ALL tags in bs                     XXX
// XXX PERF: A _tags2ar (like _bs2ar) would be more efficient          XXX
//
mword *array_find_ptr(bvm_cache *this_bvm, mword *bs, mword *ptr){ // array_find_ptr#

    mword *span_array     = _bs2ar(this_bvm, bs);
    mword size_span_array = size(span_array);
    mword size_inte;
    mword *ptr_list       = nil;
    mword *curr_span_elem;
    mword *curr_inte_elem;

    int i,j;
    for(i=0; i<size_span_array; i++){

        curr_span_elem = rci(span_array,i);

        if(is_inte(curr_span_elem)){ // check each element

            size_inte = size(curr_span_elem);

            for(j=0;j<size_inte;j++){

                curr_inte_elem = rci(curr_span_elem,j);

                if(is_nil(curr_inte_elem)){
                    continue;
                }

                if(is_inte(curr_inte_elem)){

                    if( !_arcmp(curr_inte_elem, ptr) ){

                        // push onto ptr_list
                        if(is_nil(ptr_list)){
                            ptr_list = _cons(this_bvm, (curr_span_elem+j), nil);
                        }
                        else{
                            _unshift(this_bvm, ptr_list, (curr_span_elem+j));
                        }

                    }

                }

            }

        }

    }

    return ptr_list;

}




/*****************************************************************************
 *                                                                           *
 *                              ARRAY OPERATORS                              *
 *                                                                           *
 ****************************************************************************/


#define TH_R8D_OPERATIONS \
    result0 = _th8(this_bvm, oi1.data, *oi0.data);

OPERATORA_R2_W1_D(th_r8d,
        TH_R8D_OPERATIONS,
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 0)


#define TH_RMD_OPERATIONS \
    result0 = read_with_pbp(this_bvm, oi1.data, oi0.data);

OPERATORA_R2_W1_D(th_rmd,
        TH_RMD_OPERATIONS,
        0, OI_MASK_LEAF,                           0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR, 0, 0)


#define TH_WMD_OPERATIONS \
    write_with_pbp(this_bvm, oi2.data, oi1.data, oi0.data);

OPERATORA_R3_W0_D(th_wmp,
        TH_WMD_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR, 0, 0,
        0, OI_MASK_LEAF,                           0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR, 0, 0)


#define CAT_MD_OPERATIONS \
    result0 = _arcat(this_bvm, oi1.data, oi0.data);

OPERATORA_R2_W1_D(cat_md,
        CAT_MD_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0)


#define CAT_8D_OPERATIONS \
    result0 = _arcat8(this_bvm, oi1.data, oi0.data);

OPERATORA_R2_W1_D(cat_8d,
        CAT_8D_OPERATIONS,
        0, OI_MASK_LEAF, 0, 0,
        0, OI_MASK_LEAF, 0, 0)


#define MOVE_MD_OPERATIONS \
    move(this_bvm, oi4.data, *oi3.data, oi2.data, *oi1.data, *oi0.data, ACCESS_SIZE_MWORD);

OPERATORA_R5_W0_D(move_md,
        MOVE_MD_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0)


#define MOVE_8D_OPERATIONS \
    move(this_bvm, oi4.data, *oi3.data, oi2.data, *oi1.data, *oi0.data, ACCESS_SIZE_BYTE);

OPERATORA_R5_W0_D(move_8d,
        MOVE_8D_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0)


#define ARLEN_D_OPERATIONS \
    result0 = _val( this_bvm, size(oi0.data) );

OPERATORA_R1_W1_D(arlen_md, ARLEN_D_OPERATIONS, nil, OI_MASK_INTE|OI_MASK_LEAF, 0, 0)


#define ARLEN_8D_OPERATIONS \
    result0 = _val( this_bvm, _arlen8( this_bvm, oi0.data ) );

OPERATORA_R1_W1_D(arlen_8d, ARLEN_8D_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)


#define SLICE_MD_OPERATIONS \
    result0 = _slice( this_bvm, oi2.data, *oi1.data, *oi0.data );

OPERATORA_R3_W1_D(slice_md, SLICE_MD_OPERATIONS,
        nil, OI_MASK_LEAF, 0, 0,
        nil, OI_MASK_LEAF, 0, 0,
        nil, OI_MASK_INTE|OI_MASK_LEAF, 0, 0)


#define SLICE_8D_OPERATIONS \
    result0 = _slice8( this_bvm, oi2.data, *oi1.data, *oi0.data );

OPERATORA_R3_W1_D(slice_8d, SLICE_8D_OPERATIONS,
        nil, OI_MASK_LEAF, 0, 0,
        nil, OI_MASK_LEAF, 0, 0,
        nil, OI_MASK_INTE|OI_MASK_LEAF, 0, 0)


#define CMP_MD_OPERATIONS \
    result0 = _val( this_bvm, _arcmp(oi1.data, oi0.data) );

OPERATORA_R2_W1_D(arcmp_md,
        CMP_MD_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0)


#define STRCMP_8D_OPERATIONS \
    result0 = _val( this_bvm, array_cmp_alpha(this_bvm, oi1.data, oi0.data, ARRAY_CMP_ALPHA, ACCESS_SIZE_BYTE) );

OPERATORA_R2_W1_D(strcmp_8d,
        STRCMP_8D_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0)


#define STRCMP_MD_OPERATIONS \
    result0 = _val( this_bvm, array_cmp_alpha(this_bvm, oi1.data, oi0.data, ARRAY_CMP_ALPHA, ACCESS_SIZE_MWORD) );

OPERATORA_R2_W1_D(strcmp_md,
        STRCMP_MD_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0)


#define CMP_8D_OPERATIONS \
    result0 = _val( this_bvm, _arcmp8(this_bvm, oi1.data, oi0.data) );

OPERATORA_R2_W1_D(arcmp_8d,
        CMP_8D_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0,
        0, OI_MASK_LEAF|OI_MASK_INTE, 0, 0)


#define SORTVAL_D_OPERATIONS \
    _msort( this_bvm, oi0.data );

OPERATORA_R1_W0_D(sortval_d, 
        SORTVAL_D_OPERATIONS, 
        nil, OI_MASK_LEAF, 0, 0)


#define SORT_MD_OPERATIONS \
    array_ptr_sort( this_bvm, oi1.data, oi0.data );

OPERATORA_R2_W0_D(merge_sort_d, 
        SORT_MD_OPERATIONS, 
        nil, OI_MASK_INTE|OI_MASK_TPTR, 0, 0,
        nil, OI_MASK_ANY, 0, 0)


// Clayton Bauman 2014

