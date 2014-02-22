// bstruct.c
//

#include "babel.h"
#include "bstruct.h"
#include "load.h"
//#include "bvm_opcodes.h"
#include "stack.h"
#include "bvm.h"
#include "array.h"
#include "list.h"
#include "except.h"
#include "io.h"
#include "hash.h"
#include "alloc.h"
#include "tptr.h"
#include "string.h"
#include "mem.h"
#include "util.h"

/* bstruct operator
**unload**
> Unloads a bstruct to an offset-relative form - undoes load
*/

/* bstruct operator
**load**
> Flattens a relative-offset Babel structure into its literal 
> memory pointer footprint. This operator is applied by the 
> interpreter to a Babel file when it is initially loaded from file 
> into memory.
*/

/* bstruct operator
**dump**
> Prints TOS to STDOUT in Graphviz dot-language format
*/

/* bstruct operator
**show** ($)
> Prints TOS to string format
*/

/* bstruct operator
**msize**
> Puts MWORD_SIZE on TOS
*/

// recursively cleans a bstruct after traversal
//
void rclean(bvm_cache *this_bvm, mword *bs){ // rclean#

    int i;
    mword bs_size;

//trace;

    if( !(s(bs) & CTL_MASK) ){ //Already cleaned
        return;
    }

    s(bs) = s(bs) & ~CTL_MASK; //Mark clean

//_mema(bs);

    if( is_inte(bs) ){
//trace;

        int num_elem = size(bs);
        for(i=0; i<num_elem; i++){
            rclean(this_bvm, (mword *)*(bs+i));
        }
    }
    else if(is_tptr(bs)){
//trace;
        rclean(this_bvm, bs+HASH_SIZE+1);
    }

}

//
//
void _recurse(bvm_cache *this_bvm, mword *bs, bstruct_op_fn_ptr bfn, void *v){ // _recurse#

    _fn_recurse(this_bvm, bs, bfn, v);
    rclean(this_bvm, bs);

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
        MARK_TRAVERSED(bs);
        _fn_recurse(this_bvm, bs+HASH_SIZE+1,bfn,v);
    }
    else{
        MARK_TRAVERSED(bs);
    }
    return 1;

}

/* bstruct operator
**mu**
> "Memory usage" - by analogy to Unix 'du'
> Returns the total memory usage of the object on TOS measured in 
> mwords.  
> `{X}| -> {mu(X)}|`  
> `[X]| -> {mu(X)}|`  
>
> This operator is recursive  
*/
//NOTE: If the expected total is off by 7, don't forget to 
//count nil...
bvm_cache *mu(bvm_cache *this_bvm){ // mu#

    mword *result = _newva( this_bvm, _mu(this_bvm, dstack_get(this_bvm,0)));
    popd(this_bvm);

    pushd(this_bvm, result, MORTAL);

    return this_bvm;

}


// _mu -> memory usage (mnemonic: *nix du)
// _mu(this_bvm, x) = _nin(this_bvm, x) + _nlf(this_bvm, x) + _ntag(this_bvm, x)*(HASH_SIZE+1) + _nptr(this_bvm, x) + _nva(this_bvm, x)
//
mword _mu(bvm_cache *this_bvm, mword *bs){ // _mu#

    mword counter=0;
    _recurse(this_bvm, bs, _rmu, &counter);
    return counter;

}

//
//
mword _rmu(bvm_cache *this_bvm, mword *bs, void *v){ // _rmu#

//    if( is_tptr(bs) ){
//        *v += HASH_SIZE;
//    }

    *v += alloc_size(bs)+1;

    return 1;

}

/* bstruct operator
**nlf**
> "Number of leaf-arrays"  
> Returns the total number of leaf-arrays in the object on TOS  
> `{X}| -> {1}|`  
> `[X]| -> nlf(X)|`  
>
> This operator is recursive  
*/
bvm_cache *nlf(bvm_cache *this_bvm){ // nlf#

    mword *result = _newva( this_bvm, _nlf(this_bvm, dstack_get(this_bvm,0)));
    popd(this_bvm);

    pushd(this_bvm, result, MORTAL);

    return this_bvm;

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
        *v += 1;
    }

    return 1;

}


/* bstruct operator
**nin**
> "Number of interior-arrays"  
> Returns the total number of interior-arrays in the object on TOS  
> `{X}| -> {0}|`  
> `[X]| -> nin(X)|`  
>
> This operator is recursive  
*/
bvm_cache *nin(bvm_cache *this_bvm){ // nin#

    mword *result = _newva( this_bvm, _nin(this_bvm, dstack_get(this_bvm,0)));
    popd(this_bvm);

    pushd(this_bvm, result, MORTAL);

    return this_bvm;

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
        *v += 1;
    }

    return 1;

}

/* bstruct operator
**ntag**
> "Number of tags"  
> Returns the total number of tags in the object on TOS  
*/
bvm_cache *ntag(bvm_cache *this_bvm){ // ntag#

    mword *result = _newva( this_bvm, _ntag(this_bvm, dstack_get(this_bvm,0)));
    popd(this_bvm);

    pushd(this_bvm, result, MORTAL);

    return this_bvm;

}

// _ntag -> number of tagged-lists
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
        *v += 1;
    }

    return 1;

}

/* bstruct operator
**nva**
> "Number of values"  
> Returns the total number of values in the object on TOS  
> `{X}| -> {nva(X)}|`  
> `[X]| -> {nva(X)}|`  
>
> This operator is recursive  
*/
bvm_cache *nva(bvm_cache *this_bvm){ // nva#

    mword *result = _newva( this_bvm, _nva(this_bvm, dstack_get(this_bvm,0)));
    popd(this_bvm);

    pushd(this_bvm, result, MORTAL);

    return this_bvm;

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
        *v += size(bs);
    }

    return 1;

}


/* bstruct operator
**npt**
> "Number of pointers"  
> Returns the total number of pointers in the object on TOS  
> `{X}| -> 0|`  
> `[X]| -> npt(X)|`  
>
> This operator is recursive  
*/
bvm_cache *npt(bvm_cache *this_bvm){ // npt#

    mword *result = _newva( this_bvm, _nptr(this_bvm, dstack_get(this_bvm,0)));
    popd(this_bvm);

    pushd(this_bvm, result, MORTAL);

    return this_bvm;

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
        *v += size(bs);
    }

    return 1;

}

// _lst -> generate listing
//
mword _lst(bvm_cache *this_bvm, mword *bs){ // _lst#

    mword counter=0;
    _recurse(this_bvm, bs, _rlst, &counter);
    return 1;

}

//
//
mword _rlst(bvm_cache *this_bvm, mword *bs, void *v){ // _rlst#

    int i;

    printf("%04x %08x\n", (*v * MWORD_SIZE), (s(bs) & ~CTL_MASK) );
    *v += 1;

    if( is_tptr_masked(bs) ){
        for(i=0;HASH_SIZE;i++){
            printf("%04x %08x\n", (*v * MWORD_SIZE), c(bs,i));
            *v += 1;
        }
    }
    else{
        for(i=0;i<size_masked(bs);i++){
            printf("%04x %08x\n", (*v * MWORD_SIZE), c(bs,i));
            *v += 1;
        }
    }

    return 1;

}


//
//
mword *_cp(bvm_cache *this_bvm, mword *bs){ // _cp#

    mword *temp = _unload(this_bvm, bs);

    bs = _load(this_bvm, temp, size(temp));

//    bfree(temp);
    //free(temp-1);

    return bs;    

}


//
// babel_operator
bvm_cache *bbl2str(bvm_cache *this_bvm){ // bbl2str#

//    mword *operand = get_from_stack( this_bvm, TOS_0( this_bvm ) ) ;
    mword *operand = dstack_get(this_bvm, 0);
    popd(this_bvm);

    // Figure out buffer size
    mword initial_buf_size = (16 * _mu(this_bvm, operand));

    //matching free() below
    char *buffer = malloc(initial_buf_size); // XXX WAIVER XXX

    mword buf_size=0;

    buf_size += rbbl2str(this_bvm, operand, buffer+buf_size);
    //buf_size now contains the final string size of the entire graphviz string

    rclean(this_bvm, operand);

    mword last_mword = alignment_word8(this_bvm, buf_size);
    mword length = (buf_size / MWORD_SIZE) + 1;

    if(buf_size % MWORD_SIZE != 0){
        length++;
    }

    mword *temp   = _newlf(this_bvm, length);
    mword *result = temp;

    memcpy(temp, buffer, buf_size);
    c(temp,length-1) = last_mword;
    free(buffer);

    pushd(this_bvm,result,MORTAL);

    return this_bvm;

}

//
//
mword rbbl2str(bvm_cache *this_bvm, mword *bs, char *buffer){ // rbbl2str#

    int i;
    mword buf_size=0;
    mword is_hash_ref=0;

    if( TRAVERSED(bs) ){

        buf_size += sprintf(buffer+buf_size, " ... ");
        return buf_size;

    }

    if( is_nil(bs) ){ // Don't mark nil traversed so it shows up each time

        buf_size += sprintf(buffer+buf_size, " nil ");
        return buf_size;

    }

    int num_entries = size(bs);

//    if( is_href(bs) ){
//
//        MARK_TRAVERSED(bs);
//
//        buf_size += sprintf(buffer+buf_size, "{{ ");
//
//        for(i=0; i<num_entries; i++){
//            buf_size += sprintf(buffer+buf_size, "0x%x ", *(mword *)(bs+i));
//        }
//
//        buf_size += sprintf(buffer+buf_size, "}} ");
//
//        return buf_size; 
//
//    }

    MARK_TRAVERSED(bs);

    #define INTE_OPEN  "(ptr "
    #define INTE_CLOSE ") "
    #define LEAF_OPEN  "(val "
    #define LEAF_CLOSE ") "
    #define TPTR_OPEN  "(tag "
    #define TPTR_CLOSE ") "

    if(is_inte(bs)){

        //buf_size += sprintf(buffer+buf_size, "[ ");
        buf_size += sprintf(buffer+buf_size, INTE_OPEN);

        for(i=0; i<num_entries; i++){
            buf_size += rbbl2str(this_bvm, (mword *)*(bs+i), buffer+buf_size);
        }

        //buf_size += sprintf(buffer+buf_size, "] ");
        buf_size += sprintf(buffer+buf_size, INTE_CLOSE);

    }
    else if(is_leaf(bs)){ // is_leaf

        //buf_size += sprintf(buffer+buf_size, "{ ");
        buf_size += sprintf(buffer+buf_size, LEAF_OPEN);

        for(i=0; i<num_entries; i++){
            buf_size += sprintf(buffer+buf_size, "0x%x ", *(mword *)(bs+i));
        }

        //buf_size += sprintf(buffer+buf_size, "} ");
        buf_size += sprintf(buffer+buf_size, LEAF_CLOSE);

    }
    else{ // is_tptr

        //buf_size += sprintf(buffer+buf_size, "{ ");
        buf_size += sprintf(buffer+buf_size, TPTR_OPEN);

        for(i=0; i<HASH_SIZE; i++){
            buf_size += sprintf(buffer+buf_size, "08x%x ", *(mword *)(bs+i));
        }
        buf_size += rbbl2str(this_bvm, get_tptr(bs), buffer+buf_size);

        //buf_size += sprintf(buffer+buf_size, "} ");
        buf_size += sprintf(buffer+buf_size, TPTR_CLOSE);

    }


    return buf_size;

}


//
//
bvm_cache *bs2gv(bvm_cache *this_bvm){ // bs2gv#

    mword *result = dstack_get(this_bvm,0);
    popd(this_bvm);

    result = _c2b(this_bvm, (char*)_bs2gv(this_bvm, result),1<<20);

    pushd( this_bvm, result, IMMORTAL );

    return this_bvm;

}


//Returns a string containing the Graphviz text file
// To fix _newlf DEPRECATED issue, create two versions:
//      debug version -> uses malloc, not mc_alloc
//      normal version -> uses mc_alloc
mword *_bs2gv(bvm_cache *this_bvm, mword *bs){ // _bs2gv#

    // Figure out buffer size
    // Safety buffer of 2kb + (32 * _mu) XXX: WHY 100?? Ran into problems on this before!!
    //mword initial_buf_size = (1<<11) + (100 * _mu(this_bvm, bs));

    mword initial_buf_size = 2<<16; //FIXME!!!!!!!

    //matching free() below
    char *buffer = malloc(initial_buf_size); // XXX WAIVER XXX
    mword buf_size=0;

    buf_size += sprintf(buffer+buf_size, "digraph babel {\nnode [shape=record];\n");
    buf_size += sprintf(buffer+buf_size, "graph [rankdir = \"LR\"];\n");

    buf_size += rbs2gv(this_bvm, bs, buffer+buf_size);

    buf_size += sprintf(buffer+buf_size, "}\n");
    *(buffer+buf_size) = 0;
    buf_size++;

    //buf_size now contains the final string size of the entire graphviz string

    rclean(this_bvm, bs);

    mword last_mword = alignment_word8(this_bvm, buf_size);
    mword length = (buf_size / MWORD_SIZE) + 1;

    if(buf_size % MWORD_SIZE != 0){
        length++;
    }

    mword *result = _newlf(this_bvm, length); //FIXME DEPRECATED _newlf (see above)
    memcpy(result, buffer, buf_size);
    c(result,length-1) = last_mword;
    free(buffer);

    return result;

}


//
//
mword rbs2gv(bvm_cache *this_bvm, mword *bs, char *buffer){ // rbs2gv#

    int i;
    mword buf_size=0;
    int clipped=0;

    if( TRAVERSED(bs) ){
        return 0;
    }

    int num_entries = size(bs);

//    if( is_href(bs) ){
//        buf_size += sprintf(buffer+buf_size, "s%08x [style=dashed,shape=record,label=\"", (mword)bs);
//        for(i=0; i<HASH_SIZE; i++){
//            buf_size += sprintf(buffer+buf_size, "<f%d> %x", i, *(mword *)(bs+i));
//            if(i<(HASH_SIZE-1)){
//                buf_size += sprintf(buffer+buf_size, "|");
//            }
//        }
//        buf_size += sprintf(buffer+buf_size, "\"];\n");
//
//    }
//    else if(is_inte(bs)){

    if(is_tptr(bs) && !is_nil(bs)){ // is_tptr
        //die;

//        //buf_size += sprintf(buffer+buf_size, "s%08x [style=dashed,shape=record,label=\"{{", (mword)bs);
//        buf_size += sprintf(buffer+buf_size, "s%08x [shape=record,label=\"", (mword)bs);
//
//        for(i=0; i<HASH_SIZE; i++){
//            buf_size += sprintf(buffer+buf_size, "<h%d> %x", i, *(mword *)(bs+i));
//            buf_size += sprintf(buffer+buf_size, "|");
//        }
//
//        buf_size += sprintf(buffer+buf_size, " <f0> 0 |<f1> 1\"];\n");
//
//        for(i=0; i<2; i++){ //There are 2 pointers in a cons: car and cdr
//            if(is_nil((mword *)scar(bs+HASH_SIZE+1+i))){
//                continue;
//            }
//            buf_size += sprintf(buffer+buf_size, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)bs, i, *(mword *)(bs+HASH_SIZE+1+i));
//            buf_size += rbs2gv(this_bvm, (mword *)*(bs+HASH_SIZE+1+i), buffer+buf_size);
//        }
//

        //buf_size += sprintf(buffer+buf_size, "s%08x [style=dashed,shape=record,label=\"{{", (mword)bs);
        buf_size += sprintf(buffer+buf_size, "s%08x [shape=record,label=\"", (mword)bs);

        for(i=0; i<HASH_SIZE; i++){
            buf_size += sprintf(buffer+buf_size, "<f%d> %x", i, *(mword *)(bs+i));
            if(i<(HASH_SIZE-1)){
                buf_size += sprintf(buffer+buf_size, "|");
            }
        }

        buf_size += sprintf(buffer+buf_size, "\"];\n");

        buf_size += sprintf(    buffer+buf_size, 
                                "\"s%08x\":f0 -> \"s%08x\":f0 [arrowhead=\"none\"];\n", 
                                (mword)bs, 
                                (mword *)(bs+HASH_SIZE+1));

        MARK_TRAVERSED(bs);
        buf_size += rbs2gv(this_bvm, (mword *)(bs+HASH_SIZE+1), buffer+buf_size);

    }
    else if(is_inte(bs)){

        MARK_TRAVERSED(bs);

        buf_size += sprintf(buffer+buf_size, "\"s%08x\" [shape=record,label=\"", (mword)bs);
        for(i=0; i<num_entries; i++){
            buf_size += sprintf(buffer+buf_size, "<f%d> %d", i, i);
            if(i<(num_entries-1)){
                buf_size += sprintf(buffer+buf_size, "|");
            }
        }
        buf_size += sprintf(buffer+buf_size, "\"];\n");

        for(i=0; i<num_entries; i++){
            if(is_nil((mword *)lcar(bs+i))){
                continue;
            }
            buf_size += sprintf(buffer+buf_size, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)bs, i, *(mword *)(bs+i));
            buf_size += rbs2gv(this_bvm, (mword *)*(bs+i), buffer+buf_size);
        }

    }
    else{// if(is_leaf(bs)){
        if(num_entries > 8){
            num_entries=8;
            clipped=1;
        }
        else{
            clipped=0;
        }
        buf_size += sprintf(buffer+buf_size, "s%08x [style=bold,shape=record,label=\"", (mword)bs);
        for(i=0; i<num_entries; i++){
            buf_size += sprintf(buffer+buf_size, "<f%d> %x", i, *(mword *)(bs+i));
            if(i<(num_entries-1)){
                buf_size += sprintf(buffer+buf_size, "|");
            }
        }
        if(clipped){
            buf_size += sprintf(buffer+buf_size, "|");
            buf_size += sprintf(buffer+buf_size, "<f%d> ...", i);
        }
        buf_size += sprintf(buffer+buf_size, "\"];\n");
    }

    MARK_TRAVERSED(bs);

    return buf_size;

}

/* bstruct operator
**paste**
> Writes into a bstruct.  
>
> `[X] [Y] Z paste`  
>
> Writes Y into X at offset Z.  
*/
bvm_cache *paste(bvm_cache *this_bvm){ // paste#

    mword index = icar(dstack_get(this_bvm,0));
    mword *src  = dstack_get(this_bvm,1);
    mword *dest = dstack_get(this_bvm,2);

    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);

    _wrcxr(this_bvm, dest, src, index);

    return this_bvm;

}

/* bstruct operator
**set**
> Equivalent for all X,Y:  
>
> `[X] [Y] (0) paste`  
> `[X] [Y] set`  
*/
bvm_cache *set(bvm_cache *this_bvm){ // set#

    mword *src  = dstack_get(this_bvm,0);
    mword *dest = dstack_get(this_bvm,1);

    popd(this_bvm);
    popd(this_bvm);

    if(is_leaf(src) && is_inte(dest) || is_leaf(dest) && is_inte(src)){
        fatal("Both operands must be leaf-array or interior-array");
    }

    c(dest,0) = c(src,0);

    return this_bvm;

}

//
void _wrcxr(bvm_cache *this_bvm, mword *dest, mword *src, mword offset){ // _wrcxr#

    mword dest_size = size(dest);

    if(offset > dest_size-1){
        error("_wrcxr: Can't write past the end of an array");
    }

    dest_size -= offset;
    mword src_size = size(src);
    mword iter = (src_size < dest_size) ? src_size : dest_size; //FIXME macro

    if(         (is_leaf(src) && is_leaf(dest)) 
            ||  (is_inte(src) && is_inte(dest))){ //FIXME perf
        int i;
        for(i=0;i<iter;i++){
            *(dest+i+offset) = c(src,i);
        }
    }
    else{
        error("_wrcxr: Can't write to hash-ref or non-matching arrays");
    }

}


// FIXME: Completely unsafe, no checking...
/* bstruct operator
**move**
>
> `[X] A [Y] B N move`  
>
> Writes N entries of Y from offset B into X at offset A.
> X and Y may be the same array.
*/
bvm_cache *move(bvm_cache *this_bvm){  // *move#

    mword size       = icar(dstack_get(this_bvm,0));
    mword src_index  = icar(dstack_get(this_bvm,1));
    mword *src       = dstack_get(this_bvm,2);
    mword dest_index = icar(dstack_get(this_bvm,3));
    mword *dest      = dstack_get(this_bvm,4);

    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);

    //void * memmove ( void * destination, const void * source, size_t num );
    memmove(dest+dest_index, src+src_index, (size_t)size*MWORD_SIZE);

    return this_bvm;

}




// FIXME: Completely unsafe, no checking...
/* bstruct operator
**move8**
>
> `[X] A [Y] B N move`  
>
> Writes N entries of Y from offset B into X at offset A.
> X and Y may be the same array.
*/
bvm_cache *move8(bvm_cache *this_bvm){  // move8#

    mword size       = icar(dstack_get(this_bvm,0));
    mword src_index  = icar(dstack_get(this_bvm,1));
    char  *src       = (char*)dstack_get(this_bvm,2);
    mword dest_index = icar(dstack_get(this_bvm,3));
    char  *dest      = (char*)dstack_get(this_bvm,4);

    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);

    //void * memmove ( void * destination, const void * source, size_t num );
    memmove(dest+dest_index, src+src_index, (size_t)size);

    return this_bvm;

}


// XXX DEPRECATE
// TOS_0 offset
// TOS_1 dest
// TOS_2 src
// babel_operator
bvm_cache *paste8(bvm_cache *this_bvm){  // *paste8#

    fatal("DEPRECATED");
    _wrcxr8(this_bvm, TOS_1(this_bvm),TOS_2(this_bvm),car(TOS_0(this_bvm)));

    hard_zap(this_bvm);
    swap(this_bvm);
    hard_zap(this_bvm);

    return this_bvm;

}

// XXX DEPRECATED
void _wrcxr8(bvm_cache *this_bvm, mword *dest, mword *src, mword offset){ // _wrcxr8#

    mword dest_size = _arlen8(this_bvm, dest);

    if(offset > dest_size-1){
        error("_wrcxr: Can't write past the end of an array");
    }

    dest_size -= offset;

    mword src_size = _arlen8(this_bvm, src);
    mword iter = (src_size < dest_size) ? src_size : dest_size; //FIXME macro

    char *internal_src  = (char*)src;
    char *internal_dest = (char*)dest;

    if( (is_leaf(dest) && is_leaf(dest)) ){ //FIXME perf
        int i;
        for(i=0;i<iter;i++){
            internal_dest[i+offset] = internal_src[i];
        }
    }
    else{
        error("_wrcxr: Can't write to non-leaf arrays");
    }

}

/* bstruct operator
**trav** (@)
> Traverses a bstruct.  
>
> `X (Y) trav`  
>
> Traverses bstruct X according to the sequence of offsets specified 
> in list Y. Performs an iterated `th`.  
*/
bvm_cache *trav(bvm_cache *this_bvm){ // *trav#

    // XXX BROKEN
    mword *result = _trav(this_bvm, dstack_get(this_bvm,1),dstack_get(this_bvm,0));
    popd(this_bvm);
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

//
mword *_trav(bvm_cache *this_bvm, mword *bs, mword *trav_list){ // *_trav#

    if(is_nil((mword*)cdr(trav_list))){// return bs;
        //return _cxr(bs, car(car(trav_list))); //FIXME cxr change
    }
    else{
        if (!is_inte(bs)){ error("_trav: Can't traverse non-interior array"); die; }

        return _trav(this_bvm, (mword*)c(bs,(mword)car(car(trav_list))),(mword*)cdr(trav_list));
    }

}


/* bstruct operator
**cp**
> Named after the Unix command. Does a deep-copy of a bstruct.  
>
> `[X]| -> [copy-of-X]|`  
>
> Example:  
> `[(0 (1))] cp`  
>
> There is now (0 (1)) on TOS but the original object
> still exists as-is. The original is removed from the
> stack. If you want to have the original and the copy
> both on the stack after cp, you will need to say:
> 
> `dup cp`
>
> This operator is recursive.              
*/
bvm_cache *cp(bvm_cache *this_bvm){ // cp#

    mword *bs  = get_from_udr_stack(this_bvm,this_bvm->dstack_ptr,0); //XXX
        //NOTE: The above line is correct - we CAN'T use dstack_get here
    popd(this_bvm);

    mword *result = _cp(this_bvm, bs);

//    mword *result = _unload(this_bvm, bs);
//
//    result = _load(this_bvm, result,size(result));
//
    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;


}

//
bvm_cache *ducp(bvm_cache *this_bvm){ // *ducp#

    fatal("XXX DEPRECATED");
    mword *temp = TOS_0(this_bvm);

    zap(this_bvm);

    mword *result = _unload(this_bvm, temp);

    push_alloc(this_bvm, result, MORTAL);

    load(this_bvm);

}

/* bstruct operator
**span**  
> Makes a "spanning array" of the bstruct on TOS  
> `X| -> Y`  
> ... where Y is an array containing a pointer into every array and
> href in X  
*/
bvm_cache *span(bvm_cache *this_bvm){ // *span#

    fatal("stack fix not done");
    mword *result = _bs2ar(this_bvm, TOS_0(this_bvm));

    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends on inputs

    return this_bvm;

}

//Creates an interior array with one pointer to each array
//in a bstruct
mword *_bs2ar(bvm_cache *this_bvm, mword *bs){  // *_bs2ar#

    mword num_arrays  = _nin  (this_bvm, bs);
          num_arrays += _nlf  (this_bvm, bs);
//          num_arrays += _nhref(bs);

    mword *arr_list = _newin(this_bvm, num_arrays);
    mword offset = 0;

    rbs2ar(this_bvm, bs, arr_list, &offset);
    rclean(this_bvm, bs);

    return arr_list;

}

//
void rbs2ar(bvm_cache *this_bvm, mword *bs, mword *arr_list, mword *offset){ // rbs2ar#

    int i;

    if( TRAVERSED(bs) ){ //Already dumped
        return;
    }

    int num_elem = size(bs);
    
    c(arr_list,*offset) = (mword)bs;
    *offset = *offset+1;

    if(is_inte(bs)){
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            rbs2ar(this_bvm, (mword*)c(bs,i),arr_list,offset);
        }
    }

    MARK_TRAVERSED(bs);

}

//// Returns a list of matches
//mword *_find(bvm_cache *this_bvm, mword *addr){
//
//    mword *result = _rfind(this_bvm, bs);
//    rclean(this_bvm, bs);
//
//    return size;
//
//}
//
//mword *_rfind(bvm_cache *this_bvm, mword *bs){
//
//    int i;
//    mword count = 0;
//
//    if( TRAVERSED(bs) ){
//        return 0;
//    }
//
//    int num_elem = size(bs);
//    count = num_elem + 1;
//
//    if(is_inte(bs)){
//        MARK_TRAVERSED(bs);
//        for(i=0; i<num_elem; i++){
//            count += _rmu(this_bvm, (mword *)*(bs+i));
//        }
//    }
//    else{
//        MARK_TRAVERSED(bs);
//    }
//
//    return count;
//
//}



// Clayton Bauman 2012

