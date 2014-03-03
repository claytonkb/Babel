// eval.h
// 

#ifndef EVAL_H
#define EVAL_H

bvm_cache *dieop(bvm_cache *this_bvm);
bvm_cache *eval(bvm_cache *this_bvm);
bvm_cache *gotoop(bvm_cache *this_bvm);
bvm_cache *loop(bvm_cache *this_bvm);
bvm_cache *times(bvm_cache *this_bvm);
bvm_cache *whileop(bvm_cache *this_bvm);
bvm_cache *each(bvm_cache *this_bvm);
bvm_cache *ifte(bvm_cache *this_bvm);
bvm_cache *iter(bvm_cache *this_bvm);
bvm_cache *next(bvm_cache *this_bvm);
bvm_cache *_next(bvm_cache *this_bvm);
bvm_cache *last(bvm_cache *this_bvm);
bvm_cache *let(bvm_cache *this_bvm);
bvm_cache *cond(bvm_cache *this_bvm);
bvm_cache *alt(bvm_cache *this_bvm);

bvm_cache *eachar(bvm_cache *this_bvm);
bvm_cache *conjure(bvm_cache *this_bvm);
void _eval(bvm_cache *this_bvm, mword *eval_body, mword *eval_return);
bvm_cache *ifop(bvm_cache *this_bvm);
bvm_cache *pass(bvm_cache *this_bvm);
bvm_cache *fail(bvm_cache *this_bvm);
bvm_cache *seq(bvm_cache *this_bvm);

#define EVAL_RSTACK_ENTRIES  1
#define EVAL_RSTACK_RETURN   0

#define IFTE_RSTACK_ENTRIES 4
#define IFTE_RSTACK_RETURN  0
#define IFTE_RSTACK_THEN    1
#define IFTE_RSTACK_ELSE    2
#define IFTE_RSTACK_SELECT  3

#define IFTE_BODY 0
#define IFTE_COND 1

#define COND_BODY   0
#define COND_COND   1
#define COND_RETURN 2

#define LET_RSTACK_ENTRIES  2
#define LET_RSTACK_RETURN   0
#define LET_RSTACK_LIST     1

#define IFOP_RSTACK_ENTRIES 3
#define IFOP_RSTACK_RETURN  0
#define IFOP_RSTACK_THEN    1
#define IFOP_RSTACK_SELECT  2

#define IFOP_BODY 0
#define IFOP_COND 1

#define NEST_RSTACK_ENTRIES 4
#define NEST_RSTACK_RETURN  0
#define NEST_RSTACK_BODY    1
#define NEST_RSTACK_STACK   2
#define NEST_RSTACK_USTACK  3

#define LOOP_RSTACK_ENTRIES 3
#define LOOP_RSTACK_RETURN  0
#define LOOP_RSTACK_BODY    1
#define LOOP_RSTACK_ITER    2

#define TIMES_RSTACK_ENTRIES  4
#define TIMES_RSTACK_RETURN   0
#define TIMES_RSTACK_BODY     1
#define TIMES_RSTACK_ITER     2
#define TIMES_RSTACK_COUNT    3

#define WHILE_RSTACK_ENTRIES  5
#define WHILE_RSTACK_RETURN   0
#define WHILE_RSTACK_BODY     1
#define WHILE_RSTACK_ITER     2
#define WHILE_RSTACK_COND     3
#define WHILE_RSTACK_SELECT   4

#define WHILE_BODY 0
#define WHILE_COND 1

#define EACH_RSTACK_ENTRIES  4
#define EACH_RSTACK_RETURN   0
#define EACH_RSTACK_BODY     1
#define EACH_RSTACK_ITER     2
#define EACH_RSTACK_LIST     3

#define EACHAR_RSTACK_ENTRIES  5
#define EACHAR_RSTACK_RETURN   0
#define EACHAR_RSTACK_BODY     1
#define EACHAR_RSTACK_ITER     2
#define EACHAR_RSTACK_COUNT    3
#define EACHAR_RSTACK_ARRAY    4

#define EACHAR_INIT_INDEX      0

//void eval(void);
//void gotoop(void);
////void call(void);
////void ret(void);
//void loop(void);
//void last(void);
//void next(void);
//void whileop(void);
//void each(void);
//void times(void);
//void eachar(void);
//mword _end_of_code(void);
//mword _end_of_code2(void);
//void breakop(void);
//void continueop(void);
//

#endif //EVAL_H

// Clayton Bauman 2011

