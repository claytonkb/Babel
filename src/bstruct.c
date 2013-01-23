// bstruct.c
//

#include "babel.h"
#include "bstruct.h"
#include "load.h"
#include "bvm_opcodes.h"
#include "stack.h"
#include "bvm.h"
#include "array.h"
#include "list.h"
#include "except.h"
#include "io.h"
#include "hash.h"

// recursively cleans a bstruct after traversal
void rclean(mword *bs){

    int i;
    mword bs_size;

    if( !(s(bs) & CTL_MASK) ){ //Already cleaned
        return;
    }

    s(bs) = s(bs) & ~CTL_MASK; //Mark clean

    if( is_inte(bs) ){
        int num_elem = size(bs);
        for(i=0; i<num_elem; i++){
            rclean((mword *)*(bs+i));
        }

    }

}

//
// babel_operator
bvm_cache *bbl2str(bvm_cache *this_bvm){

    mword *operand = get_from_stack( this_bvm, TOS_0( this_bvm ) ) ;

    // Figure out buffer size
    mword initial_buf_size = (16 * _mu(operand));
    char *buffer = malloc(initial_buf_size); //FIXME: malloc

    mword buf_size=0;

    buf_size += rbbl2str(operand, buffer+buf_size);
    //buf_size now contains the final string size of the entire graphviz string

    rclean(operand);
//    hard_zap(this_bvm);
    zap(this_bvm);

    mword last_mword = alignment_word8(buf_size);
    mword length = (buf_size / MWORD_SIZE) + 1;

    if(buf_size % MWORD_SIZE != 0){
        length++;
    }

    mword *temp   = _newlf(length);
    mword *result = _newin(1);
    (mword*)*result = temp;

    memcpy(temp, buffer, buf_size);
    c(temp,length-1) = last_mword;
    free(buffer);

    push_alloc(this_bvm,result,MORTAL);

    return this_bvm;

}

//
//
mword rbbl2str(mword *bs, char *buffer){

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

    if(is_inte(bs)){

        buf_size += sprintf(buffer+buf_size, "[ ");

        for(i=0; i<num_entries; i++){
            buf_size += rbbl2str((mword *)*(bs+i), buffer+buf_size);
        }

        buf_size += sprintf(buffer+buf_size, "] ");

    }
    else{ // is_leaf

        buf_size += sprintf(buffer+buf_size, "{ ");

        for(i=0; i<num_entries; i++){
            buf_size += sprintf(buffer+buf_size, "0x%x ", *(mword *)(bs+i));
        }

        buf_size += sprintf(buffer+buf_size, "} ");

    }

    return buf_size;

}

//
// babel_operator
bvm_cache *bs2gv(bvm_cache *this_bvm){

//    _dump(this_bvm->stack_ptr)
//        die

    mword *result = new_atom;
    (mword*)*result = _bs2gv(TOS_0(this_bvm));

    zap(this_bvm);
    
    push_alloc(this_bvm,result,MORTAL);

    return this_bvm;

}

//Returns a string containing the Graphviz text file
mword *_bs2gv(mword *bs){

    // Figure out buffer size
    // Safety buffer of 2kb + (32 * _mu) XXX: WHY 100?? Ran into problems on this before!!
    //mword initial_buf_size = (1<<11) + (100 * _mu(bs));

    mword initial_buf_size = 2<<16;

    char *buffer = malloc(initial_buf_size); //FIXME: malloc
    mword buf_size=0;

    buf_size += sprintf(buffer+buf_size, "digraph babel {\nnode [shape=record];\n");
    buf_size += sprintf(buffer+buf_size, "graph [rankdir = \"LR\"];\n");

    buf_size += rbs2gv(bs, buffer+buf_size);

    buf_size += sprintf(buffer+buf_size, "}\n");
    //buf_size now contains the final string size of the entire graphviz string

    rclean(bs);

    mword last_mword = alignment_word8(buf_size);
    mword length = (buf_size / MWORD_SIZE) + 1;

    if(buf_size % MWORD_SIZE != 0){
        length++;
    }

    mword *result = _newlf(length);
    memcpy(result, buffer, buf_size);
    c(result,length-1) = last_mword;
    free(buffer);

    return result;

}

//
mword rbs2gv(mword *bs, char *buffer){

    int i;
    mword buf_size=0;
    int clipped=0;

    die;

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
    if(is_inte(bs)){

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
            if(is_nil((mword *)scar(bs+i))){
                continue;
            }
            buf_size += sprintf(buffer+buf_size, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)bs, i, *(mword *)(bs+i));
            buf_size += rbs2gv((mword *)*(bs+i), buffer+buf_size);
        }

    }
    else{ // is_leaf
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

// TOS_0 offset
// TOS_1 dest
// TOS_2 src
// babel_operator
bvm_cache *paste(bvm_cache *this_bvm){

    fatal("stack fix not done");
    _wrcxr(TOS_1(this_bvm),TOS_2(this_bvm),car(TOS_0(this_bvm)));

    hard_zap(this_bvm);
    swap(this_bvm);
    hard_zap(this_bvm);

    return this_bvm;

}

// TOS_0 dest
// TOS_1 src
// babel_operator
bvm_cache *set(bvm_cache *this_bvm){

    mword *src  = get_from_stack( this_bvm, TOS_0( this_bvm ) ) ;
    mword *dest = get_from_stack( this_bvm, TOS_1( this_bvm ) ) ;

    _dump(dest);
    die;

    *dest = *src;

    return this_bvm;

}

//
void _wrcxr(mword *dest, mword *src, mword offset){

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

// TOS_0 offset
// TOS_1 dest
// TOS_2 src
// babel_operator
bvm_cache *paste8(bvm_cache *this_bvm){

    fatal("stack fix not done");
    _wrcxr8(TOS_1(this_bvm),TOS_2(this_bvm),car(TOS_0(this_bvm)));

    hard_zap(this_bvm);
    swap(this_bvm);
    hard_zap(this_bvm);

    return this_bvm;

}

//
void _wrcxr8(mword *dest, mword *src, mword offset){

    mword dest_size = _arlen8(dest);

    if(offset > dest_size-1){
        error("_wrcxr: Can't write past the end of an array");
    }

    dest_size -= offset;

    mword src_size = _arlen8(src);
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

//
// babel_operator
bvm_cache *trav(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _trav(TOS_1(this_bvm),TOS_0(this_bvm));

    hard_zap(this_bvm);
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends on inputs

    return this_bvm;

}

//
mword *_trav(mword *bs, mword *trav_list){

    if(is_nil((mword*)cdr(trav_list))){// return bs;
        return _cxr(bs, car(car(trav_list)));
    }
    else{
        if (!is_inte(bs)){ error("_trav: Can't traverse non-interior array"); die; }

        return _trav((mword*)c(bs,(mword)car(car(trav_list))),(mword*)cdr(trav_list));
    }

}

//
// babel_operator
bvm_cache *mu(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = _mu(TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;
    
//    printf("size %d\n", size_tree(global_VM));

}

// Returns the size of a bstruct in mwords
// mu -> mem usage (mnemonic: *nix du)
mword _mu(mword *bs){

    mword size = _rmu(bs);
    rclean(bs);

    return size;

}

mword _rmu(mword *bs){

    int i;
    mword count = 0;

    if( TRAVERSED(bs) ){
        return 0;
    }

    int num_elem = size(bs);
    count = num_elem + 1;

    if(is_inte(bs)){
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            count += _rmu((mword *)*(bs+i));
        }
    }
    else{
        MARK_TRAVERSED(bs);
    }

    return count;

}

//
// babel_operator
bvm_cache *nlf(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = _nlf(TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
mword _nlf(mword *bs){

    mword size = _rnlf(bs);
    rclean(bs);

    return size;

}

//
mword _rnlf(mword *bs){

    int i;
    mword count = 0;

    if( TRAVERSED(bs) ){
        return 0;
    }

    if(is_inte(bs)){
        int num_elem = size(bs);
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            count += _rnlf((mword *)*(bs+i));
        }
    }
//    else if(is_href(bs)){
//        MARK_TRAVERSED(bs);
//        count = 0;
//    }
    else if(is_leaf(bs)){ // is_leaf
        MARK_TRAVERSED(bs);
        count = 1;
    }

    return count;

}

//
// babel_operator
bvm_cache *nhref(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = _nhref(TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
mword _nhref(mword *bs){

    mword size = _rnhref(bs);
    rclean(bs);

    return size;

}

//
mword _rnhref(mword *bs){

    int i;
    mword count = 0;

    if( TRAVERSED(bs) ){
        return 0;
    }

    if(is_inte(bs)){
        int num_elem = size(bs);
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            count += _rnhref((mword *)*(bs+i));
        }
    }
//    else if(is_href(bs)){
//        MARK_TRAVERSED(bs);
//        count = 1;
//    }
    else if(is_leaf(bs)){
        MARK_TRAVERSED(bs);
        count = 0;
    }


    return count;

}

//
// babel_operator
bvm_cache *nin(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = _nin(TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
mword _nin(mword *bs){

    mword size = _rnin(bs);
    rclean(bs);

    return size;

}

//
mword _rnin(mword *bs){

    int i;
    mword count = 0;

    if( TRAVERSED(bs) ){
        return 0;
    }

//    if(is_leaf(bs) || is_href(bs)){
    if(is_leaf(bs)){
        MARK_TRAVERSED(bs);
        count = 0;
    }
    else{
        int num_elem = size(bs);
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            count += _rnin((mword *)*(bs+i));
        }
        count++;
    }

    return count;

}

//
// babel_operator
bvm_cache *nva(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = _nva(TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
mword _nva(mword *bs) {

    mword size = _rnva(bs);
    rclean(bs);

    return size;

}

// 
mword _rnva(mword *bs){

    int i;
    mword count = 0;

    if( TRAVERSED(bs) ){
        return 0;
    }

    int num_elem = size(bs);

    if(is_inte(bs)){
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            count += _rnva((mword *)*(bs+i));
        }
    }
    else if(is_leaf(bs)){
        count = num_elem;
    }

    return count;

}


//
// babel_operator
bvm_cache *nhword(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = _nhword(TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

//
mword _nhword(mword *bs) {

    mword size = _rnhword(bs);
    rclean(bs);

    return size;

}

// 
mword _rnhword(mword *bs){

    int i;
    mword count = 0;

    if( TRAVERSED(bs) ){
        return 0;
    }

    if(is_inte(bs)){
        int num_elem = size(bs);
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            count += _rnhword((mword *)*(bs+i));
        }
    }
//    else if(is_href(bs)){
//        count = size(bs);
//        MARK_TRAVERSED(bs);
//    }
    else if(is_leaf(bs)){
        MARK_TRAVERSED(bs);
        count = 0;
    }

    return count;

}


//
// babel_operator
bvm_cache *npt(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;

    *result = _npt(TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
mword _npt(mword *bs) {

    mword size = _rnpt(bs);
    rclean(bs);

    return size;

}

// 
mword _rnpt(mword *bs){

    int i;
    mword count = 0;

    if( TRAVERSED(bs) ){
        return 0;
    }

    if(is_inte(bs)){
        int num_elem = size(bs);
        MARK_TRAVERSED(bs);
        for(i=0; i<num_elem; i++){
            count += _rnpt((mword *)*(bs+i));
        }
        count += num_elem;
    }

    return count;

}

//
// babel_operator
bvm_cache *cp(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _unload(TOS_0(this_bvm));
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL);

    load(this_bvm);

}

//
bvm_cache *ducp(bvm_cache *this_bvm){

    fatal("deprecated");
    mword *temp = TOS_0(this_bvm);

    zap(this_bvm);

    mword *result = _unload(temp);

    push_alloc(this_bvm, result, MORTAL);

    load(this_bvm);

}

//
// babel_operator
bvm_cache *span(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _bs2ar(TOS_0(this_bvm));

    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends on inputs

    return this_bvm;

}

//Creates an interior array with one pointer to each array
//in a bstruct
//TODO: Look into re-writing load/unload to utilize bs2ar...
mword *_bs2ar(mword *bs){

    mword num_arrays  = _nin  (bs);
          num_arrays += _nlf  (bs);
          num_arrays += _nhref(bs);

    mword *arr_list = _newin(num_arrays);
    mword offset = 0;

    rbs2ar(bs, arr_list, &offset);
    rclean(bs);

    return arr_list;

}

//
void rbs2ar(mword *bs, mword *arr_list, mword *offset){

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
            rbs2ar((mword*)c(bs,i),arr_list,offset);
        }
    }

    MARK_TRAVERSED(bs);

}

//// Returns a list of matches
//mword *_find(bvm_cache *this_bvm, mword *addr){
//
//    mword *result = _rfind(this_bvm, bs);
//    rclean(bs);
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
//            count += _rmu((mword *)*(bs+i));
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

