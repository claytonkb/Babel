// XXX STACK FIX DONE
// eval.c
//

#include "babel.h"
#include "eval.h"
#include "stack.h"
#include "count.h"
#include "debug.h"
#include "bvm_opcodes.h"

void eval(void){

//    d(car(car(car(TOS_0))))
//        die

    mword temp_TOS_0 = TOS_0;
    zap();

    push_alloc_rstack((mword*)cdr(code_ptr), EVAL);

    code_ptr = temp_TOS_0;
//    car(code_ptr) = TOS_0;
//    zap();

}

void gotoop(void){

    code_ptr = car(car(stack_ptr)); // PRE STACK FIX
//    code_ptr = car(stack_ptr);
    zap();

}

void call(void){

    push_alloc_rstack((mword*)cdr(code_ptr), CALL);
    code_ptr = car(car(stack_ptr)); // PRE STACK FIX
//    code_ptr = car(stack_ptr);
    zap();

}

void ret(void){

    // FIXME: CHECK FOR EMPTY RSTACK
    code_ptr = car(pop_rstack());

}

void loop(void){

    push_alloc_rstack((mword*)code_ptr, LOOP);

//    push_rstack((mword*)TOS_1);
//    push_rstack((mword*)TOS_0);
//    zap();
//    zap();
//
//    push_rstack((mword*)cdr(code_ptr));

}

void last(void){
    mword *discard = (mword*)car(pop_rstack());
    code_ptr = nil;
}

void next(void){
    (mword*)code_ptr = (mword*)car(pop_rstack());
//    code_ptr = nil;
}

// (body) (cond) while
void whileop(void){

    //body   RTOS-0
    //return RTOS-1
    //cond   RTOS-2

    mword *cond_block = (mword*)TOS_0;
    push_alloc_rstack((mword*)cond_block,    WHILEOP);
    push_alloc_rstack((mword*)cdr(code_ptr), WHILEOP);

    zap();

    push_alloc_rstack((mword*)TOS_0, WHILEOP); //loop body

    zap();

    (mword*)code_ptr = cond_block;
    
}

// (body) [x] times
void times(void){

    //times  RTOS-2
    //return RTOS-1
    //body   RTOS-0

    if(car(car(TOS_0)) > 0){
        push_alloc_rstack((mword*)TOS_0, TIMES);
        push_alloc_rstack((mword*)cdr(code_ptr), TIMES);

        zap();
        mword temp_TOS_0 = TOS_0;
        zap();
        code_ptr = temp_TOS_0;
        push_alloc_rstack((mword*)code_ptr, TIMES);
    }
    else{
        code_ptr = cdr(code_ptr);
    }

}

void dieop(void){
    fprintf(stderr, "Died.\n");
    exit(0);
}

// Clayton Bauman 2011

