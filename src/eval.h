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
bvm_cache *next(bvm_cache *this_bvm);
bvm_cache *continueop(bvm_cache *this_bvm);
bvm_cache *each(bvm_cache *this_bvm);
bvm_cache *ifop(bvm_cache *this_bvm);
bvm_cache *eachar(bvm_cache *this_bvm);

#define LOOP_RSTACK_ENTRIES 2
#define LOOP_RSTACK_BODY    0
#define LOOP_RSTACK_RETURN  1

#define TIMES_RSTACK_ENTRIES  3
#define TIMES_RSTACK_COUNT    0
#define TIMES_RSTACK_BODY     1
#define TIMES_RSTACK_RETURN   2

#define WHILE_RSTACK_ENTRIES  4
#define WHILE_RSTACK_COND     0
#define WHILE_RSTACK_BODY     1
#define WHILE_RSTACK_RETURN   2
#define WHILE_RSTACK_SELECT   3

#define WHILE_BODY 0
#define WHILE_COND 1

#define EACH_RSTACK_ENTRIES  4
#define EACH_RSTACK_LIST     0
#define EACH_RSTACK_BODY     1
#define EACH_RSTACK_RETURN   2

#define EACHAR_RSTACK_ENTRIES  4
#define EACHAR_RSTACK_ARRAY    0
#define EACHAR_RSTACK_BODY     1
#define EACHAR_RSTACK_RETURN   2
#define EACHAR_RSTACK_COUNT    3
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

