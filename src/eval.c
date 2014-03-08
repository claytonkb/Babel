// eval.c
//

#include "babel.h"
#include "eval.h"
#include "stack.h"
#include "bstruct.h"
//#include "debug.h"
//#include "bvm_opcodes.h"
#include "array.h"
#include "bvm.h"
#include "list.h"
#include "tptr.h"
#include "pearson16.h"
#include "string.h"
#include "mem.h"
#include "interp.h"
#include "load.h"

/* flow-control operator
**eval** (!)  
> Named after the Perl function  
> Evaluates TOS  
> `[X]| -> |`  
*/
bvm_cache *eval(bvm_cache *this_bvm){ // eval#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm); 

//    operand_info oinfo;
//    operand_info **oinfo_ptr;
//    oinfo_ptr[0] = &oinfo;
//
//    oinfo.default = nil;
//    oinfo.mask = OI_MASK_NONE | OI_MASK_INTE;
//    oinfo.min_size = 0;
//    oinfo.max_size = 0;
//
//    get_operands(this_bvm, oinfo_ptr, 1);
//
//    mword *op0 = oinfo.data;

    _eval(this_bvm, op0, (mword*)icdr(icar(this_bvm->code_ptr)));

    return this_bvm;

}


// Impersonates eval operator...
//
void _eval(bvm_cache *this_bvm, mword *eval_body, mword *eval_return){ // _eval#

    pushr(this_bvm, eval_return, _hash8(this_bvm, C2B("/babel/tag/eval")));
    
    this_bvm->code_ptr = consa(this_bvm, eval_body,nil);

    this_bvm->advance_type = BVM_CONTINUE;

}


/* flow-control operator
**loop**
> Loops indefinitely. Use `last` to break out.  
*/
bvm_cache *loop(bvm_cache *this_bvm){ // loop#

    mword *loop_body = dstack_get(this_bvm,0);
    mword *iteration = _newva( this_bvm, 0);

    popd(this_bvm); 

    // ADD this for `last` operator
    mword *loop_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *loop_rstack_entry = consa(this_bvm, iteration,
                                    consa(this_bvm, loop_body,
                                        consa(this_bvm, loop_return, nil)));

    pushr(this_bvm, loop_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/loop")));

    this_bvm->code_ptr = consa(this_bvm, loop_body,nil);

    this_bvm->advance_type = BVM_CONTINUE;

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

    this_bvm->code_ptr = consa(this_bvm, goto_point,nil);

    this_bvm->advance_type = BVM_CONTINUE;

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

    mword *iterations  = _newva( this_bvm, icar(dstack_get(this_bvm,0)));
    mword *times_body  = dstack_get(this_bvm,1);

    popd(this_bvm); 
    popd(this_bvm);

    if(c(iterations,0) == 0){
        return this_bvm;
    }

    mword *times_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *times_rstack_entry = consa(this_bvm, iterations,
                                    consa(this_bvm, times_body,
                                        consa(this_bvm, times_return, nil)));

    pushr(this_bvm, times_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/times")));

    this_bvm->code_ptr = consa(this_bvm, times_body,nil);

    //_dump(this_bvm->code_ptr);

    this_bvm->advance_type = BVM_CONTINUE;    

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
> `0 (1 2 3 4) {+} each --> Leaves 10 on TOS`  
*/
bvm_cache *each(bvm_cache *this_bvm){ // each#

    mword *each_body  = dstack_get(this_bvm,0);
    mword *each_list  = dstack_get(this_bvm,1);

    popd(this_bvm); 
    popd(this_bvm);

    if(is_nil(each_list))
        return this_bvm;

    mword *iteration = _newva( this_bvm, 0);

    mword *each_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *each_rstack_entry = consa(this_bvm, iteration,
                                    consa(this_bvm, each_body,
                                        consa(this_bvm, each_return, 
                                            consa(this_bvm, each_list, nil))));

    pushr(this_bvm, each_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/each")));

    pushd(this_bvm, (mword*)icar(each_list), IMMORTAL);

    this_bvm->code_ptr = consa(this_bvm, each_body,nil);

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;

}


/* flow-control operator
**ifte**  
> If-Then-Else. Like the Joy operator.  
>  
> `{x}[Y][Z]| -> eval(Y)| --> if x == 0`  
> `{x}[Y][Z]| -> eval(Z)| --> if x == 1`  
*/
bvm_cache *ifte(bvm_cache *this_bvm){ // ifte#

    mword *else_clause = dstack_get(this_bvm, 0);
    mword *then_clause = dstack_get(this_bvm, 1);
    mword *cond_clause = dstack_get(this_bvm, 2);

    mword *ifte_return = (mword*)icdr(icar(this_bvm->code_ptr));

    popd(this_bvm);
    popd(this_bvm);
    popd(this_bvm);

    mword *ifte_select = _newva( this_bvm, IFTE_COND);

    mword *ifte_rstack_entry = consa(this_bvm, ifte_select,
                                consa(this_bvm, then_clause,
                                    consa(this_bvm, ifte_return,
                                        consa(this_bvm, else_clause, nil ))));

    pushr(this_bvm, ifte_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/ifte")));

    this_bvm->code_ptr = consa(this_bvm, cond_clause,nil);

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;    

}


/* flow-control operator
**while**  
> Like the C keyword.
>  
*/
bvm_cache *whileop(bvm_cache *this_bvm){ // whileop# while#

    mword *while_body  = dstack_get(this_bvm, 0);
    mword *cond_clause = dstack_get(this_bvm, 1);

    mword *while_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *iteration = _newva( this_bvm, 0);

    popd(this_bvm);
    popd(this_bvm);

    mword *while_select = _newva( this_bvm, WHILE_COND);

    mword *while_rstack_entry = consa(this_bvm, iteration,
                                consa(this_bvm, while_body,
                                    consa(this_bvm, while_return,
                                        consa(this_bvm, cond_clause,
                                            consa(this_bvm, while_select, nil )))));

    pushr(this_bvm, while_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/while")));

    this_bvm->code_ptr = consa(this_bvm, cond_clause,nil);

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;    

}


// FIXME: Needs to dig down rstack...
/* flow-control operator
**iter**
Places the current loop iteration (zero-based) on TOS.  
*/
bvm_cache *iter(bvm_cache *this_bvm){ // iter#

    mword *result;

    int rstack_depth = _len(this_bvm, (mword*)icar(this_bvm->rstack_ptr));
    int i = 0;

    mword *tag;

    while(i<=rstack_depth){

        if(i == rstack_depth) //no looping constructs: iter == fnord
            return this_bvm;

        tag = (mword*)icar(rstack_get_tag(this_bvm, i));

        if( tageq(tag,BABEL_TAG_LOOP,TAG_SIZE)
            || tageq(tag,BABEL_TAG_TIMES,TAG_SIZE)
            || tageq(tag,BABEL_TAG_EACH,TAG_SIZE)
            || tageq(tag,BABEL_TAG_EACHAR,TAG_SIZE)
            || tageq(tag,BABEL_TAG_WHILE,TAG_SIZE)){
            break;
        }
        else{
            i++;
        }

    }

    mword *rtos = rstack_get(this_bvm,i);

    result = _newva( this_bvm, *(mword*)icar(rtos));

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


/* flow-control operator
**next**  
> Goes to next iteration of current loop  
*/
bvm_cache *next(bvm_cache *this_bvm){ // next#

    int rstack_depth = _len(this_bvm, (mword*)icar(this_bvm->rstack_ptr));
    int i = 0;

    mword *tag;

    while(i<=rstack_depth){

        if(i == rstack_depth) //no looping constructs: iter == fnord
            return this_bvm;

        tag = (mword*)icar(rstack_get_tag(this_bvm, i));

        //_mem(tag);

        if(    tageq(tag, BABEL_TAG_LOOP  ,TAG_SIZE)
            || tageq(tag, BABEL_TAG_TIMES ,TAG_SIZE)
            || tageq(tag, BABEL_TAG_EACH  ,TAG_SIZE)
            || tageq(tag, BABEL_TAG_WHILE ,TAG_SIZE)){
            break;
        }
        else{
            i++;
        }

    }

    for(;i>0;i--){
        last(this_bvm);
        //_next(this_bvm);
    }

    _next(this_bvm);

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;

}

// FIXME: Needs to dig down rstack...
bvm_cache *_next(bvm_cache *this_bvm){ // _next#

    mword *rtos     = rstack_get(this_bvm,0);
    mword *tag      = (mword*)icar(rstack_get_tag(this_bvm, 0));

    mword *sink;
    mword *iter;

    if(tageq(tag,BABEL_TAG_EVAL,TAG_SIZE)){

        sink = popr(this_bvm);
        this_bvm->code_ptr = consa(this_bvm, rtos,nil);

    }
    else if(tageq(tag,BABEL_TAG_LOOP,TAG_SIZE)){

        iter = (mword*)icar(rtos);
        *iter = *iter + 1;
        this_bvm->code_ptr = consa(this_bvm, (mword*)icar(icdr(rtos)),nil);

    }
    else if(tageq(tag,BABEL_TAG_TIMES,TAG_SIZE)){

        //_dump(rtos);
        iter = (mword*)icar(rtos);
        *iter = *iter - 1;

        if(*iter == 0){

            sink = popr(this_bvm);
            //this_bvm->code_ptr = consa(this_bvm, (mword*)icar(icdr(icdr(rtos))),nil);
            //this_bvm->code_ptr = consa(this_bvm, _ith(this_bvm, rtos,2),nil);
            //(mword*)c(this_bvm->code_ptr,0) = _ith(this_bvm, rtos,2);
            set_code_ptr(this_bvm, _ith(this_bvm, rtos,2));

        }
        else{

            //this_bvm->code_ptr = consa(this_bvm, (mword*)icar(icdr(rtos)),nil);
            //this_bvm->code_ptr = consa(this_bvm, _ith(this_bvm, rtos,1),nil);
            //(mword*)c(this_bvm->code_ptr,0) = _ith(this_bvm, rtos,1);
            set_code_ptr(this_bvm, _ith(this_bvm, rtos,1));

        }
    }
    else if(tageq(tag,BABEL_TAG_EACH,TAG_SIZE)){

        iter = (mword*)icar(rtos);
        *iter = *iter + 1;

        mword *list = (mword*)icdr(icdr(icdr(rtos)));

        if(is_nil(icdr(icar(list)))){

            sink = popr(this_bvm);
            this_bvm->code_ptr = consa(this_bvm, (mword*)icar(icdr(icdr(rtos))),nil);

        }
        else{

            *list = icdr(icar(list));
            pushd(this_bvm, (mword*)icar(icar(list)), IMMORTAL);
            this_bvm->code_ptr = consa(this_bvm, (mword*)icar(icdr(rtos)),nil);

        }
    }
    else if(tageq(tag,BABEL_TAG_EACHAR,TAG_SIZE)){

        iter = (mword*)icar(rtos);
        *iter = *iter + 1;

        mword *array = _ith(this_bvm, rtos,3);

        if(*iter >= size(array)){

            sink = popr(this_bvm);
            set_code_ptr(this_bvm, _ith(this_bvm, rtos,2));

        }
        else{

            mword *entry;
            if(is_leaf(array)){
                entry = _newva( this_bvm, c(array,*iter));
            }
            else{
                entry = (mword*)c(array,*iter);
            }

            pushd(this_bvm, entry, IMMORTAL);
            //pushd(this_bvm, (mword*)c(array,), IMMORTAL);

            set_code_ptr(this_bvm, _ith(this_bvm, rtos,1));

        }
    }
    else if(tageq(tag,BABEL_TAG_IFTE,TAG_SIZE)){

        mword *ifte_select = (mword*)icar(rtos);

        if(*ifte_select == IFTE_BODY){

            sink = popr(this_bvm);
            this_bvm->code_ptr = consa(this_bvm, (mword*)icar(icdr(icdr(rtos))),nil);

        }
        else{

            *ifte_select = IFTE_BODY;

            mword *cond = dstack_get(this_bvm, 0);
            popd(this_bvm);

            if(!is_false(cond)){

                this_bvm->code_ptr = consa(this_bvm, (mword*)icar(icdr(rtos)),nil);
    
            }
            else{

                this_bvm->code_ptr = consa(this_bvm, (mword*)icar(icdr(icdr(icdr(rtos)))),nil);
    
            }

        }
    }
    else if(tageq(tag,BABEL_TAG_WHILE,TAG_SIZE)){

        iter = (mword*)icar(rtos);
        *iter = *iter + 1;

        mword *while_select = _ith(this_bvm, rtos,4);

        if(*while_select == WHILE_BODY){

            *while_select = WHILE_COND;

            mword *cond_clause = _ith(this_bvm, rtos,3);
            this_bvm->code_ptr = consa(this_bvm, cond_clause,nil);
            
        }
        else{

            *while_select = WHILE_BODY;

            mword *cond = dstack_get(this_bvm, 0);
            popd(this_bvm);

            if(!is_false(cond)){

                mword *while_body = _ith(this_bvm, rtos,1);
                this_bvm->code_ptr = consa(this_bvm, while_body,nil);
    
            }
            else{

                mword *while_return = _ith(this_bvm, rtos,2);
                this_bvm->code_ptr = consa(this_bvm, while_return,nil);
                sink = popr(this_bvm);

            }
        }
    }
    else if(tageq(tag,BABEL_TAG_NEST,TAG_SIZE)){

        mword *save_TOS = dstack_get(this_bvm,0);
        popd(this_bvm);

//        (mword*)icar(this_bvm->dstack_ptr) = _ith(this_bvm, rtos,0);
//        (mword*)icar(this_bvm->ustack_ptr) = _ith(this_bvm, rtos,1);

        set_dstack_ptr(this_bvm, _ith(this_bvm, rtos,0));
        set_ustack_ptr(this_bvm, _ith(this_bvm, rtos,1));

        pushd(this_bvm, save_TOS, IMMORTAL);

//        this_bvm->dstack_ptr = consa(this_bvm, _ith(this_bvm, rtos,0),nil);
//        this_bvm->ustack_ptr = consa(this_bvm, _ith(this_bvm, rtos,1),nil);

        this_bvm->code_ptr         = consa(this_bvm, _ith(this_bvm, rtos,2),nil);
        sink = popr(this_bvm);   

    }
    else if(tageq(tag,BABEL_TAG_LET,TAG_SIZE)){

        mword *walker = _ith(this_bvm, rtos,0);

        while(!is_nil(walker)){
            (mword*)icar(icar(icar(walker))) = (mword*)icdr(icar(walker));
            walker = (mword*)icdr(walker);
        }

        set_code_ptr(this_bvm, _ith(this_bvm, rtos,2));

        sink = popr(this_bvm);

    }
    else if(tageq(tag,BABEL_TAG_COND,TAG_SIZE)){ // FIXME: use _pop(this_bvm) instead of manual list-mods

        mword *cond_select = _ith(this_bvm, rtos,0);

        if(*cond_select == COND_BODY){

            set_code_ptr(this_bvm, _ith(this_bvm, rtos,2));
            popr(this_bvm);

        }
        else{// if(*cond_select == COND_COND){

            mword *cond = dstack_get(this_bvm, 0);
            popd(this_bvm);

            if(!is_false(cond)){

                *cond_select = COND_BODY;

                mword *cond_list = _ith(this_bvm, rtos,1);

                if(is_nil(cond_list)){ // _len of cond_list is odd

                    set_code_ptr(this_bvm, _ith(this_bvm, rtos,2));
                    popr(this_bvm);

                }
                else{
                    
                    set_code_ptr(this_bvm,_ith(this_bvm, cond_list,0));

                }
            }
            else{ // if(is_false(cond))

                //*cond_select = COND_COND;
                mword *cond_list = _ith(this_bvm, rtos,1);

                if( is_nil(cond_list) ){ // _len of cond_list is odd

                    set_code_ptr(this_bvm, _ith(this_bvm, rtos,2));
                    popr(this_bvm);

                }
                else if( is_nil(icdr(cond_list)) ){ // we've reached the end...

                    set_code_ptr(this_bvm, _ith(this_bvm, rtos,2));
                    popr(this_bvm);

                }
                else{

                    //set_code_ptr(this_bvm,(mword*)icar(icdr(cond_list)));
                    set_code_ptr(this_bvm,(mword*)_ith(this_bvm, cond_list,1));

                    // Not sure which is uglier...                    
                    //cond_list = (mword*)icdr(rtos);
                    //(mword*)*cond_list = _pop(this_bvm, (mword*)icar(cond_list));
                    //(mword*)*cond_list = _pop(this_bvm, (mword*)icar(cond_list));

                    *(mword*)icdr(rtos) = icdr(icdr(icar(icdr(rtos))));

                }
            }
        }
    }
    else if(tageq(tag,BABEL_TAG_ALT,TAG_SIZE)){

        mword *alt = dstack_get(this_bvm, 0);

        if( is_tptr(alt)
            && tageq(alt,BABEL_TAG_PASS,TAG_SIZE) ){ // quit the alt...

            set_code_ptr(this_bvm, _ith(this_bvm, rtos,1));
            popr(this_bvm);

        }
        else{

            icar(rtos) = icdr(icar(rtos));

            mword *alt_list = _ith(this_bvm, rtos,0);

            if( is_nil(alt_list) ){ // we've reached the end...

                if( !(is_tptr(alt)
                    && tageq(alt,BABEL_TAG_FAIL,TAG_SIZE)) ){
                    fail(this_bvm); // signal that we've eval'd all clauses without passing
                }

                bvm_flush_cache(this_bvm); // Needed due to dstack/ustack save-restore feature

                mword *alt_save_restore_list = _ith(this_bvm, rtos,2);
                int i;
                int list_length = _len(this_bvm, alt_save_restore_list);
                for(i=0;i<list_length;i++){
                    mword *alt_save_restore_entry = _ith(this_bvm, alt_save_restore_list, i); // XXX perf: re-traversal
                    mword *alt_restore_point      = _ith(this_bvm, alt_save_restore_entry, 0);
                    mword *alt_save_restore       = _ith(this_bvm, alt_save_restore_entry, 1);
                    alt_save_restore = _load(this_bvm, alt_save_restore, size(alt_save_restore));
                    (mword*)c(alt_restore_point,0) = alt_save_restore;
                }

                bvm_update_cache(this_bvm); // Needed due to dstack/ustack save-restore feature

                set_code_ptr(this_bvm, _ith(this_bvm, rtos,1));
                popr(this_bvm);

            }
            else{ // go to the next clause...

                if( is_tptr(alt)
                    && tageq(alt,BABEL_TAG_FAIL,TAG_SIZE) ){
                    popd(this_bvm);
                }

                set_code_ptr(this_bvm,(mword*)_ith(this_bvm, alt_list,0));

                bvm_flush_cache(this_bvm); // Needed due to dstack/ustack save-restore feature

                mword *alt_save_restore_list = _ith(this_bvm, rtos,2);
                int i;
                int list_length = _len(this_bvm, alt_save_restore_list);
                for(i=0;i<list_length;i++){
                    mword *alt_save_restore_entry = _ith(this_bvm, alt_save_restore_list, i); // XXX perf: re-traversal
                    mword *alt_restore_point      = _ith(this_bvm, alt_save_restore_entry, 0);
                    mword *alt_save_restore       = _ith(this_bvm, alt_save_restore_entry, 1);
                    alt_save_restore = _load(this_bvm, alt_save_restore, size(alt_save_restore));
                    (mword*)c(alt_restore_point,0) = alt_save_restore;
                }

                bvm_update_cache(this_bvm); // Needed due to dstack/ustack save-restore feature

            }
        }
    }
    else if(tageq(tag,BABEL_TAG_SEQ,TAG_SIZE)){

        mword *seq = dstack_get(this_bvm, 0);

        if( is_tptr(seq)
            && tageq(seq,BABEL_TAG_FAIL,TAG_SIZE) ){ // quit the seq...

            set_code_ptr(this_bvm, _ith(this_bvm, rtos,1));

//            mword *seq_restore_point = _ith(this_bvm, rtos,2);
//            mword *seq_save_restore  = _ith(this_bvm, rtos,3);
//            seq_save_restore = _load(this_bvm, seq_save_restore, size(seq_save_restore));
//            (mword*)c(seq_restore_point,0) = seq_save_restore;

            bvm_flush_cache(this_bvm); // Needed due to dstack/ustack save-restore feature

            mword *seq_save_restore_list = _ith(this_bvm, rtos,2);
            int i;
            int list_length = _len(this_bvm, seq_save_restore_list);
            for(i=0;i<list_length;i++){
                mword *seq_save_restore_entry = _ith(this_bvm, seq_save_restore_list, i); // XXX perf: re-traversal
                mword *seq_restore_point      = _ith(this_bvm, seq_save_restore_entry, 0);
                mword *seq_save_restore       = _ith(this_bvm, seq_save_restore_entry, 1);
                seq_save_restore = _load(this_bvm, seq_save_restore, size(seq_save_restore));
                (mword*)c(seq_restore_point,0) = seq_save_restore;
            }

            bvm_update_cache(this_bvm); // Needed due to dstack/ustack save-restore feature

//_dump(this_bvm->dstack_ptr);
//die;

            popr(this_bvm);

        }
        else{

            icar(rtos) = icdr(icar(rtos));

            mword *seq_list = _ith(this_bvm, rtos,0);

            if( is_nil(seq_list) ){ // we've reached the end...

                if( !( is_tptr(seq)
                    && tageq(seq,BABEL_TAG_PASS,TAG_SIZE)) ){
                    pass(this_bvm); // signal that we've passed all clauses without failing
                }

                set_code_ptr(this_bvm, _ith(this_bvm, rtos,1));
                popr(this_bvm);

            }
            else{ // go to the next clause...

                if( is_tptr(seq)
                    && tageq(seq,BABEL_TAG_PASS,TAG_SIZE) ){
                    popd(this_bvm);
                }

                set_code_ptr(this_bvm,(mword*)_ith(this_bvm, seq_list,0));

            }
        }

    }
    else{
        _mem(tag);
        fatal("unrecognized tag");
    }

    this_bvm->advance_type = BVM_ADVANCE;

    return this_bvm;

}


// FIXME: Needs to dig down rstack...
/* flow-control operator
**last**  
> Breaks out of current loop  
*/
bvm_cache *last(bvm_cache *this_bvm){ // last#

    int rstack_depth = _len(this_bvm, (mword*)icar(this_bvm->rstack_ptr));
    int i = 0;

    mword *tag;

    while(i<=rstack_depth){

        if(i == rstack_depth){ //no looping constructs: iter == fnord
            return this_bvm;
        }

        tag = (mword*)icar(rstack_get_tag(this_bvm, i));

        if(    tageq(tag,BABEL_TAG_LOOP,TAG_SIZE)
            || tageq(tag,BABEL_TAG_TIMES,TAG_SIZE)
            || tageq(tag,BABEL_TAG_EACH,TAG_SIZE)
            || tageq(tag,BABEL_TAG_WHILE,TAG_SIZE)){
            break;
        }
        else{
            i++;
        }

    }

    mword *rtos = rstack_get(this_bvm,i);

    mword *last_return = _ith(this_bvm, rtos,2);

    this_bvm->code_ptr = consa(this_bvm, last_return,nil);

    for(;i>=0;i--){
        popr(this_bvm);
    }

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;

}


/* flow-control operator
**let**
> Defines a lexical-variable scope. Inspired by Lisp's let but
> only saves the old variables - does not initialize the new
> variables.
> Usage: list body let
*/
bvm_cache *let(bvm_cache *this_bvm){ // let#

    mword *let_body = dstack_get(this_bvm,0);
    mword *let_list = dstack_get(this_bvm,1);

    popd(this_bvm);
    popd(this_bvm);

    mword *walker = let_list;
    while(!is_nil(walker)){
        mword *let_list_entry = consa(this_bvm,  (mword*)icar(walker), (mword*)car(icar(walker)) );
        (mword*)icar(walker) = let_list_entry;
        walker = (mword*)icdr(walker);
    }

    mword *let_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *let_rstack_entry = consa(this_bvm, let_list,
                                    consa(this_bvm, let_body,
                                        consa(this_bvm, let_return, nil)));

    pushr(this_bvm, let_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/let")));

    this_bvm->code_ptr = consa(this_bvm, let_body,nil);

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;

}


/* flow-control operator
**alt**
> Defines an alternation.
> See also: seq, pass, fail
*/
bvm_cache *alt(bvm_cache *this_bvm){ // alt#

    mword *alt_clause_list  = dstack_get(this_bvm, 0);
    mword *alt_restore_list = dstack_get(this_bvm, 1);

    popd(this_bvm);
    popd(this_bvm);

    if( is_nil(alt_clause_list)
        || _len(this_bvm, alt_clause_list) < 1){
        return this_bvm;
    }

//    if( is_nil((mword*)icar(alt_restore_list)) ){
//    
//        alt_restore_list = consa(this_bvm, 
//                                this_bvm->dstack_ptr, 
//                                consa(this_bvm, 
//                                    this_bvm->ustack_ptr, 
//                                    (mword*)cdr(alt_restore_list) ));
//
//    }

    mword *alt_return = (mword*)icdr(icar(this_bvm->code_ptr));

    int i;
    int list_length = _len(this_bvm, alt_restore_list);
    mword *last_cons = nil;
    for(i=0;i<list_length;i++){
        mword *alt_restore_point = _ith(this_bvm, alt_restore_list, i);
        mword *alt_save_restore  = (mword*)icar(alt_restore_point);
        last_cons = 
                consa( this_bvm,
                    consa( this_bvm, alt_restore_point, 
                    consa( this_bvm, _unload( this_bvm, alt_save_restore), nil)),
                last_cons);
    }

//    _dump(last_cons);
//    die;

    mword *alt_rstack_entry = consa(this_bvm, alt_clause_list, 
                                    consa(this_bvm, alt_return, 
                                        consa(this_bvm, last_cons, nil)));

    pushr(this_bvm, alt_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/alt")));

    set_code_ptr(this_bvm,(mword*)icar(alt_clause_list));

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;

}


//
//
bvm_cache *pass(bvm_cache *this_bvm){ // pass#

    mword *result = new_tptr(
                        this_bvm,
                        _hash8(this_bvm, C2B("/babel/tag/pass")), 
                        nil);

    pushd(this_bvm, result, IMMORTAL);    

    //last(this_bvm);    

    return this_bvm;

}


//
//
bvm_cache *fail(bvm_cache *this_bvm){ // fail#

    mword *result = new_tptr(
                        this_bvm,
                        _hash8(this_bvm, C2B("/babel/tag/fail")), 
                        nil);

    pushd(this_bvm, result, IMMORTAL);    

    //last(this_bvm);    

    return this_bvm;

}


/* flow-control operator
**cond** (??)   
>   
> `(([x] 0 gt)  
>     ("positive" say)  
> ( [x] 0 lt)  
>     ("negative" say)  
> ( [x] 0 eq)  
>     ("zero" say))  
> cond`  
*/
bvm_cache *cond(bvm_cache *this_bvm){ // cond#

    mword *cond_list = dstack_get(this_bvm, 0);
    popd(this_bvm);

    if(_len(this_bvm, cond_list) < 2)
        return this_bvm;

    mword *cond_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *cond_select = _newva( this_bvm, COND_COND);

    mword *cond_rstack_entry = consa(this_bvm, cond_select,
                                consa(this_bvm, (mword*)icdr(cond_list), //XXX Loop bug?
                                    consa(this_bvm, cond_return, nil )));

    pushr(this_bvm, cond_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/cond")));

    //dup(this_bvm); // Perform this step before eval'ing each cond

    //this_bvm->code_ptr = consa(this_bvm, (mword*)icar(cond_list),nil);
    set_code_ptr(this_bvm,(mword*)icar(cond_list));

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;        

}



/* flow-control operator
**seq** 
> see also: alt, pass, fail
*/
bvm_cache *seq(bvm_cache *this_bvm){ // seq#

    mword *seq_clause_list  = dstack_get(this_bvm, 0);
    mword *seq_restore_list = dstack_get(this_bvm, 1);

    popd(this_bvm);
    popd(this_bvm);

    if( is_nil(seq_clause_list)
        || _len(this_bvm, seq_clause_list) < 1){
        return this_bvm;
    }

//    if( is_nil((mword*)icar(seq_restore_list)) ){
//    
//        seq_restore_list = consa(this_bvm, 
//                                this_bvm->dstack_ptr, 
//                                consa(this_bvm, 
//                                    this_bvm->ustack_ptr, 
//                                    (mword*)cdr(seq_restore_list) ));
//
//    }

    mword *seq_return = (mword*)icdr(icar(this_bvm->code_ptr));

    int i;
    int list_length = _len(this_bvm, seq_restore_list);
    mword *last_cons = nil;
    for(i=0;i<list_length;i++){
        mword *seq_restore_point = _ith(this_bvm, seq_restore_list, i);
        mword *seq_save_restore  = (mword*)icar(seq_restore_point);
        last_cons = 
                consa( this_bvm,
                    consa( this_bvm, seq_restore_point, 
                    consa( this_bvm, _unload( this_bvm, seq_save_restore), nil)),
                last_cons);
    }

    mword *seq_rstack_entry = consa(this_bvm, seq_clause_list, 
                                    consa(this_bvm, seq_return, 
                                        consa(this_bvm, last_cons, nil)));

    pushr(this_bvm, seq_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/seq")));

    set_code_ptr(this_bvm,(mword*)icar(seq_clause_list));

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;        

//    mword *seq_save_restore = _unload(this_bvm, (mword*)icar(seq_restore_list));
//    
//    mword *seq_rstack_entry = consa(this_bvm, seq_clause_list, 
//                                    consa(this_bvm, seq_return, 
//                                        consa(this_bvm, seq_restore_list,
//                                            consa(this_bvm, seq_save_restore,  nil))));

}


/* flow-control operator
**eachar**
Iterates across an array. See `each`.  
*/
bvm_cache *eachar(bvm_cache *this_bvm){ // *eachar#

// FIXME: Catch the empty-list condition...

//    fatal("stack fix not done");
//    mword *result;
//
//    mword *body = TOS_0(this_bvm);
//    hard_zap(this_bvm);
//
//    mword *array = TOS_0(this_bvm);
//    hard_zap(this_bvm);
//
//    mword *count = new_atom;
//    *count = EACHAR_INIT_INDEX;
//
//    mword *temp = _newin(this_bvm, EACHAR_RSTACK_ENTRIES);
//    (mword*)c(temp,EACHAR_RSTACK_ARRAY)  = array;
//    (mword*)c(temp,EACHAR_RSTACK_BODY)   = body;
//            c(temp,EACHAR_RSTACK_RETURN) = cdr(this_bvm->code_ptr);
//    (mword*)c(temp,EACHAR_RSTACK_COUNT)  = count;
//
//     mword *iter_temp = new_atom;
//    *iter_temp = 0;
//    (mword*)c(temp,EACHAR_RSTACK_ITER)   = iter_temp;
//
//    push_alloc_rstack(this_bvm, temp, EACHAR);
//
//    icar(this_bvm->advance_type) = BVM_CONTINUE;
//
//    this_bvm->code_ptr = body;
//
//    if(is_leaf(array)){
//        result = new_atom;
//        *result = c(array,EACHAR_INIT_INDEX);
//    }
//    else{
//        result = (mword*)c(array,EACHAR_INIT_INDEX);
//    }
//
//    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Revisit
//
//    return this_bvm;

/////////////////////////////////////////

    mword *each_body  = dstack_get(this_bvm,0);
    mword *each_array = dstack_get(this_bvm,1);

    popd(this_bvm); 
    popd(this_bvm);

    if(is_nil(each_array))
        return this_bvm;

    mword *iteration = _newva( this_bvm, 0);

    mword *each_return = (mword*)icdr(icar(this_bvm->code_ptr));

    mword *each_rstack_entry = consa(this_bvm, iteration,
                                    consa(this_bvm, each_body,
                                        consa(this_bvm, each_return, 
                                            consa(this_bvm, each_array, nil))));

    pushr(this_bvm, each_rstack_entry, _hash8(this_bvm, C2B("/babel/tag/eachar")));

    mword *entry;
    if(is_leaf(each_array)){
        entry = _newva( this_bvm, c(each_array,0));
    }
    else{
        entry = (mword*)c(each_array,0);
    }

    pushd(this_bvm, entry, IMMORTAL);

    this_bvm->code_ptr = consa(this_bvm, each_body,nil);

    this_bvm->advance_type = BVM_CONTINUE;

    return this_bvm;


}


//FIXME: Busted. See ifte to get it right.
// babel_operator
bvm_cache *ifop(bvm_cache *this_bvm){ // *ifop#

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
//    mword *temp = _newin(this_bvm, IFOP_RSTACK_ENTRIES);
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
**conjure**
Attempts to call a function in the parent BVM.  
*/
bvm_cache *conjure(bvm_cache *this_bvm){ // *conjure#

    fatal("stack fix not done");
    this_bvm->code_ptr = (mword*)cdr(this_bvm->code_ptr);

    this_bvm->advance_type = BVM_RETURN;

    return this_bvm;

}


/* flow-control operator
**zipeach**
**carteach**
*/

// Clayton Bauman 2011

