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

//
// babel_operator
bvm_cache *eval(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *body = TOS_0(this_bvm);
    hard_zap(this_bvm);

//    push_alloc_rstack(this_bvm, (mword*)scdr(this_bvm->code_ptr), EVAL);
//
//    this_bvm->code_ptr = body;
//
//    car(this_bvm->advance_type) = BVM_CONTINUE;

    _eval(this_bvm, body, (mword*)scdr(this_bvm->code_ptr));

    return this_bvm;

}

// Impersonates eval operator...
void _eval(bvm_cache *this_bvm, mword *eval_body, mword *eval_return){

    push_alloc_rstack(this_bvm, eval_return, EVAL);

    this_bvm->code_ptr = eval_body;

    car(this_bvm->advance_type) = BVM_CONTINUE;

}

//FIXME: Busted, need to handle empty stack
// babel_operator
bvm_cache *nest(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *body = TOS_0(this_bvm);
    hard_zap(this_bvm);

    mword *new_stack = TOS_0(this_bvm);
    hard_zap(this_bvm);
//    push_alloc_rstack(this_bvm, (mword*)this_bvm->stack_ptr, NEST);

//    clear(this_bvm);

    mword *save_stack  = this_bvm->stack_ptr;
    mword *save_ustack = this_bvm->ustack_ptr;

//    this_bvm->stack_ptr = nil; // clear the stack

    clear(this_bvm); // clear the stack

//    rgive(this_bvm, new_stack);

    push_alloc(this_bvm, new_stack, IMMORTAL);
//    push_alloc(this_bvm, body, IMMORTAL);

    mword *temp = _newin(NEST_RSTACK_ENTRIES);

    (mword*)c(temp,NEST_RSTACK_BODY)   = body;
            c(temp,NEST_RSTACK_RETURN) = cdr(this_bvm->code_ptr);
    (mword*)c(temp,NEST_RSTACK_STACK)  = save_stack;
    (mword*)c(temp,NEST_RSTACK_USTACK) = save_ustack;

    push_alloc_rstack(this_bvm, temp, NEST);

    this_bvm->code_ptr = body;

    car(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;    

}

// [x] (a) (b) ifte  
// !is_false(x) --> (a) eval
// is_false(x)  --> (b) eval
// babel_operator
bvm_cache *ifte(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *else_clause = get_from_stack( this_bvm, TOS_0( this_bvm ) );
    hard_zap(this_bvm);

    mword *then_clause = get_from_stack( this_bvm, TOS_0( this_bvm ) );
    hard_zap(this_bvm);

    mword *cond_clause = get_from_stack( this_bvm, TOS_0( this_bvm ) );
    hard_zap(this_bvm);

    dup(this_bvm); //Almost always we need to dup the TOS...

    mword *temp = _newin(IFTE_RSTACK_ENTRIES);

    mword *ifte_select = new_atom;
    *ifte_select = IFTE_COND;

    (mword*)c(temp,IFTE_RSTACK_RETURN) = (mword*)cdr(this_bvm->code_ptr);
    (mword*)c(temp,IFTE_RSTACK_THEN)   = then_clause;
    (mword*)c(temp,IFTE_RSTACK_ELSE)   = else_clause;
    (mword*)c(temp,IFTE_RSTACK_SELECT) = ifte_select;

    push_alloc_rstack(this_bvm, temp, IFTE);

    car(this_bvm->advance_type) = BVM_CONTINUE;

    this_bvm->code_ptr = cond_clause;

    return this_bvm;

}


//
// babel_operator
bvm_cache *let(bvm_cache *this_bvm){

    fatal("implementation broken");
    mword *body = get_from_stack( this_bvm, TOS_0( this_bvm ) );
    hard_zap(this_bvm);

    mword *lex_list = get_from_stack( this_bvm, TOS_0( this_bvm ) );
    hard_zap(this_bvm);

    // get length of lex var list
    mword length = _len(lex_list);
          length += LET_RSTACK_ENTRIES;

    // allocate rstack block big enough to fit
    mword *temp = _newin(length);
    mword *list = lex_list;

    (mword*)c(temp,LET_RSTACK_RETURN) = (mword*)cdr(this_bvm->code_ptr);
    (mword*)c(temp,LET_RSTACK_LIST)   = list;

    // copy current contents of the lex variables 
    //   to the rstack block
    mword i = LET_RSTACK_ENTRIES;
    while(!is_nil(list)){ //FIXME Breaks due to car/cdr can't handle hash-refs
        c(temp,i) = car(car(car(list)));
//        c(temp,i) = car(list);
        list = (mword*)cdr(list);
        i++;
    }

    // eval the body
    push_alloc_rstack(this_bvm, temp, LET);

    this_bvm->code_ptr = body;

    car(this_bvm->advance_type) = BVM_CONTINUE;

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

//
// babel_operator
bvm_cache *gotoop(bvm_cache *this_bvm){

    fatal("stack fix not done");
    this_bvm->code_ptr = (mword*)scar(scar(this_bvm->stack_ptr));

    hard_zap(this_bvm);

    car(this_bvm->advance_type) = BVM_CONTINUE;

    return this_bvm;

}

//
// babel_operator
bvm_cache *loop(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *body = TOS_0(this_bvm);
    hard_zap(this_bvm);

    mword *temp = _newin(LOOP_RSTACK_ENTRIES);

    mword *iter_temp = new_atom;
    *iter_temp = 0;

    (mword*)c(temp,LOOP_RSTACK_BODY)   = body;
            c(temp,LOOP_RSTACK_RETURN) = cdr(this_bvm->code_ptr);
    (mword*)c(temp,LOOP_RSTACK_ITER)   = iter_temp;

    push_alloc_rstack(this_bvm, temp, LOOP);

    car(this_bvm->advance_type) = BVM_CONTINUE;

    this_bvm->code_ptr = body;

    return this_bvm;

}

//
// babel_operator
bvm_cache *last(bvm_cache *this_bvm){ 

    fatal("stack fix not done");
    mword *rstack_entry;
    mword *result;
    int done=0;

    //FIXME: Handle empty rstack
    while( !done ){     
        
//        if(     return_type(this_bvm->rstack_ptr) == DOWN 
//            ||  return_type(this_bvm->rstack_ptr) == NEST){
//
//            up(this_bvm);
//
//        }
        if(return_type(this_bvm->rstack_ptr) == EVAL){

            //rstack_entry = (mword*)RTOS_0(this_bvm);
            //this_bvm->code_ptr = (mword*)rstack_entry[EVAL_RSTACK_RETURN];
            pop_rstack(this_bvm);

        }
        else{
            done = 1;
        }

    }

    if(return_type(this_bvm->rstack_ptr) == LOOP){

        rstack_entry = (mword*)RTOS_0(this_bvm);
        this_bvm->code_ptr = (mword*)rstack_entry[LOOP_RSTACK_RETURN];
        pop_rstack(this_bvm);

//        rstack_entry = (mword*)RTOS_0(this_bvm);
//        this_bvm->code_ptr = (mword*)scar((mword*)RTOS_0(this_bvm));

    }
    else if(return_type(this_bvm->rstack_ptr) == TIMES){

        rstack_entry = (mword*)RTOS_0(this_bvm);

        this_bvm->code_ptr = (mword*)rstack_entry[TIMES_RSTACK_RETURN];
        pop_rstack(this_bvm);

    }
    else if(return_type(this_bvm->rstack_ptr) == WHILEOP){ //XXX buggy...

            this_bvm->code_ptr = (mword*)rstack_entry[WHILE_RSTACK_RETURN];
            pop_rstack(this_bvm);
            hard_zap(this_bvm); //FIXME: Yipes!

    }
    else if(return_type(this_bvm->rstack_ptr) == EACH){

        rstack_entry = (mword*)RTOS_0(this_bvm);

        this_bvm->code_ptr = (mword*)rstack_entry[EACH_RSTACK_RETURN];
        pop_rstack(this_bvm);

    }
    else if(return_type(this_bvm->rstack_ptr) == EACHAR){

        rstack_entry = (mword*)RTOS_0(this_bvm);

        this_bvm->code_ptr = (mword*)rstack_entry[EACHAR_RSTACK_RETURN];
        pop_rstack(this_bvm);


    }
    else{
        error("last: unknown return_type");
        die;
    }

    car(this_bvm->advance_type) = BVM_CONTINUE;

}

////
//bvm_cache *continueop(bvm_cache *this_bvm){
//
//    next(this_bvm);
//    next(this_bvm);
//
//    return this_bvm;
//
//}

//
// babel_operator
bvm_cache *next(bvm_cache *this_bvm){ // XXX: Lots of perf issues in here

    mword *rstack_entry;
    mword *result;
    mword *temp;

    //FIXME: Handle empty rstack
    warn("implementation unstable");

    if(return_type(this_bvm->rstack_ptr) == EVAL){

        this_bvm->code_ptr = (mword*)car(pop_rstack(this_bvm));

    }
    else if(return_type(this_bvm->rstack_ptr) == LET){

        rstack_entry = (mword*)RTOS_0(this_bvm);

        // last/next
        //   restore prior contents of the lex variables from the rstack
        //   pop rstack
        mword* list = (mword*)rstack_entry[LET_RSTACK_LIST];

        // copy current contents of the lex variables 
        //   to the rstack block
        mword i = LET_RSTACK_ENTRIES;
        while(!is_nil(list)){ //FIXME Breaks due to car/cdr can't handle hash-refs
            c((mword*)c((mword*)c(list,0),0),0) = c(rstack_entry,i);
//            *(mword*)car(list) = *(mword*)c(rstack_entry,i);
            list = (mword*)cdr(list);
            i++;
        }

        this_bvm->code_ptr = (mword*)rstack_entry[TIMES_RSTACK_RETURN];
        pop_rstack(this_bvm);

    }
    else if(return_type(this_bvm->rstack_ptr) == LOOP){

        rstack_entry = (mword*)RTOS_0(this_bvm);
        this_bvm->code_ptr = (mword*)rstack_entry[LOOP_RSTACK_BODY];

        *(mword*)rstack_entry[LOOP_RSTACK_ITER] = car(rstack_entry[LOOP_RSTACK_ITER]) + 1;

    }
    else if(return_type(this_bvm->rstack_ptr) == TIMES){

        rstack_entry = (mword*)RTOS_0(this_bvm);
        if( car(rstack_entry[TIMES_RSTACK_COUNT]) > 1 ){
            *(mword*)rstack_entry[TIMES_RSTACK_COUNT] = car(rstack_entry[TIMES_RSTACK_COUNT]) - 1;
            this_bvm->code_ptr = (mword*)rstack_entry[TIMES_RSTACK_BODY];
            *(mword*)rstack_entry[TIMES_RSTACK_ITER] = car(rstack_entry[TIMES_RSTACK_ITER]) + 1;
        }
        else{
//            this_bvm->code_ptr = (mword*)car(rstack_entry[TIMES_RSTACK_RETURN]);
            this_bvm->code_ptr = (mword*)rstack_entry[TIMES_RSTACK_RETURN];
            pop_rstack(this_bvm);
        }

    }
    else if(return_type(this_bvm->rstack_ptr) == IFTE){

        rstack_entry = (mword*)RTOS_0(this_bvm);
        mword *clause;

        if( car(rstack_entry[IFTE_RSTACK_SELECT]) == IFTE_BODY ){
            this_bvm->code_ptr = (mword*)rstack_entry[IFTE_RSTACK_RETURN];
            pop_rstack(this_bvm);
        }
        else{ // car(rstack_entry[IFTE_RSTACK_SELECT]) == IFTE_COND
            if( is_false( get_from_stack( this_bvm, TOS_0( this_bvm ) ) ) ){
                clause = (mword*)rstack_entry[IFTE_RSTACK_ELSE];
            }
            else{
                clause = (mword*)rstack_entry[IFTE_RSTACK_THEN];
            }
            hard_zap(this_bvm);
            this_bvm->code_ptr = clause;
            *(mword*)rstack_entry[IFTE_RSTACK_SELECT] = IFTE_BODY;
        }

    }
    else if(return_type(this_bvm->rstack_ptr) == IFOP){

        rstack_entry = (mword*)RTOS_0(this_bvm);
        mword *clause;

        if( is_false( get_from_stack( this_bvm, TOS_0( this_bvm ) ) ) ){
            this_bvm->code_ptr = (mword*)rstack_entry[IFOP_RSTACK_RETURN];
            pop_rstack(this_bvm);

        }
        else{
            clause = (mword*)rstack_entry[IFOP_RSTACK_THEN];
            pop_rstack(this_bvm);
            _eval(this_bvm, clause, (mword*)rstack_entry[IFOP_RSTACK_RETURN]);
        }
        hard_zap(this_bvm);

    }
    else if(return_type(this_bvm->rstack_ptr) == WHILEOP){ //XXX buggy...

        rstack_entry = (mword*)RTOS_0(this_bvm);
        if( car(rstack_entry[WHILE_RSTACK_SELECT]) == WHILE_BODY ){
            this_bvm->code_ptr = (mword*)rstack_entry[WHILE_RSTACK_COND];
            *(mword*)rstack_entry[WHILE_RSTACK_SELECT] = WHILE_COND;
            *(mword*)rstack_entry[WHILE_RSTACK_ITER] = car(rstack_entry[WHILE_RSTACK_ITER]) + 1;
        }
        else{ // car(rstack_entry[WHILE_RSTACK_SELECT]) == WHILE_COND
            if( is_false(TOS_0(this_bvm)) ){
                this_bvm->code_ptr = (mword*)rstack_entry[WHILE_RSTACK_RETURN];
                pop_rstack(this_bvm);
            }
            else{
                this_bvm->code_ptr = (mword*)rstack_entry[WHILE_RSTACK_BODY];
                *(mword*)rstack_entry[WHILE_RSTACK_SELECT] = WHILE_BODY;
            }
            hard_zap(this_bvm);
        }

    }
    else if(return_type(this_bvm->rstack_ptr) == EACH){

        rstack_entry = (mword*)RTOS_0(this_bvm);

        if( is_nil((mword*)cdr(rstack_entry[EACH_RSTACK_LIST])) ){
            this_bvm->code_ptr = (mword*)rstack_entry[EACH_RSTACK_RETURN];
            pop_rstack(this_bvm);
        }
        else{
            this_bvm->code_ptr = (mword*)rstack_entry[EACH_RSTACK_BODY];
            rstack_entry[EACH_RSTACK_LIST] = cdr((mword*)rstack_entry[EACH_RSTACK_LIST]);
            push_alloc(this_bvm, (mword*)car((mword*)rstack_entry[EACH_RSTACK_LIST]), IMMORTAL); //FIXME: Revisit
            *(mword*)rstack_entry[EACH_RSTACK_ITER] = car(rstack_entry[EACH_RSTACK_ITER]) + 1;
        }

    }
    else if(return_type(this_bvm->rstack_ptr) == EACHAR){

        rstack_entry = (mword*)RTOS_0(this_bvm);

        if( car(rstack_entry[EACHAR_RSTACK_COUNT]) >= (size(rstack_entry[EACHAR_RSTACK_ARRAY])-1) ){

            this_bvm->code_ptr = (mword*)rstack_entry[EACHAR_RSTACK_RETURN];
            pop_rstack(this_bvm);

        }
        else{

            *(mword*)rstack_entry[EACHAR_RSTACK_COUNT] = car(rstack_entry[EACHAR_RSTACK_COUNT]) + 1;

            if(is_leaf(rstack_entry[EACHAR_RSTACK_ARRAY])){
                result  = new_atom;
                *result = c((mword*)rstack_entry[EACHAR_RSTACK_ARRAY],car(rstack_entry[EACHAR_RSTACK_COUNT]));
            }
            else{
                result = (mword*)c((mword*)rstack_entry[EACHAR_RSTACK_ARRAY],car(rstack_entry[EACHAR_RSTACK_COUNT]));
            }

            this_bvm->code_ptr = (mword*)rstack_entry[EACHAR_RSTACK_BODY];

            *(mword*)rstack_entry[EACHAR_RSTACK_ITER] = car(rstack_entry[EACHAR_RSTACK_ITER]) + 1;

            push_alloc(this_bvm, result, IMMORTAL); //FIXME: Revisit

        }

    }
    else if(return_type(this_bvm->rstack_ptr) == NEST){ //FIXME: Busted - need to handle empty stack

        rstack_entry = (mword*)RTOS_0(this_bvm);

        this_bvm->code_ptr = (mword*)rstack_entry[NEST_RSTACK_RETURN];
        pop_rstack(this_bvm);

        temp = TOS_0(this_bvm);

        this_bvm->stack_ptr  = (mword*)rstack_entry[NEST_RSTACK_STACK];
        this_bvm->ustack_ptr = (mword*)rstack_entry[NEST_RSTACK_USTACK];
        push_alloc(this_bvm,temp,IMMORTAL); //FIXME: Revisit

//        error("next: found NEST while trying to execute NEXT");
//        die;

    }
    else{
        error("next: unknown return_type");
        die;
    }

}

// (body) [x] times
// babel_operator
bvm_cache *times(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *temp;
    mword *times;

    if(car(TOS_0(this_bvm)) > 0){

        times = new_atom;
        *times = car(TOS_0(this_bvm));
        hard_zap(this_bvm);

        mword *body = TOS_0(this_bvm);
        hard_zap(this_bvm);

        mword *temp = _newin(TIMES_RSTACK_ENTRIES);

        (mword*)c(temp,TIMES_RSTACK_COUNT)  = times;
        (mword*)c(temp,TIMES_RSTACK_BODY)   = body;
                c(temp,TIMES_RSTACK_RETURN) = cdr(this_bvm->code_ptr);

        mword *iter_temp = new_atom;
        *iter_temp = 0;
        (mword*)c(temp,TIMES_RSTACK_ITER)   = iter_temp;

        push_alloc_rstack(this_bvm, temp, TIMES);

        car(this_bvm->advance_type) = BVM_CONTINUE;

        this_bvm->code_ptr = body;
    
    }

    return this_bvm;

}

// FIXME: Broken - need to add an "iter" field to 
// all looping rstack entries - also, iter should 
// "dig" through the rstack until it finds a
// looping rstack entry in case we're buried in evals
// or nesting.
// babel_operator
bvm_cache *iter(bvm_cache *this_bvm){

//    if(return_type(this_bvm->rstack_ptr) != TIMES){
//        return this_bvm;
//    }

    fatal("stack fix not done");
    mword *rstack_entry = (mword*)RTOS_0(this_bvm);

    mword *result = new_atom;



    if(return_type(this_bvm->rstack_ptr) == LOOP){
        *result = car(rstack_entry[LOOP_RSTACK_ITER]);
    }
    else if(return_type(this_bvm->rstack_ptr) == TIMES){
        *result = car(rstack_entry[TIMES_RSTACK_ITER]);
    }
    else if(return_type(this_bvm->rstack_ptr) == WHILEOP){ //XXX buggy...
        *result = car(rstack_entry[WHILE_RSTACK_ITER]);
    }
    else if(return_type(this_bvm->rstack_ptr) == EACH){
        *result = car(rstack_entry[EACH_RSTACK_ITER]);
    }
    else if(return_type(this_bvm->rstack_ptr) == EACHAR){
        *result = car(rstack_entry[EACHAR_RSTACK_ITER]);
    }
    else{
        error("iter: unsupported return_type");
        die;
    }

    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

// (body) (cond) while
// babel_operator
bvm_cache *whileop(bvm_cache *this_bvm){ //XXX buggy...

    fatal("stack fix not done");
    mword *cond_block = TOS_0(this_bvm);
    hard_zap(this_bvm);

    mword *body = TOS_0(this_bvm);
    hard_zap(this_bvm);

    mword *block_sel = new_atom;
    *block_sel = WHILE_COND;

    mword *temp = _newin(WHILE_RSTACK_ENTRIES);
    (mword*)c(temp,WHILE_RSTACK_COND)   = cond_block;
    (mword*)c(temp,WHILE_RSTACK_BODY)   = body;
            c(temp,WHILE_RSTACK_RETURN) = cdr(this_bvm->code_ptr);
    (mword*)c(temp,WHILE_RSTACK_SELECT) = block_sel;

    mword *iter_temp = new_atom;
    *iter_temp = 0;
    (mword*)c(temp,WHILE_RSTACK_ITER)   = iter_temp;

    push_alloc_rstack(this_bvm, temp, WHILEOP);

    car(this_bvm->advance_type) = BVM_CONTINUE;

    this_bvm->code_ptr = cond_block;
    
    return this_bvm;

}

// (body) (list) each
// babel_operator
bvm_cache *each(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *body = TOS_0(this_bvm);
    hard_zap(this_bvm);

// FIXME: Catch the empty-list condition...
    mword *list = TOS_0(this_bvm);
    hard_zap(this_bvm);

    mword *temp = _newin(EACH_RSTACK_ENTRIES);
    (mword*)c(temp,EACH_RSTACK_LIST)   = list;
    (mword*)c(temp,EACH_RSTACK_BODY)   = body;
            c(temp,EACH_RSTACK_RETURN) = cdr(this_bvm->code_ptr);

     mword *iter_temp = new_atom;
    *iter_temp = 0;
    (mword*)c(temp,EACH_RSTACK_ITER)   = iter_temp;
         
    push_alloc_rstack(this_bvm, temp, EACH);

    car(this_bvm->advance_type) = BVM_CONTINUE;

    this_bvm->code_ptr = body;

    push_alloc(this_bvm, (mword*)car(list), IMMORTAL); //FIXME: Revisit
    
    return this_bvm;

}

//
// babel_operator
bvm_cache *dieop(bvm_cache *this_bvm){
    fprintf(stderr, "Died.\n");
    exit(0);
}


// (body) [array] eachar
// (body) {array} eachar
// babel_operator
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

    car(this_bvm->advance_type) = BVM_CONTINUE;

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

//
// babel_operator
bvm_cache *conjure(bvm_cache *this_bvm){

    fatal("stack fix not done");
    this_bvm->code_ptr = (mword*)cdr(this_bvm->code_ptr);

    car(this_bvm->advance_type) = BVM_RETURN;

    return this_bvm;

}

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

