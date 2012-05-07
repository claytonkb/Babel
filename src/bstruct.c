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

// FIXME: Doesn't handle hash-refs
bvm_cache *bbl2str(bvm_cache *this_bvm){

    // Figure out buffer size
    mword initial_buf_size = (16 * _mu((mword*)TOS_0(this_bvm)));
    char *buffer = malloc(initial_buf_size); //FIXME: malloc

    mword buf_size=0;

    buf_size += rbbl2str((mword*)TOS_0(this_bvm), buffer+buf_size);
    //buf_size now contains the final string size of the entire graphviz string

    rclean((mword*)TOS_0(this_bvm));
//    hard_zap(this_bvm);
    zap(this_bvm);

    mword last_mword = alignment_word8(buf_size);
    mword length = (buf_size / MWORD_SIZE) + 1;

    if(buf_size % MWORD_SIZE != 0){
        length++;
    }

    mword *result = _newlf(length);
    memcpy(result, buffer, buf_size);
    c(result,length-1) = last_mword;
    free(buffer);

    push_alloc(this_bvm,result,BBL2STR);

    

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

    if( is_href(bs) ){

        MARK_TRAVERSED(bs);

        buf_size += sprintf(buffer+buf_size, "{{ ");

        for(i=0; i<num_entries; i++){
            buf_size += sprintf(buffer+buf_size, "0x%x ", *(mword *)(bs+i));
        }

        buf_size += sprintf(buffer+buf_size, "}} ");

        return buf_size; 

    }

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
bvm_cache *bs2gv(bvm_cache *this_bvm){

//    _dump(this_bvm->stack_ptr)
//        die

    mword *result = _bs2gv((mword*)TOS_0(this_bvm));

    zap(this_bvm);
    
    push_alloc(this_bvm,result,BBL2GV);

    

    return this_bvm;

}

//Returns a string containing the Graphviz text file
mword *_bs2gv(mword *bs){

    // Figure out buffer size
    // Safety buffer of 2kb + (32 * _mu) XXX: WHY 32??
    mword initial_buf_size = (1<<11) + (32 * _mu(bs));

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

    if( TRAVERSED(bs) ){
        return 0;
    }

    int num_entries = size(bs);

    if( is_href(bs) ){
        buf_size += sprintf(buffer+buf_size, "s%08x [style=dashed,shape=record,label=\"", (mword)bs);
        for(i=0; i<HASH_SIZE; i++){
            buf_size += sprintf(buffer+buf_size, "<f%d> %x", i, *(mword *)(bs+i));
            if(i<(HASH_SIZE-1)){
                buf_size += sprintf(buffer+buf_size, "|");
            }
        }
        buf_size += sprintf(buffer+buf_size, "\"];\n");

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
            if(is_nil((mword *)scar(bs+i))){
                continue;
            }
            buf_size += sprintf(buffer+buf_size, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)bs, i, *(mword *)(bs+i));
            buf_size += rbs2gv((mword *)*(bs+i), buffer+buf_size);
        }

    }
    else{ // is_leaf
        buf_size += sprintf(buffer+buf_size, "s%08x [style=bold,shape=record,label=\"", (mword)bs);
        for(i=0; i<num_entries; i++){
            buf_size += sprintf(buffer+buf_size, "<f%d> %x", i, *(mword *)(bs+i));
            if(i<(num_entries-1)){
                buf_size += sprintf(buffer+buf_size, "|");
            }
        }
        buf_size += sprintf(buffer+buf_size, "\"];\n");
    }

    MARK_TRAVERSED(bs);

    return buf_size;

}

// TOS_0 offset
// TOS_1 dest
// TOS_2 src
bvm_cache *paste(bvm_cache *this_bvm){

    _wrcxr((mword*)TOS_1(this_bvm),(mword*)TOS_2(this_bvm),car(TOS_0(this_bvm)));

    hard_zap(this_bvm);
    swap(this_bvm);
    hard_zap(this_bvm);

    

    return this_bvm;

}

// TOS_0 dest
// TOS_1 src
bvm_cache *set(bvm_cache *this_bvm){

    _wrcxr((mword*)TOS_0(this_bvm),(mword*)TOS_1(this_bvm),0);

    swap(this_bvm);
    hard_zap(this_bvm);

    

    return this_bvm;

}

//
void _wrcxr(mword *dest, mword *src, mword offset){

    mword dest_size = size(dest);

    if(offset > dest_size){
        error("_wrcxr: Can't write past the end of an array");
    }

    dest_size -= offset;
    mword src_size = size(src);
    mword iter = (src_size < dest_size) ? src_size : dest_size;

    if(         (is_leaf(dest) && is_leaf(dest)) 
            ||  (is_inte(dest) && is_inte(dest))){
        int i;
        for(i=0;i<iter;i++){
            *(dest+i+offset) = c(src,i);
        }
    }
    else{
        error("_wrcxr: Can't write to hash-ref or non-matching arrays");
    }

}

//
bvm_cache *trav(bvm_cache *this_bvm){

    mword *result = _trav((mword*)TOS_1(this_bvm),(mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, TRAV);

    

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
bvm_cache *mu(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = _mu((mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MU);

    

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
bvm_cache *nlf(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = _nlf((mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, NLF);

    

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
    else if(is_href(bs)){
        MARK_TRAVERSED(bs);
        count = 0;
    }
    else if(is_leaf(bs)){ // is_leaf
        MARK_TRAVERSED(bs);
        count = 1;
    }

    return count;

}

//
bvm_cache *nhref(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = _nhref((mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, NHREF);

    

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
    else if(is_href(bs)){
        MARK_TRAVERSED(bs);
        count = 1;
    }
    else if(is_leaf(bs)){
        MARK_TRAVERSED(bs);
        count = 0;
    }


    return count;

}

//
bvm_cache *nin(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = _nin((mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, NIN);

    

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

    if(is_leaf(bs) || is_href(bs)){
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
bvm_cache *nva(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = _nva((mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, NVA);

    

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
    else{ // is_leaf
        count = num_elem;
    }

    return count;

}

//
bvm_cache *npt(bvm_cache *this_bvm){

    mword *result    = new_atom;

    *result = _npt((mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, NPT);

    

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
bvm_cache *cp(bvm_cache *this_bvm){

    mword *result = _unload((mword*)TOS_0(this_bvm));

    push_alloc(this_bvm, result, CP);

    load(this_bvm);

}

//
bvm_cache *span(bvm_cache *this_bvm){

    mword *result = _bs2ar((mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);

    push_alloc(this_bvm, result, SPAN);

    

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

// Clayton Bauman 2012

