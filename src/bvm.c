// XXX STACK FIX DONE
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
#include "count.h"
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

void bvmexec(void){

    _bvmexec((mword*)TOS_0);

    zap();

//    zap();
//    push_alloc(temp_cons, BVMEXEC);

}

void _bvmexec(mword *bvm){

    mword *saved_bvm = internal_global_VM;

    internal_global_VM = bvm;
    global_VM = (mword *)cdr(internal_global_VM);

    bvm_interp();

    internal_global_VM = saved_bvm;
    global_VM = (mword *)cdr(internal_global_VM);

}

void _bvm_init(mword *bvm){

    pearson16_init();

    time_t rawtime;
    char time_string[30];
    time( &rawtime );    
    strcpy( time_string, ctime(&rawtime) );
    mword *time_string_key = _c2b(time_string, 30);

    // This needs to be enhanced to look in the hidden section for a 
    // pre-defined seed, it should also save the value it used in the
    // hidden section
    mword *time_hash = new_hash();
    mword *hash_init = new_hash();
    time_hash = _pearson16(hash_init, time_string_key);
    init_by_array(time_hash, HASH_SIZE*(sizeof(mword)/sizeof(unsigned long)));

    internal_global_VM = bvm+1;
    global_VM = (mword *)cdr(internal_global_VM);

}

//bvm_init
//
void bvm_init(void){

    int i;

    bvm_check();

    _load(global_VM-1);

    internal_global_VM = global_VM; // A BVM is never permitted to access internal_global_VM
//    d(car(car(internal_global_VM)))
  //      die
    global_VM = (mword *)cdr(global_VM);

//    STACK_TRACE = 0; // FIXME: This needs to be removed at some point...

}

//bvm_check
//
// Checks the "spine" of a BVM
//
void bvm_check(void){
   
    //Check size of BVM
//    if(global_machine_page_size < SMALLEST_VALID_BVM){
//        except("bvm_check: the loaded file is smaller than SMALLEST_VALID_BVM", __FILE__, __LINE__);
//    }
//
}

//bvm_interp
//
void bvm_interp(void){

    while(1){

        if(car(code_ptr) == (mword)nil){
            if(!rstack_empty){
                pop_rstack();
            }
            else{
                break;
            }
        }

        if( is_inte((mword *)car(code_ptr)) ){
            push_alloc((mword*)car(car(code_ptr)), SELF_ALLOC);
        }
        else if( is_leaf((mword *)car(code_ptr)) ){
            opcode_switch(car(car(code_ptr)))
        }
        else{
            except("bvm_interp: error detected during execution", __FILE__, __LINE__);
        }

        code_ptr = cdr(code_ptr);

    }

}

void bvmroot(void){

    push_alloc(global_VM, BVMROOT);

}

//#ifdef DEBUG
//
//void internal_bvmroot(void){
//
//    push_alloc(internal_global_VM, INTERNAL_BVMROOT);
//
//}
//
//#endif


void bbl2gv(void){

    printf("digraph babel {\nnode [shape=record];\n");
    printf("graph [rankdir = \"LR\"];\n");
    
//        d(global_VM)
//        d(car(car(car(stack_ptr))))
//        die
        tree_bbl2gv((mword*)TOS_0);

    printf("}\n");

    clean_tree((mword*)TOS_0);
    zap();

}


// Returns 
//
mword tree_bbl2gv(mword *tree){

    int i;
//    mword count = 0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    s(tree) |= 0x1;

    if(is_inte(tree)){

        printf("\"s%08x\" [shape=record,label=\"", (mword)tree);
        for(i=0; i<num_elem; i++){
            if(i==0){
                if(tree == (mword*)code_list){
                    printf("<f0> code_list");
                }
                else if(tree == (mword*)data_list){
                    printf("<f0> data_list");
                }
                else if(tree == (mword*)stack_list){
                    printf("<f0> stack_list");
                }
                else if(tree == (mword*)rstack_list){
                    printf("<f0> rstack_list");
                }
                else if(tree == (mword*)jump_table){
                    printf("<f0> jmp_table");
                }
                else if(tree == (mword*)sym_table){
                    printf("<f0> sym_table");
                }
                else if(tree == (mword*)nada){
                    printf("<f0> nada");
                }
                else if(tree == (mword*)nil){
                    printf("<f0> nil");
                }
                else{
                    printf("<f0> 0");
                }            
            }
            else{
                printf("<f%d> %d", i, i);
            }
            if(i<(num_elem-1)){
                printf("|");
            }
        }
        printf("\"];\n");

        for(i=0; i<num_elem; i++){
            if(*(mword *)(tree+i) == nil && tree != (mword*)nada){
                continue;
            }
            printf("\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)tree, i, *(mword *)(tree+i));
            _tree_bbl2gv((mword *)*(tree+i));
        }

    }
    else{ // is_leaf
        printf("s%08x [style=bold,shape=record,label=\"", (mword)tree);
        for(i=0; i<num_elem; i++){
            printf("<f%d> %x", i, *(mword *)(tree+i));
            if(i<(num_elem-1)){
                printf("|");
            }
        }
        printf("\"];\n");
    }

//    return count;

}


void _bbl2gv(void){

    // Figure out buffer size
    // Base size 2kb (size of minimal minimal.pb.bbl when converted to gv)
    // 32 * _mu
    mword initial_buf_size = (1<<11) + (32 * _mu((mword*)TOS_0));
    char *buffer = malloc(initial_buf_size);
    mword n;

    mword final_buf_size=0;

    n = sprintf(buffer, "digraph babel {\nnode [shape=record];\n");
    buffer         += n;
    final_buf_size += n;

    n = sprintf(buffer, "graph [rankdir = \"LR\"];\n");
    buffer         += n;
    final_buf_size += n;
    
//        d(global_VM)
//        d(car(car(car(stack_ptr))))
//        die
        _tree_bbl2gv((mword*)TOS_0);

    n = sprintf(buffer, "}\n");
    buffer         += n;
    final_buf_size += n;

    clean_tree((mword*)TOS_0);
    zap();

    push_alloc((mword*)buffer,BBL2GV);

}


// Returns 
//
mword _tree_bbl2gv(mword *tree){

    int i;
//    mword count = 0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    s(tree) |= 0x1;

    if(is_inte(tree)){

        printf("\"s%08x\" [shape=record,label=\"", (mword)tree);
        for(i=0; i<num_elem; i++){
            if(i==0){
                if(tree == (mword*)code_list){
                    printf("<f0> code_list");
                }
                else if(tree == (mword*)data_list){
                    printf("<f0> data_list");
                }
                else if(tree == (mword*)stack_list){
                    printf("<f0> stack_list");
                }
                else if(tree == (mword*)rstack_list){
                    printf("<f0> rstack_list");
                }
                else if(tree == (mword*)jump_table){
                    printf("<f0> jmp_table");
                }
                else if(tree == (mword*)sym_table){
                    printf("<f0> sym_table");
                }
                else if(tree == (mword*)nada){
                    printf("<f0> nada");
                }
                else if(tree == (mword*)nil){
                    printf("<f0> nil");
                }
                else{
                    printf("<f0> 0");
                }            
            }
            else{
                printf("<f%d> %d", i, i);
            }
            if(i<(num_elem-1)){
                printf("|");
            }
        }
        printf("\"];\n");

        for(i=0; i<num_elem; i++){
            if(*(mword *)(tree+i) == nil && tree != (mword*)nada){
                continue;
            }
            printf("\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)tree, i, *(mword *)(tree+i));
            _tree_bbl2gv((mword *)*(tree+i));
        }

    }
    else{ // is_leaf
        printf("s%08x [style=bold,shape=record,label=\"", (mword)tree);
        for(i=0; i<num_elem; i++){
            printf("<f%d> %x", i, *(mword *)(tree+i));
            if(i<(num_elem-1)){
                printf("|");
            }
        }
        printf("\"];\n");
    }

//    return count;

}



// Clayton Bauman 2011

