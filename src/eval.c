// XXX STACK FIX DONE
// eval.c
//

#include "babel.h"
#include "eval.h"
#include "stack.h"
#include "bstruct.h"
#include "debug.h"
#include "bvm_opcodes.h"
#include "array.h"
#include "bvm.h"
#include "list.h"

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

//    push_alloc_rstack((mword*)code_ptr, LOOP);

//    push_rstack((mword*)TOS_1);
//    push_rstack((mword*)TOS_0);
//    zap();
//    zap();
//
//    push_rstack((mword*)cdr(code_ptr));

    mword temp_TOS_0 = TOS_0;
    zap();

    push_alloc_rstack((mword*)cdr(code_ptr), LOOP);
    push_alloc_rstack((mword*)temp_TOS_0, LOOP);

    code_ptr = temp_TOS_0;

}

void last(void){
    mword *discard = (mword*)car(pop_rstack());
    code_ptr = nil;
}

mword _last(void){

    mword* discard;

    if(!rstack_empty){
        while(alloc_type(RTOS_0) == DOWN){
            up();
        }
        if(alloc_type(RTOS_0) == NEST){
            up();
        }
        if(alloc_type(RTOS_0) == TIMES){
            if(car(car(RTOS_2)) > 1){
                c((mword*)car(RTOS_2),0) = car(car(RTOS_2)) - 1;
                (mword*)code_ptr = (mword*)car(RTOS_0);
            }
            else{
                discard = pop_rstack();
                (mword*)code_ptr = (mword*)car(pop_rstack());
                discard = pop_rstack();
            }
        }
        else if(alloc_type(RTOS_0) == WHILEOP){
            if(!is_false((mword*)TOS_0)){
                zap();
                (mword*)code_ptr = (mword*)car(RTOS_0);
                push_alloc_rstack((mword*)car(RTOS_2), 0);
            }
            else{
                zap();
                discard = pop_rstack();
                (mword*)code_ptr = (mword*)car(pop_rstack());
                discard = pop_rstack();
            }
        }
        else if(alloc_type(RTOS_0) == EACH){
            if(cdr(car(RTOS_2)) != nil){
                c((mword*)RTOS_2,0) = cdr(car(RTOS_2));
                push_alloc((mword*)car(car(RTOS_2)),EACH);
                (mword*)code_ptr = (mword*)car(RTOS_0);
            }
            else{
                discard = pop_rstack();
                (mword*)code_ptr = (mword*)car(pop_rstack());
                discard = pop_rstack();
            }
        }
        else if(alloc_type(RTOS_0) == EACHAR){
            if(car(car(RTOS_3)) < size((mword*)car(RTOS_2))-1){
                *((mword*)car(RTOS_3)) = car(car(RTOS_3)) + 1;
                push_alloc((mword*)c((mword*)car(RTOS_2),car(car(RTOS_3))),EACHAR);
                (mword*)code_ptr = (mword*)car(RTOS_0);
            }
            else{
                discard = pop_rstack();
                (mword*)code_ptr = (mword*)car(pop_rstack());
                discard = pop_rstack();
                discard = pop_rstack();
            }
        }
        else if(alloc_type(RTOS_0) == LOOP){
            (mword*)code_ptr = (mword*)car(RTOS_0);
        }
        else{
            (mword*)code_ptr = (mword*)car(pop_rstack());
        }
        return 1;
    }

    return 0;

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

    if(car(TOS_0) > 0){
        mword *times = new_atom();
        *times = car(TOS_0);
        push_alloc_rstack(times, TIMES);
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

void each(void){

    //body   RTOS-0
    //return RTOS-1
    //list   RTOS-2

    if(TOS_0 != nil){
        push_alloc_rstack((mword*)TOS_0, EACH);
        mword *temp_list = (mword*)TOS_0;
        push_alloc_rstack((mword*)cdr(code_ptr), EACH);

        zap();
        mword temp_code_ptr = TOS_0;
        zap();
        code_ptr = temp_code_ptr;
        push_alloc_rstack((mword*)code_ptr, EACH);
        push_alloc((mword*)car(temp_list),EACH);

    }
    else{
        code_ptr = cdr(code_ptr);
    }

}

void eachar(void){

    //body   RTOS-0
    //return RTOS-1
    //array  RTOS-2
    //count  RTOS-3

    if(TOS_0 != nil){
        mword *count = _newlf(1);
        *count = 0;
        push_alloc_rstack((mword*)count,EACHAR);
        push_alloc_rstack((mword*)TOS_0, EACHAR);
        mword *temp_array = (mword*)TOS_0;
        push_alloc_rstack((mword*)cdr(code_ptr), EACHAR);

        zap();
        mword temp_code_ptr = TOS_0;
        zap();
        code_ptr = temp_code_ptr;
        push_alloc_rstack((mword*)code_ptr, EACHAR);
        push_alloc((mword*)c(temp_array,*count),EACHAR);
    }
    else{
        code_ptr = cdr(code_ptr);
    }

}

// Clayton Bauman 2011

