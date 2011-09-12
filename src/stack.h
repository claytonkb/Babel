// stack.h
// 

#ifndef STACK_H
#define STACK_H

#define SELF_ALLOC 0
#define AUTO_ALLOC 1

void push_alloc(mword *operand, mword alloc_type);
void zap(void);
void push_rstack(mword *ret);
void pop_rstack(void);
void sel(void);
void _zap(mword **target);
void dup(void);
void swap(void);

#ifdef DEBUG
mword STACK_TRACE;
void toggle_stack_trace(void);
#endif

#endif //STACK_H

// Clayton Bauman 2011

