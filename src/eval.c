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

//    push_rstack((mword*)TOS_1);
//    push_rstack((mword*)TOS_0);
//    zap();
//    zap();
//
//    push_rstack((mword*)cdr(code_ptr));

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

//    mword temp_TOS_0;
//    mword *temp_code_ptr;
//
//    if(is_leaf((mword*)car(car(TOS_0)))){ //counter
//        printf("is_leaf\n");
//        if(car(car(TOS_0))>1){
//            printf("is > 1\n");
//            *((mword*)car(TOS_0)) = car(car(TOS_0)) - 1;
//            zap();
//            temp_code_ptr = (mword*)car(rstack_ptr);
//            push_rstack((mword*)code_ptr);
//            (mword*)code_ptr = temp_code_ptr;
//        }
//        else{
//            printf("!is > 1\n");
//            zap();
//            pop_rstack(); //discard
//            code_ptr = cdr(code_ptr);
//        }
//    }
//    else{ //acts almost like eval
//        printf("!is_leaf\n");
//        die
//        temp_TOS_0 = TOS_0;
//        zap();
//
//        push_rstack((mword*)temp_TOS_0); //diff from eval
//        push_rstack((mword*)code_ptr);
//
//        code_ptr = temp_TOS_0;
//    }
//
//
//
//    mword temp_TOS_1 = TOS_1;
//
//    if(car(car(TOS_0))>1){
//        *((mword*)car(TOS_0)) = car(car(TOS_0)) - 1;
//        zap();
//        zap();
//        push_rstack((mword*)code_ptr);
//        code_ptr = temp_TOS_1;
//    }
//    else{
//        zap();
//        zap();
//        code_ptr = cdr(code_ptr); //go to the next instr
//    }

//    mword *save_code_ptr;
//    if(car(car(TOS_0))>1){
//        *((mword*)car(TOS_0)) = car(car(TOS_0)) - 1;
//        zap();
//        pop_rstack();
//    }
//    else{
//        zap();
//        save_code_ptr = (mword*)cdr(code_ptr);
//        pop_rstack();
//        (mword*)code_ptr = save_code_ptr;
//    }

//    mword *count     = pop_val_rstack();
//    mword *loop_body = pop_val_rstack();
//
//    if(car(car(count))>1){
//        *((mword*)car(count)) = car(car(count)) - 1;
////        zap();
////        pop_rstack();
//        code_ptr = car(loop_body);
//    }
//    else{
//        (mword*)code_ptr = (mword*)cdr(code_ptr);
//    }

}

void dieop(void){
    printf("Died.\n");
    exit(0);
}

// Clayton Bauman 2011

