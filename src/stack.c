// XXX STACK FIX DONE
// stack.c
//

#include "babel.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"
#include "bvm_opcodes.h"
#include "array.h"
#include "alloc.h"
#include "bvm_stack.h"

//mword *cons_alloc(mword *car, mword *cdr){
//
//    mword *temp = new_cons();
//    cons(temp,car,cdr);
//    return temp;
//
//}
//
//void push_alloc_rstack(mword *operand, mword alloc_type){
//
//    mword *temp_consA = new_cons();
//    mword *temp_consB = new_cons();
//    mword *temp_consC = new_cons();
//    mword *type       = new_atom();
//
//    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc
//
//    cons(temp_consA,   type,         nil);
//    cons(temp_consB,   operand,      temp_consA);
//    cons(temp_consC,   temp_consB,   rstack_ptr);
////    cons(temp_consC,   rstack_ptr, temp_consB);
//
//    (mword*)rstack_ptr = temp_consC;
//
////    d(car(car(cdr(car(rstack_ptr)))))
////        die
//
//}

#define STACK_ENTRY_SIZE 2

#define STACK_ENTRY_VAL(x) ((mword*)c(x,0))
#define STACK_ENTRY_TYP(x) ((mword*)c(x,1))

//
void push_alloc(bvm_cache *this_bvm, mword *operand, mword alloc_type){

    mword *new_stack_cons  = new_cons;
    mword *new_stack_entry = _newin(STACK_ENTRY_SIZE);
    mword *type            = new_atom;

    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc
    STACK_ENTRY_VAL(new_stack_entry) = operand;
    STACK_ENTRY_TYP(new_stack_entry) = type;

    cons(new_stack_cons, new_stack_entry, this_bvm->stack_ptr);

    this_bvm->stack_ptr = new_stack_cons;

}

//
bvm_cache *hard_zap(bvm_cache *this_bvm){

    if(is_nil(this_bvm->stack_ptr)){
        return this_bvm;
    }

    mword *temp_stack_ptr = (mword*)cdr(this_bvm->stack_ptr);

    bfree(STACK_ENTRY_TYP((mword*)car(this_bvm->stack_ptr)));
    bfree(car(this_bvm->stack_ptr));
    bfree(this_bvm->stack_ptr);

    this_bvm->stack_ptr = temp_stack_ptr;

    return this_bvm;

}

//
bvm_cache *zap(bvm_cache *this_bvm){

    if(is_nil(this_bvm->stack_ptr)){
        return this_bvm;
    }

    mword *temp_stack_ptr = (mword*)cdr(this_bvm->stack_ptr);

    mword *val = STACK_ENTRY_VAL((mword*)car(this_bvm->stack_ptr));

    zap_switch(car(STACK_ENTRY_TYP((mword*)car(this_bvm->stack_ptr))))

    bfree(STACK_ENTRY_TYP((mword*)car(this_bvm->stack_ptr)));
    bfree(car(this_bvm->stack_ptr));
    bfree(this_bvm->stack_ptr);

    this_bvm->stack_ptr = temp_stack_ptr;

    return this_bvm;

}

// TODO: Implement a "soft zap" that performs per-operator de-allocation

////
////
//void _zap(mword **target){
//
//    //Implement a "generic zap" at some point...
//
//}
//
////
////
//void push_rstack(mword *ret){
//
//    mword *temp_cons = new_cons();
//
//    cons(temp_cons,   ret,   rstack_ptr);
//
//    (mword*)rstack_ptr = temp_cons;
//
//}
//
////
////
//mword *pop_rstack(void){
//
//    //If rstack is empty: except()
//
//    mword *temp_rstack = (mword*)rstack_ptr;
////    code_ptr = car(rstack_ptr);
//
//    (mword*)rstack_ptr = (mword*)cdr(rstack_ptr);
//    //free((mword*)(temp_rstack-1)); //FIXME: THERE'S NO CHECKING!!!
//    //    mword *temp_cons = new_cons();
//
//    return (mword*)car(temp_rstack);
//
//}
//

//
//
bvm_cache *sel(bvm_cache *this_bvm){

    mword *select = (mword*)TOS_2(this_bvm);

    // stack_ptr -> A -> B -> C -> D
    // B->D
    // FIXME: mem leak
    mword *temp = (mword*)cdr(this_bvm->stack_ptr);
    cdr(temp) = cdr(cdr(temp));

    if(is_false(select)){
        hard_zap(this_bvm);
    }
    else{
        hard_zap(
        swap(this_bvm));
    }

    return this_bvm;

}

//
//
bvm_cache *dup(bvm_cache *this_bvm){

    mword *result;

    if(is_leaf((mword*)TOS_0(this_bvm))){
        result = _newlf(size((mword*)TOS_0(this_bvm)));
    }
    else{
        result = _newin(size((mword*)TOS_0(this_bvm)));
    }    

    //TODO: memcpy!
    mword i;
    for(    i=0;
            i<size((mword*)TOS_0(this_bvm));
            i++
        ){

        c(result,i) = c((mword*)TOS_0(this_bvm),i);

    }    

    push_alloc(this_bvm, result, DUP);

    return this_bvm;

}

//
bvm_cache *swap(bvm_cache *this_bvm){

    // stack_ptr -> A -> B -> C
    // 
    // stack_ptr -> B
    // B -> A
    // A -> C

    // stack must have two items on it - depth() could be used here, 
    // but we don't need to take the length of the stack; this is faster:
    if( 
        is_nil(this_bvm->stack_ptr)
            ||
        is_nil(scdr(this_bvm->stack_ptr))
    ){
        return;
    }

    mword *A = this_bvm->stack_ptr;
    mword *B = (mword*)cdr(this_bvm->stack_ptr);
    mword *C = (mword*)scdr(B);

    this_bvm->stack_ptr = B;
    (mword*)cdr(B) = A;
    (mword*)cdr(A) = C;

    return this_bvm;

}

//void down(void){
//
//    push_alloc_rstack((mword*)TOS_0,DOWN);
//    zap();
//
//}
//
//void up(void){
//
//    if(alloc_type(RTOS_0) == DOWN){
//        push_alloc((mword*)car(pop_rstack()),UP);
//    }
//    else{ //NEST
//        stack_ptr = car(pop_rstack());
//    }
//
//}

//FIXME: This is wrong, just do an iterative pop()
bvm_cache *take(bvm_cache *this_bvm){

    mword count = car(TOS_0(this_bvm));
//    mword *temp;

    hard_zap(this_bvm);

    mword *result = rtake(this_bvm, count);

    push_alloc(this_bvm, result, TAKE);

}

//
mword *rtake(bvm_cache *this_bvm, mword count){

    if(count == 0)
        return nil;

    mword *A = new_cons;
    mword *B = (mword*)car(car(this_bvm->stack_ptr));

    hard_zap(this_bvm);

    cons(   A,
            B,
            rtake(this_bvm,count-1));

    return A;

}

//
bvm_cache *depth(bvm_cache *this_bvm){

    mword *result = new_atom;
    *result = _len((mword*)this_bvm->stack_ptr);

    push_alloc(this_bvm, result, DEPTH);
    
    return this_bvm;

}

bvm_cache *give(bvm_cache *this_bvm){

    mword *list = (mword*)TOS_0(this_bvm);

    hard_zap(this_bvm);

    give_tree(this_bvm,(mword*)list);

}

void give_tree(bvm_cache *this_bvm, mword *list){

    if(is_nil(list))
        return;

    give_tree(this_bvm,(mword*)cdr(list));

    push_alloc(this_bvm, (mword*)car(list), GIVE);

}

//
bvm_cache *clear(bvm_cache *this_bvm){

    while(!is_nil((mword*)TOS_0(this_bvm))){
        hard_zap(this_bvm);
    }

}

//void nest(void){
//
//    mword *new_stack = (mword*)TOS_0;
//
//    zap();
//    push_alloc_rstack((mword*)stack_ptr,NEST);
//
//    clear();
//
//    give_tree(new_stack);
//
//}
//

// Clayton Bauman 2011

