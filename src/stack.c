// stack.c
//

// XXX up/down should be able to perform non-alloc moves between 
// dstack and ustack.

#include "babel.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"
//#include "bvm_opcodes.h"
#include "array.h"
#include "alloc.h"
//#include "bvm_stack.h"
#include "eval.h"
#include "hash.h"
#include "ref.h"
#include "tptr.h"
#include "pearson16.h"
#include "string.h"
#include "mem.h"




//
//
void free_lumbar(bvm_cache *this_bvm, mword *stack_entry){ // free_lumbar#

    mc_free(this_bvm, (mword*)icdr(stack_entry));
    mc_free(this_bvm, stack_entry);

}


//
//
mword *new_dstack_entry(bvm_cache *this_bvm, mword *operand, mword alloc_type){ // new_dstack_entry#

    //return

    mword *temp = consa(this_bvm,  operand,
            consa(this_bvm,  _newva(this_bvm,  alloc_type), nil )); //FIXME DEPRECATED _newva

    return temp;

}


//
//
mword *new_dstack_entry2(bvm_cache *this_bvm, mword *operand, mword *alloc_type){ // new_dstack_entry2#

    return
        consa(this_bvm, operand,
            consa(this_bvm, alloc_type, nil ));

}


//
//
void push_udr_stack(bvm_cache *this_bvm, mword *stack_ptr, mword *stack_entry){ // push_udr_stack#

    (mword*)c(stack_ptr,0) = _push(this_bvm, (mword*)c(stack_ptr,0), stack_entry);

}


//
//
mword *pop_udr_stack(bvm_cache *this_bvm, mword *stack_ptr){ // pop_udr_stack#

    //mword *temp = (mword*)icar(icar(stack_ptr));
    mword *temp = (mword*)icar(stack_ptr);

    (mword*)*stack_ptr = _pop(this_bvm, (mword*)icar(stack_ptr));

    free_lumbar(this_bvm, temp);

    return (mword*)icar(temp);

    //return temp;

}


//
//
mword *new_rstack_entry(bvm_cache *this_bvm, mword *operand, mword *eval_type){ // new_rstack_entry#

    return
        consa(this_bvm,  operand,
            consa(this_bvm,  eval_type, nil ));

}


//
//
inline mword *get_from_udr_stack(bvm_cache *this_bvm, mword *stack_ptr, mword stack_index){ // get_from_udr_stack#

//    return _chain_deref( this_bvm->sym_table, (mword*)icar( _ith(this_bvm,  (mword*)icar( stack_ptr ), stack_index )) );
//    return (mword*)icar( _ith(this_bvm,  (mword*)icar( stack_ptr ), stack_index ));
    return _chain_deref( this_bvm, (mword*)icar( _ith(this_bvm,  (mword*)icar( stack_ptr ), stack_index )) );

}


//
//
mword *get_tag_from_udr_stack(bvm_cache *this_bvm, mword *stack_ptr, mword stack_index){ // get_tag_from_udr_stack#

//    return _chain_deref( this_bvm->sym_table, (mword*)icdr( _ith(this_bvm,  (mword*)icar( stack_ptr ), stack_index )) );
    return (mword*)icdr( _ith(this_bvm,  (mword*)icar( stack_ptr ), stack_index ));

}


//
//
inline mword *set_in_udr_stack(bvm_cache *this_bvm, mword *stack_ptr, mword stack_index, mword *bs){ // set_in_udr_stack#

//    mword *stack_entry = _chain_deref( this_bvm->sym_table, (mword*)icar( icar( _ith(this_bvm,  stack_ptr, stack_index ))) );
    mword *stack_entry = (mword*)icar( icar( _ith(this_bvm,  stack_ptr, stack_index )));

    (mword*)c(stack_entry,0) = bs;

    return stack_entry;

}


// "Selective zap", i.e. "zap the nth item on stack"
//
mword *remove_from_udr_stack(bvm_cache *this_bvm, mword *stack_ptr, mword stack_index){ // remove_from_udr_stack#

    mword *temp;
    mword *zapped;
    mword *tail;
    mword length;
    mword *work_stack = (mword*)car(stack_ptr);

    if(stack_index==0){
        pop_udr_stack(this_bvm, stack_ptr);
        //work_stack = pop_udr_stack(this_bvm, stack_ptr);

        //temp = pop_udr_stack(this_bvm, work_stack);
        //(mword*)c(temp,0) = nil;
        //_del(temp);
    }
    else{
        length = _len(this_bvm, work_stack);
        if( length > stack_index+1 ){
            zapped = _list_cut(this_bvm, work_stack,   stack_index);
            tail   = _list_cut(this_bvm, zapped,      1);
            (mword*)c(icar(zapped),0) = nil;
            //_dump(zapped);
            //die;
            //_del(zapped);
            _append_direct(this_bvm, work_stack,tail);
        }
        else if( length > stack_index ){
            zapped = _list_cut(this_bvm, work_stack, stack_index);
            (mword*)c(zapped,0) = nil;
            //_del(zapped);
        }
        // else do nothing
        (mword*)*stack_ptr = work_stack;
    }


//    _dump(stack_ptr);
//    die;

    return stack_ptr;

}


// same as pop_udr_stack except calls mc_free if PACMAN tag
//
void zap_udr_stack(bvm_cache *this_bvm, mword *stack_ptr){ // zap_udr_stack#

    mword *free_ptr = (mword*)icar(stack_ptr);

    mword *tag = (mword*)icar(icdr(icar(icar(stack_ptr))));
    mword *temp = (mword*)icar(icar(icar(stack_ptr)));

    (mword*)*stack_ptr = _pop(this_bvm, (mword*)icar(stack_ptr));

    //_dump(stack_ptr);

//    trace;
//    _mem(temp);
//    printf("\n");

    if(is_tptr(tag) && tageq(tag,BABEL_TAG_PACMAN,TAG_SIZE)){
        //printf("MATCH\n");
        mc_free(this_bvm, temp);
    }

    free_lumbar(this_bvm, free_ptr);

}


//
inline mword *zap_from_udr_stack(bvm_cache *this_bvm, mword *stack_ptr, mword stack_index){ // zap_from_udr_stack#

    mword *temp;
    mword *zapped;
    mword *tail;
    mword length;
    mword *work_stack = (mword*)car(stack_ptr);

    if(stack_index==0){
        pop_udr_stack(this_bvm, stack_ptr);
        //work_stack = pop_udr_stack(this_bvm, stack_ptr);

        //temp = pop_udr_stack(this_bvm, work_stack);
        //(mword*)c(temp,0) = nil;
        //_del(temp);
    }
    else{
        length = _len(this_bvm, work_stack);
        if( length > stack_index+1 ){
            zapped = _list_cut(this_bvm, work_stack,   stack_index);
            tail   = _list_cut(this_bvm, zapped,      1);
            (mword*)c(icar(zapped),0) = nil;
            //_dump(zapped);
            //die;
            //_del(zapped);
            _append_direct(this_bvm, work_stack,tail);
        }
        else if( length > stack_index ){
            zapped = _list_cut(this_bvm, work_stack, stack_index);
            (mword*)c(zapped,0) = nil;
            //_del(zapped);
        }
        // else do nothing
    }

    (mword*)icar(stack_ptr) = work_stack;

//    _dump(stack_ptr);
//    die;

    return stack_ptr;

}


/* stack operator
**zap**
> Named after the Joy operator  
> Removes the TOS  
*/
bvm_cache *zap(bvm_cache *this_bvm){ // zap#

    popd(this_bvm);

    return this_bvm;

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
bvm_cache *sel(bvm_cache *this_bvm){ // sel#

    mword *temp = pop_udr_stack(this_bvm, this_bvm->dstack_ptr);

    if(!is_false(icar(temp))){
        popd(this_bvm);
    }
    else{
        remove_from_udr_stack(this_bvm, this_bvm->dstack_ptr, 1);
    }

    return this_bvm;

}


/* stack operator
**dup** (*2)  
> Named after the Joy operator  
> Duplicates the TOS. Shallow-copy.  
> `{X}| -> {X} {X}|`  
> `[X]| -> [X] [X]|`  
*/
bvm_cache *dup(bvm_cache *this_bvm){ // dup#

    if(dstack_empty(this_bvm))
        return this_bvm;

#define babel_dup_operator \
    result = dstack_get(this_bvm,0);

    babel_operator_typeD( 
            this_bvm, 
            babel_dup_operator );

}


/* stack operator
**swap** (<->)  
> Named after the Joy operator  
> Swaps the top two items on the stack.  
> `{X} {Y}| -> {Y} {X}|`  
> `[X] {Y}| -> {Y} [X]|`  
> etc.  
*/
bvm_cache *swap(bvm_cache *this_bvm){ // swap#

    // stack_ptr -> A -> B -> C
    // 
    // stack_ptr -> B
    // B -> A
    // A -> C

    //FIXME: Depth-check the stack

    mword *A = (mword*)icar(this_bvm->dstack_ptr);
    mword *B = (mword*)icdr(A);
    mword *C = (mword*)icdr(B);

    (mword*)c(this_bvm->dstack_ptr,0) = B;
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
bvm_cache *down(bvm_cache *this_bvm){ // down#

    if(is_nil(this_bvm->dstack_ptr))
        return this_bvm;

    pushu(this_bvm, dstack_get(this_bvm,0), IMMORTAL);
    popd(this_bvm);

    return this_bvm;

}


/* stack operator
**up** (->)  
> Undoes down  
*/
bvm_cache *up(bvm_cache *this_bvm){ // up#

    if(is_nil(this_bvm->ustack_ptr)) // FIXME: Wrong
        return this_bvm;

    pushd(this_bvm, ustack_get(this_bvm,0), IMMORTAL);
    popu(this_bvm);

    return this_bvm;

}


/* stack operator
**take**  
> Takes TOS items from the stack and puts them into a list. 
> If TOS == -1, the entire stack is taken.  
*/
bvm_cache *take(bvm_cache *this_bvm){ // take#

    int count = (int)icar(dstack_get(this_bvm,0));
    popd(this_bvm);

    mword *result = nil;
    mword *list_entry;
    mword *temp;
    int i;

    if(count == -1){
//        while(!is_nil(this_bvm->dstack_ptr)){
        while(!dstack_empty(this_bvm)){
            list_entry = dstack_get(this_bvm,0);
            result = consa(this_bvm, list_entry, result);
            popd(this_bvm);
        }
    }
    else{
        for(i=0;i<count;i++){
            list_entry = dstack_get(this_bvm,0);
            result = consa(this_bvm, list_entry, result);
            popd(this_bvm);
        }
    }
    
    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* stack operator
**depth**  
> Places the depth of the stack on TOS. To gather up the entire stack
> into a list:  
>  
> depth take  
*/
bvm_cache *depth(bvm_cache *this_bvm){ // depth#

    mword *result = _newva( this_bvm, 1);

    *result = _len(this_bvm, (mword*)icar(this_bvm->dstack_ptr));

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* stack operator
**give**  
> Undoes take  
*/
bvm_cache *give(bvm_cache *this_bvm){ // give#

    mword *list = dstack_get(this_bvm,0);
    popd(this_bvm);

    rgive(this_bvm,(mword*)list);

    return this_bvm;

}


//
//
void rgive(bvm_cache *this_bvm, mword *list){ // rgive#

    if(is_nil(list))
        return;

    pushd(this_bvm, (mword*)car(list), IMMORTAL);

    rgive(this_bvm,(mword*)cdr(list));

}


/* stack operator
**clear**  
> Empties the stack  
*/
bvm_cache *clear(bvm_cache *this_bvm){ // clear#

    (mword*)icar(this_bvm->dstack_ptr) = nil;
    (mword*)icar(this_bvm->ustack_ptr) = nil;

    return this_bvm;

}


/* stack operator
**flip**  
*/
bvm_cache *flip(bvm_cache *this_bvm){ // flip#

    mword *temp          = this_bvm->dstack_ptr;
    this_bvm->dstack_ptr  = this_bvm->ustack_ptr;
    this_bvm->ustack_ptr = temp;

    return this_bvm;

}


//FIXME: Busted, need to handle empty stack
/* stack operator
**nest**  
> Creates a fresh stack with current TOS on it. Saves current dstack onto
> rstack.
*/
bvm_cache *nest(bvm_cache *this_bvm){ // nest#

    mword *nest_body = dstack_get(this_bvm,0);
//    mword *new_stack = dstack_get(this_bvm,1);
    mword *save_TOS = dstack_get(this_bvm,1);

    popd(this_bvm);
    popd(this_bvm);

    mword *save_dstack = (mword*)icar(this_bvm->dstack_ptr);
    mword *save_ustack = (mword*)icar(this_bvm->ustack_ptr);

    clear(this_bvm); // clear the stack

    //rgive(this_bvm, new_stack);
    pushd(this_bvm, save_TOS, IMMORTAL);

    mword *nest_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *nest_rstack_entry = consa(this_bvm, save_dstack,
                                    consa(this_bvm, save_ustack,
                                        consa(this_bvm, nest_return, nil)));

    pushr(this_bvm, nest_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/nest")));

    this_bvm->code_ptr = consa(this_bvm, nest_body,nil);

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;    

}


// XXX DEPRECATE
//
mword *get_from_stack(bvm_cache *this_bvm, mword *stack_entry){ // *get_from_stack#

    fatal("Why are you here??")

    return (mword*)stack_entry;

}


// XXX DEPRECATE
//
void push_alloc(bvm_cache *this_bvm, mword *operand, mword alloc_type){ // push_alloc#

    fatal("Why are you here??");

}



// XXX DEPRECATE
//
void push_alloc_rstack(bvm_cache *this_bvm, mword *operand, mword alloc_type){ // push_alloc_rstack#

    fatal("Why are you here??");

}

// XXX DEPRECATE
//
void push_alloc_ustack(bvm_cache *this_bvm, mword *operand, mword alloc_type){ // push_alloc_ustack#

    fatal("Why are you here??");

}

// XXX DEPRECATE
//
bvm_cache *hard_zap(bvm_cache *this_bvm){ // *hard_zap#

    fatal("Why are you here??");

    return this_bvm;

}


// XXX DEPRECATE
void free_stack_entry(bvm_cache *this_bvm){ // free_stack_entry#

    fatal("Why are you here??");
    
}


// XXX DEPRECATE
mword *pop_rstack(bvm_cache *this_bvm){ // *pop_rstack#

    fatal("Why are you here??");

    mword *temp_rstack;// = this_bvm->rstack_ptr;

    return (mword*)car(temp_rstack);

}

// XXX DEPRECATE
mword *pop_ustack(bvm_cache *this_bvm){ // *pop_ustack#

    fatal("Why are you here??");

    mword *temp_ustack;// = this_bvm->ustack_ptr;

    return (mword*)car(temp_ustack);

}



// Clayton Bauman 2011

