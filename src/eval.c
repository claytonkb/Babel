// eval.c
//

#include "babel.h"
#include "eval.h"
#include "list.h"
#include "cache.h"
#include "rstack.h"
#include "introspect.h"
#include "interp.h"
#include "bstruct.h"
#include "operator.h"
#include "logic.h"
#include "stack.h"

////
////
//bvm_cache *sel(bvm_cache *this_bvm){ // sel#
//
//
//
//    return this_bvm;
//
//}
//
//
////
////
//bvm_cache *selr(bvm_cache *this_bvm){ // selr#
//
//    return this_bvm;
//
//}


//
//
void _eval(bvm_cache *this_bvm, mword *eval_body, mword *eval_return){ // _eval#

    rstack_push(this_bvm, 
            mk_rstack_entry_eval(this_bvm, 
                BABEL_TAG_EVAL, 
                eval_return));

    this_bvm->code_ptr = _cons(this_bvm, eval_body, nil); //FIXME: MEMORY ... do we really need to create new memory here??

    set_advance_type(this_bvm, BVM_CONTINUE);

    return;

}


//
//
void _nest(bvm_cache *this_bvm, mword *eval_body, mword *eval_return, mword *list){ // _nest#

    // save dstack onto rstack
    rstack_push(this_bvm, 
            mk_rstack_entry_nest(this_bvm, 
                BABEL_TAG_NEST, 
                eval_return,
                rci(this_bvm->dstack_ptr,0)));

    stack_clear(this_bvm, this_bvm->dstack_ptr);
//    cache_update(this_bvm);

    interp_push_operand(this_bvm, list);

    this_bvm->code_ptr = _cons(this_bvm, eval_body, nil);

    set_advance_type(this_bvm, BVM_CONTINUE);

    return;

}


//
//
void _loop(bvm_cache *this_bvm, mword *eval_body, mword *eval_return){ // _loop#

    rstack_push(this_bvm, 
            mk_rstack_entry_loop(this_bvm, 
                BABEL_TAG_LOOP, 
                eval_return,
                eval_body,
                _val(this_bvm, 0)));

    this_bvm->code_ptr = _cons(this_bvm, eval_body, nil); //FIXME: MEMORY ... do we really need to create new memory here??

    set_advance_type(this_bvm, BVM_CONTINUE);

    return;

}


//
//
void _times(bvm_cache *this_bvm, mword *eval_body, mword *eval_return, mword *counter){ // _times#
//_trace;

    if(rci(counter,0) == 0)
        return;

    lcl(counter,0) = rcl(counter,0)-1;

//_trace;
//
//mword *temp0 = _cp(this_bvm, counter);
//_trace;
//
//mword *temp = mk_rstack_entry_times(this_bvm, 
//                BABEL_TAG_TIMES, 
//                eval_return,
//                eval_body,
//                temp0);
//_trace;
//
//    rstack_push(this_bvm, temp);

    rstack_push(this_bvm, 
            mk_rstack_entry_times(this_bvm, 
                BABEL_TAG_TIMES, 
                eval_return,
                eval_body,
                _cp(this_bvm, counter)));

    this_bvm->code_ptr = _cons(this_bvm, eval_body, nil);

    set_advance_type(this_bvm, BVM_CONTINUE);

    return;

}


//
//
void _each(bvm_cache *this_bvm, mword *eval_body, mword *eval_return, mword *list){ // _each#

    if(is_nil(list))
        return;

//    stack_push(this_bvm,
//            rci(this_bvm->dstack_ptr,0),
//            stack_new_entry(
//                this_bvm,
//                rci(list,0),
//                nil));

    dstack_push(this_bvm,
            stack_new_entry(
                this_bvm,
                rci(list,0),
                nil));

    list = rci(list,1);

    rstack_push(this_bvm, 
            mk_rstack_entry_each(this_bvm, 
                BABEL_TAG_EACH, 
                eval_return,
                eval_body,
                _val(this_bvm,0),
                list));

    this_bvm->code_ptr = _cons(this_bvm, eval_body, nil);

    set_advance_type(this_bvm, BVM_CONTINUE);

    return;

}


//
//
void _eachar(bvm_cache *this_bvm, mword *eval_body, mword *eval_return, mword *array){ // _eachar#

    if(is_nil(array))
        return;

    mword *entry;
    if(is_leaf(array)){
        entry = _val( this_bvm, rcl(array,0));
    }
    else{
        entry = rci(array,0);
    }

//    stack_push(this_bvm,
//            rci(this_bvm->dstack_ptr,0),
//            stack_new_entry(
//                this_bvm,
//                entry,
//                nil));

    dstack_push(this_bvm,
            stack_new_entry(
                this_bvm,
                entry,
                nil));

//    array = rci(array,1);

    rstack_push(this_bvm, 
            mk_rstack_entry_each(this_bvm, 
                BABEL_TAG_EACHAR, 
                eval_return,
                eval_body,
                _val(this_bvm,0),
                array));

    this_bvm->code_ptr = _cons(this_bvm, eval_body, nil);

    set_advance_type(this_bvm, BVM_CONTINUE);

    return;

}


// Dispatcher for next operator or end-of-code-list
//
bvm_cache *_next(bvm_cache *this_bvm){ // _next#

//_dump(rstack_ith(this_bvm,0));
//_dump(this_bvm->rstack_ptr);
//_dump(rci(rstack_ith(this_bvm,0),0));

    mword *tag = get_rstack_entry_tag(this_bvm,rstack_ith(this_bvm,0));

//_dump(this_bvm->rstack_ptr);
//_die;

    if(tageq(tag,BABEL_TAG_EVAL,TAG_SIZE)){
        _next_eval(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_TIMES,TAG_SIZE)){
        _next_times(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_LOOP,TAG_SIZE)){
        _next_loop(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_EACH,TAG_SIZE)){
        _next_each(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_EACHAR,TAG_SIZE)){
        _next_eachar(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_WHILE,TAG_SIZE)){
        _next_while(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_NEST,TAG_SIZE)){
        _next_nest(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_LET,TAG_SIZE)){
        _next_let(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_COND,TAG_SIZE)){ // FIXME: use _pop(this_bvm) instead of manual list-mods
        _next_cond(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_ALT,TAG_SIZE)){
        _next_alt(this_bvm);
    }
    else if(tageq(tag,BABEL_TAG_SEQ,TAG_SIZE)){
        _next_seq(this_bvm);
    }
    else{
        _dump(this_bvm->rstack_ptr);
        _fatal("unrecognized tag");
    }

    set_advance_type(this_bvm, BVM_ADVANCE);

    return this_bvm;

}


//
//
mword is_entry_looplike(mword *entry_tag){ // is_entry_looplike#

    if(     tageq(entry_tag, BABEL_TAG_TIMES,  TAG_SIZE)
        ||  tageq(entry_tag, BABEL_TAG_LOOP,   TAG_SIZE)
        ||  tageq(entry_tag, BABEL_TAG_EACH,   TAG_SIZE)
        ||  tageq(entry_tag, BABEL_TAG_EACHAR, TAG_SIZE)
        ||  tageq(entry_tag, BABEL_TAG_WHILE,  TAG_SIZE)){
        return 1;
    }

    return 0;

}


//
//
bvm_cache *return_op(bvm_cache *this_bvm){ // return_op#

    this_bvm->flags->BVM_RETURN_TOS_ON_EXIT = FLAG_SET;

    set_advance_type(this_bvm, BVM_RETURN);

    return this_bvm;

}


//
//
bvm_cache *last(bvm_cache *this_bvm){ // last#

    mword *entry = rstack_first_looplike_entry(this_bvm, rci(this_bvm->rstack_ptr,0));

    if(is_nil(entry)){
        return this_bvm;
    }

    set_code_ptr(this_bvm, get_rstack_entry_eval_return(this_bvm,entry));

    rstack_pop_til_looplike_entry(this_bvm);

    set_advance_type(this_bvm, BVM_CONTINUE);

    return this_bvm;

}

//        return get_rstack_entry_times_counter(this_bvm,rci(rstack_entry,0));

// returns bool indicating if update occurred
//
void update_iter(bvm_cache *this_bvm, mword *rstack_entry, int delta){ // update_iter#

    mword *times_counter = get_rstack_entry_times_counter(this_bvm,rstack_entry);
    lcl(times_counter,0) = rcl(times_counter,0) + delta;

}


//
//
mword *_iter(bvm_cache *this_bvm, mword *rstack_entry){ // _iter#

    if(is_nil(rstack_entry)){
        return nil;
    }

    if(is_entry_looplike(get_rstack_entry_tag(this_bvm,rci(rstack_entry,0)))){
        return get_rstack_entry_times_counter(this_bvm,rci(rstack_entry,0));
    }
    else{
        return _iter(this_bvm, rci(rstack_entry,1));
    }

    return nil; // silence compiler warning

}


////
////
//bvm_cache *fail(bvm_cache *this_bvm){
//
//    _next_eval(this_bvm);
//    interp_push_operand(this_bvm, _cp(this_bvm, BABEL_TAG_FAIL));
//
//    return this_bvm;
//
//}
//
//
////
////
//bvm_cache *pass(bvm_cache *this_bvm){
//
//    _next_eval(this_bvm);
//    interp_push_operand(this_bvm, _cp(this_bvm, BABEL_TAG_PASS));
//
//    return this_bvm;
//
//}


//
//
bvm_cache *_next_eval(bvm_cache *this_bvm){ // _next_eval#

    if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
        fprintf(stderr, "%08x     _next_eval\n", this_bvm->interp->global_tick_count);
    }

    mword *eval_return = 
        get_rstack_entry_eval_return(this_bvm,
                rstack_ith(this_bvm,0));

    set_code_ptr(this_bvm, eval_return);

    rstack_pop(this_bvm);

    return this_bvm;

}


//
//
bvm_cache *_next_times(bvm_cache *this_bvm){ // _next_times#

    if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
        fprintf(stderr, "%08x     _next_times\n", this_bvm->interp->global_tick_count);
    }

    mword *eval_return;

    mword *top_of_rstack = rstack_ith(this_bvm,0);

    mword counter = rcl( _iter(this_bvm, _ith(this_bvm,this_bvm->rstack_ptr,0)), 0);

    if(counter == 0){

        eval_return = 
            get_rstack_entry_eval_return(this_bvm,
                top_of_rstack);

        rstack_pop(this_bvm);

    }
    else{

        eval_return = 
            get_rstack_entry_eval_body(this_bvm,
                top_of_rstack);

    }

    update_iter(this_bvm, top_of_rstack, -1);

    set_code_ptr(this_bvm, eval_return);

    return this_bvm;

}




//
//
bvm_cache *_next_loop(bvm_cache *this_bvm){ // _next_loop#

    if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
        fprintf(stderr, "%08x     _next_loop\n", this_bvm->interp->global_tick_count);
    }

    mword *eval_return = 
        get_rstack_entry_eval_body(this_bvm,
                rstack_ith(this_bvm,0));

    update_iter(this_bvm, rstack_ith(this_bvm,0), 1);

    set_code_ptr(this_bvm, eval_return);

    return this_bvm;

}


//
//
bvm_cache *_next_each(bvm_cache *this_bvm){ // _next_each#

    if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
        fprintf(stderr, "%08x     _next_each\n", this_bvm->interp->global_tick_count);
    }

    mword *eval_return;

    mword *top_of_rstack = rstack_ith(this_bvm,0);

    mword *list = get_rstack_entry_each_list(this_bvm,top_of_rstack);

//    if(is_nil(list) || is_nil(rci(list,0))){
    if(is_nil(list)){

        eval_return = 
            get_rstack_entry_eval_return(this_bvm,
                top_of_rstack);

        rstack_pop(this_bvm);

    }
    else{

//        stack_push(this_bvm,
//                rci(this_bvm->dstack_ptr,0),
//                stack_new_entry(
//                    this_bvm,
//                    rci(list,0),
//                    nil));

        dstack_push(this_bvm,
                stack_new_entry(
                    this_bvm,
                    rci(list,0),
                    nil));

        list = rci(list,1);

        eval_return = 
            get_rstack_entry_eval_body(this_bvm,
                top_of_rstack);

        set_rstack_entry_each_list(this_bvm, top_of_rstack, list);

    }

    update_iter(this_bvm, top_of_rstack, 1);

    set_code_ptr(this_bvm, eval_return);

    return this_bvm;

}


//
//
bvm_cache *_next_eachar(bvm_cache *this_bvm){ // _next_eachar#

    if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
        fprintf(stderr, "%08x     _next_eachar\n", this_bvm->interp->global_tick_count);
    }

    mword *eval_return;

    mword *top_of_rstack = rstack_ith(this_bvm,0);

    mword *array = get_rstack_entry_each_list(this_bvm,top_of_rstack);

    mword counter = rcl( _iter(this_bvm, _ith(this_bvm,this_bvm->rstack_ptr,0)), 0);

    if(counter >= (size(array)-1)){

        eval_return = 
            get_rstack_entry_eval_return(this_bvm,
                top_of_rstack);

        rstack_pop(this_bvm);

    }
    else{

        mword *entry;
        if(is_leaf(array)){
            entry = _val( this_bvm, rcl(array,counter+1));
        }
        else{
            entry = rci(array,counter+1);
        }

//        stack_push(this_bvm,
//            rci(this_bvm->dstack_ptr,0),
//            stack_new_entry(
//                this_bvm,
//                entry,
//                nil));

        dstack_push(this_bvm,
            stack_new_entry(
                this_bvm,
                entry,
                nil));

        eval_return = 
            get_rstack_entry_eval_body(this_bvm,
                top_of_rstack);

    }

    update_iter(this_bvm, top_of_rstack, 1);

    set_code_ptr(this_bvm, eval_return);

    return this_bvm;

}


//
//
bvm_cache *_next_while(bvm_cache *this_bvm){ // _next_while#
    return this_bvm;
}


//
//
bvm_cache *_next_nest(bvm_cache *this_bvm){ // _next_nest#

    if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
        fprintf(stderr, "%08x     _next_nest\n", this_bvm->interp->global_tick_count);
    }

    mword *eval_return = 
        get_rstack_entry_eval_return(this_bvm,
                rstack_ith(this_bvm,0));

    set_code_ptr(this_bvm, eval_return);

    mword stack_empty = is_stack_empty(this_bvm, rci(this_bvm->dstack_ptr,0));
    mword *nest_return = nil;

    mword *dstack = rci(this_bvm->dstack_ptr,0);

    if(!stack_empty){
        nest_return = 
            rci( stack_entry_get_data(this_bvm,
                rci( stack_pop(this_bvm, dstack), 0)), 0);
    }

    mword *old_dstack = get_rstack_entry_nest_dstack(this_bvm,
                rstack_ith(this_bvm,0));

    lci(this_bvm->dstack_ptr,0) = old_dstack;

    if(!stack_empty){
        interp_push_operand(this_bvm, nest_return);
    }

    cache_flush(this_bvm);
    dstack_depth_update(this_bvm);

    rstack_pop(this_bvm);

    return this_bvm;

}


//
//
bvm_cache *_next_let(bvm_cache *this_bvm){ // _next_let#
    return this_bvm;
}


//
//
bvm_cache *_next_cond(bvm_cache *this_bvm){ // _next_cond#
    return this_bvm;
}


//
//
bvm_cache *_next_alt(bvm_cache *this_bvm){ // _next_alt#
    return this_bvm;
}


//
//
bvm_cache *_next_seq(bvm_cache *this_bvm){ // _next_seq#
    return this_bvm;
}


/*****************************************************************************
 *                                                                           *
 *                             IO OPERATORS                                  *
 *                                                                           *
 ****************************************************************************/


#define NEST_OPS \
    _nest(this_bvm, oi0.data, get_code_next_entry(this_bvm), oi1.data);

OPERATOR_EVAL_R2_W0_D(nest,NEST_OPS,
        nil,OI_MASK_INTE,0,0,
        nil,OI_MASK_ANY,0,1)

//OPERATOR_R2_W0_P(nest, NEST_OPS,
//        nil, OI_MASK_INTE, 0, 0,
//        nil, OI_MASK_INTE, 0, 0)


#define EVAL_OPS \
    _eval(this_bvm, oi0.data, get_code_next_entry(this_bvm));

OPERATORA_R1_W0_D(eval,EVAL_OPS,
        nil,OI_MASK_INTE,0,0)


#define FAIL_OPS \
    _next_eval(this_bvm); \
    set_advance_type(this_bvm, BVM_CONTINUE); \
    result0 = _cp(this_bvm, BABEL_TAG_FAIL);

OPERATORA_R0_W1(fail_op,FAIL_OPS)


#define PASS_OPS \
    _next_eval(this_bvm); \
    set_advance_type(this_bvm, BVM_CONTINUE); \
    result0 = _cp(this_bvm, BABEL_TAG_PASS);

OPERATORA_R0_W1(pass_op,PASS_OPS)


#define REJECT_OPS \
    _next_eval(this_bvm); \
    set_advance_type(this_bvm, BVM_CONTINUE); \
    result0 = _cp(this_bvm, BABEL_TAG_REJECT);

OPERATORA_R0_W1(reject_op,REJECT_OPS)


#define ACCEPT_OPS \
    _next_eval(this_bvm); \
    set_advance_type(this_bvm, BVM_CONTINUE); \
    result0 = _cp(this_bvm, BABEL_TAG_ACCEPT);

OPERATORA_R0_W1(accept_op,ACCEPT_OPS)


#define SEL_OPERATIONS       \
    if(is_false(oi0.data)){  \
        result0 = oi2.data;  \
    }                        \
    else{                    \
        result0 = oi1.data;  \
    }

OPERATORA_R3_W1_D(sel, 
        SEL_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 0)


#define SELR_OPERATIONS      \
    if(is_false(oi2.data)){  \
        result0 = oi1.data;  \
    }                        \
    else{                    \
        result0 = oi0.data;  \
    }

OPERATORA_R3_W1_D(selr, 
        SELR_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 0)


#define TIMES_OPS \
    _times(this_bvm, oi1.data, get_code_next_entry(this_bvm), _cp(this_bvm, oi0.data));

OPERATORA_R2_W0_D(times,TIMES_OPS,
        nil,OI_MASK_LEAF,0,0,
        nil,OI_MASK_INTE,0,1)


#define EACH_OPS \
    _each(this_bvm, oi0.data, get_code_next_entry(this_bvm), oi1.data);

//    _each(this_bvm, oi0.data, rci(rci(this_bvm->code_ptr,0),1), oi1.data);

OPERATOR_EVAL_R2_W0_D(each,EACH_OPS,
        nil,OI_MASK_INTE,0,0,
        nil,OI_MASK_INTE,0,1)


#define EACHAR_OPS \
    _eachar(this_bvm, oi0.data, get_code_next_entry(this_bvm), oi1.data);

OPERATOR_EVAL_R2_W0_D(eachar,EACHAR_OPS,
        nil,OI_MASK_INTE|OI_MASK_LEAF,0,0,
        nil,OI_MASK_INTE|OI_MASK_LEAF,0,1)


//bvm_cache *each(bvm_cache *this_bvm){
//
//    oinfo oi0;
//    oi0.default_data    = nil;
//    oi0.required_tag    = nil;
//    oi0.mask            = OI_MASK_INTE;
//    oi0.min_size        = 0;
//    oi0.max_size        = 0;
//
//    oinfo oi1;
//    oi1.default_data    = nil;
//    oi1.required_tag    = nil;
//    oi1.mask            = OI_MASK_INTE;
//    oi1.min_size        = 0;
//    oi1.max_size        = 0;
//
//    _each(this_bvm, , , );
//
//}

#define LOOP_OPS \
    _loop(this_bvm, oi0.data, get_code_next_entry(this_bvm));

OPERATORA_R1_W0_D(loop,LOOP_OPS,
        nil,OI_MASK_INTE,0,0)


#define DIE_OPS \
    _msg("Babel interpreter terminating itself on program request"); \
    _die;

OPERATOR_R0_W0(die,DIE_OPS)


#define ITER_OPERATIONS \
    result0 = _val( this_bvm, rcl( _iter(this_bvm, _ith(this_bvm,this_bvm->rstack_ptr,0)), 0) );

OPERATORA_R0_W1(iter, ITER_OPERATIONS)


// Clayton Bauman 2014

