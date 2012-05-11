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

//    alloc_type
//    // return     -> -3
//    // cond_block -> -2
//    // body       -> -1
//    // next       -> 

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
void push_alloc_rstack(bvm_cache *this_bvm, mword *operand, mword alloc_type){

    mword *new_stack_cons  = new_cons;
    mword *new_stack_entry = _newin(STACK_ENTRY_SIZE);
    mword *type            = new_atom;

    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc
    STACK_ENTRY_VAL(new_stack_entry) = operand;
    STACK_ENTRY_TYP(new_stack_entry) = type;

    cons(new_stack_cons, new_stack_entry, this_bvm->rstack_ptr);

    this_bvm->rstack_ptr = new_stack_cons;

}

//
bvm_cache *hard_zap(bvm_cache *this_bvm){

    if(is_nil(this_bvm->stack_ptr)){
        return this_bvm;
    }

    mword *temp_stack_ptr = (mword*)cdr(this_bvm->stack_ptr);

//    bfree(STACK_ENTRY_TYP((mword*)car(this_bvm->stack_ptr)));
//    bfree(car(this_bvm->stack_ptr));
//    bfree(this_bvm->stack_ptr);

    free_stack_entry(this_bvm);

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
//    if(car(STACK_ENTRY_TYP((mword*)car(this_bvm->stack_ptr))) == SCOPE_STACK){
//        
//    }

//    bfree(STACK_ENTRY_TYP((mword*)car(this_bvm->stack_ptr)));
//    bfree(car(this_bvm->stack_ptr));
//    bfree(this_bvm->stack_ptr);

    free_stack_entry(this_bvm);

    this_bvm->stack_ptr = temp_stack_ptr;

    return this_bvm;

}

void free_stack_entry(bvm_cache *this_bvm){

    bfree(STACK_ENTRY_TYP((mword*)car(this_bvm->stack_ptr)));
    bfree(car(this_bvm->stack_ptr));
    bfree(this_bvm->stack_ptr);

}

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

mword *pop_rstack(bvm_cache *this_bvm){

    //If rstack is empty: except()

    mword *temp_rstack = this_bvm->rstack_ptr;
//    code_ptr = car(rstack_ptr);

    this_bvm->rstack_ptr = (mword*)scdr(this_bvm->rstack_ptr);
    //FIXME: Leaky!

    return (mword*)car(temp_rstack);

}

//
bvm_cache *sel(bvm_cache *this_bvm){

    mword *select = (mword*)TOS_2(this_bvm);

    // FIXME YUCK!!!!!!
    down(this_bvm);
    down(this_bvm);
    hard_zap(this_bvm); // XXX leaky
    up(this_bvm);
    up(this_bvm);

    // stack_ptr -> A -> B -> C -> D
    // B->D
//    mword *temp = (mword*)cdr(this_bvm->stack_ptr);
//    cdr(temp) = cdr(cdr(temp));

    if(!is_false(select)){
        swap(this_bvm);
    }

    zap(this_bvm);

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

//
bvm_cache *down(bvm_cache *this_bvm){

    push_alloc_rstack(this_bvm, (mword*)TOS_0(this_bvm), DOWN);
    hard_zap(this_bvm);

    return this_bvm;

}

//
bvm_cache *up(bvm_cache *this_bvm){

    mword *temp;

    if(return_type(this_bvm->rstack_ptr) == DOWN){
        push_alloc(this_bvm,(mword*)car(pop_rstack(this_bvm)),UP);
    }
    else if(return_type(this_bvm->rstack_ptr) == NEST){
//        this_bvm->stack_ptr = (mword*)car(pop_rstack(this_bvm));

        // XXX fairly yucky code:
        temp = new_atom;
        *temp = (mword)-1;

        push_alloc(this_bvm,temp,UP);

        take(this_bvm);

        temp = (mword*)TOS_0(this_bvm);

        this_bvm->stack_ptr = (mword*)car(pop_rstack(this_bvm));
        push_alloc(this_bvm,temp,UP);

    }
    else{
        error("up: There was an error\n");
        die;
    }

    return this_bvm;
    
}

//
bvm_cache *take(bvm_cache *this_bvm){

    int count = (int)car(TOS_0(this_bvm));
    zap(this_bvm);

    mword *result = nil;
    mword *list_entry;
    mword *temp;
    int i;

    if(count == -1){
        while(!is_nil(this_bvm->stack_ptr)){
            list_entry = (mword*)TOS_0(this_bvm);
            temp = new_cons;
            cons(   temp,
                    list_entry,
                    result);
            result = temp;
            hard_zap(this_bvm); // XXX leaky
        }
    }
    else{
        for(i=0;i<count;i++){
            list_entry = (mword*)TOS_0(this_bvm);
            temp = new_cons;
            cons(   temp,
                    list_entry,
                    result);
            result = temp;
            hard_zap(this_bvm); // XXX leaky
        }
    }
    
//    _dump(result);
//    die;

    push_alloc(this_bvm, result, TAKE);

    return this_bvm;

}

////
//mword *rtake(bvm_cache *this_bvm, mword count){
//
//    if(count == 0)
//        return nil;
//
//    mword *A = new_cons;
//    mword *B = (mword*)car(car(this_bvm->stack_ptr));
//
//    hard_zap(this_bvm);
//
//    cons(   A,
//            B,
//            rtake(this_bvm,count-1));
//
//    return A;
//
//}

//
bvm_cache *depth(bvm_cache *this_bvm){

    mword *result = new_atom;
    *result = _len((mword*)this_bvm->stack_ptr);

    push_alloc(this_bvm, result, DEPTH);

    return this_bvm;

}

//
bvm_cache *give(bvm_cache *this_bvm){

    mword *list = (mword*)TOS_0(this_bvm);

    zap(this_bvm);

    rgive(this_bvm,(mword*)list);

    return this_bvm;

}

//
void rgive(bvm_cache *this_bvm, mword *list){

    if(is_nil(list))
        return;

    push_alloc(this_bvm, (mword*)car(list), GIVE);

    rgive(this_bvm,(mword*)cdr(list));

}

// FIXME Why not just set stack_ptr to nil?
bvm_cache *clear(bvm_cache *this_bvm){

    while(!is_nil((mword*)TOS_0(this_bvm))){
        hard_zap(this_bvm);
    }

    return this_bvm;

}

//
bvm_cache *nest(bvm_cache *this_bvm){

    mword *new_stack = (mword*)TOS_0(this_bvm);

    hard_zap(this_bvm);
    push_alloc_rstack(this_bvm, (mword*)this_bvm->stack_ptr, NEST);

//    clear(this_bvm);

    this_bvm->stack_ptr = nil; // clear the stack

    rgive(this_bvm, new_stack);

}


// Clayton Bauman 2011

