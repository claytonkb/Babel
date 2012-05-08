// bvm.c
//

#include "babel.h"
#include "bvm.h"
#include "load.h"
#include "stack.h"
#include "bvm_opcodes.h"
#include "bvm_interp.h"
#include "arith.h"
#include "io.h"
#include "array.h"
#include "eval.h"
#include "list.h"
#include "debug.h"
#include "pearson16.h"
#include "except.h"
#include "cmp.h"
#include "string.h"
#include "shift.h"
#include "logic.h"
#include "util.h"
#include <time.h>
#include "mt19937ar.h"
#include "hash.h"
#include "bstruct.h"
#include "alloc.h"
#include "ref.h"

//void bvmstep(void){
//
////    global_steps = car(TOS_0);
////    zap();
////
////    mword *bvm = (mword*)TOS_0;
////    zap();
////
////    _bvmstep(bvm);
//
//    global_steps = (mword)car(TOS_0);
//    zap();
//
//    _bvmstep((mword*)TOS_0);
//
//}
//
//void bvmexec(void){
//
////    bbl2gv();
////    die
//
//    global_steps = (mword) -1;
//    _bvmstep((mword*)TOS_0);
//    zap();
//
//}
//
//void _bvmstep(mword *bvm){
//
////    mword *saved_bvm = internal_global_VM;
////    mword *saved_global_argv = (mword*)global_argv; //FIXME: All this global_argv code needs to be re-implemented CORRECTLY
////
////    internal_global_VM = bvm;
////    global_VM = (mword *)cdr(internal_global_VM);
////    (mword*)global_argv = saved_global_argv;  //FIXME
////
////    bvm_interp();
////
//////    mword *after_interp_global_VM = internal_global_VM;
////
////    internal_global_VM = saved_bvm;
////    global_VM = (mword*)cdr(internal_global_VM);
////
//////    push_alloc(after_interp_global_VM, BVMKILL); //FIXME: Appropriating BVMKILL...
//
//    mword *saved_bvm = internal_global_VM;
//    mword *saved_global_argv = (mword*)global_argv; //FIXME: All this global_argv code needs to be re-implemented CORRECTLY
//
//    internal_global_VM = bvm;
//    global_VM = (mword *)cdr(internal_global_VM);
//
////FIXME NIL-CONVERSION:
////    (mword*)global_argv = saved_global_argv;  //FIXME
//
////    bvm_interp();
//    bvm_interp2();
//
//    internal_global_VM = saved_bvm;
//    global_VM = (mword*)cdr(internal_global_VM);
//
//}
//
////
//void _bvmexec(mword *bvm){
//
//    mword *saved_bvm = internal_global_VM;
//    mword *saved_global_argv = (mword*)global_argv; //FIXME: All this global_argv code needs to be re-implemented CORRECTLY
//
//    internal_global_VM = bvm;
//    global_VM = (mword *)cdr(internal_global_VM);
//
//// FIXME NIL-CONVERSION:
////    (mword*)global_argv = saved_global_argv;  //FIXME
//
//    bvm_interp();
//
//    internal_global_VM = saved_bvm;
//    global_VM = (mword*)cdr(internal_global_VM);
//
//}
//
//void _bvm_init(mword *bvm, int argc, char **argv){
//
//    pearson16_init();
//
//    time_t rawtime;
//    char time_string[30];
//    time( &rawtime );    
//    strcpy( time_string, ctime(&rawtime) );
//    mword *time_string_key = _c2b(time_string, 30);
//
//    // FIXME: strcpy and strlen... get rid
//    // This needs to be enhanced to look in the hidden section for a 
//    // pre-defined seed, it should also save the value it used in the
//    // hidden section
//    mword *time_hash = new_hash();
//    mword *hash_init = new_hash();
//    time_hash = _pearson16(hash_init, time_string_key, (mword)strlen((char*)time_string_key));
//    init_by_array(time_hash, HASH_SIZE*(sizeof(mword)/sizeof(unsigned long)));
//
//    //Set global_nil
//    mword *hash_init = new_hash();
//    mword *nil_string = C2B("nil");
//    global_nil = _pearson16(hash_init, nil_string, (mword)strlen((char*)nil_string));
//
////    printf("  0x%x ", global_nil[0]);
////    printf("0x%x ", global_nil[1]);
////    printf("0x%x ", global_nil[2]);
////    printf("0x%x\n", global_nil[3]);
////
////    mword *test_string = C2B("test");
////    mword *test = _pearson16(hash_init, test_string, (mword)strlen((char*)test_string));
////
////    printf("%d\n", is_nil(test));
//
////    internal_global_VM = bvm+1;
//    internal_global_VM = bvm;
//    global_VM = (mword *)cdr(internal_global_VM);
//
//    init_global_argv(argc, argv);
//
//}

//////bvm_init
//////
//void bvm_init(mword *bvm){
//
//    pearson16_init();
//
//    time_t rawtime;
//    char time_string[30];
//    time( &rawtime );    
//    strcpy( time_string, ctime(&rawtime) );
//    mword *time_string_key = _c2b(time_string, 30);
//
//    // FIXME: strcpy and strlen... get rid
//    // This needs to be enhanced to look in the hidden section for a 
//    // pre-defined seed, it should also save the value it used in the
//    // hidden section
//    mword *time_hash = new_hash();
//    mword *hash_init = new_hash();
//    time_hash = _pearson16(hash_init, time_string_key, (mword)strlen((char*)time_string_key));
//    init_by_array(time_hash, HASH_SIZE*(sizeof(mword)/sizeof(unsigned long)));
//
////    internal_global_VM = bvm+1;
//    internal_global_VM = bvm;
//    global_VM = (mword *)cdr(internal_global_VM);
//
//}

//DEPRECATED:
////bvm_interp
////
//void bvm_interp(void){
////
////    mword* discard;
////
////    while(global_steps--){//FIXME: This is not correct long-term
////
////        if(car(code_ptr) == (mword)nil){
////            if(_end_of_code()) continue;
////            break;
//////            if(!rstack_empty){
//////                while(alloc_type(RTOS_0) == DOWN){
//////                    up();
//////                }
//////                if(alloc_type(RTOS_0) == NEST){
//////                    up();
//////                }
//////                if(alloc_type(RTOS_0) == TIMES){
//////                    if(car(car(RTOS_2)) > 1){
//////                        c((mword*)car(RTOS_2),0) = car(car(RTOS_2)) - 1;
//////                        (mword*)code_ptr = (mword*)car(RTOS_0);
//////                    }
//////                    else{
//////                        discard = pop_rstack();
//////                        (mword*)code_ptr = (mword*)car(pop_rstack());
//////                        discard = pop_rstack();
//////                    }
//////                }
//////                else if(alloc_type(RTOS_0) == WHILEOP){
//////                    if(!is_false((mword*)TOS_0)){
//////                        zap();
//////                        (mword*)code_ptr = (mword*)car(RTOS_0);
//////                        push_alloc_rstack((mword*)car(RTOS_2), 0);
//////                    }
//////                    else{
//////                        zap();
//////                        discard = pop_rstack();
//////                        (mword*)code_ptr = (mword*)car(pop_rstack());
//////                        discard = pop_rstack();
//////                    }
//////                }
//////                else if(alloc_type(RTOS_0) == EACH){
//////                    if(cdr(car(RTOS_2)) != nil){
//////                        c((mword*)RTOS_2,0) = cdr(car(RTOS_2));
//////                        push_alloc((mword*)car(car(RTOS_2)),EACH);
//////                        (mword*)code_ptr = (mword*)car(RTOS_0);
//////                    }
//////                    else{
//////                        discard = pop_rstack();
//////                        (mword*)code_ptr = (mword*)car(pop_rstack());
//////                        discard = pop_rstack();
//////                    }
//////                }
//////                else if(alloc_type(RTOS_0) == EACHAR){
//////                    if(car(car(RTOS_3)) < size((mword*)car(RTOS_2))-1){
//////                        *((mword*)car(RTOS_3)) = car(car(RTOS_3)) + 1;
//////                        push_alloc((mword*)c((mword*)car(RTOS_2),car(car(RTOS_3))),EACHAR);
//////                        (mword*)code_ptr = (mword*)car(RTOS_0);
//////                    }
//////                    else{
//////                        discard = pop_rstack();
//////                        (mword*)code_ptr = (mword*)car(pop_rstack());
//////                        discard = pop_rstack();
//////                        discard = pop_rstack();
//////                    }
//////                }
//////                else if(alloc_type(RTOS_0) == LOOP){
//////                    (mword*)code_ptr = (mword*)car(RTOS_0);
//////                }
//////                else{
//////                    (mword*)code_ptr = (mword*)car(pop_rstack());
//////                }
//////                continue;
//////            }
//////            else{
//////                break;
//////            }
////        }
////
////        if( is_inte((mword *)car(code_ptr)) ){
////            push_alloc((mword*)car(car(code_ptr)), SELF_ALLOC);
////        }
////        else if( is_leaf((mword *)car(code_ptr)) ){
////            opcode_switch(car(car(code_ptr)))
////        }
////        else if( is_href((mword *)car(code_ptr)) ){ //FIXME: Implement href operator calls!
////            except("bvm_interp: hash-reference detected in code", __FILE__, __LINE__);
////        }
////        else{
////            except("bvm_interp: error detected during execution", __FILE__, __LINE__);
////        }
////
////        code_ptr = cdr(code_ptr);
////
////    }
//
//}

//
bvm_cache *bvm_interp(bvm_cache *this_bvm){

    bvm_cache *discard;
    babel_op op_ptr;

    while(this_bvm->steps--){//FIXME: This is not correct long-term

        if(is_nil((mword*)scar(this_bvm->code_ptr))){
            if(!is_nil(this_bvm->rstack_ptr)){
                next(this_bvm);
                continue;
            }
            break; // XXX An unexpected nil in code-stream can cause an exit... may be bad
        }

        if( is_inte(car(this_bvm->code_ptr)) ){
//            if(is_href((mword*)car(car(this_bvm->code_ptr)))){
//                error("bvm_interp: hash-reference detected on stack");
//                die;
//            }
            push_alloc(this_bvm, (mword*)car(car(this_bvm->code_ptr)), SELF_ALLOC);
//            this_bvm->code_ptr = (mword*)cdr(this_bvm->code_ptr);
        }
        else if( is_leaf(car(this_bvm->code_ptr)) ){
//            opcode_switch(car(car(this_bvm->code_ptr)));
//            d(car(car(this_bvm->code_ptr)))
            mword opcode = car(car(this_bvm->code_ptr));
            op_ptr = (babel_op)this_bvm->jump_table[ opcode % NUM_INTERP_OPCODES ];
            discard = op_ptr(this_bvm);
        }
        else if( is_href(car(this_bvm->code_ptr)) ){ //TODO: Implement href operator calls!
            error("bvm_interp: hash-reference detected in code");
            die;
        }
        else{
            error("bvm_interp: error detected during execution");
            die;
        }

        if(this_bvm->advance_type == BVM_ADVANCE){
            this_bvm->code_ptr = (mword*)cdr(this_bvm->code_ptr);
        }
        else{
            this_bvm->advance_type = BVM_ADVANCE;
        }

    }

    return this_bvm;

}

//
bvm_cache *babelop(bvm_cache *this_bvm){

    bvm_cache new_bvm;

    //initialize bvm
    new_bvm.hidden        = nil;
    new_bvm.sym_table     = nil;
    new_bvm.code_ptr      = (mword*)TOS_0(this_bvm);
    new_bvm.stack_ptr     = nil; //FIXME allow stack to be init'd
    new_bvm.rstack_ptr    = nil;
    new_bvm.jump_table    = this_bvm->jump_table;
    new_bvm.thread_id     = this_bvm->thread_id+1;
    new_bvm.steps         = (mword)-1;
    new_bvm.advance_type  = BVM_ADVANCE;
    new_bvm.argv          = this_bvm->argv; //FIXME: shift off 0th argv

    //FIXME: pearson_init and argv

    hard_zap(this_bvm);

    bvm_interp(&new_bvm);

    //FIXME - push stack of new_bvm onto this_bvm when bvm_interp returns
    //push_alloc(this_bvm, new_bvm.stack_ptr, BVMEXEC);

    return this_bvm;

}

//
bvm_cache *bvmcode(bvm_cache *this_bvm){

    push_alloc(this_bvm, this_bvm->code_ptr, BVMCODE);

    return this_bvm;

}

//
bvm_cache *bvmstack(bvm_cache *this_bvm){

    push_alloc(this_bvm, this_bvm->stack_ptr, BVMSTACK);

    return this_bvm;

}

//
bvm_cache *bvmrstack(bvm_cache *this_bvm){

    push_alloc(this_bvm, this_bvm->rstack_ptr, BVMRSTACK);

    return this_bvm;

}

//
bvm_cache *rsvd(bvm_cache *this_bvm){

    d(car(car(this_bvm->code_ptr)))

    error("An attempt was made to execute a reserved opcode");
    die;

}

////#ifdef DEBUG
////
////void internal_bvmroot(void){
////
////    push_alloc(internal_global_VM, INTERNAL_BVMROOT);
////
////}
////
////#endif

////DEPRECATED
//void bbl2gv(mword *tree){
//
//    int i;
//
//    printf("digraph babel {\nnode [shape=record];\n");
//    printf("graph [rankdir = \"LR\"];\n");
//
//    tree_bbl2gv(tree);
//
//    printf("}\n");
//
//    rclean(tree);
//
//}
//
//
////  
////
//mword tree_bbl2gv(mword *tree){
//
//    int i;
////    mword count = 0;
//
//    if( s(tree) & (MWORD_SIZE-1) ){
//        return 0;
//    }
//
//    if( is_href(tree) ){
//        printf("s%08x [style=dashed,shape=record,label=\"", (mword)tree);
//        for(i=0; i<HASH_SIZE; i++){
//            printf("<f%d> %x", i, *(mword *)(tree+i));
//            if(i<(HASH_SIZE-1)){
//                printf("|");
//            }
//        }
//        printf("\"];\n");
//        s(tree) |= 0x1;
//        return 0;
//    }
//
//    int num_elem = size(tree);
//
////    count = num_elem + 1;
//
//    s(tree) |= 0x1;
//
//    if(is_inte(tree)){
//
//        printf("\"s%08x\" [shape=record,label=\"", (mword)tree);
//        for(i=0; i<num_elem; i++){
//            printf("<f%d> %d", i, i);
//            if(i<(num_elem-1)){
//                printf("|");
//            }
//        }
//        printf("\"];\n");
//
//        for(i=0; i<num_elem; i++){
////            if(*(mword *)(tree+i) == nil && tree != (mword*)nada){
//            if(is_nil((mword *)scar(tree+i))){// == nil && tree != (mword*)nada){
//                continue;
//            }
//            printf("\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)tree, i, *(mword *)(tree+i));
//            tree_bbl2gv((mword *)*(tree+i));
//        }
//
//    }
//    else{ // is_leaf
//        printf("s%08x [style=bold,shape=record,label=\"", (mword)tree);
//        for(i=0; i<num_elem; i++){
//            printf("<f%d> %x", i, *(mword *)(tree+i));
//            if(i<(num_elem-1)){
//                printf("|");
//            }
//        }
//        printf("\"];\n");
//    }
//
////    return count;
//
//}

// Clayton Bauman 2011

