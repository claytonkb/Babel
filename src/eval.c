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
#include "tptr.h"
#include "pearson16.h"
#include "string.h"

/* flow-control operator
**eval** (!)  
> Named after the Perl function  
> Evaluates TOS  
> `[X]| -> |`  
*/
bvm_cache *eval(bvm_cache *this_bvm){ // eval#

    mword *op0 = dstack_get(this_bvm,0);               

    popd(this_bvm); 

    _eval(this_bvm, op0, (mword*)icdr(icar(this_bvm->code_ptr)));

    return this_bvm;

}


// Impersonates eval operator...
//
void _eval(bvm_cache *this_bvm, mword *eval_body, mword *eval_return){ // _eval#

    pushr(this_bvm, eval_return, _hash8(C2B("/babel/tag/eval")));

    this_bvm->code_ptr = consa(eval_body,nil);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

}


/* flow-control operator
**loop**
> Loops indefinitely. Use `last` to break out.  
*/
bvm_cache *loop(bvm_cache *this_bvm){ // loop#

    mword *loop_body = dstack_get(this_bvm,0);
    mword *iteration = _newva(0);

    popd(this_bvm); 

    // ADD this for `last` operator
    mword *loop_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *loop_rstack_entry = consa(iteration,
                                    consa(loop_body,
                                        consa(loop_return, nil)));

    pushr(this_bvm, loop_rstack_entry, _hash8(C2B("/babel/tag/loop")));

    this_bvm->code_ptr = consa(loop_body,nil);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;

}


/* flow-control operator
**goto**  
> Classic goto  
> `[X]| -> |`  
*/
bvm_cache *gotoop(bvm_cache *this_bvm){ // gotoop# goto#

    mword *goto_point = dstack_get(this_bvm,0);               
    popd(this_bvm); 

    this_bvm->code_ptr = consa(goto_point,nil);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;

}


/* flow-control operator
**times**  
> Like eval but executes n iterations. Use 'last' or 'next' to modify
> the loop's behavior.  
> 
> `[X] n| -> |`  
> 
> Example:  
> 
> `(1) 4 times 4 take   --> Leaves (1 1 1 1) on TOS`  
*/
bvm_cache *times(bvm_cache *this_bvm){ // times#

    mword *iterations  = _newva(icar(dstack_get(this_bvm,0)));
    mword *times_body  = dstack_get(this_bvm,1);

    popd(this_bvm); 
    popd(this_bvm);

    if(c(iterations,0) == 0)
        return this_bvm;

    mword *times_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *times_rstack_entry = consa(iterations,
                                    consa(times_body,
                                        consa(times_return, nil)));

    pushr(this_bvm, times_rstack_entry, _hash8(C2B("/babel/tag/times")));

    this_bvm->code_ptr = consa(times_body,nil);

    //_dump(this_bvm->code_ptr);

    icar(this_bvm->advance_type) = BVM_CONTINUE;    

    return this_bvm;

}


/* eval operator
**die**
> Exits a BVM
*/
bvm_cache *dieop(bvm_cache *this_bvm){ // dieop# die#

    fprintf(stderr, "Died.\n");
    exit(0);
}


/* flow-control operator
**each** (...)
> Iterates across a list  
> `0 (code cuadd) (list 1 2 3 4) each --> Leaves 10 on TOS`  
*/
bvm_cache *each(bvm_cache *this_bvm){

    mword *each_list  = dstack_get(this_bvm,0);
    mword *each_body  = dstack_get(this_bvm,1);

    popd(this_bvm); 
    popd(this_bvm);

    if(is_nil(each_list))
        return this_bvm;

    mword *iteration = _newva(0);

    mword *each_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *each_rstack_entry = consa(iteration,
                                    consa(each_body,
                                        consa(each_return, 
                                            consa(each_list, nil))));

    pushr(this_bvm, each_rstack_entry, _hash8(C2B("/babel/tag/each")));

    pushd(this_bvm, (mword*)icar(each_list), IMMORTAL);

    this_bvm->code_ptr = consa(each_body,nil);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;

}


/* flow-control operator
**ifte**  
> If-Then-Else. Like the Joy operator.  
>  
> `{x}[Y][Z]| -> eval(Y)| --> if x == 0`  
> `{x}[Y][Z]| -> eval(Z)| --> if x == 1`  
*/
bvm_cache *ifte(bvm_cache *this_bvm){

    mword *else_clause = dstack_get(this_bvm, 0);
    mword *then_clause = dstack_get(this_bvm, 1);
    mword *cond_clause = dstack_get(this_bvm, 2);


    mword *ifte_return = (mword*)icdr(icar(this_bvm->code_ptr));

    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);

    mword *ifte_select = _newva(IFTE_COND);

    mword *ifte_rstack_entry = consa(ifte_select,
                                consa(then_clause,
                                    consa(ifte_return,
                                        consa(else_clause, nil ))));

    pushr(this_bvm, ifte_rstack_entry, _hash8(C2B("/babel/tag/ifte")));

    this_bvm->code_ptr = consa(cond_clause,nil);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;    

}


/* flow-control operator
**while**  
> Like the C keyword.
>  
*/
bvm_cache *whileop(bvm_cache *this_bvm){

    mword *while_body  = dstack_get(this_bvm, 0);
    mword *cond_clause = dstack_get(this_bvm, 1);

    mword *while_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *iteration = _newva(0);

    popd(this_bvm);
    popd(this_bvm);

    mword *while_select = _newva(WHILE_COND);

    mword *while_rstack_entry = consa(iteration,
                                consa(while_body,
                                    consa(while_return,
                                        consa(cond_clause,
                                            consa(while_select, nil )))));

    pushr(this_bvm, while_rstack_entry, _hash8(C2B("/babel/tag/while")));

    this_bvm->code_ptr = consa(cond_clause,nil);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;    

}


// FIXME: Needs to dig down rstack...
/* flow-control operator
**iter**
Places the current loop iteration (zero-based) on TOS.  
*/
bvm_cache *iter(bvm_cache *this_bvm){ // iter#

    mword *result;
    mword *rtos     = rstack_get(this_bvm,0);
    result = _newva(*(mword*)icar(rtos));

    //mword *tag      = (mword*)icar(rstack_get_tag(this_bvm, 0));

    //while(rstack-not-empty and tag-not-recognized){...}

//    if(tageq(tag,BABEL_TAG_LOOP,TAG_SIZE)){
//    }
//    else if(tageq(tag,BABEL_TAG_TIMES,TAG_SIZE)){
//    }
//    else if(tageq(tag,BABEL_TAG_EACH,TAG_SIZE)){
//    }
//    else if(tageq(tag,BABEL_TAG_WHILE,TAG_SIZE)){
//    }

    pushd(this_bvm, result, MORTAL);

    return this_bvm;

}


/* flow-control operator
**next**  
> Goes to next iteration of current loop  
*/
bvm_cache *next(bvm_cache *this_bvm){ // next#

    _next(this_bvm);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;

}

bvm_cache *_next(bvm_cache *this_bvm){ // _next#

    mword *rtos     = rstack_get(this_bvm,0);
    mword *tag      = (mword*)icar(rstack_get_tag(this_bvm, 0));

    mword *sink;
    mword *iter;

    if(tageq(tag,BABEL_TAG_EVAL,TAG_SIZE)){
        sink = popr(this_bvm);
        this_bvm->code_ptr = consa(rtos,nil);
       

    }
    else if(tageq(tag,BABEL_TAG_LOOP,TAG_SIZE)){
//                    this_bvm->code_ptr = consa(rtos,nil);
//                    
//                    continue;
        iter = (mword*)icar(rtos);
        *iter = *iter + 1;
        this_bvm->code_ptr = consa((mword*)icar(icdr(rtos)),nil);
        
    
    }
    else if(tageq(tag,BABEL_TAG_TIMES,TAG_SIZE)){
        //_dump(rtos);
        iter = (mword*)icar(rtos);
        *iter = *iter - 1;
        if(*iter == 0){
            sink = popr(this_bvm);
            //this_bvm->code_ptr = consa((mword*)icar(icdr(icdr(rtos))),nil);
            //this_bvm->code_ptr = consa(_ith(rtos,2),nil);
            //(mword*)c(this_bvm->code_ptr,0) = _ith(rtos,2);
            set_code_ptr(this_bvm, _ith(rtos,2));
            
    
        }
        else{
            //this_bvm->code_ptr = consa((mword*)icar(icdr(rtos)),nil);
            //this_bvm->code_ptr = consa(_ith(rtos,1),nil);
            //(mword*)c(this_bvm->code_ptr,0) = _ith(rtos,1);
            set_code_ptr(this_bvm, _ith(rtos,1));
            
    
        }
    }
    else if(tageq(tag,BABEL_TAG_EACH,TAG_SIZE)){

        iter = (mword*)icar(rtos);
        *iter = *iter + 1;

        mword *list = (mword*)icdr(icdr(icdr(rtos)));

        if(is_nil(icdr(icar(list)))){
            sink = popr(this_bvm);
            this_bvm->code_ptr = consa((mword*)icar(icdr(icdr(rtos))),nil);
            
    
        }
        else{
            *list = icdr(icar(list));
            pushd(this_bvm, (mword*)icar(icar(list)), IMMORTAL);
            this_bvm->code_ptr = consa((mword*)icar(icdr(rtos)),nil);
            
    
        }
    }
    else if(tageq(tag,BABEL_TAG_IFTE,TAG_SIZE)){

        mword *ifte_select = (mword*)icar(rtos);

        if(*ifte_select == IFTE_BODY){

            sink = popr(this_bvm);
            this_bvm->code_ptr = consa((mword*)icar(icdr(icdr(rtos))),nil);
            
    

        }
        else{

            *ifte_select = IFTE_BODY;

            mword *cond = dstack_get(this_bvm, 0);
            popd(this_bvm);

            if(!is_false(cond)){
                this_bvm->code_ptr = consa((mword*)icar(icdr(rtos)),nil);
                
    
            }
            else{
                this_bvm->code_ptr = consa((mword*)icar(icdr(icdr(icdr(rtos)))),nil);
                
    
            }

        }
    }
    else if(tageq(tag,BABEL_TAG_WHILE,TAG_SIZE)){

        iter = (mword*)icar(rtos);
        *iter = *iter + 1;

        mword *while_select = _ith(rtos,4);

//                    _dump(while_select);
//                    die;

        if(*while_select == WHILE_BODY){

            *while_select = WHILE_COND;

            mword *cond_clause = _ith(rtos,3);
            this_bvm->code_ptr = consa(cond_clause,nil);
            
    

        }
        else{

            *while_select = WHILE_BODY;

            mword *cond = dstack_get(this_bvm, 0);
            popd(this_bvm);

            if(!is_false(cond)){
                mword *while_body = _ith(rtos,1);
                this_bvm->code_ptr = consa(while_body,nil);
                
    
            }
            else{
                mword *while_return = _ith(rtos,2);
                this_bvm->code_ptr = consa(while_return,nil);
                sink = popr(this_bvm);
            }

        }
    }
    else if(tageq(tag,BABEL_TAG_NEST,TAG_SIZE)){
        (mword*)icar(this_bvm->dstack_ptr) = _ith(rtos,0);
        (mword*)icar(this_bvm->ustack_ptr) = _ith(rtos,1);
        this_bvm->code_ptr         = consa(_ith(rtos,2),nil);
        sink = popr(this_bvm);   
    }
    else if(tageq(tag,BABEL_TAG_LET,TAG_SIZE)){

        mword *walker = _ith(rtos,0);
        while(!is_nil(walker)){
            (mword*)icar(icar(icar(walker))) = (mword*)icdr(icar(walker));
            walker = (mword*)icdr(walker);
        }

//        _dump(_ith(rtos,0));
//        die;

        set_code_ptr(this_bvm, _ith(rtos,2));

        sink = popr(this_bvm);

    }
    else{
        _mem(tag);
        fatal("unrecognized tag");
    }

    icar(this_bvm->advance_type) = BVM_ADVANCE;

    return this_bvm;

}


/* flow-control operator
**last**  
> Breaks out of current loop  
*/
bvm_cache *last(bvm_cache *this_bvm){ 

    mword *rtos        = rstack_get(this_bvm,0);
    mword *last_return = _ith(rtos,2);
    popr(this_bvm);

    //mword *tag      = (mword*)icar(rstack_get_tag(this_bvm, 0));

    //while(rstack-not-empty and tag-not-recognized){...}

//    if(tageq(tag,BABEL_TAG_LOOP,TAG_SIZE)){
//    }
//    else if(tageq(tag,BABEL_TAG_TIMES,TAG_SIZE)){
//    }
//    else if(tageq(tag,BABEL_TAG_EACH,TAG_SIZE)){
//    }
//    else if(tageq(tag,BABEL_TAG_WHILE,TAG_SIZE)){
//    }

//    pushd(this_bvm, result, MORTAL);

    this_bvm->code_ptr = consa(last_return,nil);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;

}


/* flow-control operator
**let**
> Defines a lexical-variable scope
*/
bvm_cache *let(bvm_cache *this_bvm){

    mword *let_body = dstack_get(this_bvm,0);
    mword *let_list = dstack_get(this_bvm,1);

    popd(this_bvm);
    popd(this_bvm);

    mword *walker = let_list;
    while(!is_nil(walker)){
        mword *let_list_entry = consa( (mword*)icar(walker), (mword*)car(icar(walker)) );
        (mword*)icar(walker) = let_list_entry;
        walker = (mword*)icdr(walker);
    }

    mword *let_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *let_rstack_entry = consa(let_list,
                                    consa(let_body,
                                        consa(let_return, nil)));

    pushr(this_bvm, let_rstack_entry, _hash8(C2B("/babel/tag/let")));

    this_bvm->code_ptr = consa(let_body,nil);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;

}


//FIXME: Busted. See ifte to get it right.
// babel_operator
bvm_cache *ifop(bvm_cache *this_bvm){

    fatal("unimplemented");
//   { > 0 } { "greater than zero\n" << } if

//    mword *then_clause = get_from_stack( this_bvm, (mword*)TOS_0( this_bvm ) );
//    hard_zap(this_bvm);
//
//    mword *cond_clause = get_from_stack( this_bvm, (mword*)TOS_0( this_bvm ) );
//    hard_zap(this_bvm);
//
//    dup(this_bvm); //Almost always we need to dup the TOS...
//
//    mword *temp = _newin(IFOP_RSTACK_ENTRIES);
//
//    (mword*)c(temp,IFOP_RSTACK_RETURN) = (mword*)cdr(this_bvm->code_ptr);
//    (mword*)c(temp,IFOP_RSTACK_THEN)   = then_clause;
//
//    push_alloc_rstack(this_bvm, temp, IFOP);
//
//    car(this_bvm->advance_type) = BVM_CONTINUE;
//
//    this_bvm->code_ptr = cond_clause;

    return this_bvm;

}


/* flow-control operator
**continue**  
> Same as next but can be used within an if or eval  
*/

//bvm_cache *continueop(bvm_cache *this_bvm){
//
//    next(this_bvm);
//    next(this_bvm);
//
//    return this_bvm;
//
//}


/* flow-control operator
**cond** (??)   
>   
> `(([x] 0 gt)  
>     ("positive" say)  
> ( [x] 0 lt)  
>     ("negative" say)  
> ( [x] 0 eq)  
>     ("zero" say))`  
> cond  
*/



//bvm_cache *whileop(bvm_cache *this_bvm){ //XXX buggy...
//
//    fatal("stack fix not done");
//    mword *cond_block = TOS_0(this_bvm);
//    hard_zap(this_bvm);
//
//    mword *body = TOS_0(this_bvm);
//    hard_zap(this_bvm);
//
//    mword *block_sel = new_atom;
//    *block_sel = WHILE_COND;
//
//    mword *temp = _newin(WHILE_RSTACK_ENTRIES);
//    (mword*)c(temp,WHILE_RSTACK_COND)   = cond_block;
//    (mword*)c(temp,WHILE_RSTACK_BODY)   = body;
//            c(temp,WHILE_RSTACK_RETURN) = cdr(this_bvm->code_ptr);
//    (mword*)c(temp,WHILE_RSTACK_SELECT) = block_sel;
//
//    mword *iter_temp = new_atom;
//    *iter_temp = 0;
//    (mword*)c(temp,WHILE_RSTACK_ITER)   = iter_temp;
//
//    push_alloc_rstack(this_bvm, temp, WHILEOP);
//
//    icar(this_bvm->advance_type) = BVM_CONTINUE;
//
//    this_bvm->code_ptr = cond_block;
//    
//    return this_bvm;
//
//}



/* flow-control operator
**eachar**
Iterates across an array. See `each`.  
*/
bvm_cache *eachar(bvm_cache *this_bvm){

// FIXME: Catch the empty-list condition...

    fatal("stack fix not done");
    mword *result;

    mword *body = TOS_0(this_bvm);
    hard_zap(this_bvm);

    mword *array = TOS_0(this_bvm);
    hard_zap(this_bvm);

    mword *count = new_atom;
    *count = EACHAR_INIT_INDEX;

    mword *temp = _newin(EACHAR_RSTACK_ENTRIES);
    (mword*)c(temp,EACHAR_RSTACK_ARRAY)  = array;
    (mword*)c(temp,EACHAR_RSTACK_BODY)   = body;
            c(temp,EACHAR_RSTACK_RETURN) = cdr(this_bvm->code_ptr);
    (mword*)c(temp,EACHAR_RSTACK_COUNT)  = count;

     mword *iter_temp = new_atom;
    *iter_temp = 0;
    (mword*)c(temp,EACHAR_RSTACK_ITER)   = iter_temp;

    push_alloc_rstack(this_bvm, temp, EACHAR);

    icar(this_bvm->advance_type) = BVM_CONTINUE;

    this_bvm->code_ptr = body;

    if(is_leaf(array)){
        result = new_atom;
        *result = c(array,EACHAR_INIT_INDEX);
    }
    else{
        result = (mword*)c(array,EACHAR_INIT_INDEX);
    }

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Revisit
    
    return this_bvm;

}

/* flow-control operator
**conjure**
Attempts to call a function in the parent BVM.  
*/
bvm_cache *conjure(bvm_cache *this_bvm){

    fatal("stack fix not done");
    this_bvm->code_ptr = (mword*)cdr(this_bvm->code_ptr);

    icar(this_bvm->advance_type) = BVM_RETURN;

    return this_bvm;

}

/* flow-control operator
**zipeach**
**carteach**
*/

//void eachar(void){
//
//    //body   RTOS-0
//    //return RTOS-1
//    //array  RTOS-2
//    //count  RTOS-3
//
////    if(TOS_0 != nil){
////        mword *count = _newlf(1);
////        *count = 0;
////        push_alloc_rstack((mword*)count,EACHAR);
////        push_alloc_rstack((mword*)TOS_0, EACHAR);
////        mword *temp_array = (mword*)TOS_0;
////        push_alloc_rstack((mword*)cdr(code_ptr), EACHAR);
////
////        zap();
////        mword temp_code_ptr = TOS_0;
////        zap();
////        code_ptr = temp_code_ptr;
////        push_alloc_rstack((mword*)code_ptr, EACHAR);
////        push_alloc((mword*)c(temp_array,*count),EACHAR);
////    }
////    else{
////        code_ptr = cdr(code_ptr);
////    }
//
//    //return RTOS-3
//    //body   RTOS-2
//    //array  RTOS-1
//    //count  RTOS-0
//
//    mword *temp;
//    if(TOS_0 != nil){
//
//        mword *count = _newlf(1);
//        *count = 0;
//
//        mword *temp_list = (mword*)TOS_0;
//        mword *temp_code_ptr = (mword*)cdr(code_ptr);
//
//        zap();
//
//        code_ptr = TOS_0;
//
//        zap();
//
//        temp = cons_alloc(temp_code_ptr, (mword*)nil);
//        temp = cons_alloc((mword*)code_ptr, temp);
//        temp = cons_alloc(temp_list, temp);
//        temp = cons_alloc(count, temp);
//        
//        push_alloc_rstack(temp, EACHAR);       
//        push_alloc((mword*)c(temp_list,*count),EACHAR);
//
//    }
//    else{
//
//        code_ptr = cdr(code_ptr);
//
//    }
//
//
//}

// Clayton Bauman 2011

