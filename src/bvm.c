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
#include "hash.h"
#include "bstruct.h"

void bvmstep(void){

//    global_steps = car(TOS_0);
//    zap();
//
//    mword *bvm = (mword*)TOS_0;
//    zap();
//
//    _bvmstep(bvm);

    global_steps = car(TOS_0);
    zap();

    _bvmstep((mword*)TOS_0);

}

void bvmexec(void){

    global_steps = (mword) -1;
    _bvmstep((mword*)TOS_0);
    zap();

}

void _bvmstep(mword *bvm){

//    mword *saved_bvm = internal_global_VM;
//    mword *saved_global_argv = (mword*)global_argv; //FIXME: All this global_argv code needs to be re-implemented CORRECTLY
//
//    internal_global_VM = bvm;
//    global_VM = (mword *)cdr(internal_global_VM);
//    (mword*)global_argv = saved_global_argv;  //FIXME
//
//    bvm_interp();
//
////    mword *after_interp_global_VM = internal_global_VM;
//
//    internal_global_VM = saved_bvm;
//    global_VM = (mword*)cdr(internal_global_VM);
//
////    push_alloc(after_interp_global_VM, BVMKILL); //FIXME: Appropriating BVMKILL...

    mword *saved_bvm = internal_global_VM;
    mword *saved_global_argv = (mword*)global_argv; //FIXME: All this global_argv code needs to be re-implemented CORRECTLY

    internal_global_VM = bvm;
    global_VM = (mword *)cdr(internal_global_VM);
    (mword*)global_argv = saved_global_argv;  //FIXME

    bvm_interp();

    internal_global_VM = saved_bvm;
    global_VM = (mword*)cdr(internal_global_VM);

}


void _bvmexec(mword *bvm){

    mword *saved_bvm = internal_global_VM;
    mword *saved_global_argv = (mword*)global_argv; //FIXME: All this global_argv code needs to be re-implemented CORRECTLY

    internal_global_VM = bvm;
    global_VM = (mword *)cdr(internal_global_VM);
    (mword*)global_argv = saved_global_argv;  //FIXME

    bvm_interp();

    internal_global_VM = saved_bvm;
    global_VM = (mword*)cdr(internal_global_VM);

}

void _bvm_init(mword *bvm){

    pearson16_init();

    time_t rawtime;
    char time_string[30];
    time( &rawtime );    
    strcpy( time_string, ctime(&rawtime) );
    mword *time_string_key = _c2b(time_string, 30);

    // FIXME: strcpy and strlen... get rid
    // This needs to be enhanced to look in the hidden section for a 
    // pre-defined seed, it should also save the value it used in the
    // hidden section
    mword *time_hash = new_hash();
    mword *hash_init = new_hash();
    time_hash = _pearson16(hash_init, time_string_key, (mword)strlen((char*)time_string_key));
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

    mword* discard;

    while(global_steps--){

        if(car(code_ptr) == (mword)nil){
            if(!rstack_empty){
                if(alloc_type(RTOS_0) == TIMES){
                    if(car(car(car(RTOS_2))) > 1){
                        c((mword*)car(car(RTOS_2)),0) = car(car(car(RTOS_2))) - 1;
                        (mword*)code_ptr = (mword*)car(RTOS_0);
                    }
                    else{
                        discard = pop_rstack();
                        (mword*)code_ptr = (mword*)car(pop_rstack());
                        discard = pop_rstack();
                    }
                }
                else if(alloc_type(RTOS_0) == WHILEOP){
                    if(car(TOS_0) != 0){
                        zap();
                        (mword*)code_ptr = (mword*)car(RTOS_0);
                        push_alloc_rstack( (mword*)car(RTOS_2), 0);
                    }
                    else{
                        zap();
                        discard = pop_rstack();
                        (mword*)code_ptr = (mword*)car(pop_rstack());
                        discard = pop_rstack();
                    }
                }
                else{
                    (mword*)code_ptr = (mword*)car(pop_rstack());
                }
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
        tree_bbl2gv((mword*)global_VM);

    printf("}\n");

    clean_tree((mword*)global_VM);
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
            tree_bbl2gv((mword *)*(tree+i));
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
    clean_tree((mword*)TOS_0);
    char *buffer = malloc(initial_buf_size);

    mword buf_size=0;

    buf_size += sprintf(buffer+buf_size, "digraph babel {\nnode [shape=record];\n");

    buf_size += sprintf(buffer+buf_size, "graph [rankdir = \"LR\"];\n");
    
//        d(global_VM)
//        d(car(car(car(stack_ptr))))
//        die
        buf_size += _tree_bbl2gv((mword*)TOS_0, buffer+buf_size);

    buf_size += sprintf(buffer+buf_size, "}\n");
    //buf_size now contains the final string size of the entire graphviz string

    clean_tree((mword*)TOS_0);
    zap();

    mword last_mword = alignment_word8(buf_size);
    mword length = (buf_size / MWORD_SIZE) + 1;

    if(buf_size % MWORD_SIZE != 0){
        length++;
    }

//    d(char_length)
//    d(length)

    mword *result = _newlf(length);
    memcpy(result, buffer, buf_size);
    c(result,length-1) = last_mword;
    free(buffer);

    push_alloc(result,BBL2GV);

}


// Returns 
//
mword _tree_bbl2gv(mword *tree, char *buffer){

    int i;
    mword buf_size=0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    s(tree) |= 0x1;

    if(is_inte(tree)){

        buf_size += sprintf(buffer+buf_size, "\"s%08x\" [shape=record,label=\"", (mword)tree);
        for(i=0; i<num_elem; i++){
            if(i==0){
                if(tree == (mword*)code_list){
                    buf_size += sprintf(buffer+buf_size, "<f0> code_list");
                }
                else if(tree == (mword*)data_list){
                    buf_size += sprintf(buffer+buf_size, "<f0> data_list");
                }
                else if(tree == (mword*)stack_list){
                    buf_size += sprintf(buffer+buf_size, "<f0> stack_list");
                }
                else if(tree == (mword*)rstack_list){
                    buf_size += sprintf(buffer+buf_size, "<f0> rstack_list");
                }
                else if(tree == (mword*)jump_table){
                    buf_size += sprintf(buffer+buf_size, "<f0> jmp_table");
                }
                else if(tree == (mword*)sym_table){
                    buf_size += sprintf(buffer+buf_size, "<f0> sym_table");
                }
                else if(tree == (mword*)nada){
                    buf_size += sprintf(buffer+buf_size, "<f0> nada");
                }
                else if(tree == (mword*)nil){
                    buf_size += sprintf(buffer+buf_size, "<f0> nil");
                }
                else{
                    buf_size += sprintf(buffer+buf_size, "<f0> 0");
                }            
            }
            else{
                buf_size += sprintf(buffer+buf_size, "<f%d> %d", i, i);
            }
            if(i<(num_elem-1)){
                buf_size += sprintf(buffer+buf_size, "|");
            }
        }
        buf_size += sprintf(buffer+buf_size, "\"];\n");

        for(i=0; i<num_elem; i++){
            if(*(mword *)(tree+i) == nil && tree != (mword*)nada){
                continue;
            }
            buf_size += sprintf(buffer+buf_size, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)tree, i, *(mword *)(tree+i));
            buf_size += _tree_bbl2gv((mword *)*(tree+i), buffer+buf_size);
        }

    }
    else{ // is_leaf
        buf_size += sprintf(buffer+buf_size, "s%08x [style=bold,shape=record,label=\"", (mword)tree);
        for(i=0; i<num_elem; i++){
            buf_size += sprintf(buffer+buf_size, "<f%d> %x", i, *(mword *)(tree+i));
            if(i<(num_elem-1)){
                buf_size += sprintf(buffer+buf_size, "|");
            }
        }
        buf_size += sprintf(buffer+buf_size, "\"];\n");
    }

    return buf_size;

}


void bbl2str(void){

    // Figure out buffer size
    mword initial_buf_size = (16 * _mu((mword*)TOS_0));
    clean_tree((mword*)TOS_0);
    char *buffer = malloc(initial_buf_size);

    mword buf_size=0;

//    buf_size += sprintf(buffer+buf_size, "digraph babel {\nnode [shape=record];\n");

//    buf_size += sprintf(buffer+buf_size, "graph [rankdir = \"LR\"];\n");
    
//        d(global_VM)
//        d(car(car(car(stack_ptr))))
//        die
        buf_size += tree_bbl2str((mword*)TOS_0, buffer+buf_size);

//    buf_size += sprintf(buffer+buf_size, "}\n");
    //buf_size now contains the final string size of the entire graphviz string

    clean_tree((mword*)TOS_0);
    zap();

    mword last_mword = alignment_word8(buf_size);
    mword length = (buf_size / MWORD_SIZE) + 1;

    if(buf_size % MWORD_SIZE != 0){
        length++;
    }

//    d(char_length)
//    d(length)

    mword *result = _newlf(length);
    memcpy(result, buffer, buf_size);
    c(result,length-1) = last_mword;
    free(buffer);

    push_alloc(result,BBL2STR);

}


// Returns 
//
mword tree_bbl2str(mword *tree, char *buffer){

    int i;
    mword buf_size=0;

    if( s(tree) & (MWORD_SIZE-1) ){
        return 0;
    }

    int num_elem = size(tree);
//    count = num_elem + 1;

    s(tree) |= 0x1;

    if(is_inte(tree)){

//        buf_size += sprintf(buffer+buf_size, "\"s%08x\" [shape=record,label=\"", (mword)tree);
        buf_size += sprintf(buffer+buf_size, "[ ");

        for(i=0; i<num_elem; i++){
//            if(*(mword *)(tree+i) == nil && tree != (mword*)nada){
//                continue;
//            }
//            buf_size += sprintf(buffer+buf_size, "\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)tree, i, *(mword *)(tree+i));
            buf_size += tree_bbl2str((mword *)*(tree+i), buffer+buf_size);
        }

        buf_size += sprintf(buffer+buf_size, "] ");

    }
    else{ // is_leaf
//        buf_size += sprintf(buffer+buf_size, "s%08x [style=bold,shape=record,label=\"", (mword)tree);
        buf_size += sprintf(buffer+buf_size, "{ ");
        for(i=0; i<num_elem; i++){
            buf_size += sprintf(buffer+buf_size, "0x%x ", *(mword *)(tree+i));
//            if(i<(num_elem-1)){
//                buf_size += sprintf(buffer+buf_size, "|");
//            }
        }
        buf_size += sprintf(buffer+buf_size, "} ");
    }

    return buf_size;

}


// Clayton Bauman 2011

