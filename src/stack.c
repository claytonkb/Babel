// stack.c
//

#include "babel.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"
#include "introspect.h"
#include "operator.h"
#include "tptr.h"
#include "cache.h"
#include "trie.h"


// stack cell:
// [ptr stack_entry [ptr stack_prev stack_next]]
//
// stack entry:
// [ptr data [ptr type nil]]

mword *stack_new(bvm_cache *this_bvm){ // stack_new#

    mword *monkey = _mkls(this_bvm, 2, nil, tptr_new(this_bvm, BABEL_TAG_MONKEY, nil));
    mword *robot  = _mkls(this_bvm, 2, nil, tptr_new(this_bvm, BABEL_TAG_ROBOT,  nil));

    mword *empty_stack = _mkdls(this_bvm, 2, robot, monkey);

    mword *stack_cons = _cons(this_bvm, empty_stack, rci(empty_stack, 1));

    mword *new_stack = _mkls(this_bvm, 3, _val(this_bvm, 0), _val(this_bvm, 0), stack_cons);

    return new_stack;

}


//
//
mword stack_bottom(bvm_cache *this_bvm, mword *stack){ // stack_bottom#

    return 1;

}


//
//
mword stack_twist(bvm_cache *this_bvm, mword *stack){ // stack_twist#

    if(is_stack_empty(this_bvm, stack))
        return 0;

    mword dir = stack_get_dir(this_bvm,stack);

    mword *TOS   = stack_TOS(this_bvm, stack);
    mword *TOS_1 = rci(TOS, 1+dir);

    mword *BOS   = stack_BOS(this_bvm, stack);
    mword *BOS_1 = rci(BOS, 2-dir);

    if(TOS_1 == BOS_1)
        return 1;

    _dlist_cut(this_bvm, TOS, 1,   dir);
    _dlist_cut(this_bvm, BOS, 1, 1-dir);

    // reverse dlist
    _reverse_dlist(this_bvm, TOS_1, TOS_1, dir);

    // join reversed dlist back into stack...
    if(dir){
        _append_direct_dlist(this_bvm, BOS_1, TOS  );
        _append_direct_dlist(this_bvm, BOS,   TOS_1);
    }
    else{
        _append_direct_dlist(this_bvm, TOS,   BOS_1);
        _append_direct_dlist(this_bvm, TOS_1, BOS  );
    }

    return 1;

}


//
//
void stack_give(bvm_cache *this_bvm, mword *list){ // stack_give#

    if(is_nil(list))
        return;

    dstack_push(this_bvm, 
            stack_new_entry(this_bvm,
                (mword*)icar(list),
                nil));

//    stack_push(this_bvm, 
//            rci(this_bvm->dstack_ptr,0), 
//            stack_new_entry(this_bvm,
//                (mword*)icar(list),
//                nil));

    stack_give(this_bvm, (mword*)icdr(list));

}


//
//
void stack_save(bvm_cache *this_bvm, mword *tag){ // stack_save#

    stack_pop( this_bvm, rci(this_bvm->dstack_ptr,0) ); // Manually remove operand

    insym(this_bvm, 
        tag,
        rci(this_bvm->dstack_ptr,0));

    stack_clear(this_bvm, this_bvm->dstack_ptr);

}


//
//
void stack_restore(bvm_cache *this_bvm, mword *tag){ // stack_restore#

    mword *new_stack = lusym(this_bvm, tag);

    lci(this_bvm->dstack_ptr,0) = new_stack;

//    cache_flush(this_bvm);

}


//
//
mword *stack_take(bvm_cache *this_bvm, mword *stack, mword count){ // stack_take#

    mword stack_direction = stack_dir(this_bvm, stack);
    mword depth           = stack_depth(this_bvm, stack);

    mword *result = nil;
    mword *cut_point = nil;
    mword *TOS = stack_TOS(this_bvm, stack);

    cut_point = _dlist_cut(this_bvm, TOS, MIN(count,depth)+1, stack_direction-1);
    result    = _dlist_cut(this_bvm, TOS, 1,                  stack_direction-1);

    if(stack_direction == STACK_NEXT_IS_CDR){
        _append_direct_dlist(this_bvm,TOS,cut_point);
    }
    else{
        _append_direct_dlist(this_bvm,cut_point,TOS);
    }

//    result = _reverse( this_bvm, stack_list_to_list(this_bvm, result, stack_direction), nil );
    result = stack_list_to_list(this_bvm, result, nil, stack_direction);

    return result;

}


// stack-list must not contain a loop
// direction = {1,2}
mword *stack_list_to_list(bvm_cache *this_bvm, mword *stack_list, mword *tail_list, mword direction){  // stack_list_to_list#

    if(is_nil(stack_list))
        return tail_list;

    mword *bs = _cons( this_bvm, rci(rci(rci(stack_list,0),1),0), tail_list );

    return stack_list_to_list(
        this_bvm, 
        rci(stack_list, direction), 
        bs,
        direction);

}


//
//
mword *stack_find_bottom(bvm_cache *this_bvm, mword *stack){ // stack_find_bottom#

    if(is_stack_empty(this_bvm, stack))
        return nil;

//    mword stack_direction = 1+stack_get_dir(this_bvm, stack);
    mword stack_direction = stack_dir(this_bvm, stack);
    mword *curr_entry = rci(stack_TOS(this_bvm, stack), stack_direction);

    if(is_stack_sentinel(this_bvm, curr_entry))
        return nil;

    while(!is_stack_sentinel(this_bvm, rci(curr_entry, stack_direction))){ // XXX PERF
        curr_entry = rci(curr_entry, stack_direction);
    }

    return curr_entry;

}


//
//
mword stack_depth(bvm_cache *this_bvm, mword *stack){ // stack_depth#

    mword stack_direction = stack_dir(this_bvm, stack);
    mword *curr_entry = rci(stack_TOS(this_bvm, stack), stack_direction);
    mword depth=0;

    while(!is_stack_sentinel(this_bvm, curr_entry)){
        depth++;
        curr_entry = rci(curr_entry, stack_direction);
    }

    return depth;

}


//
//
mword stack_diameter(bvm_cache *this_bvm, mword *stack){ // stack_diameter#

    mword stack_direction = stack_dir(this_bvm, stack);

    mword *curr_entry = rci(stack_TOS(this_bvm, stack), stack_direction);
    mword diameter=0;

    if(is_stack_sentinel(this_bvm, curr_entry)){
        return 0;
    }

    while(!is_stack_sentinel(this_bvm, curr_entry)){
        diameter++;
        curr_entry = rci(curr_entry, stack_direction);
    }

    stack_direction = stack_dir_rev(this_bvm, stack);
    curr_entry = rci(stack_TOS(this_bvm, stack), stack_direction);

    while(!is_stack_sentinel(this_bvm, curr_entry)){
        diameter++;
        curr_entry = rci(curr_entry, stack_direction);
    }

    return diameter;

}


//
//
mword *stack_plumb(bvm_cache *this_bvm){ // stack_plumb#

    mword *stack = rci(this_bvm->dstack_ptr,0);    
    mword stack_direction = stack_dir(this_bvm, stack);

    mword *curr_entry = rci(stack_TOS(this_bvm, stack), stack_direction);
    mword diameter=0;

    mword *result = _newlfi(this_bvm, 2, 0);

    if(is_stack_sentinel(this_bvm, curr_entry)){
        return result;
    }

    while(!is_stack_sentinel(this_bvm, curr_entry)){
        diameter++;
        curr_entry = rci(curr_entry, stack_direction);
    }

    lcl(result,0) = diameter;

    stack_direction = stack_dir_rev(this_bvm, stack);
    curr_entry = rci(stack_TOS(this_bvm, stack), stack_direction);

    while(!is_stack_sentinel(this_bvm, curr_entry)){
        diameter++;
        curr_entry = rci(curr_entry, stack_direction);
    }

    lcl(result,1) = diameter;

    return result;

}


//
//
mword stack_dia(bvm_cache *this_bvm, mword *stack){ // stack_dia#

    mword *TOS = stack_TOS(this_bvm, stack);
    return _len_dlist(this_bvm, TOS) - 2; // discount the stack sentinels

}


// swap TOS and TOS+/-1
//
void stack_move_needle(bvm_cache *this_bvm, mword *stack, mword direction){ // stack_move_needle#

    mword *TOS_1;
    mword *TOS       = stack_TOS(this_bvm, stack);

    if(direction == DOWN_DIRECTION){
        if(is_stack_empty(this_bvm, stack))
            return;
        TOS_1 = rci(TOS, 1+stack_get_dir(this_bvm,stack));
//        TOS_1 = rci(TOS, stack_dir(this_bvm,stack));
    }
    else{ // UP_DIRECTION
//        if(is_ustack_empty(this_bvm, stack))
        if(is_ustack_empty2(this_bvm, stack))
            return;
        TOS_1 = rci(TOS, 2-stack_get_dir(this_bvm,stack));
//        TOS_1 = rci(TOS, stack_dir_rev(this_bvm,stack));
    }

    mword *stack_dls = stack_get_dls(this_bvm, stack);

    // swap
    mword *temp   = rci(TOS,   0);
    lci(TOS,   0) = rci(TOS_1, 0);
    lci(TOS_1, 0) = temp;

    // patch up the stack_dls
    lci(stack_dls, stack_get_orient(this_bvm,stack)) = TOS_1;

}


//// swap TOS and TOS+/-1
////
//void stack_move_needle(bvm_cache *this_bvm, mword *stack, mword direction){ // stack_move_needle#
//
//    mword *TOS_1;
//    mword *TOS       = stack_TOS(this_bvm, stack);
//
//    if(direction == DOWN_DIRECTION){
//
////        mword stack_empty1 = is_stack_empty(this_bvm, stack);
////        mword stack_empty2 = is_stack_empty2(this_bvm, stack);
////
////        if(stack_empty1 != stack_empty2){
////            _d(stack_empty1);
////            _d(stack_empty2);
////            _dumpf(this_bvm->dstack_ptr);
////            _msg("_dumpf(this_bvm->dstack_ptr);");
////            _die;
////        }
//
//        if(is_stack_empty2(this_bvm, stack)){
//            return;
//        }
//        TOS_1 = rci(TOS, 1+stack_get_dir(this_bvm,stack));
////        TOS_1 = rci(TOS, stack_dir(this_bvm,stack));
//    }
//    else{
//
//        if(is_ustack_empty2(this_bvm, stack)){ // XXX was is_ustack_empty()
//            return;
//        }
//
//        TOS_1 = rci(TOS, 2-stack_get_dir(this_bvm,stack));
////        TOS_1 = rci(TOS, stack_dir_rev(this_bvm,stack));
//    }
//
//    mword *stack_dls = stack_get_dls(this_bvm, stack);
//
//    // swap
//    mword *temp   = rci(TOS,   0);
//    lci(TOS,   0) = rci(TOS_1, 0);
//    lci(TOS_1, 0) = temp;
//
//    // patch up the stack_dls
//    lci(stack_dls, stack_get_orient(this_bvm,stack)) = TOS_1;
//
//}


//
//
void stack_reset(bvm_cache *this_bvm, mword *stack){ // stack_reset#

    stack_clear(  this_bvm, stack);
    stack_default(this_bvm, stack);

}


//
//
void stack_default(bvm_cache *this_bvm, mword *stack){ // stack_default#

    stack_set_orient(this_bvm, stack, 0);
    stack_set_dir(   this_bvm, stack, 0);

}


//
//
void stack_clear(bvm_cache *this_bvm, mword *stack){ // stack_clear#

    *(mword**)stack = stack_new(this_bvm);
    cache_flush(this_bvm);

}


//
//
void stack_push(bvm_cache *this_bvm, mword *stack, mword *stack_entry){ // stack_push#

    if(stack_get_dir(this_bvm, stack)){ //prev
        _insdls_prev(this_bvm,stack_entry,stack_TOS(this_bvm, stack));
    }
    else{ //next
        _insdls(this_bvm,stack_entry,stack_TOS(this_bvm, stack));
    }

}


//
//
mword *stack_pop(bvm_cache *this_bvm, mword *stack){ // stack_pop#

    if(is_stack_empty(this_bvm, stack)){
        return nil;
    }

    mword *TOS = stack_TOS(this_bvm, stack);
    mword  dir = stack_get_dir(this_bvm, stack);

    mword *to_pop = rci(TOS,   1+dir);
    mword *rest   = rci(to_pop,1+dir);

//mword *popped = rci(rci(rci(to_pop,0),1),0);
//if(is_leaf(popped)){
//    _memi(popped);
//}

//XXX THIS NEEDS TO BE TESTED XXX
    _dlist_cut(this_bvm, TOS,    1, dir);
    _dlist_cut(this_bvm, to_pop, 1, dir);

    if(dir){
        _append_direct_dlist(this_bvm,rest,TOS);
    }
    else{
        _append_direct_dlist(this_bvm,TOS,rest);
    }

    return to_pop;

}


//
//
mword *stack_new_entry(bvm_cache *this_bvm, mword *data, mword *type){ // stack_new_entry#

//    if(this_bvm->flags->MC_GC_PNR == FLAG_SET){
//        _warn("MC_GC_PNR set");
//    }

    mword *entry = _new_dcons(this_bvm);
    lci(entry,0) = _mkls(this_bvm, 2, data, type);
    return entry;

}


//
//
mword is_ustack_empty(bvm_cache *this_bvm, mword *stack){ // is_ustack_empty#

//#define stack_next(bvm,stack,entry) rci(entry, stack_dir(bvm, stack))
    
    return is_stack_sentinel(this_bvm, rci(stack_TOS(this_bvm, stack),2));

}


//
//
mword is_stack_empty2(bvm_cache *this_bvm, mword *stack){ // is_stack_empty2#

    mword *next_bs = stack_next(this_bvm, stack, stack_TOS(this_bvm, stack));

    return is_stack_sentinel(this_bvm, next_bs);

}


//
//
mword is_ustack_empty2(bvm_cache *this_bvm, mword *stack){ // is_ustack_empty2#

    mword *next_bs = stack_next(this_bvm, stack, stack_BOS(this_bvm, stack));

    return is_stack_sentinel(this_bvm, next_bs);

}


//
//
mword is_stack_sentinel(bvm_cache *this_bvm, mword *stack_entry){ // is_stack_sentinel#

//    stack_entry = rci(rci(stack_entry_get_type(this_bvm, stack_entry),0),0);
    stack_entry = (mword*)car((mword*)car(stack_entry_get_type(this_bvm, stack_entry)));

    return (   tageq(stack_entry,BABEL_TAG_MONKEY,TAG_SIZE) 
            || tageq(stack_entry,BABEL_TAG_ROBOT, TAG_SIZE)); 

}


//
//
void dstack_push(bvm_cache *this_bvm, mword *stack_entry){ // dstack_push#

    mword *stack = rci(this_bvm->dstack_ptr, 0);

    if(stack_get_dir(this_bvm, stack)){ //prev
        _insdls_prev(this_bvm,stack_entry,stack_TOS(this_bvm, stack));
    }
    else{ //next
        _insdls(this_bvm,stack_entry,stack_TOS(this_bvm, stack));
    }

}






/*****************************************************************************
 *                                                                           *
 *                             STACK OPERATORS                               *
 *                                                                           *
 ****************************************************************************/


#define ZAP_OPERATIONS 

OPERATORA_R1_W0_D(
        zap, 
        ZAP_OPERATIONS, 
        nil, 
        OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR, 
        0, 
        0)


#define DUP_OPERATIONS \
    result0 = oi0.data;

OPERATORA_R1_W1_P(
        dup, 
        DUP_OPERATIONS, 
        nil, 
        OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR, 
        0, 
        0)


#define DOWN_OPERATIONS \
    stack_down(this_bvm, rci(this_bvm->dstack_ptr,0));

OPERATORS_R0_W0( // XXX WAIVER(OPERATOR) XXX
        down, 
        DOWN_OPERATIONS)


#define UP_OPERATIONS \
    stack_up(this_bvm, rci(this_bvm->dstack_ptr,0));

OPERATORS_R0_W0( // XXX WAIVER(OPERATOR) XXX
        up, 
        UP_OPERATIONS)


#define SWAP_OPERATIONS \
    result0 = oi0.data; \
    result1 = oi1.data;

OPERATORA_R2_W2_D(swap,SWAP_OPERATIONS,
    0,OI_MASK_ANY,0,0,
    0,OI_MASK_ANY,0,0)


// flip toggles the stack direction
#define FLIP_OPERATIONS \
    stack_flip(this_bvm, rci(this_bvm->dstack_ptr,0)); \
    cache_flush(this_bvm);

OPERATORS_R0_W0(
        flip, 
        FLIP_OPERATIONS)


// rot toggles the stack orientation
#define ROT_OPERATIONS \
    stack_rot(this_bvm, rci(this_bvm->dstack_ptr,0)); \
    cache_flush(this_bvm);

OPERATORS_R0_W0(
        rot, 
        ROT_OPERATIONS)


#define TWIST_OPERATIONS \
    stack_twist(this_bvm, rci(this_bvm->dstack_ptr,0)); \
    cache_flush(this_bvm);

OPERATORS_R0_W0(
        twist, 
        TWIST_OPERATIONS)


#define BOTTOM_OPERATIONS \
    stack_bottom(this_bvm, rci(this_bvm->dstack_ptr,0));

OPERATORS_R0_W0(
        bottom, 
        BOTTOM_OPERATIONS)


#define DEPTH_OPERATIONS \
    result0 = _val( this_bvm, stack_depth(this_bvm, rci(this_bvm->dstack_ptr,0)) );

OPERATORA_R0_W1(depth, DEPTH_OPERATIONS)


#define DIA_OPERATIONS \
    result0 = _val( this_bvm, stack_diameter(this_bvm, rci(this_bvm->dstack_ptr,0)) );

OPERATORA_R0_W1(dia, DIA_OPERATIONS)


#define CLEAR_OPERATIONS \
    stack_clear(this_bvm, this_bvm->dstack_ptr); \
    cache_update(this_bvm);

OPERATORS_R0_W0(
        clear, 
        CLEAR_OPERATIONS)


#define TAKE_OPERATIONS \
    stack_pop( this_bvm, rci(this_bvm->dstack_ptr,0) ); \
    result0 = stack_take( this_bvm, rci(this_bvm->dstack_ptr,0), *oi0.data );

OPERATORS_R1_W1_P(take, 
        TAKE_OPERATIONS, 
        0, OI_MASK_LEAF, 0, 0)


#define GIVE_OPERATIONS \
    stack_pop( this_bvm, rci(this_bvm->dstack_ptr,0) ); \
    stack_give( this_bvm, oi0.data );

OPERATORS_R1_W0_P(give, 
        GIVE_OPERATIONS,
        0, OI_MASK_INTE|OI_MASK_TPTR, 0, 0)

//void stack_save(bvm_cache *this_bvm, mword *tag);
//void stack_restore(bvm_cache *this_bvm, mword *tag);

#define STACK_SAVE_OPERATIONS \
    stack_save( this_bvm, oi0.data ); \
    cache_update(this_bvm);

OPERATORS_R1_W0_P(stack_save_op, 
        STACK_SAVE_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_TPTR, 0, 0)

#define STACK_RESTORE_OPERATIONS \
    stack_restore(this_bvm, oi0.data ); \
    cache_update(this_bvm);

OPERATORS_R1_W0_P(stack_restore_op, 
        STACK_RESTORE_OPERATIONS,
        0, OI_MASK_LEAF|OI_MASK_TPTR, 0, 0)


//bvm_cache *take(bvm_cache *this_bvm){ // 
//bvm_cache *give(bvm_cache *this_bvm){ //
//    XXX flip    swap dstack/ustack
//    XXX rot     swap TOS/BOS
//    XXX twist   reverse just the dstack, leave ustack unchanged
//    XXX up      move to prev of TOS
//    XXX down    move to next of TOS    
//    XXX bottom  next of TOS until TOS = BOS (put everything on ustack)
//    XXX depth   size of dstack
//    XXX dia     size ("diameter") of entire stack
//    XXX clear   clears the stack

//        get     grabs N items off dstack and saves them on rstack
//        put     puts items from rstack back onto dstack
//        nest    save current stack, create fresh stack
//        give    push all elements of list on TOS onto the stack
//        take    collect N items off the stack

// Clayton Bauman 2014

