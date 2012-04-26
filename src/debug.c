// XXX STACK FIX DONE
// debug.c
//
// Utilities for debugging

#include "babel.h"
#include "debug.h"
#include "bstruct.h"
#include "load.h"
#include "stack.h"
#include "bvm_opcodes.h"

////dump_obj
////
//void dump_obj(void){
//
////    mword *disp_VM = internal_global_VM-1;
////    int i;
////
////    for(i=0; i<global_machine_page_size; i++){
////        printf("%08x %08x\n", (mword)(disp_VM+i), disp_VM[i]);
////    }
////
//}
//
////void bbl2gv(void) {
////
////    printf("digraph babel {\nnode [shape=record];\n");
////    printf("graph [rankdir = \"LR\"];\n");
////    
//////        d(global_VM)
//////        d(car(car(car(stack_ptr))))
//////        die
////        dot((mword*)TOS_0, 0);
////
////    printf("}\n");
////
////    clean_tree((mword*)TOS_0);
////    zap();
////}
////
//
//void tree_dot(void) {
//
//    printf("digraph babel {\nnode [shape=record];\n");
//    printf("graph [rankdir = \"LR\"];\n");
//
//        dot(global_VM, 0);
//
//    printf("}\n");
//
//    clean_tree(global_VM);
//
//}
//
//// Returns 
////
//mword dot(mword *tree, int f_number){
//
//    int i;
////    mword count = 0;
//
//    if( s(tree) & (MWORD_SIZE-1) ){
//        return 0;
//    }
//
//    int num_elem = size(tree);
////    count = num_elem + 1;
//
//    s(tree) |= 0x1;
//
//    if(is_inte(tree)){
//
//        printf("\"s%08x\" [shape=record,label=\"", (mword)tree);
//        for(i=0; i<num_elem; i++){
//            if(i==0){
//                if(tree == (mword*)code_list){
//                    printf("<f0> code_list");
//                }
//                else if(tree == (mword*)data_list){
//                    printf("<f0> data_list");
//                }
//                else if(tree == (mword*)stack_list){
//                    printf("<f0> stack_list");
//                }
//                else if(tree == (mword*)rstack_list){
//                    printf("<f0> rstack_list");
//                }
//                else if(tree == (mword*)jump_table){
//                    printf("<f0> jmp_table");
//                }
//                else if(tree == (mword*)sym_table){
//                    printf("<f0> sym_table");
//                }
//                else if(tree == (mword*)nada){
//                    printf("<f0> nada");
//                }
//                else if(tree == (mword*)nil){
//                    printf("<f0> nil");
//                }
//                else{
//                    printf("<f0> 0");
//                }            
//            }
//            else{
//                printf("<f%d> %d", i, i);
//            }
//            if(i<(num_elem-1)){
//                printf("|");
//            }
//        }
//        printf("\"];\n");
//
//        for(i=0; i<num_elem; i++){
//            if(*(mword *)(tree+i) == nil && tree != (mword*)nada){
//                continue;
//            }
//            printf("\"s%08x\":f%d -> \"s%08x\":f0;\n", (mword)tree, i, *(mword *)(tree+i));
//            dot((mword *)*(tree+i), i);
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

