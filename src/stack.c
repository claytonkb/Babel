// stack.c
//

// Also, up/down should be able to perform non-alloc moves between 
// stack and ustack.

#include "babel.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"
#include "bvm_opcodes.h"
#include "array.h"
#include "alloc.h"
#include "bvm_stack.h"
#include "eval.h"
#include "hash.h"
#include "ref.h"
#include "tptr.h"

//
//
mword *new_dstack_entry(mword *operand, mword alloc_type){ // new_dstack_entry#

    return
        consa( operand,
            consa( _newva(alloc_type), nil ));

}


//
//
void push_udr_stack(mword *stack_ptr, mword *stack_entry){ // push_udr_stack#

    (mword*)c(stack_ptr,0) = _unshift((mword*)c(stack_ptr,0), stack_entry);

}


//
//
mword *pop_udr_stack(mword *stack_ptr){ // pop_udr_stack#

//    _dump(stack_ptr);
//    die;
//    if (is_nil(cdr(stack_ptr))) return nil;

    return _shift(stack_ptr);

}


//
//
mword *new_rstack_entry(mword *operand, mword *eval_type){ // new_rstack_entry#

    return
        consa( operand,
            consa( eval_type, nil ));

}


//
//
inline mword *get_from_udr_stack(bvm_cache *this_bvm, mword *stack_ptr, mword stack_index){ // get_from_udr_stack#

    return _chain_deref( this_bvm->sym_table, (mword*)icar( _ith( (mword*)icar( stack_ptr ), stack_index )) );

}


//XXX: Not tested
//
inline mword *set_in_udr_stack(bvm_cache *this_bvm, mword *stack_ptr, mword stack_index, mword *bs){ // set_in_udr_stack#

    mword *stack_entry = _chain_deref( this_bvm->sym_table, (mword*)icar( icar( _ith( stack_ptr, stack_index ))) );

    (mword*)c(stack_entry,0) = bs;

    return stack_entry;

}


//
//
inline mword *zap_from_udr_stack(mword *stack_ptr, mword stack_index){ // zap_from_udr_stack#

    mword *temp;
    mword *zapped;
    mword *tail;
    mword length;
    mword *work_stack = (mword*)car(stack_ptr);

    if(stack_index==0){
        work_stack = pop_udr_stack(work_stack);
        //temp = pop_udr_stack(work_stack);
        //(mword*)c(temp,0) = nil;
        //_del(temp);
    }
    else{
        length = _len(work_stack);
        if( length > stack_index+1 ){
            zapped = _list_cut(work_stack,   stack_index);
            tail   = _list_cut(zapped,      1);
            (mword*)c(icar(zapped),0) = nil;
            //_dump(zapped);
            //die;
            _del(zapped);
            _append_direct(work_stack,tail);
        }
        else if( length > stack_index ){
            zapped = _list_cut(work_stack, stack_index);
            (mword*)c(zapped,0) = nil;
            _del(zapped);
        }
        // else do nothing
    }

    (mword*)icar(stack_ptr) = work_stack;

    return stack_ptr;

}


//
//
void push_alloc(bvm_cache *this_bvm, mword *operand, mword alloc_type){

    mword *new_stack_cons  = new_cons;
    mword *new_stack_entry = _newin(STACK_ENTRY_SIZE);
    mword *type            = new_atom;

    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc
    STACK_ENTRY_VAL(new_stack_entry) = operand;
    STACK_ENTRY_LIF(new_stack_entry) = type;

    //cons(new_stack_cons, new_stack_entry, this_bvm->dstack_ptr);

    this_bvm->dstack_ptr = new_stack_cons;

}



//
void push_alloc_rstack(bvm_cache *this_bvm, mword *operand, mword alloc_type){

    mword *new_stack_cons  = new_cons;
    mword *new_stack_entry = _newin(STACK_ENTRY_SIZE);
    mword *type            = new_atom;

    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc
    STACK_ENTRY_VAL(new_stack_entry) = operand;
    STACK_ENTRY_LIF(new_stack_entry) = type;

    cons(new_stack_cons, new_stack_entry, this_bvm->rstack_ptr);

    this_bvm->rstack_ptr = new_stack_cons;

}

//
void push_alloc_ustack(bvm_cache *this_bvm, mword *operand, mword alloc_type){

    mword *new_stack_cons  = new_cons;
    mword *new_stack_entry = _newin(STACK_ENTRY_SIZE);
    mword *type            = new_atom;

    c(type,0) = alloc_type; // alloc_type = operand for auto-alloc
    STACK_ENTRY_VAL(new_stack_entry) = operand;
    STACK_ENTRY_LIF(new_stack_entry) = type;

    cons(new_stack_cons, new_stack_entry, this_bvm->ustack_ptr);

    this_bvm->ustack_ptr = new_stack_cons;

}

//
bvm_cache *hard_zap(bvm_cache *this_bvm){

    if(is_nil(this_bvm->dstack_ptr)){
        return this_bvm;
    }

    mword *temp_stack_ptr = (mword*)cdr(this_bvm->dstack_ptr);

//    bfree(STACK_ENTRY_TYP((mword*)car(this_bvm->dstack_ptr)));
//    bfree(car(this_bvm->dstack_ptr));
//    bfree(this_bvm->dstack_ptr);

    free_stack_entry(this_bvm);

    this_bvm->dstack_ptr = temp_stack_ptr;

    return this_bvm;

}

/* stack operator
**zap**
> Named after the Joy operator  
> Removes the TOS  
*/
bvm_cache *zap(bvm_cache *this_bvm){

    if(is_nil(this_bvm->dstack_ptr)){
        return this_bvm;
    }

    mword *temp_stack_ptr = (mword*)cdr(this_bvm->dstack_ptr);

    mword *val = STACK_ENTRY_VAL((mword*)car(this_bvm->dstack_ptr));
    mword lifetime = car(STACK_ENTRY_LIF((mword*)car(this_bvm->dstack_ptr)));

//    zap_switch(car(STACK_ENTRY_TYP((mword*)car(this_bvm->dstack_ptr))))

//    if( lifetime == MORTAL ){
//        _del(STACK_ENTRY_VAL((mword*)car(this_bvm->dstack_ptr)));
//    }

//    bfree(STACK_ENTRY_TYP((mword*)car(this_bvm->dstack_ptr)));
//    bfree(car(this_bvm->dstack_ptr));
//    bfree(this_bvm->dstack_ptr);

    free_stack_entry(this_bvm);

    this_bvm->dstack_ptr = temp_stack_ptr;    

    return this_bvm;

}

void free_stack_entry(bvm_cache *this_bvm){

    bfree(STACK_ENTRY_LIF((mword*)car(this_bvm->dstack_ptr)));
    bfree(car(this_bvm->dstack_ptr));
    bfree(this_bvm->dstack_ptr);

}

//
mword *pop_rstack(bvm_cache *this_bvm){

    //If rstack is empty: except()

    mword *temp_rstack = this_bvm->rstack_ptr;
//    code_ptr = car(rstack_ptr);

    this_bvm->rstack_ptr = (mword*)scdr(this_bvm->rstack_ptr);
    //FIXME: Leaky!

    return (mword*)car(temp_rstack);

}

//
mword *pop_ustack(bvm_cache *this_bvm){

    //If rstack is empty: except()

    mword *temp_ustack = this_bvm->ustack_ptr;
//    code_ptr = car(rstack_ptr);

    this_bvm->ustack_ptr = (mword*)scdr(this_bvm->ustack_ptr);
    //FIXME: Leaky!

    return (mword*)car(temp_ustack);

}

/* stack operator
**sel** (?)  
> Selects one of the top two values on the stack:  
> `{f} {X} {Y}| -> {X}|`  
> `{t} {X} {Y}| -> {Y}|`  
> `{f} [X] {Y}| -> [X]|`  
> `{t} [X] {Y}| -> {Y}|`  
> etc.  
>  
> Where f = 0 and t != 0  
*/
bvm_cache *sel(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *select = TOS_2(this_bvm);

    // FIXME YUCK!!!!!!
    down(this_bvm);
    down(this_bvm);
    hard_zap(this_bvm); // XXX leaky
    up(this_bvm);
    up(this_bvm);

    // stack_ptr -> A -> B -> C -> D
    // B->D
//    mword *temp = (mword*)cdr(this_bvm->dstack_ptr);
//    cdr(temp) = cdr(cdr(temp));

    if(!is_false(select)){
        swap(this_bvm);
    }

    zap(this_bvm);

    return this_bvm;

}

/* stack operator
**dup** (*2)  
> Named after the Joy operator  
> Duplicates the TOS. Shallow-copy.  
> `{X}| -> {X} {X}|`  
> `[X]| -> [X] [X]|`  
*/
bvm_cache *dup(bvm_cache *this_bvm){

//    if(is_leaf((mword*)TOS_0(this_bvm))){
//        result = _newlf(size((mword*)TOS_0(this_bvm)));
//    }
//    else if(is_inte((mword*)TOS_0(this_bvm))){
//        result = _newin(size((mword*)TOS_0(this_bvm)));
//    }
//    else{
////        result = _newin(size((mword*)TOS_0(this_bvm)));
//        result = _newref((mword*)TOS_0(this_bvm));
//    }    
//
//    //TODO: memcpy!
//    mword i;
//    for(    i=0;
//            i<size((mword*)TOS_0(this_bvm));
//            i++
//        ){
//
//        c(result,i) = c((mword*)TOS_0(this_bvm),i);
//
//    }

    fatal("stack fix not done");
    if(is_nil(this_bvm->dstack_ptr))
        return this_bvm;

    mword *result = TOS_0(this_bvm); //XXX: Note we do NOT use get_from_stack()

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

/* stack operator
**swap** (<->)  
> Named after the Joy operator  
> Swaps the top two items on the stack.  
> `{X} {Y}| -> {Y} {X}|`  
> `[X] {Y}| -> {Y} [X]|`  
> etc.  
*/
bvm_cache *swap(bvm_cache *this_bvm){

    // stack_ptr -> A -> B -> C
    // 
    // stack_ptr -> B
    // B -> A
    // A -> C

    fatal("stack fix not done");
    // stack must have two items on it - depth() could be used here, 
    // but we don't need to take the length of the stack; this is faster:
    if( 
        is_nil(this_bvm->dstack_ptr)
            ||
        is_nil(scdr(this_bvm->dstack_ptr))
    ){
        return;
    }

    mword *A = this_bvm->dstack_ptr;
    mword *B = (mword*)cdr(this_bvm->dstack_ptr);
    mword *C = (mword*)scdr(B);

    this_bvm->dstack_ptr = B;
    (mword*)icdr(B) = A;
    (mword*)icdr(A) = C;

    return this_bvm;

}

/* stack operator
**down** (<-)  
> Analogous to doing a cdr on the stack itself. If the code\_ptr
> becomes nil after executing one or more downs without a balancing
> up, as many ups as are required to undo the downs will be executed
> automatically.  
*/
bvm_cache *down(bvm_cache *this_bvm){

    fatal("stack fix not done");
    if(is_nil(this_bvm->dstack_ptr))
        return this_bvm;

    push_alloc_ustack(this_bvm, TOS_0(this_bvm), DOWN);
    hard_zap(this_bvm);

    return this_bvm;

}

/* stack operator
**up** (->)  
> Undoes down/nest.  
*/
bvm_cache *up(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *temp;

    if(is_nil(this_bvm->ustack_ptr))
        return this_bvm;

    if(return_type(this_bvm->ustack_ptr) == DOWN){
        push_alloc(this_bvm,(mword*)car(pop_ustack(this_bvm)),IMMORTAL); //FIXME: Revisit
    }
//    else if(return_type(this_bvm->rstack_ptr) == NEST){
////        this_bvm->dstack_ptr = (mword*)car(pop_rstack(this_bvm));
//
//        // XXX fairly yucky code:
//        temp = new_atom;
//        *temp = (mword)-1;
//
//        push_alloc(this_bvm,temp,IMMORTAL); //FIXME: Revisit
//
//        take(this_bvm);
//
//        temp = (mword*)TOS_0(this_bvm);
//
//        this_bvm->dstack_ptr = (mword*)car(pop_rstack(this_bvm));
//        push_alloc(this_bvm,temp,IMMORTAL); //FIXME: Revisit
//
//    }
    else{
        error("up: There was an error\n");
        die;
    }

    return this_bvm;
    
}

/* stack operator
**take**  
> Takes TOS items from the stack and puts them into a list. Does not
> zap the items. If TOS == -1, the entire stack is taken.  
*/
bvm_cache *take(bvm_cache *this_bvm){

    fatal("stack fix not done");
    int count = (int)car(TOS_0(this_bvm));
    zap(this_bvm);

    mword *result = nil;
    mword *list_entry;
    mword *temp;
    int i;

    if(count == -1){
        while(!is_nil(this_bvm->dstack_ptr)){
            list_entry = TOS_0(this_bvm);
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
            list_entry = TOS_0(this_bvm);
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

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

////
//mword *rtake(bvm_cache *this_bvm, mword count){
//
//    if(count == 0)
//        return nil;
//
//    mword *A = new_cons;
//    mword *B = (mword*)car(car(this_bvm->dstack_ptr));
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

/* stack operator
**depth**  
> Places the depth of the stack on TOS. To gather up the entire stack
> into a list:  
>  
> depth take  
*/
bvm_cache *depth(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = new_atom;
    *result = _len((mword*)this_bvm->dstack_ptr);

    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

/* stack operator
**give**  
> Undoes take  
*/
bvm_cache *give(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *list = TOS_0(this_bvm);

    zap(this_bvm);

    rgive(this_bvm,(mword*)list);

    return this_bvm;

}

//
void rgive(bvm_cache *this_bvm, mword *list){

    if(is_nil(list))
        return;

    push_alloc(this_bvm, (mword*)car(list), IMMORTAL); //FIXME: Depends

    rgive(this_bvm,(mword*)cdr(list));

}

/* stack operator
**clear**  
> Empties the stack  
*/
bvm_cache *clear(bvm_cache *this_bvm){

//    while(!is_nil((mword*)TOS_0(this_bvm))){
//        hard_zap(this_bvm);
//    }

    fatal("stack fix not done");
    this_bvm->dstack_ptr  = nil;
    this_bvm->ustack_ptr = nil;

    return this_bvm;

}

////
//bvm_cache *nest(bvm_cache *this_bvm){
//
//    mword *new_stack = (mword*)TOS_0(this_bvm);
//
//    hard_zap(this_bvm);
//    push_alloc_rstack(this_bvm, (mword*)this_bvm->dstack_ptr, NEST);
//
////    clear(this_bvm);
//
//    this_bvm->dstack_ptr = nil; // clear the stack
//
////    rgive(this_bvm, new_stack);
//
//    push_alloc(this_bvm, new_stack, IMMORTAL);
//
//    return this_bvm;
//
//}

////
//bvm_cache *unnest(bvm_cache *this_bvm){
//
//    mword *temp;
//
////    if(return_type(this_bvm->rstack_ptr) == NEST){
////        push_alloc(this_bvm,(mword*)car(pop_ustack(this_bvm)),IMMORTAL); //FIXME: Revisit
////    }
//    if(return_type(this_bvm->rstack_ptr) == NEST){
////        this_bvm->dstack_ptr = (mword*)car(pop_rstack(this_bvm));
//
//        // XXX fairly yucky code:
////        temp = new_atom;
////        *temp = (mword)-1;
////
////        push_alloc(this_bvm,temp,IMMORTAL); //FIXME: Revisit
////
////        take(this_bvm);
//
//        temp = (mword*)TOS_0(this_bvm);
//
//        this_bvm->dstack_ptr = (mword*)car(pop_rstack(this_bvm));
//        push_alloc(this_bvm,temp,IMMORTAL); //FIXME: Revisit
//
//    }
//    else{
//        error("unnest: There was an error\n");
//        die;
//    }
//
//    return this_bvm;
//    
//}

/* stack operator
**nest**  
> Analogous to doing a car on the stack. If the code\_ptr becomes
> nil after a nest and with no intervening up, an up will be
> executed automatically.  
*/

/* stack operator
**flip**  
*/
bvm_cache *flip(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *temp          = this_bvm->dstack_ptr;
    this_bvm->dstack_ptr  = this_bvm->ustack_ptr;
    this_bvm->ustack_ptr = temp;

    return this_bvm;

}


//
//
mword *get_from_stack(bvm_cache *this_bvm, mword *stack_entry){

    //FIXME: href
//    while(is_href(stack_entry)){
//        stack_entry = _luha(this_bvm->sym_table, stack_entry);
//    }

    return (mword*)stack_entry;

}





// Clayton Bauman 2011

