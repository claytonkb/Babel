// misc.c
//

#include "babel.h"
#include "misc.h"
#include "bstruct.h"
#include "mt19937ar.h"
#include "operator.h"
#include "introspect.h"
#include "stack.h"
#include "operator.h"
#include "trie.h"
#include "list.h"
#include "tptr.h"
#include "string.h"
#include "bvm.h"
#include "hash.h"
#include "pearson16.h"
#include "cache.h"
#include "bpdl.h"
#include "interp.h"
#include "lib_babel.h"
#include "arith.h"
#include "xoroshiro128.h"
#include "array.h"
#include "mem.h"

//
//
mword *_randlf(bvm_cache *this_bvm, mword num_mwords){ // _randlf#

    mword *result    = _newlfi(this_bvm, num_mwords,0);

    int i;
    for(i=0;i<num_mwords;i++){
//        lcl(result,i) = (mword)genrand_int32(); // FIXME: 32-bit specific
        lcl(result,i) = (mword)xoroshiro128_next();
    }

    return result;

}


// Dev operator - we can keep the operator's function but this is the wrong
// way to do it
void _semicolon(bvm_cache *this_bvm, mword *bs){
    mword *show_bs = _bs2str(this_bvm, bs);
    _say((char*)show_bs);
}


#ifdef DEV_MODE
// XXX: Need to get stack operands working properly
//
bvm_cache *dev(bvm_cache *this_bvm){

    oinfo oi;
    oi.default_data = nil;
    oi.required_tag = nil;
    oi.mask = OI_MASK_LEAF;
    oi.min_size = 0;
    oi.max_size = 1;

    get_operands(this_bvm,1,&oi);
//    mword *TOS;

    mword *temp;

//_d((mword)rci(this_bvm->dstack_ptr,0));
//_dump(this_bvm->dstack_ptr);

    if(rcl(oi.data,0) == 0){ // reserved for custom code
        _say("0  dev --> prints this message");
        _say("1  dev --> developer one-off");
        _say("2  dev --> puts root BVM pointer on TOS");
        _say("3  dev --> dumps dstack to test.dot");
        _say("4  dev --> dumps rstack to test.dot");
        _say("5  dev --> user-friendly stack dump");
        _say("6  dev --> dumps symbol-table to test.dot");
        _say("7  dev --> puts symbol-table pointer on TOS");
        _say("8  dev --> _meml(TOS)");
        _say("9  dev --> cache_flush");
        _say("10 dev --> cache_update");
        _say("11 dev --> cache_dump");
        _say("12 dev --> mkin from arbitrary pointer (USE CAUTION!)");
        _say("13 dev --> deref arbitrary pointer (USE CAUTION!)");
        _say("14 dev --> mem_dump_core");
        _say("15 dev --> puts opcode_table pointer on TOS");
        _say("16 dev --> toggle BVM_INTERP_OP_TRACE");
        _say("17 dev --> show time in milliseconds");
        _say("18 dev --> puts root ptr on TOS");
        _say("19 dev --> puts rstack ptr on TOS");
        _say("20 dev --> MC_GC_PENDING = FLAG_SET");
        _say("21 dev --> Toggles MC_USE_MALLOC");
        _say("22 dev --> mem_stats_array");
        _say("23 dev --> cache_stats_array");
    }
    else if(rcl(oi.data,0) == 1){//developer one-off

_dumpf(this_bvm->rstack_ptr);
_msg("_dumpf(this_bvm->rstack_ptr);");
_die;

//        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
//        cache_update(this_bvm);
//
//        oi.mask = OI_MASK_ANY;
//        get_operands(this_bvm,1,&oi);
//
//        stack_push(this_bvm,
//                rci(this_bvm->dstack_ptr,0),
//                stack_new_entry(
//                    this_bvm,
//                    lib_bpdl(this_bvm, oi.data, nil),
//                    nil));

//_d((mword)this_bvm->rstack_ptr);
//_d((mword)nil);

//        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
//        cache_update(this_bvm);
//
//        oi.mask = OI_MASK_INTE;
//        get_operands(this_bvm,1,&oi);
//        mword *catted_list = _lscat8(this_bvm, oi.data);
//
//        stack_push(this_bvm,
//                rci(this_bvm->dstack_ptr,0),
//                stack_new_entry(
//                    this_bvm,
//                    catted_list,
//                    nil));

//        mword *test = _cons(this_bvm, nil, _cons(this_bvm, _val(this_bvm, 2), _cons(this_bvm, _val(this_bvm, 3), nil)));
//        lci(test,0) = test;
//        stack_push(this_bvm,
//                rci(this_bvm->dstack_ptr,0),
//                stack_new_entry(
//                    this_bvm,
//                    test,
//                    nil));

//        _dumpf(this_bvm->soft_root);

//        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
//        cache_update(this_bvm);
//        stack_push(this_bvm,
//                rci(this_bvm->dstack_ptr,0),
//                stack_new_entry(
//                    this_bvm,
//                    stack_new_entry(this_bvm, _ptr(this_bvm, nil), nil),
//                    nil));

//void _bsprintf( bvm_cache *this_bvm, mword *buf, mword *offset, const char *format, ... ){
//        mword *my_string = _newstr(this_bvm, 27, 0);
//        mword offset = 0;
//        _bsprintf(this_bvm, my_string, &offset, "%iTEST%x %s", 3489, 173, "hello");

//        stack_push(this_bvm,
//                rci(this_bvm->dstack_ptr,0),
//                stack_new_entry(
//                    this_bvm,
//                    _bs2gv2(this_bvm, this_bvm->self),
//                    nil));

//        mword root_mu       = BYTE_SIZE( _mu(this_bvm, this_bvm->self));
//        mword root_unload_s = s(_unload_fast(this_bvm, this_bvm->self));
//
//        _d(root_mu);
//        _d(root_unload_s);

    }
    else if(rcl(oi.data,0) == 2){
//        _dumpf(this_bvm->self);
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
//        cache_update(this_bvm);
        stack_push(this_bvm,
                rci(this_bvm->dstack_ptr,0),
                stack_new_entry(
                    this_bvm,
                    this_bvm->self,
                    nil));
    }
    else if(rcl(oi.data,0) == 3){
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        _dumpf(this_bvm->dstack_ptr);
    }
    else if(rcl(oi.data,0) == 4){
        _dumpf(this_bvm->rstack_ptr);
    }
    else if(rcl(oi.data,0) == 5){
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        _stack_dump(this_bvm, rci(this_bvm->dstack_ptr,0));
    }
    else if(rcl(oi.data,0) == 6){ 
        mword *test = bvm_sym(this_bvm, BABEL_SYM_SOFT_ROOT);
        _dumpf(test);
    }
    else if(rcl(oi.data,0) == 7){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        stack_push(this_bvm,
                rci(this_bvm->dstack_ptr,0),
                stack_new_entry(
                    this_bvm,
                    bvm_sym(this_bvm, BABEL_SYM_SOFT_ROOT),
                    nil));
    }
    else if(rcl(oi.data,0) == 8){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
//        _dump(this_bvm->dstack_ptr);
        _meml( 
                rci(
                rci(
                rci(
                    rci(
                        stack_TOS(this_bvm, rci(this_bvm->dstack_ptr,0)), 
                        stack_dir(this_bvm, rci(this_bvm->dstack_ptr,0))), 
                0 ),
                1 ),
                0 ) );
    }
    else if(rcl(oi.data,0) == 9){ 
        cache_flush(this_bvm);
    }
    else if(rcl(oi.data,0) == 10){ 
        cache_update(this_bvm);
    }
    else if(rcl(oi.data,0) == 11){ 
        cache_dump(this_bvm);
    }
    else if(rcl(oi.data,0) == 12){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        get_operands(this_bvm,1,&oi);
        stack_push(this_bvm,
                rci(this_bvm->dstack_ptr,0),
                stack_new_entry(
                    this_bvm,
                    _ptr(this_bvm, (mword*)rcl(oi.data,0)),
                    nil));
    }
    else if(rcl(oi.data,0) == 13){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        get_operands(this_bvm,1,&oi);
        _d(**(mword**)oi.data);
    }
    else if(rcl(oi.data,0) == 14){ 
        mem_dump_core(this_bvm);
    }
    else if(rcl(oi.data,0) == 15){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        stack_push(this_bvm,
                rci(this_bvm->dstack_ptr,0),
                stack_new_entry(
                    this_bvm,
                    bpdl_opcode_table,
                    nil));
    }
    else if(rcl(oi.data,0) == 16){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_CLR){
            this_bvm->flags->BVM_INTERP_OP_TRACE = FLAG_SET;
        }
        else{
            this_bvm->flags->BVM_INTERP_OP_TRACE = FLAG_CLR;
        }
    }
    else if(rcl(oi.data,0) == 17){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        mword time_in_ms = time_ms();
        _d(time_in_ms);
    }
    else if(rcl(oi.data,0) == 18){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        stack_push(this_bvm,
                rci(this_bvm->dstack_ptr,0),
                stack_new_entry(
                    this_bvm,
                    this_bvm->self,
                    nil));
    }
    else if(rcl(oi.data,0) == 19){ 
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        stack_push(this_bvm,
                rci(this_bvm->dstack_ptr,0),
                stack_new_entry(
                    this_bvm,
                    this_bvm->rstack_ptr,
                    nil));
    }
    else if(rcl(oi.data,0) == 20){
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        mem_swap_banks(this_bvm);
        this_bvm->flags->MC_GC_PENDING = FLAG_SET;
    }
    else if(rcl(oi.data,0) == 21){
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);
        TOGGLE_FLAG(this_bvm->flags->MC_USE_MALLOC);
    }
    else if(rcl(oi.data,0) == 22){

        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);

        temp = mem_stats_array(this_bvm);

        stack_push(this_bvm,
                rci(this_bvm->dstack_ptr,0),
                stack_new_entry(
                    this_bvm,
                    temp,
                    nil));

//        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
//        cache_update(this_bvm);
//        mword *temp = _newlfi(this_bvm, 3, 0);
//
//        lcl(temp,0) = this_bvm->interp->mem->primary->size;
//        lcl(temp,1) = mem_bank_in_use(this_bvm->interp->mem->primary);
//        lcl(temp,2) = (rcl(temp,0) - rcl(temp,1));
//
//        stack_push(this_bvm,
//                rci(this_bvm->dstack_ptr,0),
//                stack_new_entry(
//                    this_bvm,
//                    temp,
//                    nil));

    }
    else if(rcl(oi.data,0) == 23){

        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));
        cache_flush(this_bvm);

        temp = cache_stats_array(this_bvm);

        stack_push(this_bvm,
                rci(this_bvm->dstack_ptr,0),
                stack_new_entry(
                    this_bvm,
                    temp,
                    nil));

    }
    else{
        _msg("Huh? Unrecognized dev code");
    }

    return this_bvm;

}
#else
bvm_cache *dev(bvm_cache *this_bvm){
    return rsvd(this_bvm);
}
#endif //DEV_MODE


//
//
mword time_ms(void){

#ifdef WINDOWS_TARGET
    DWORD raw_time = GetTickCount();
    return (mword)raw_time;
#endif

}


#ifdef DEV_MODE
//mword *_deva(bvm_cache *this_bvm, mword *op0, mword *op1, mword *op2){
//mword *_deva(bvm_cache *this_bvm, mword *op0, mword *op1){
mword *_deva(bvm_cache *this_bvm, mword *op0){ // _deva#

    return bstruct_load_fast(this_bvm, op0, size(op0));

//    mword *result0 = string_base64e_encode(this_bvm, op0, BYTE_SIZE(size(op0)));
////    mword *result0 = string_base64e_encode(this_bvm, op0, _arlen8(this_bvm, op0));
//    mword *result1 = string_base64e_decode(this_bvm, result0, ACCESS_SIZE_MWORD);
//
//    return _cons(this_bvm, result0, _cons(this_bvm, result1, nil));

//    return string_ascii_str_match(this_bvm, (char*)op1, (char*)op0, -1);

//    mword *string_ascii_char_match_all(bvm_cache *this_bvm, mword *string, mword character){
//    mword *result = string_ascii_char_match_all(this_bvm, op0, '/');
//    return result;

//    bstruct_substitute(this_bvm, op2, op1, op0);
//    return op2;

//    array_ptr_sort(this_bvm, op1, op0);
//    _msort(this_bvm, op0);
//    return op0;

//    if(size(op0) > size(op1)){
//        return badd_array(this_bvm, op0, op1);
//    }
//
//    return badd_array(this_bvm, op1, op0);

//    return lib_bpdl(this_bvm, op0, nil);

//    return lib_babelc(this_bvm, op0, nil);

//    return lib_babelc(this_bvm, lib_bpdl(this_bvm, op0, nil), nil);

//    return lib_babelc(this_bvm, lib_bpdl(this_bvm, op0, nil), nil);

//    return tptr_find_tag_unsafe(this_bvm, op0, BABEL_TAG_TIMES);

//    bstruct_substitute_tag(this_bvm, op0, BABEL_TAG_TIMES, _val(this_bvm, 0xdeadbeef));

//mword *stack = dstack_begin;
////mword *next_bs = stack_next(this_bvm, stack, stack_TOS(this_bvm, stack));
////_dumpf(next_bs);
//
//mword *prev_bs = stack_prev(this_bvm, stack, stack_TOS(this_bvm, stack));
//_dumpf(prev_bs);
//
////_dump(this_bvm->dstack_ptr);
//
//_msg("mword *next_bs = stack_next(this_bvm, stack, stack_TOS(this_bvm, stack));");
//_die;
//
//    return nil;

//    mword *sub_list = bstruct_find(this_bvm, op0, tptr_new(this_bvm, BABEL_TAG_TRIE_ENTRY, nil));
//    mword *result = nil;
//
//    while(!is_nil(sub_list)){
//        result = _cons(this_bvm, rci(rci(sub_list,0),0), result);
//        sub_list = (mword*)icdr(sub_list);
//    }
//
//    return result;

}
#endif


#define DEVA1_OPS \
    result0 = _deva(this_bvm, oi0.data);

#define DEVA2_OPS \
    result0 = _deva(this_bvm, oi0.data, oi1.data);

#define DEVA3_OPS \
    result0 = _deva(this_bvm, oi0.data, oi1.data, oi2.data);

OPERATORA_R1_W1_D(deva,DEVA1_OPS,
        0,OI_MASK_ANY,0,1)

//OPERATORA_R2_W1_D(deva,DEVA2_OPS,
//        0,OI_MASK_ANY,0,1,
//        0,OI_MASK_ANY,0,1)

//OPERATORA_R3_W1_D(deva,DEVA3_OPS,
//        0,OI_MASK_ANY,0,1,
//        0,OI_MASK_ANY,0,1,
//        0,OI_MASK_ANY,0,1)


#define TIMEMS_OPERATIONS \
    result0 = _val(this_bvm, time_ms()); 

OPERATORA_R0_W1(timems, TIMEMS_OPERATIONS)


#define RANDLF_OPS \
    result0 = _randlf(this_bvm, rcl(oi0.data,0));

OPERATORA_R1_W1_D(randlf_d,RANDLF_OPS,
        0,OI_MASK_LEAF,0,1)


#define SEMICOLON_OPERATIONS \
    _semicolon( this_bvm, oi0.data );

OPERATORA_R1_W0_P(semicolon, 
        SEMICOLON_OPERATIONS,
        nil, OI_MASK_ANY, 0, 0)


#define FNORD_OPS

OPERATOR_R0_W0(fnord,FNORD_OPS)


#define FAKE_FNORD_OPS \
    result0 = _val(this_bvm, ~(*oi0.data) );

//OPERATORA_R1_W1_D(fnord,
//        FAKE_FNORD_OPS,
//        nil, OI_MASK_ANY, 0, 0)


// Clayton Bauman 2014 
