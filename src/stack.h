// stack.h
// 

#ifndef STACK_H
#define STACK_H

bvm_cache *hard_zap     (bvm_cache *this_bvm);
bvm_cache *swap         (bvm_cache *this_bvm);
bvm_cache *sel          (bvm_cache *this_bvm);
bvm_cache *dup          (bvm_cache *this_bvm);
bvm_cache *depth        (bvm_cache *this_bvm);
bvm_cache *clear        (bvm_cache *this_bvm);
bvm_cache *give         (bvm_cache *this_bvm);
bvm_cache *take         (bvm_cache *this_bvm);

void push_alloc(bvm_cache *this_bvm, mword *operand, mword alloc_type);
void give_tree(bvm_cache *this_bvm, mword *list);
mword *rtake(bvm_cache *this_bvm, mword count);

//#include "list.h"
//
#define SELF_ALLOC 0
#define AUTO_ALLOC 1
//
//void push_alloc_rstack(mword *operand, mword alloc_type);
//void zap(void);
//void push_rstack(mword *ret);
////void pop_rstack(void);
//mword *pop_rstack(void);
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

