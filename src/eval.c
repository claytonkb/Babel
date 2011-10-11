// XXX STACK FIX DONE
// eval.c
//

#include "babel.h"
#include "eval.h"
#include "stack.h"
#include "count.h"
#include "debug.h"

void eval(void){

//    d(car(car(car(TOS_0))))
//        die

    mword temp_TOS_0 = TOS_0;
    zap();

    push_rstack((mword*)cdr(code_ptr));

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

    push_rstack((mword*)cdr(code_ptr));
    code_ptr = car(car(stack_ptr)); // PRE STACK FIX
//    code_ptr = car(stack_ptr);
    zap();

}

void ret(void){

    // FIXME: CHECK FOR EMPTY RSTACK
    pop_rstack();

}

void loop(void){

    push_rstack((mword*)code_ptr);

//    code_ptr = TOS_0;

}

void last(void){
    pop_rstack();
    code_ptr = nil;
}

void next(void){
    pop_rstack();
//    code_ptr = nil;
}

void whileop(void){

    if(car(TOS_0)){
        zap();
        pop_rstack();
    }
    else{
        zap();
        pop_rstack();
        code_ptr = cdr(code_ptr);
    }

}

void times(void){

    mword *save_code_ptr;

    if(car(car(TOS_0))>1){
        *((mword*)car(TOS_0)) = car(car(TOS_0)) - 1;
        zap();
        pop_rstack();
    }
    else{
        zap();
        save_code_ptr = (mword*)cdr(code_ptr);
        pop_rstack();
        (mword*)code_ptr = save_code_ptr;
    }

}

void dieop(void){
    printf("Died.\n");
    exit(0);
}

// Clayton Bauman 2011

