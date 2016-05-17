// bstruct.c
//

#include "babel.h"
#include "bstruct.h"
#include "mem.h"
#include "cache.h"
#include "operator.h"
#include "trie.h"
#include "tptr.h"
#include "introspect.h"
#include "array.h"
#include "logic.h"
#include "pearson16.h"


// Use of this function requires a valid WAIVER
// Returns UNINITIALIZED leaf-memory... causes corruption of Babel-strings, etc.
//
void *_newlf(bvm_cache *this_bvm, mword size){ // WAIVER_REQUIRED _newlf#

    void *ptr = (void*)mem_alloc(this_bvm, BYTE_SIZE(size));

    return ptr;

}


// Use of this function requires a valid WAIVER
// NOT SAFE for use with tptr's
//
void *_sys_cp(bvm_cache *this_bvm, mword *bs){ // WAIVER_REQUIRED _sys_cp#

    mword byte_size = BYTE_SIZE(size(bs));

    void *ptr = (void*)mem_sys_alloc(byte_size);

    s(ptr) = s(bs);

    //memcpy(dest, src, len);
    memcpy(ptr, bs, byte_size);

    return ptr;

}


//
//
void *_newlfi(bvm_cache *this_bvm, mword size, mword init){ // _newlfi#

    void *ptr = (void*)_newlf(this_bvm, size); // XXX WAIVER(_newlf) XXX //

    memset((char*)ptr,init,BYTE_SIZE(size));

// XXX PERF: Doesn't seem to help ...
//    if(!init){
//        if(size == 1){
//            lcl(ptr,0) = 0;
//        }
//        else{
//            lcl(ptr,size-2) = 0;
//            lcl(ptr,size-1) = 0;
//        }
//    }
//    else{
//        memset((char*)ptr,init,BYTE_SIZE(size));
//    }

    return ptr;

}


// copies a new leaf-array from an initial C-array
//
mword *_newlfcp(bvm_cache *this_bvm, const mword *array, mword size){ // _newlfcp#

    mword *ptr = (void*)mem_alloc(this_bvm, BYTE_SIZE(size));
//    mword *ptr = (void*)mem_alloc(this_bvm, size+1);

//    int i;
//    for(i=0;i<size;i++){
//        lcl(ptr,i) = array[i];
//    }

    memcpy(ptr, array, (size_t)BYTE_SIZE(size));

    return ptr;

}


//
//
void *_mklf(bvm_cache *this_bvm, mword array_size, ...){ // _mklf#

    void *leaf = (void*)_newlfi(this_bvm, array_size,0);

    va_list vl;
    va_start(vl,array_size);

    int i;

    for(i=0;i<array_size;i++){
        lcl(leaf,i) = va_arg(vl,mword);
    }

    va_end(vl);

    return leaf;

}


//
//
void *_mkin(bvm_cache *this_bvm, mword array_size, ...){ // _mkin#

    void *inte = (void*)_newin(this_bvm, array_size);

    va_list vl;
    va_start(vl,array_size);

    int i;

    for(i=0;i<array_size;i++){
        lci(inte,i) = va_arg(vl,mword*);
    }

    va_end(vl);

    return inte;

}


//
//
void *_newin(bvm_cache *this_bvm, mword size){ // _newin#

    void *ptr = (void*)mem_alloc(this_bvm, -1*BYTE_SIZE(size)); // FIXME: Bare constant should be macro

    int i;
    for(i = 0; i<size; i++){ // All pointers must be valid - initialize to nil
        lci(ptr,i) = (void*)nil;
    }

    return ptr;

}


// Accepts a data value and returns a leaf-array of size 1 containing that 
// data value
//
void *_val(bvm_cache *this_bvm, mword value){ // _val#

    void *ptr = _newlfi(this_bvm,1,0);

    lcl(ptr,0) = value;

    return ptr;

}


// Accepts a single unsafe pointer and returns a safe interior-array of size
// 1 containing the contents of the unsafe pointer
//
void *_ptr(bvm_cache *this_bvm, mword *unsafe_ptr){ // _ptr#

    void *ptr = _newin(this_bvm,1);

    lci(ptr,0) = unsafe_ptr;

    return ptr;

}


// recursively cleans a bstruct after traversal
//
void _rclean(bvm_cache *this_bvm, mword *bs){ // _rclean#

    int i;

    if( !(s(bs) & CTL_MASK) ){ //Already cleaned
        return;
    }

    s(bs) = s(bs) & ~CTL_MASK; //Mark clean

    if( is_inte(bs) ){
        int num_elem = size(bs);
        for(i=0; i<num_elem; i++){
            _rclean(this_bvm, (mword *)*(bs+i));
        }
    }
    else if(is_tptr(bs)){
        _rclean(this_bvm, bs+HASH_SIZE+1);
    }

}


//
//
void _recurse(bvm_cache *this_bvm, mword *bs, bstruct_op_fn_ptr bfn, void *v){ // _recurse#

    _fn_recurse(this_bvm, bs, bfn, v);
    _rclean(this_bvm, bs);

}


//
//
mword _fn_recurse(bvm_cache *this_bvm, mword *bs, bstruct_op_fn_ptr bfn, void *v){ // _fn_recurse#

    int i;

    if( TRAVERSED(bs) ){
        return 1;
    }

    if( !bfn(this_bvm, bs,v) ){
        return 0;
    }

    if( is_inte(bs) ){
        int num_elem = size(bs);
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            if(!_fn_recurse(this_bvm, (mword *)*(bs+i),bfn,v)){
                return 0;
            }
        }
        return 1;
    }
    else if( is_tptr(bs) ){
        mword *tptr_ptr = (bs+TPTR_PTR_OFFSET);

        MARK_TRAVERSED(bs);

        MARK_TRAVERSED(tptr_ptr);

        _fn_recurse(this_bvm, (mword*)*tptr_ptr, bfn, v);
    }
    else{
        MARK_TRAVERSED(bs);
    }
    return 1;

}




////
////
//mword _fn_recurse(bvm_cache *this_bvm, mword *bs, bstruct_op_fn_ptr bfn, void *v){ // _fn_recurse#
//
//    int i;
//
//    if( TRAVERSED(bs) ){
//        return 1;
//    }
//
//    if( !bfn(this_bvm, bs,v) ){
//        return 0;
//    }
//
//    if( is_inte(bs) ){
//        int num_elem = size(bs);
//        MARK_TRAVERSED(bs);
//        for(i=0; i<num_elem; i++){
//            if(!_fn_recurse(this_bvm, (mword *)*(bs+i),bfn,v)){
//                return 0;
//            }
//        }
//        return 1;
//    }
//    else if( is_tptr(bs) ){
//        MARK_TRAVERSED(bs);
//        _fn_recurse(this_bvm, bs+HASH_SIZE+1,bfn,v);
//    }
//    else{
//        MARK_TRAVERSED(bs);
//    }
//    return 1;
//
//}
//

// _mu -> memory usage (mnemonic: *nix du)
// _mu(this_bvm, x) = 
//      + _nlf(this_bvm, x) 
//      + _nva(this_bvm, x)
//      _nin(this_bvm, x) 
//      + _nptr(this_bvm, x) 
//      + _ntag(this_bvm, x)*(HASH_SIZE+1)
//      
//  Don't forget that nil will add a "silent" HASH_SIZE+1 to 
//  your bstruct if anything in your bstruct points to it...
mword _mu(bvm_cache *this_bvm, mword *bs){ // _mu#

    mword counter=0;
    _recurse(this_bvm, bs, _rmu, &counter);
    return counter;

}


////
////
//mword _rmu(bvm_cache *this_bvm, mword *bs, void *v){ // _rmu#
//
//    if( is_tptr(bs) ){
//        *(mword*)v += HASH_SIZE+1;
//    }
//    else if( is_leaf(bs) ){
//        *(mword*)v += size(bs)+1;
//    }
//    else{
//        *(mword*)v += size(bs)+1;
//    }
//
//    return 1;
//
//}
//

//
//
mword _rmu(bvm_cache *this_bvm, mword *bs, void *v){ // _rmu#

    if( is_tptr(bs) ){
        *(mword*)v += HASH_SIZE+3;
    }
    else if( is_leaf(bs) ){
        *(mword*)v += size(bs)+1;
    }
    else{
        *(mword*)v += size(bs)+1;
    }

    return 1;

}



// _nlf -> number of leaf-arrays
//
mword _nlf(bvm_cache *this_bvm, mword *bs){ // _nlf#

    mword counter=0;
    _recurse(this_bvm, bs, _rnlf, &counter);
    return counter;

}

//
//
mword _rnlf(bvm_cache *this_bvm, mword *bs, void *v){ // _rnlf#

    if( is_leaf(bs) ){
        *(mword*)v += 1;
    }

    return 1;

}


// _nin -> number of interior-arrays
//
mword _nin(bvm_cache *this_bvm, mword *bs){ // _nin#

    mword counter=0;
    _recurse(this_bvm, bs, _rnin, &counter);
    return counter;

}

//
//
mword _rnin(bvm_cache *this_bvm, mword *bs, void *v){ // _rnin#

    if( is_inte(bs) ){
        *(mword*)v += 1;
    }

    return 1;

}


// _ntag -> number of tagged-pointers
//
mword _ntag(bvm_cache *this_bvm, mword *bs){ // _ntag#

    mword counter=0;
    _recurse(this_bvm, bs, _rntag, &counter);
    return counter;

}

//
//
mword _rntag(bvm_cache *this_bvm, mword *bs, void *v){ // _rntag#

    if( is_tptr(bs) ){
        *(mword*)v += 1;
    }

    return 1;

}


// _nva -> number of values
//
mword _nva(bvm_cache *this_bvm, mword *bs){ // _nva#

    mword counter=0;
    _recurse(this_bvm, bs, _rnva, &counter);
    return counter;

}


//
//
mword _rnva(bvm_cache *this_bvm, mword *bs, void *v){ // _rnva#

    if( is_leaf(bs) ){
        *(mword*)v += size(bs);
    }

    return 1;

}


// _nptr -> number of pointers
//
mword _nptr(bvm_cache *this_bvm, mword *bs){ // _nptr#

    mword counter=0;
    _recurse(this_bvm, bs, _rnptr, &counter);
    return counter;

}


//
//
mword _rnptr(bvm_cache *this_bvm, mword *bs, void *v){ // _rnptr#

    if( is_inte(bs) ){
        *(mword*)v += size(bs);
    }

    return 1;

}


//
//
mword *bstruct_load_fast(bvm_cache *this_bvm, mword *bs, mword size){ // bstruct_load_fast#

    mword *curr_ptr = bs+1;
    mword *base_ptr = curr_ptr;

    mword curr_size;
    int i;

    mword *bs_last_mword = bs+size-1;

    while(curr_ptr < bs_last_mword){

        if(is_inte(curr_ptr)){

            curr_size = size(curr_ptr);

            for(i=0;i<curr_size;i++){
                lcl(curr_ptr,i) = rcl(curr_ptr,i) + (mword)bs;
            }

        }
        else if(is_tptr(curr_ptr)){

            lcl(curr_ptr,TPTR_PTR_OFFSET) = rcl(curr_ptr,TPTR_PTR_OFFSET) + (mword)bs;

        }
        // else is_leaf, do nothing

        curr_ptr += alloc_size(curr_ptr);

    }

    return base_ptr;

}


//
//
mword *_load(bvm_cache *this_bvm, mword *bs, mword size){ // *_load#

    _rload(this_bvm, bs, 1);

    _rclean(this_bvm, bs+1);

    return (bs+1);

}


//_rload
//
void _rload(bvm_cache *this_bvm, mword *tree, mword offset){ // _rload#

    int i;

    mword *this_elem = tree+offset;

    if( TRAVERSED((mword*)(this_elem)) ){ 
        return;
    }

    int num_elem = size(this_elem);

    if(is_inte(this_elem)){

        MARK_TRAVERSED(this_elem);

        for(i=0; i<num_elem; i++){
            _rload(this_bvm, tree, rcl(this_elem,i)/MWORD_SIZE );
            lci(this_elem,i) = rcl(this_elem,i)/MWORD_SIZE + tree; // add base offset
        }

    }
    else if(is_leaf(this_elem)){

        MARK_TRAVERSED(this_elem);

    }
    else{

        MARK_TRAVERSED(this_elem);
        MARK_TRAVERSED(this_elem+TPTR_PTR_OFFSET);

        _rload(this_bvm, tree, rcl(this_elem,TPTR_PTR_OFFSET)/MWORD_SIZE );
        lci(this_elem,TPTR_PTR_OFFSET) = rcl(this_elem,TPTR_PTR_OFFSET)/MWORD_SIZE + tree;

    }

}


//
//
mword _get_rel_offset(bvm_cache *this_bvm, mword *LUT_abs, mword *LUT_rel, mword *entry){ // get_rel_offset#

    int i=0;
    int LUT_size = size(LUT_abs);

    for(;i<LUT_size;i++){
        if(rcl(LUT_abs,i) == (mword)entry) 
            return rcl(LUT_rel,i);
    }
    return (mword)nil;

}



// XXX PERF: add leaf-array detection and do memcpy()
//
mword *_cp(bvm_cache *this_bvm, mword *bs){ // _cp#

    mword *temp = _unload_fast(this_bvm, bs);

    bs = bstruct_load_fast(this_bvm, temp, size(temp));

    return bs;    

}


// XXX DEPRECATED XXX
// Just switch everything to _cp()
//
mword *_cp_fast(bvm_cache *this_bvm, mword *bs){ // _cp_fast#

    mword *temp = _unload_fast(this_bvm, bs);

    bs = bstruct_load_fast(this_bvm, temp, size(temp));

    return bs;    

}


//
//
void _trunc(bvm_cache *this_bvm, mword *operand, mword new_size){ // _trunc#

    if(is_leaf(operand)){
        s(operand) = (new_size*MWORD_SIZE);
    }
    else if(is_inte(operand)){ //is_inte
        s(operand) = (int)-1*(new_size*MWORD_SIZE);
    }
    //hash-refs can't be trunc'd

}


//
//
mword *_unload_fast(bvm_cache *this_bvm, mword *bs){ // _unload_fast#

    mword *dest;

    if(is_leaf(bs)){
        dest = _newlfi(this_bvm, alloc_size(bs), 0);
        memcpy(dest,(bs-1),(size_t)BYTE_SIZE(size(dest)));
        return dest;
    }

    dest         = _newlfi(this_bvm, _mu(this_bvm, bs), 0);

    mword *span_array   = _bs2ar(this_bvm, bs);
    _msort(this_bvm, span_array);

    mword *offset_array = _newlfi(this_bvm, size(span_array), 0xff);

    mword dest_offset = 0;

    _runload_fast(this_bvm, bs, dest, &dest_offset, span_array, offset_array);
    _rclean(this_bvm, bs);

    return dest;

}


//
//
mword _runload_fast(
        bvm_cache   *this_bvm, 
        mword       *bs, 
        mword       *dest, 
        mword       *dest_offset, 
        mword       *span_array, 
        mword       *offset_array){ // _runload_fast#

    int i;

    if( TRAVERSED(bs) ){
        return offset_array[get_offset_from_ptr(this_bvm, span_array, bs)];
    }

    int num_elem = size(bs);

    *(dest+(*dest_offset))   = s(bs);
    *dest_offset             = *dest_offset+1;

    mword local_offset = *dest_offset;
    mword this_offset  = (*dest_offset)*MWORD_SIZE;

    set_offset_for_ptr(this_bvm, span_array, bs, offset_array, this_offset);

    if(is_tptr(bs)){ // is_tptr

        MARK_TRAVERSED(bs);

        for(i=0; i<=HASH_SIZE; i++){
            lcl(dest,(*dest_offset)) = rcl(bs,i);
            *dest_offset = *dest_offset+1;
        }

        mword *tptr_ptr = (bs+TPTR_PTR_OFFSET);

        MARK_TRAVERSED(tptr_ptr); //FIXME: Does this actually need to be here??

        mword tptr_offset = *dest_offset;

        *dest_offset = *dest_offset + 1;

        lcl(dest,tptr_offset)
            = _runload_fast(this_bvm, (mword*)*tptr_ptr, dest, dest_offset, span_array, offset_array);

    }
    else if(is_inte(bs)){

        MARK_TRAVERSED(bs);

        *dest_offset = *dest_offset + num_elem;

        for(i=0; i<num_elem; i++){
            lcl(dest,local_offset+i) 
                = _runload_fast(this_bvm, rci(bs,i), dest, dest_offset, span_array, offset_array);
        }

    }
    else{// if(is_leaf(bs))

        MARK_TRAVERSED(bs);

//_d(s(bs));
//_bounds_check(this_bvm, bs);

        for(i=0; i<num_elem; i++){
            lcl(dest,(*dest_offset)) = rcl(bs,i);
            *dest_offset = *dest_offset+1;
        }

    }

    return this_offset;

}


//
//
mword get_offset_from_ptr(bvm_cache *this_bvm, mword *span_array, mword *ptr){ // get_offset_from_ptr#

    int span_array_size = size(span_array);
    int i;

    if(span_array_size < 4){ // linear search
        for(i=0; i<span_array_size; i++){
            if(rci(span_array,i) == ptr){
                return i;
            }
        }
        return -1;
    }

    int shift       = span_array_size >> 1;
    int guess_index = shift;

    shift >>= 1;

    mword last_guess_index = -1;

    while(1){ // binary search sweetness...
        if(guess_index == last_guess_index
                || guess_index < 0
                || guess_index >= span_array_size){
            _fatal("ptr not found in span_array");
        }
        mword *guess = rci(span_array,guess_index);
        last_guess_index = guess_index;
        if(guess < ptr){
            guess_index += shift;
        }
        else if(guess > ptr){
            guess_index -= shift;
        }
        else if(guess == ptr){
            return guess_index;
        }
        shift >>= 1;
        shift = (shift == 0) ? 1 : shift;
    }
_die;
    return -1; //error

}


//
//
void set_offset_for_ptr(
        bvm_cache *this_bvm, 
        mword     *span_array,      mword *ptr, 
        mword     *offset_array,    mword this_offset){ // set_offset_for_ptr#

    mword span_offset = get_offset_from_ptr(this_bvm, span_array, ptr);

    offset_array[span_offset] = this_offset;

}


//Creates an interior array with one pointer to each array in a bstruct
mword *_bs2ar(bvm_cache *this_bvm, mword *bs){  // _bs2ar#

    mword num_arrays  = _nin(this_bvm, bs);
          num_arrays += _nlf(this_bvm, bs);
          num_arrays += _ntag(this_bvm, bs);

    mword *arr_list = _newin(this_bvm, num_arrays);
    mword offset = 0;

    _rbs2ar(this_bvm, bs, arr_list, &offset);
    _rclean(this_bvm, bs);

    return arr_list;

}


//
//
void _rbs2ar(bvm_cache *this_bvm, mword *bs, mword *arr_list, mword *offset){ // _rbs2ar#

    int i;

    if( TRAVERSED(bs) ){ //Already dumped
        return;
    }

    int num_elem = size(bs);

    lci(arr_list,*offset) = bs;
    *offset = *offset+1;

    if(is_tptr(bs)){

        MARK_TRAVERSED(bs);

        mword *tptr_ptr = (bs+TPTR_PTR_OFFSET);

        MARK_TRAVERSED(tptr_ptr);

        _rbs2ar(this_bvm, (mword*)*tptr_ptr, arr_list, offset);

    }
    else if(is_inte(bs)){

        MARK_TRAVERSED(bs);

        for(i=0; i<num_elem; i++){
            _rbs2ar(this_bvm, (mword*)rci(bs,i), arr_list, offset);
        }

    }
    //else(is_leaf(bs))

    MARK_TRAVERSED(bs);

}


// Use of this function requires a valid WAIVER
// Can return MID-ARRAY bstruct pointer
// pbp --> pure-Babel pointer
trav_return _trav(bvm_cache *this_bvm, mword *bs, mword *pbp){ // WAIVER_REQUIRED _trav#

    trav_return r;

    mword pbp_size = size(pbp);

    r.bs         = tptr_detag(this_bvm, bs);
    r.index      = 0;

    r.unsafe_ptr = (r.bs + rcl(pbp,r.index));

    if(is_leaf(r.bs)){
        return r;
    }
    else if(is_tptr(r.bs)){
        r.unsafe_ptr = r.bs;
        return r;
    }

    for(; r.index<pbp_size-1; r.index++){

        r.bs = rci(r.bs,rcl(pbp,r.index));
        r.bs = tptr_detag(this_bvm, r.bs);    //bypass any tptrs

        r.unsafe_ptr = (r.bs + rcl(pbp,r.index+1));

        if(is_leaf(r.bs)){
            return r;
        }
        else if(is_tptr(r.bs)){
            r.unsafe_ptr = r.bs;
            return r;
        }

    }

    return r;

}


//
//
mword _bscmp(bvm_cache *this_bvm, mword *left, mword *right){ // _bscmp#

    // short-circuit check for inequality
    if(s(left) != s(right))
        return 0;

    mword *unloaded_left  = _unload_fast(this_bvm, left);
    mword *unloaded_right = _unload_fast(this_bvm, right);

    return (_arcmp(unloaded_left, unloaded_right) == 0);

}


//
//
void bstruct_substitute(bvm_cache *this_bvm, mword *target_bs, mword *find_bs, mword *sub_bs){ // bstruct_substitute#

    mword *sub_list;

    if(is_tptr(find_bs)){
        sub_list = tptr_find_tag_unsafe(this_bvm, target_bs, find_bs);
    }
    else if(is_leaf(find_bs)){
        sub_list = array_find_val(this_bvm, target_bs, find_bs);
    }
    //else{ // is_inte(find_bs)
    //}
    
// Inte substitutes every pointer that points AT find_bs from within target_bs

    if(is_nil(sub_list)){
        return;
    }

    while(!is_nil(sub_list)){
        *(mword**)(lci(sub_list,0)) = sub_bs;
        sub_list = (mword*)icdr(sub_list);
    }

}


//
//
mword *bstruct_find(bvm_cache *this_bvm, mword *target_bs, mword *find_bs){ // bstruct_find#

    mword *result;

    if(is_tptr(find_bs)){
        result = tptr_find_tag_unsafe(this_bvm, target_bs, find_bs);
    }
    else if(is_leaf(find_bs)){
        result = array_find_val(this_bvm, target_bs, find_bs);
    }
    else{ // is_inte(find_bs)
        result = array_find_ptr(this_bvm, target_bs, find_bs);
    }

    return result;

}


//
//
mword *bstruct_load_template(bvm_cache *this_bvm, mword *template_bs, mword *rel_offsets, mword *abs_offsets){ // bstruct_load_template#

    mword *result = _cp(this_bvm, template_bs);

    int i;
    int num_offsets = size(rel_offsets);

    for(i=0;i<num_offsets;i++){
        lcl(result, rcl(rel_offsets,i)) = rcl(abs_offsets,i);
    }

    return result;

}


/*****************************************************************************
 *                                                                           *
 *                           BSTRUCT OPERATORS                               *
 *                                                                           *
 ****************************************************************************/


#define REPLACE_D_OPERATIONS \
    bstruct_substitute(this_bvm, oi2.data, oi1.data, oi0.data);

OPERATORA_R3_W0_D(replace_d, 
        REPLACE_D_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 0)


#define LOAD_D_OPERATIONS \
   result0 = _load(this_bvm, oi0.data, size(oi0.data)); 

OPERATORA_R1_W1_D(load_d, LOAD_D_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)


#define NEWVAL_D_OPERATIONS \
   result0 = _newlfi(this_bvm, rcl(oi0.data,0), 0);

OPERATORA_R1_W1_D(newval_d, NEWVAL_D_OPERATIONS, nil, OI_MASK_LEAF, 0, 1)


#define CP_D_OPERATIONS \
    result0 = _cp(this_bvm, oi0.data);

//OPERATOR_R1_W1_D(cp_d, CP_D_OPERATIONS, nil, OI_MASK_INTE|OI_MASK_LEAF|OI_MASK_TPTR, 1, 1)
OPERATORA_R1_W1_D(cp_d, CP_D_OPERATIONS, nil, OI_MASK_INTE|OI_MASK_LEAF|OI_MASK_TPTR, 1, 1)


#define UNLOAD_D_OPERATIONS \
    result0 = _unload_fast(this_bvm, oi0.data);

OPERATORA_R1_W1_D(unload_d, 
        UNLOAD_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define MSIZE_D_OPERATIONS \
    result0 = _val(this_bvm, _mu(this_bvm, oi0.data) );

OPERATORA_R1_W1_D(msize_d, 
        MSIZE_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define MU_D_OPERATIONS \
    result0 = _val(this_bvm, BYTE_SIZE( _mu(this_bvm, oi0.data) ) );

OPERATORA_R1_W1_D(mu_d, 
        MU_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define SFIELD_D_OPERATIONS \
    result0 = _val(this_bvm, s(oi0.data) );

OPERATORA_R1_W1_D(sfield_d, 
        SFIELD_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define NVA_D_OPERATIONS \
    result0 = _val(this_bvm, _nva(this_bvm, oi0.data) );

OPERATORA_R1_W1_D(nva_d, 
        NVA_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define NPT_D_OPERATIONS \
    result0 = _val(this_bvm, _nptr(this_bvm, oi0.data) );

OPERATORA_R1_W1_D(npt_d, 
        NPT_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define NLF_D_OPERATIONS \
    result0 = _val(this_bvm, _nlf(this_bvm, oi0.data) );

OPERATORA_R1_W1_D(nlf_d, 
        NLF_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define NIN_D_OPERATIONS \
    result0 = _val(this_bvm, _nin(this_bvm, oi0.data) );

OPERATORA_R1_W1_D(nin_d, 
        NIN_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define NTAG_D_OPERATIONS \
    result0 = _val(this_bvm, _ntag(this_bvm, oi0.data) );

OPERATORA_R1_W1_D(ntag_d, 
        NTAG_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define ISVAL_D_OPERATIONS \
    result0 = _val(this_bvm, is_leaf(oi0.data));

OPERATORA_R1_W1_D(isval_d, 
        ISVAL_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define ISPTR_D_OPERATIONS \
    result0 = _val(this_bvm, is_inte(oi0.data));

OPERATORA_R1_W1_D(isptr_d, 
        ISPTR_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define ISTAG_D_OPERATIONS \
    result0 = _val(this_bvm, is_tptr(oi0.data));

OPERATORA_R1_W1_D(istag_d, 
        ISTAG_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define NEWPTR_D_OPERATIONS \
    result0 = _newin(this_bvm, *oi0.data);

OPERATORA_R1_W1_D(newptr_d, 
        NEWPTR_D_OPERATIONS, 
        nil, 
        OI_MASK_LEAF, 1, 1)

//_hash8(this_bvm,bpdl_label)
#define NEWTAG_D_OPERATIONS \
    result0 = tptr_new(this_bvm, _hash8( this_bvm, oi0.data ), oi1.data);

OPERATORA_R2_W1_D(newtag_d, 
        NEWTAG_D_OPERATIONS, 
        nil, OI_MASK_LEAF, 1, 1,
        nil, OI_MASK_ANY, 1, 1)


#define GET_TAG_D_OPERATIONS \
    result0 = tptr_extract_hash(this_bvm, oi0.data);

OPERATORA_R1_W1_D(get_tag_d, 
        GET_TAG_D_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define TAGEQ_D_OPERATIONS \
    result0 = _val(this_bvm, tageq(oi0.data, oi1.data, TAG_SIZE));

OPERATORA_R2_W1_D(tageq_d, 
        TAGEQ_D_OPERATIONS, 
        nil, OI_MASK_TPTR|OI_MASK_LEAF, 1, 1,
        nil, OI_MASK_TPTR|OI_MASK_LEAF, 1, 1)


#define IS_NIL_OPERATIONS \
    result0 = _val(this_bvm, is_nil(oi0.data));

OPERATORA_R1_W1_D(is_nilop, 
        IS_NIL_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0)


#define IS_FALSE_OPERATIONS \
    result0 = _val(this_bvm, is_false(oi0.data));

OPERATORA_R1_W1_D(is_falseop, 
        IS_FALSE_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define IS_TRUE_OPERATIONS \
    result0 = _val(this_bvm, !is_false(oi0.data));

OPERATORA_R1_W1_D(is_trueop, 
        IS_TRUE_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define IS_REJECT_OPERATIONS \
    result0 = _val(this_bvm, is_reject(oi0.data));

OPERATORA_R1_W1_D(is_rejectop, 
        IS_REJECT_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define IS_ACCEPT_OPERATIONS \
    result0 = _val(this_bvm, !is_reject(oi0.data));

OPERATORA_R1_W1_D(is_acceptop, 
        IS_ACCEPT_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)

#define SPAN_OPERATIONS \
    result0 = _bs2ar(this_bvm, oi0.data);

OPERATORA_R1_W1_D(span_d, 
        SPAN_OPERATIONS, 
        nil, 
        OI_MASK_ANY, 1, 1)


#define BSCMP_OPERATIONS \
    result0 = _val(this_bvm, _bscmp(this_bvm, oi0.data, oi1.data));

OPERATORA_R2_W1_D(bscmp_d, 
        BSCMP_OPERATIONS, 
        nil, OI_MASK_ANY, 1, 1,
        nil, OI_MASK_ANY, 1, 1)



// Clayton Bauman 2014

