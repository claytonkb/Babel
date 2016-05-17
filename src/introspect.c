// introspect.c
//

#include "babel.h"
#include "introspect.h"
#include "bstruct.h"
#include "array.h"
#include "operator.h"
#include "string.h"

#define INTE_OPEN  "[ptr "
#define INTE_CLOSE "] "
#define LEAF_OPEN  "[val "
#define LEAF_CLOSE "] "
#define TPTR_OPEN  "[tptr "
#define TPTR_CLOSE "] "


//Returns a string containing the Graphviz text file
// XXX DEBUG USE ONLY, THIS BS2GV USES MALLOC XXX ///
mword *_bs2gv(bvm_cache *this_bvm, mword *bs){ // _bs2gv#

    // Figure out buffer size
    // Safety buffer of 2kb + (32 * _mu) XXX: WHY 100?? Ran into problems on this before!!
    mword initial_buf_size = 1<<19; //FIXME!!!!!!!

    //matching free() below
    char *buffer = malloc(initial_buf_size); // XXX WAIVER(temp_dev) XXX //
    mword buf_size=0;

    buf_size += sprintf(buffer+buf_size, "digraph babel {\nnode [shape=record];\n");
    buf_size += sprintf(buffer+buf_size, "graph [rankdir = \"LR\"];\n");

    buf_size += _rbs2gv(this_bvm, bs, buffer+buf_size, 1);

    buf_size += sprintf(buffer+buf_size, "}\n");
    *(buffer+buf_size) = 0;
    buf_size++;

    //buf_size now contains the final string size of the entire graphviz string

    _rclean(this_bvm, bs);

    mword last_mword = _alignment_word8(this_bvm, buf_size);
    mword length = (buf_size / MWORD_SIZE) + 1;

    if(buf_size % MWORD_SIZE != 0){
        length++;
    }

    mword *result = _newlfi(this_bvm, length, 0);
    memcpy(result, buffer, buf_size); // XXX WAIVER(temp_dev) XXX //
    lcl(result,length-1) = last_mword;
    free(buffer);

    return result;

}


//
//
mword _rbs2gv(bvm_cache *this_bvm, mword *bs, char *buffer, int initial){ // _rbs2gv#

    int i;
    mword buf_size=0;
    int clipped=0;

    if( TRAVERSED(bs) ){
        return 0;
    }

    int num_entries = size(bs);

    if(is_tptr(bs)){ // is_tptr

        if(!is_nil(bs)){

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
                                    (mword)(bs+HASH_SIZE+1));

            MARK_TRAVERSED(bs);
            buf_size += _rbs2gv(this_bvm, (mword *)(bs+HASH_SIZE+1), buffer+buf_size, 0);

        }
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

        if(initial){
            buf_size += sprintf(buffer+buf_size, "\",style=\"dashed\"];\n");
        }
        else{
            buf_size += sprintf(buffer+buf_size, "\"];\n");
        }

        for(i=0; i<num_entries; i++){
            if(is_nil((mword *)lcar(bs+i))){
                continue;
            }
            buf_size += sprintf(buffer+buf_size, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)bs, i, *(mword *)(bs+i));
            buf_size += _rbs2gv(this_bvm, (mword *)*(bs+i), buffer+buf_size, 0);
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


//
//
mword introspect_calc_buf_length(
        bvm_cache *this_bvm,
        mword *bs,
        mword const_padding,
        mword nlf_scaling,
        mword nva_scaling,
        mword nin_scaling,
        mword nptr_scaling,
        mword ntag_scaling){

     return
        ( const_padding
            + _nlf(this_bvm, bs)  * nlf_scaling
            + _nva(this_bvm, bs)  * nva_scaling
            + _nin(this_bvm, bs)  * nin_scaling
            + _nptr(this_bvm, bs) * nptr_scaling
            + _ntag(this_bvm, bs) * ntag_scaling );

}


//Returns a string containing the Graphviz text file
//
mword *_bs2gv2(bvm_cache *this_bvm, mword *bs){ // _bs2gv2#

    mword result_string_length = 
                introspect_calc_buf_length(this_bvm, bs, (1<<10), 65, 23, 33, 68, 140);

    mword *result = _newstr(this_bvm, result_string_length, 0);

    mword offset  = 0;

    bsprintf(this_bvm, result, &offset, "digraph babel {\nnode [shape=record];\n");
    bsprintf(this_bvm, result, &offset, "graph [rankdir = \"LR\"];\n");

    _rbs2gv2(this_bvm, bs, result, &offset, 1);

    _rclean(this_bvm, bs);

    bsprintf(this_bvm, result, &offset, "}\n");

    wstrim(this_bvm, result);

    return result;

}


//
//
void _rbs2gv2(bvm_cache *this_bvm, mword *bs, mword *result, mword *offset, int initial){ // _rbs2gv2#

    int i;
    int clipped=0;

    if( TRAVERSED(bs) ){
        return;
    }

    int num_entries = size(bs);

    if(is_tptr(bs)){ // is_tptr

        if(!is_nil(bs)){ // XXX 140 * NTAG

            bsprintf(this_bvm, result, offset, "s%08x [shape=record,label=\"", (mword)bs); // XXX 31

            for(i=0; i<HASH_SIZE; i++){ // XXX 13 * 4 = 52

                bsprintf(this_bvm, result, offset, "<f%d> %x", i, *(mword *)(bs+i)); // XXX 12

                if(i<(HASH_SIZE-1)){
                    bsprintf(this_bvm, result, offset, "|"); // XXX 1
                }

            }

            bsprintf(this_bvm, result, offset, "\"];\n"); // XXX 4

            bsprintf(this_bvm, result, offset,
                        "\"s%08x\":f0 -> \"s%08x\":f0 [arrowhead=\"none\"];\n", // XXX 53
                        (mword)bs, 
                        (mword)(bs+HASH_SIZE+1));

            MARK_TRAVERSED(bs);

            _rbs2gv2(this_bvm, (mword *)(bs+HASH_SIZE+1), result, offset, 0);

        }
    }
    else if(is_inte(bs)){ // XXX 33 * NIN

        MARK_TRAVERSED(bs);

        bsprintf(this_bvm, result, offset, "\"s%08x\" [shape=record,label=\"", (mword)bs); // XXX 33

        for(i=0; i<num_entries; i++){ // XXX 25

            bsprintf(this_bvm, result, offset, "<f%d> %d", i, i); // XXX 24

            if(i<(num_entries-1)){
                bsprintf(this_bvm, result, offset, "|"); // XXX 1
            }
        }

        if(initial){ // XXX constant-padding
            bsprintf(this_bvm, result, offset, "\",style=\"dashed\"];\n");
        }
        else{
            bsprintf(this_bvm, result, offset, "\"];\n");
        }

        for(i=0; i<num_entries; i++){

            if(is_nil((mword *)lcar(bs+i))){
                continue;
            }

            bsprintf(this_bvm, result, offset, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)bs, i, *(mword *)(bs+i)); // XXX 43
            _rbs2gv2(this_bvm, (mword *)*(bs+i), result, offset, 0);

        }

        // XXX (25 + 43) * NPT

    }
    else{// if(is_leaf(bs)){  // XXX 65 * NLF

        if(num_entries > 8){
            num_entries=8;
            clipped=1;
        }
        else{
            clipped=0;
        }

        bsprintf(this_bvm, result, offset, "s%08x [style=bold,shape=record,label=\"", (mword)bs); // XXX 43

        for(i=0; i<num_entries; i++){ // XXX 23 * NVA

            bsprintf(this_bvm, result, offset, "<f%d> %x", i, *(mword *)(bs+i));              // XXX 22

            if(i<(num_entries-1)){
                bsprintf(this_bvm, result, offset, "|");              // XXX 1
            }

        }
        if(clipped){

            bsprintf(this_bvm, result, offset, "|");              // XXX 1
            bsprintf(this_bvm, result, offset, "<f%d> ...", i);   // XXX <=17

        }

        bsprintf(this_bvm, result, offset, "\"];\n");             // XXX 4

    }

    MARK_TRAVERSED(bs);

}


//Returns a string that fully or partially represents the babel-struct
mword *_bs2str(bvm_cache *this_bvm, mword *bs){ // _bs2str#

    mword result_string_length = 
                introspect_calc_buf_length(this_bvm, bs, (1<<10), 65, 23, 33, 68, 140);

    mword *result = _newstr(this_bvm, result_string_length, 0);

    mword offset  = 0;

    _rbs2str(this_bvm, bs, result, &offset);

    _rclean(this_bvm, bs);

    wstrim(this_bvm, result);

    return result;

}


//
//
void _rbs2str(bvm_cache *this_bvm, mword *bs, mword *result, mword *offset){ // _rbs2str#

    int i;

    if( TRAVERSED(bs) ){
        bsprintf(this_bvm, result, offset, " ... ");
        return;
    }

    int num_entries = size(bs);

    if(is_tptr(bs)){ // is_tptr

        if(!is_nil(bs)){ // XXX 140 * NTAG

            bsprintf(this_bvm, result, offset, TPTR_OPEN);

            for(i=0; i<HASH_SIZE; i++){ // XXX 13 * 4 = 52
                bsprintf(this_bvm, result, offset, "0x%08x ", *(mword *)(bs+i));
            }

            MARK_TRAVERSED(bs);

            _rbs2str(this_bvm, (mword *)(bs+HASH_SIZE+1), result, offset);

            bsprintf(this_bvm, result, offset, TPTR_CLOSE);

        }
        else{
            bsprintf(this_bvm, result, offset, " nil ");
        }
}
    else if(is_inte(bs)){ // XXX 33 * NIN

        MARK_TRAVERSED(bs);

        bsprintf(this_bvm, result, offset, INTE_OPEN);

        for(i=0; i<num_entries; i++){ // XXX 25
            _rbs2str(this_bvm, (mword *)*(bs+i), result, offset);
        }

        bsprintf(this_bvm, result, offset, INTE_CLOSE);

    }
    else{// if(is_leaf(bs)){  // XXX 65 * NLF

        MARK_TRAVERSED(bs);

        bsprintf(this_bvm, result, offset, LEAF_OPEN);

        for(i=0; i<num_entries; i++){ // XXX 23 * NVA
            bsprintf(this_bvm, result, offset, "0x%x ", *(mword *)(bs+i));
        }

        bsprintf(this_bvm, result, offset, LEAF_CLOSE);

    }

}


//
//
void _stack_dump(bvm_cache *this_bvm, mword *stack){ // _stack_dump#

    mword stack_direction = stack_dir(this_bvm, stack);
    mword *curr_entry = rci(stack_TOS(this_bvm, stack), stack_direction);

    printf("---TOS---\n");

    while(!is_stack_sentinel(this_bvm, curr_entry)){
        printf("%s\n",(char*)_bs2str(this_bvm, rci(stack_entry_get_data(this_bvm,rci(curr_entry,0)),0)));
        curr_entry = rci(curr_entry, stack_direction);
    }

    stack_direction = stack_dir_rev(this_bvm, stack);
    curr_entry = rci(stack_TOS(this_bvm, stack), stack_direction);

    printf("---BOS---\n");

    while(!is_stack_sentinel(this_bvm, curr_entry)){
        printf("%s\n",(char*)_bs2str(this_bvm, rci(stack_entry_get_data(this_bvm,rci(curr_entry,0)),0)));
        curr_entry = rci(curr_entry, stack_direction);
    }

}


// XXX Recognize nil in the inte-clause... SKIP it in the tptr clause
//
mword *_bs2str2(bvm_cache *this_bvm, mword *bs){

    if(is_nil_leaf_based(bs+1)){
        return _cp(this_bvm, C2B("[bs s0 nil]\n"));
    }

    mword result_string_length = 
                introspect_calc_buf_length(this_bvm, bs, (1<<16), 65,  23,  33,  68,   140);
              //introspect_calc_buf_length(this_bvm, bs, const,   nlf, nva, nin, nptr, ntag)

    mword *result = _newstr(this_bvm, result_string_length, 0);

    mword bs_size = size(bs);

    mword current_bs_size;
    mword current_ptr;

    mword *bs_base; // XXX bs_base usually contains UNSAFE pointers
    mword bs_offset  = 0;

    mword str_offset = 0;

    int s_field      = 0;
    int i;

    char *offset_format = (char*)_newstr(this_bvm, 7, 0);

    if(bs_size < (1<<4)){
        offset_format = "s0%x ";
    }
    else if(bs_size < (1<<8)){
        offset_format = "s0%02x ";
    }
    else if(bs_size < (1<<16)){
        offset_format = "s0%04x ";
    }
    else{
        offset_format = "s0%08x ";
    }

    bsprintf(this_bvm, result, &str_offset, "[bs ");

    while(bs_offset < bs_size){

        s_field = (int)lcl(bs,bs_offset);
        bs_offset++;
        bs_base = (bs + bs_offset);

        if(is_nil_leaf_based(bs_base)){ // skip nil
            bs_offset += TPTR_SIZE-1;
            continue;
        }

        current_bs_size = abs(s_field)>>LOG_MWORD_SIZE;

        bsprintf(this_bvm, result, &str_offset, "\n  ");
        bsprintf(this_bvm, result, &str_offset, offset_format, bs_offset);

        if(s_field == 0){ // tptr

            bs_offset += TPTR_SIZE-1;

            bsprintf(this_bvm, result, &str_offset, "[tptr ");

            // print tag
            for(i=0; i < HASH_SIZE; i++){            
                bsprintf(this_bvm, result, &str_offset, "0x%08x ", (unsigned)rcl(bs_base,i));
            }

            // print inte ptr
            bsprintf(this_bvm, result, &str_offset, "[ptr ");
            bsprintf(this_bvm, result, &str_offset, offset_format, (unsigned)(rcl(bs_base,HASH_SIZE+1) >> LOG_MWORD_SIZE)); 
            bsprintf(this_bvm, result, &str_offset, "] ");

        }
        else{

            bs_offset += current_bs_size;

            if(s_field > 0){ // -- leaf

                bsprintf(this_bvm, result, &str_offset, "[val ");

                for(i=0; i < current_bs_size; i++){
                    bsprintf(this_bvm, result, &str_offset, "0x%x ", (unsigned)rcl(bs_base,i)); 
                }
            }
            else{ // s_field < 0 -- inte

                bsprintf(this_bvm, result, &str_offset, "[ptr ");

                for(i=0; i < current_bs_size; i++){

                    current_ptr = (rcl(bs_base,i) >> LOG_MWORD_SIZE);

                    // Use memcmp with BABEL_TAG_INTERP_NIL
                    if(is_nil_leaf_based((mword*)(bs+current_ptr))){
                        bsprintf(this_bvm, result, &str_offset, "nil "); 
                    }
                    else{
                        bsprintf(this_bvm, result, &str_offset, offset_format, (unsigned)(rcl(bs_base,i) >> LOG_MWORD_SIZE)); 
                    }
                }

            }
        }

        bsprintf(this_bvm, result, &str_offset, "]");

    }

    bsprintf(this_bvm, result, &str_offset, " ]\n");

    wstrim(this_bvm, result);

    return result;

}


//
//
mword is_nil_leaf_based(mword *bs){

//_d(BABEL_TAG_INTERP_NIL[0]);
//_d(BABEL_TAG_INTERP_NIL[1]);
//_d(BABEL_TAG_INTERP_NIL[2]);
//_d(BABEL_TAG_INTERP_NIL[3]);
//_d(bs[0]);

    return ( memcmp(bs, BABEL_TAG_INTERP_NIL, BYTE_SIZE(HASH_SIZE)) == 0 );

}


/*****************************************************************************
 *                                                                           *
 *                          INTROSPECT OPERATORS                             *
 *                                                                           *
 ****************************************************************************/

#define BS2GV_OPERATIONS \
    result0 = _bs2gv2(this_bvm, oi0.data);

OPERATORA_R1_W1_D(bs2gv_d, BS2GV_OPERATIONS, nil, OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR, 0, 0)


#define BS2STR_OPERATIONS \
    result0 = _bs2str(this_bvm, oi0.data);

//    result0 = _bbl2str(this_bvm, oi0.data); 

OPERATORA_R1_W1_D(bs2str_d, BS2STR_OPERATIONS, nil, OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR, 0, 0)


#define BS2STR2_OPERATIONS \
    result0 = _bs2str2(this_bvm, _unload_fast(this_bvm, oi0.data));

//    result0 = _bbl2str(this_bvm, oi0.data); 

OPERATORA_R1_W1_D(bs2str2_d, BS2STR2_OPERATIONS, nil, OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR, 0, 0)


// Clayton Bauman 2014

