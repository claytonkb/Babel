// stack.h
// 

#ifndef STACK_H
#define STACK_H

//stack -> rci(this_bvm->dstack_ptr,0)

#define stack_get_dls(bvm,stack)                _ith(bvm, stack, 0) // stack_get_dls#
//#define stack_set_dls(bvm,stack,x)    (lci(lci(_cdri(bvm, stack, 0), 0), 0) = (x))

#define stack_get_orient(bvm,stack)        rcl( _ith(bvm, stack, 1), 0) // stack_get_orient#
#define stack_set_orient(bvm,stack,x) (lcl(lci(_cdri(bvm, stack, 1), 0), 0) = (x)) // stack_set_orient#
#define stack_rot(bvm,stack)          (lcl(lci(_cdri(bvm, stack, 1), 0), 0) = !(rcl( _ith(bvm, stack, 1), 0))) // stack_rot#

#define stack_get_dir(bvm,stack)           rcl( _ith(bvm, stack, 2), 0) // stack_get_dir#
#define stack_set_dir(bvm,stack,x)    (lcl(lci(_cdri(bvm, stack, 2), 0), 0) = (x)) // stack_set_dir#

#define stack_flip(bvm,stack)         (lcl(lci(_cdri(bvm, stack, 2), 0), 0) = !(rcl( _ith(bvm, stack, 2), 0))) // stack_flip#

#define stack_dir(bvm,stack)       (1 + ( stack_get_dir(bvm,stack) )) // stack_dir#
#define stack_dir_rev(bvm,stack)   (3 - ( stack_dir(bvm,stack)     )) // stack_dir_rev#

#define stack_next(bvm,stack,entry) rci(entry, stack_dir(bvm, stack))
#define stack_prev(bvm,stack,entry) rci(entry, stack_dir_rev(bvm, stack))

//#define stack_ith_entry(bvm, stack, i) rci( rci( stack_TOS(bvm, stack), stack_dir(bvm, stack)), 0)

//#define stack_mkcell(bvm,data,type) _mkls(bvm,2,type,data)

#define stack_entry_get_type(bvm,entry) _cdri(bvm, entry, 0) // stack_entry_get_type#
#define stack_entry_get_data(bvm,entry) _cdri(bvm, entry, 1) // stack_entry_get_data#

#define stack_cell_set_next(bvm,cell,x)  (lci(_cdri(bvm,cell,0),0)=(x)) // stack_cell_set_next#
#define stack_cell_set_prev(bvm,cell,x)  (lci(_cdri(bvm,cell,1),0)=(x)) // stack_cell_set_prev#

#define stack_TOS(bvm,stack) rci(stack_get_dls(bvm,stack),   stack_get_orient(bvm,stack)) // stack_TOS#
#define stack_BOS(bvm,stack) rci(stack_get_dls(bvm,stack), 1-stack_get_orient(bvm,stack)) // stack_BOS#

#define DOWN_DIRECTION 0 // DOWN_DIRECTION#
#define   UP_DIRECTION 1 // UP_DIRECTION#

#define STACK_NEXT_IS_CDR 1 // STACK_NEXT_IS_CDR#
#define STACK_NEXT_IS_CPR 2 // STACK_NEXT_IS_CPR#

#define stack_down(bvm, stack) stack_move_needle(this_bvm, stack, DOWN_DIRECTION) // stack_down#
#define stack_up(bvm, stack)   stack_move_needle(this_bvm, stack, UP_DIRECTION) // stack_up#

#define dstack_begin rci(this_bvm->dstack_ptr,0)

#define is_stack_empty(bvm, stack) (stack_depth(bvm,stack)==0) // is_stack_empty#

#define dstack_inc(x) \
    x->dstack_depth++; \
    x->dstack_diameter++; \

#define dstack_dec(x) \
    x->dstack_depth--; \
    x->dstack_diameter--; \

#define dstack_dec_down(x) \
    x->dstack_depth--; \
    x->dstack_diameter++; \

#define dstack_inc_up(x) \
    x->dstack_depth++; \
    x->dstack_diameter--; \

#define dstack_depth_swap(x) \
    mword _dstack_depth_swap_temp = x->dstack_depth; \
    x->dstack_depth = x->dstack_diameter; \
    x->dstack_diameter = _dstack_depth_swap_temp;

#define dstack_depth_zero(x) \
    x->dstack_depth = 0; \
    x->dstack_diameter = 0;

#define dstack_depth_adj(x,y) \
    x->dstack_depth += (y); \
    x->dstack_diameter += (y);

#define dstack_depth_update(x) \
    x->dstack_depth = stack_depth(x, rci(x->dstack_ptr,0)); \
    x->dstack_diameter = stack_diameter(x, rci(x->dstack_ptr,0));

mword is_ustack_empty2(bvm_cache *this_bvm, mword *stack);
mword is_stack_empty2(bvm_cache *this_bvm, mword *stack);

//mword *stack_list_to_list(bvm_cache *this_bvm, mword *stack_list, mword direction);
mword *stack_list_to_list(bvm_cache *this_bvm, mword *stack_list, mword *tail_list, mword direction);

mword *stack_new(bvm_cache *this_bvm);
void   stack_push(bvm_cache *this_bvm, mword *stack, mword *stack_entry);
mword *stack_pop(bvm_cache *this_bvm, mword *stack);
mword  is_stack_sentinel(bvm_cache *this_bvm, mword *stack_entry);
mword *stack_new_entry(bvm_cache *this_bvm, mword *data, mword *type);
void   stack_clear(bvm_cache *this_bvm, mword *stack);
void   stack_default(bvm_cache *this_bvm, mword *stack);
void   stack_reset(bvm_cache *this_bvm, mword *stack);
void   stack_move_needle(bvm_cache *this_bvm, mword *stack, mword direction);
mword  is_ustack_empty(bvm_cache *this_bvm, mword *stack);
mword  stack_dia(bvm_cache *this_bvm, mword *stack);
mword  stack_depth(bvm_cache *this_bvm, mword *stack);
mword *stack_find_bottom(bvm_cache *this_bvm, mword *stack);
mword  stack_twist(bvm_cache *this_bvm, mword *stack);
mword  stack_bottom(bvm_cache *this_bvm, mword *stack);
void   stack_ins(bvm_cache *this_bvm, mword *stack_prev, mword *stack_entry);
mword  stack_diameter(bvm_cache *this_bvm, mword *stack);
mword *stack_take(bvm_cache *this_bvm, mword *stack, mword count);
void dstack_push(bvm_cache *this_bvm, mword *stack_entry);
mword *stack_plumb(bvm_cache *this_bvm);

void stack_give(bvm_cache *this_bvm, mword *list);
void stack_restore(bvm_cache *this_bvm, mword *tag);
void stack_save(bvm_cache *this_bvm, mword *tag);

bvm_cache *zap(bvm_cache *this_bvm);
bvm_cache *dup(bvm_cache *this_bvm);
bvm_cache *down(bvm_cache *this_bvm);
bvm_cache *up(bvm_cache *this_bvm);
bvm_cache *swap(bvm_cache *this_bvm);
bvm_cache *flip(bvm_cache *this_bvm);
bvm_cache *rot(bvm_cache *this_bvm);
bvm_cache *twist(bvm_cache *this_bvm);
bvm_cache *bottom(bvm_cache *this_bvm);
bvm_cache *depth(bvm_cache *this_bvm);
bvm_cache *dia(bvm_cache *this_bvm);
bvm_cache *clear(bvm_cache *this_bvm);
bvm_cache *take(bvm_cache *this_bvm);
bvm_cache *give(bvm_cache *this_bvm);

bvm_cache *stack_save_op(bvm_cache *this_bvm);
bvm_cache *stack_restore_op(bvm_cache *this_bvm);

#endif //STACK_H

// Clayton Bauman 2011

