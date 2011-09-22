// XXX STACK FIX DONE
// stack.c
//

#include "babel.h"
#include "stack.h"
#include "list.h"
#include "count.h"
#include "bvm_opcodes.h"
#include "array.h"

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

//#ifdef DEBUG
//    if(STACK_TRACE){
//        return;
//    }
//#endif

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
//    mword *temp_consB = new_cons();
//    mword *temp_consC = new_cons();
//    mword *type       = new_atom();

//    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc

//    cons(temp_consA,   type,         nil);
//    cons(temp_consB,   operand,      temp_consA);
    cons(temp_cons,   ret,   rstack_ptr);

    (mword*)rstack_ptr = temp_cons;

}

//
//
void pop_rstack(void){

    //If rstack is empty: except()

    mword *temp_rstack = (mword*)rstack_ptr;
    code_ptr = car(rstack_ptr);
    (mword*)rstack_ptr = (mword*)cdr(rstack_ptr);
    free((mword*)(temp_rstack-1)); //FIXME: THERE'S NO CHECKING!!!
    //    mword *temp_cons = new_cons();

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


////FIXME: Get rid of this crap:
//#ifdef DEBUG
//void toggle_stack_trace(void){
//    STACK_TRACE = !STACK_TRACE;
//}
//#endif

// Clayton Bauman 2011

