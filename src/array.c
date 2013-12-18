// array.c
//

#include "babel.h"
#include "array.h"
#include "list.h"
#include "stack.h"
#include "bstruct.h"
//#include "bvm_opcodes.h"
#include "except.h"
#include "bvm.h"
#include "load.h"
#include "alloc.h"
#include "tptr.h"
#include "mem.h"

/* 
**th**  
> Returns the xth element of array A:  
>
> `{A} {x}| -> {A[x]}|`    
> `[A] {x}| -> [A[x]]|`    
>
> Note that th is a degenerate case of slice:  
> `[x] th <==> [x] [x] slice`    
>
> It is also equivalent to trav:  
>
> `[x] th <==> ((x)) trav`    
*/

/* array operator
**paste8/paste1**  
*/

/* array operator
**move**  
> an in-array memmove  
> `A x y z| -> B  (A[x:y] pasted at A[z], all other words same)`    
*/

/* array operator
**move8**  
> See move  
*/

/* array operator
**cut8**
*/

/* array operator
**sfield**
> Reads the s-field and places it on TOS  
> `{X}| -> {s(X)}|`    
> `[X]| -> {s(X)}|`    
*/
bvm_cache *sfield(bvm_cache *this_bvm){ // sfield#

#define babel_sfield_operator \
    result = _newva( s( op0 ) );

    babel_operator_typeA( 
            this_bvm, 
            babel_sfield_operator );

}


/* array operator
**size** (#)
> Puts the array size on TOS  
> `{X}| -> {size(X)}|`    
> `[X]| -> {size(X)}|`    
*/
bvm_cache *arlen(bvm_cache *this_bvm){ // arlen#

#define babel_arlen_operator \
    result = _newva( size( op0 ) );

    babel_operator_typeA( 
            this_bvm, 
            babel_arlen_operator );

}


/* array operator
**lf?** 
> Tests if TOS is a leaf-array  
> `{X}| -> {1}|`    
> `[X]| -> {0}|`    
*/
bvm_cache *islf(bvm_cache *this_bvm){ // islf#

#define babel_islf_operator \
    result = _newva( is_leaf( op0 ) );

    babel_operator_typeA( 
            this_bvm, 
            babel_islf_operator );

}

/* array operator
**in?**
> Tests if TOS is an interior-array  
> `{X}| -> {0}|`    
> `[X]| -> {1}|`    
*/
bvm_cache *isinte(bvm_cache *this_bvm){ // isinte#

#define babel_isinte_operator \
    result = _newva( is_inte( op0 ) );

    babel_operator_typeA( 
            this_bvm, 
            babel_isinte_operator );

}

// creates a new leaf-array of given size
//
mword *_newlf(mword size){ // _newlf#

    mword *ptr = malloc( MWORDS(size+1) );
    if(ptr == NULL){
        error("_newlf: malloc returned NULL");
    }

    ptr[0] = size * MWORD_SIZE;

    return ptr+1;

}


// same as _newlf but with mem initialization (byte-wise)
//
mword *_newlfi(mword size, mword init){ // _newlfi#

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

}


// Accepts a data value and returns a leaf-array
// of size 1 containing that data value
//
mword *_newva(mword value){ // _newva#

//    mword *ptr = malloc( MWORDS(1) );
//
//    if(ptr == NULL){
//        error("_newva: malloc returned NULL");
//    }

    mword *ptr = mc_alloc(MWORDS(1));

    //ptr[0] = MWORD_SIZE;
    //ptr[1] = value;

    //return ptr+1;

    c(ptr,0) = value;
    return ptr;

}


//
//
mword *_newin(mword size){ // _newin#

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
mword *_newtptr(void){ // _newtptr# XXX DEPRECATE

    mword *ptr = malloc( MWORDS( TPTR_SIZE ) );

    if(ptr == NULL){
        error("_newtptr: malloc returned NULL");
    }

    //FIXME: 32-bit specific and UGLY
    ptr[0] = 0;
    ptr[1] = 0xdeadbeef;
    ptr[2] = 0xdeadbeef;
    ptr[3] = 0xdeadbeef;
    ptr[4] = 0xdeadbeef;
    ptr[5] = (mword)(-1*MWORD_SIZE);
    ptr[6] = (mword)nil;
//    ptr[7] = (mword)nil;

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

/* > XXX: DEPRECATE (redundant to cut)
**slice**
> Puts a slice of an array on TOS  
> `{X} {a} {b}| -> {X[a..b]}|`    
> `[X] {a} {b}| -> [X[a..b]]|`    
>
> Read `[a..b] as "from a to b"`   
*/
bvm_cache *slice(bvm_cache *this_bvm){

    //FIXME: There are lots of ways to break this code with
    //bad inputs
    mword start  = icar(dstack_get(this_bvm,1));
    mword end    = icar(dstack_get(this_bvm,0));
    mword *array = dstack_get(this_bvm,2);
    mword *result;

    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);

    end = (end > size(array)) ? size(array) : end;

    if(end<=start){
        result = nil;
    }
    else{

        if(is_leaf(array)){
            result = _newlf(end-start);
        }
        else{
            result = _newin(end-start);
        }

        mword i,j;
        for(    i=start, j=0;
                i<end;
                i++,j++
            ){

            c(result,j) = c(array,i);

        }    
    }

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

//
//
mword _cxr1(mword *val, mword bit){ // _cxr1#

    mword mword_select = bit/(MWORD_BIT_SIZE);
    mword bit_offset = bit % MWORD_BIT_SIZE;

    if(mword_select > size(val)-1){
        error("_cxr1 error") 
    };

    return (c(val,mword_select) & (1<<bit_offset)) >> bit_offset;

}


//
//
bvm_cache *th(bvm_cache *this_bvm){ // th#

    mword *array = dstack_get(this_bvm,1);
    mword index  = icar(dstack_get(this_bvm,0));

    popd(this_bvm);
    popd(this_bvm);

    pushd(this_bvm, _th(array,index), IMMORTAL);

    return this_bvm;

}

//
//
mword *_th(mword *bs, mword entry){ // _th#

    if(is_leaf(bs)){
        return _newva(c(bs,entry%size(bs)));
    }
    
    //we are assuming that we never get a tptr...
    return (mword*)c(bs,entry%size(bs));

}


/* array operator
**cut**  
> Cuts an array - opposite of cat  
>
> The cut operator only cuts in one place. If you want more complex
> behavior, convert the array to a list, then use append/part  
>
> `A 0| -> nil A|`    
> `A x| -> A[0:(x-1)] A[x:len(A)]|     0 < x < len(A)`    
> `A len(A)| -> A nil|`    
>
> Note that cut does not use entry-addressing. If you cut A at 0, then
> you will always get 'nil A' on the stack. The cut operator uses
> boundary-addressing instead, where the boundary before entry 0 is
> defined as the 0th boundary and the boundary between entry 0 and
> entry 1 is the 1st boundary, and so on.  
*/
bvm_cache *cut(bvm_cache *this_bvm){ // cut#

    mword *result_pre;
    mword *result_post;
    mword i;

    mword cut_point = icar(dstack_get(this_bvm,0));
    mword *src      = dstack_get(this_bvm,1);

    //hard_zap(this_bvm);
    popd(this_bvm);

    popd(this_bvm);

    if(cut_point == 0){
        pushd(this_bvm, nil, IMMORTAL);
        swap(this_bvm);
    }
    else if(cut_point >= size(src)){
        pushd(this_bvm, nil, IMMORTAL);
    }
    else{

        popd(this_bvm);

        mword num_entries = size(src);

        if(is_leaf(src)){
            result_pre  = _newlf(cut_point);
            result_post = _newlf(num_entries-cut_point);
        }
        else{
            result_pre = _newin(cut_point);
            result_post = _newin(num_entries-cut_point);
        }

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

        pushd(this_bvm, result_pre, IMMORTAL);
        pushd(this_bvm, result_post, IMMORTAL);

    }

    return this_bvm;

}


/* array operator
**size8** (#8)  
> Puts the 8-bit array length on TOS  
> `{X}| -> {size8(X)}|`    
> `[X]| -> {size8(X)}|`    
*/
bvm_cache *arlen8(bvm_cache *this_bvm){ // arlen8#

#define babel_arlen8_operator \
    result = _newva( _arlen8( op0 ) );

    babel_operator_typeA( 
            this_bvm, 
            babel_arlen8_operator );

}


// 
//
mword _arlen8(mword *string){ // _arlen8#

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


/* array operator
**newin**  
> Creates a new interior-array of size x and leaves it on TOS  
> `{x}| -> {interior-array-of-size-x}|`    
*/
bvm_cache *newin(bvm_cache *this_bvm){

    mword *result = _newin(icar(dstack_get(this_bvm,0))); //FIXME: There is no checking...

    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* array operator
**newlf**  
> Creates a new leaf-array of size x and leaves it on TOS  
> `{x}| -> {leaf-array-of-size-x}|`    
*/
bvm_cache *newlf(bvm_cache *this_bvm){

    mword *result = _newlf((mword)car(dstack_get(this_bvm,0))); //FIXME: There is no checking...
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* XXX DEPRECATE (not aesthetic)
**trunc**  
> Truncates an array X to length y:  
> `{X} {y}| -> {X[0..y]}|`    
> `[X] {y}| -> [X[0..y]]|`    
*/
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


//Returns an alignment word based on size8
//
mword alignment_word8(mword size8){

    if(size8 % MWORD_SIZE == 0)
        return 0;

    mword alignment = (mword)-1;

    return alignment << ((size8 % MWORD_SIZE) * BITS_PER_BYTE);

}


// Decodes the alignment word
//
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

/* array operator
**cat**
> Concatenate two arrays of the same type. If you want more complex   
> behavior, convert your arrays to lists, then use append/part  
*/
bvm_cache *arcat(bvm_cache *this_bvm){

    mword *result;

    mword *left  = dstack_get(this_bvm,1);
    mword *right = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

    mword size_left  = size(left);
    mword size_right = size(right);

    if      ( is_leaf(right)  &&  is_leaf(left) ){
        result = _newlf( size_left + size_right );
    }
    else if ( is_inte(right)  &&  is_inte(left) ){
        result = _newin( size_left + size_right );
    }
    else{ //FIXME: Throw an exception
        fatal("arcat: cannot concatenate leaf array and interior array");
    }

    mword i,j;
    for(    i=0;
            i<size_left;
            i++
        ){

        c((mword*)result,i) = c(left,i);

    }    

    for(    i=0,j=size_left;
            i<size_right;
            i++,j++
        ){

        c(result,j) = c(right,i);

    }    

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

mword array8_size(mword size8){ // array8_size#

    mword size = size8 / MWORD_SIZE;

    if(size8 % MWORD_SIZE != 0){ //XXX Assumes the int div rounds to floor
        size++;
    }

    return size+1; //for the alignment_word

}

/* array operator
**cat8** (.)
> Concatenate two arrays of the same type. If you want more complex   
> behavior, convert your arrays to lists, then use append/part  
*/
bvm_cache *arcat8(bvm_cache *this_bvm){ // arcat8#

    char *result;

    mword *left  = dstack_get(this_bvm,1);
    mword *right = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

    mword size_left  = _arlen8(left);
    mword size_right = _arlen8(right);
    mword total_size = size_left + size_right;
    mword array8_total_size = array8_size(total_size);

//    d(size_left);
//    d(size_right);
//    d(array8_size(total_size));

    if      ( is_leaf(right)  &&  is_leaf(left) ){
        result = (char*)_newlf( array8_total_size );
    }
    else{ //FIXME: Throw an exception
        fatal("arcat8: cannot concatenate interior array");
    }

    //FIXME: memcpy!
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

    c((mword*)result,array8_total_size-1) = alignment_word8(total_size);

    pushd(this_bvm, (mword*)result, IMMORTAL);

    return this_bvm;

}


/* XXX DEPRECATE (use cut8 instead)
**slice8**  
> TOS_0 to  
> TOS_1 from  
> TOS_2 operand  
*/
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

/* array operator
**cmp** (==)  
> does a memcmp  
> `A B| -> { 0,  A==B`    
> `        {-1,  A < B`    
> `        { 1,  A > B`    
*/
bvm_cache *arcmp(bvm_cache *this_bvm){ // arcmp#

    mword *result = _newva(1);//    = new_atom;

    mword *left = dstack_get(this_bvm,1);
    mword *right = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

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

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
int _arcmp(mword *left, mword *right){ // _arcmp#

    if(size(left) > size(right)){
        return 1;
    }
    else if(size(left) < size(right)){
        return -1;
    }

    return memcmp(left, right, size(left)*MWORD_SIZE);

}

/* array operator
**ar2ls**  
> Convert array to list. For interior-arrays, undoes bons.  
>
> Leaf-array on TOS:  
> `{1 2 3}| -> (1 2 3)|`    
>
> Interior-array on TOS:  
> `[a b c]| -> (a b c)|`    
  
*/
bvm_cache *ar2ls(bvm_cache *this_bvm){ // ar2ls#

#define babel_ar2ls_operator \
    result = _ar2ls(dstack_get(this_bvm,0));

    babel_operator_typeA( 
            this_bvm, 
            babel_ar2ls_operator );

}


//
//
mword *_ar2ls(mword *arr){ // _ar2ls#

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

/* array operator
**perm**  
> Permutes an array  
>
> `['a' 'b' 'c' 'd'] [ 3 1 2 0 ] perm --> ['d' 'b' 'c' 'a']`    
>
> Note that the operator is actually a multi-select, not a permute:  
>
> `['a' 'b' 'c' 'd'] [ 3 1 1 0 ] perm --> ['d' 'b' 'b' 'a']`    
*/
bvm_cache *perm(bvm_cache *this_bvm){ // perm#

    mword *result;

    mword *src_array   = dstack_get(this_bvm,1);
    mword *perm_matrix = dstack_get(this_bvm,0);

    popd(this_bvm);
    popd(this_bvm);

    if(is_inte(src_array)){
        result = _newin(size(src_array));
    }
    else if(is_leaf(src_array)){
        result = _newlf(size(src_array));
    }
    else{ // FIXME: Throw exception
        fatal("perm: !is_leaf && !is_inte");
    }

    _perm(src_array, result, perm_matrix);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
void _perm(mword *src, mword *dest, mword *perm_matrix){ // _perm#

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

