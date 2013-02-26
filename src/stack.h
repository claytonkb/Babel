// stack.h
// 

#ifndef STACK_H
#define STACK_H

mword *new_dstack_entry(mword *operand, mword alloc_type);
void push_udr_stack(mword *stack_ptr, mword *stack_entry);
mword *pop_udr_stack(mword *stack_ptr);
mword *new_rstack_entry(mword *operand, mword alloc_type);
mword *get_from_dstack(bvm_cache *this_bvm, mword stack_index);

bvm_cache *hard_zap     (bvm_cache *this_bvm);
bvm_cache *swap         (bvm_cache *this_bvm);
bvm_cache *sel          (bvm_cache *this_bvm);
bvm_cache *dup          (bvm_cache *this_bvm);
bvm_cache *depth        (bvm_cache *this_bvm);
bvm_cache *clear        (bvm_cache *this_bvm);
bvm_cache *give         (bvm_cache *this_bvm);
bvm_cache *take         (bvm_cache *this_bvm);
bvm_cache *zap          (bvm_cache *this_bvm);
bvm_cache *down         (bvm_cache *this_bvm);
bvm_cache *up           (bvm_cache *this_bvm);
//bvm_cache *nest         (bvm_cache *this_bvm);
//bvm_cache *unnest       (bvm_cache *this_bvm);
bvm_cache *flip         (bvm_cache *this_bvm);

mword *get_from_stack(bvm_cache *this_bvm, mword *stack_entry);

void push_alloc(bvm_cache *this_bvm, mword *operand, mword alloc_type);
void push_alloc_rstack(bvm_cache *this_bvm, mword *operand, mword alloc_type);
void rgive(bvm_cache *this_bvm, mword *list);
//mword *rtake   (bvm_cache *this_bvm, mword count);
mword *pop_rstack(bvm_cache *this_bvm);
void free_stack_entry(bvm_cache *this_bvm);

void push_alloc_ustack(bvm_cache *this_bvm, mword *operand, mword alloc_type);
mword *pop_ustack(bvm_cache *this_bvm);

#define SELF_ALLOC 0
#define AUTO_ALLOC 1

//void push_alloc_rstack(mword *operand, mword alloc_type);
//void zap(void);
//void push_rstack(mword *ret);
////void pop_rstack(void);
//void sel(void);
//void _zap(mword **target);
//void dup(void);
//void swap(void);
//void down(void);
//void up(void);
//void take(void);
//mword *take_tree(mword *stack, mword count);
//void depth(void);
//void give(void);
//void give_tree(mword *list);
//void nest(void);
//void clear(void);
//mword *cons_alloc(mword *car, mword *cdr);
//
////#ifdef DEBUG
////mword STACK_TRACE;
////void toggle_stack_trace(void);
////#endif
//
#endif //STACK_H

// Clayton Bauman 2011

