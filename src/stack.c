// XXX STACK FIX DONE
// stack.c
//

#include "babel.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"
#include "bvm_opcodes.h"
#include "array.h"



void push_alloc_rstack(mword *operand, mword alloc_type){

    mword *temp_consA = new_cons();
    mword *temp_consB = new_cons();
    mword *temp_consC = new_cons();
    mword *type       = new_atom();

    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc

    cons(temp_consA,   type,         nil);
    cons(temp_consB,   operand,      temp_consA);
    cons(temp_consC,   temp_consB,   rstack_ptr);

    (mword*)rstack_ptr = temp_consC;

//    d(car(car(cdr(car(rstack_ptr)))))
//        die

}

void push_alloc(mword *operand, mword alloc_type){

    mword *temp_consA = new_cons();
    mword *temp_consB = new_cons();
    mword *temp_consC = new_cons();
    mword *type       = new_atom();

    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc

    cons(temp_consA,   type,         nil);
    cons(temp_consB,   operand,      temp_consA);
    cons(temp_consC,   temp_consB,   stack_ptr);

    (mword*)stack_ptr = temp_consC;

}

//
//
void zap(void){

    if(stack_ptr == nil){
        return;
    }

    //TODO: Implement this... currently, we are leaking memory
    //operand_dealloc(car(cdr(car(stack_ptr))));

    mword *temp_stack_ptr = (mword*)cdr(stack_ptr);

    free((mword*)(car(cdr(car(stack_ptr)))-1));
    free((mword*)(cdr(car(stack_ptr))-1));
    free((mword*)(car(stack_ptr)-1));
    free((mword*)(stack_ptr-1));

    (mword*)stack_ptr = temp_stack_ptr;

}

//
//
void _zap(mword **target){

    //Implement a "generic zap" at some point...

}

//
//
void push_rstack(mword *ret){

    mword *temp_cons = new_cons();

    cons(temp_cons,   ret,   rstack_ptr);

    (mword*)rstack_ptr = temp_cons;

}

//
//
mword *pop_rstack(void){

    //If rstack is empty: except()

    mword *temp_rstack = (mword*)rstack_ptr;
//    code_ptr = car(rstack_ptr);

    (mword*)rstack_ptr = (mword*)cdr(rstack_ptr);
    //free((mword*)(temp_rstack-1)); //FIXME: THERE'S NO CHECKING!!!
    //    mword *temp_cons = new_cons();

    return (mword*)car(temp_rstack);

}

//
//
void sel(void){

    mword select = car(TOS_2);

//    zap();
    cdr(cdr(stack_ptr)) = cdr(cdr(cdr(stack_ptr)));

    //TODO: Add behavior for nil
    if(!select){
        zap();
    }
    else{
        cdr(stack_ptr) = cdr(cdr(stack_ptr));
    }

}

//
//
void dup(void){

    mword *result;
    if(is_leaf((mword*)TOS_0)){
        result = _newlf(size((mword*)TOS_0));
    }
    else{
        result = _newin(size((mword*)TOS_0));
    }    

    //TODO: memcpy!
    mword i;
    for(    i=0;
            i<size((mword*)TOS_0);
            i++
        ){

        c(result,i) = c((mword*)TOS_0,i);

    }    

    push_alloc(result, DUP);

}


//
//
void swap(void){

    // z -> A -> B -> C
    // 
    // z -> B
    // B -> A
    // A -> C

    if( 
        stack_ptr           == nil
            ||
        cdr(stack_ptr)      == nil
    ){
        return;
    }

    mword *tempA =     (mword*)cdr(stack_ptr);
    mword *tempB = (mword*)cdr(cdr(stack_ptr));

        cdr(cdr(stack_ptr)) = stack_ptr;
    (mword*)cdr(stack_ptr)  = tempB;
        (mword*)stack_ptr   = tempA;

}

void down(void){

    push_alloc_rstack((mword*)TOS_0,DOWN);
    zap();

}

void up(void){

    if(alloc_type(RTOS_0) == DOWN){
        push_alloc((mword*)car(pop_rstack()),UP);
    }
    else{ //NEST
        stack_ptr = car(pop_rstack());
    }

}

void take(void){

    mword count = car(TOS_0);
    mword *temp;

    zap();

    mword *result = take_tree((mword*)stack_ptr,count);

    push_alloc(result,TAKE);

}

mword *take_tree(mword *stack, mword count){

    if(count == 0)
        return (mword*)nil;

    mword *temp = new_cons();
    cons(temp,car(car(stack)),take_tree((mword*)cdr(stack),count-1));

    return temp;

}

void depth(void){

    mword *result = new_atom();
    *result = _len((mword*)stack_ptr);

    push_alloc(result, DEPTH);

}

void give(void){

    mword *list = (mword*)TOS_0;

    zap();

    give_tree((mword*)list);

}

void give_tree(mword *list){

    if(list == (mword*)nil)
        return;

    give_tree((mword*)cdr(list));

    push_alloc((mword*)car(list),GIVE);

}

void clear(void){

    while(TOS_0 != nil){
        zap();
    }

}

void nest(void){

    mword *new_stack = (mword*)TOS_0;

    zap();
    push_alloc_rstack((mword*)stack_ptr,NEST);

    clear();

    give_tree(new_stack);

}


// Clayton Bauman 2011

