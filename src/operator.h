// operator.h
//

#include "operand.h"
#include "stack.h"
#include "list.h"
#include "bvm.h"
#include "mem.h"

#ifndef OPERATOR_H
#define OPERATOR_H

//Example:
//
//#define FOO_OPS bar+baz
//
//OPERATOR(foo,
//    OPERATOR_GET1_OPERANDS(nil, OI_MASK_INTE|OI_MASK_LEAF, 0, 0),
//    FOO_OPS,
//    OPERATOR_REMOVE1_OPERANDS,
//    OPERATOR_PUSH1_RESULTS)

// NOTE: oi0 is always TOS... so you can think of oiN as TOS-N
// NOTE: After name and ops, first four operands to OPERATOR() macro are 
//          TOS, next four are TOS-1, and so on

/*****************************************************************************
 *                                                                           *
 *                           OPERATOR DEFINES                                *
 *                                                                           *
 ****************************************************************************/

#define OPERATOR_SET_GC_PNR this_bvm->flags->MC_GC_PNR = FLAG_SET;
#define OPERATOR_CLR_GC_PNR this_bvm->flags->MC_GC_PNR = FLAG_CLR;
#define OPERATOR_PRE_OP_RESTART mem_preemptive_op_restart(this_bvm);

#define OPERATOR(name,gets,declares,ops,removes,pushes)                     \
    bvm_cache *name(bvm_cache *this_bvm){                                   \
        gets                                                                \
        declares                                                            \
        ops                                                                 \
        OPERATOR_SET_GC_PNR                                                 \
        removes                                                             \
        pushes                                                              \
        return this_bvm;                                                    \
    }

//The 'A' stands for "allocating"
#define OPERATORA(name,gets,declares,allocs,ops,writes,removes,pushes)      \
    bvm_cache *name(bvm_cache *this_bvm){                                   \
        gets                                                                \
        declares                                                            \
        allocs                                                              \
        ops                                                                 \
        writes                                                              \
        OPERATOR_SET_GC_PNR                                                 \
        removes                                                             \
        pushes                                                              \
        OPERATOR_CLR_GC_PNR                                                 \
        return this_bvm;                                                    \
    }

//The 'S' stands for "stack"
#define OPERATORS(name,gets,declares,ops,removes,pushes)                    \
    bvm_cache *name(bvm_cache *this_bvm){                                   \
        OPERATOR_PRE_OP_RESTART                                             \
        OPERATOR_SET_GC_PNR                                                 \
        gets                                                                \
        declares                                                            \
        ops                                                                 \
        removes                                                             \
        pushes                                                              \
        OPERATOR_CLR_GC_PNR                                                 \
        return this_bvm;                                                    \
    }

#define OPERATOR_EVAL(name,gets,declares,ops,removes,pushes)                \
    bvm_cache *name(bvm_cache *this_bvm){                                   \
        OPERATOR_PRE_OP_RESTART                                             \
        OPERATOR_SET_GC_PNR                                                 \
        gets                                                                \
        removes                                                             \
        declares                                                            \
        ops                                                                 \
        pushes                                                              \
        OPERATOR_CLR_GC_PNR                                                 \
        return this_bvm;                                                    \
    }

#define OINFO_DECLARE(num,oidef,oimask,oimin,oimax)                         \
        oinfo oi##num;                                                      \
        oi##num.default_data = oidef;                                       \
        oi##num.required_tag = nil;                                         \
        oi##num.mask = oimask;                                              \
        oi##num.min_size = oimin;                                           \
        oi##num.max_size = oimax;

#define OPERATOR_GET0_OPERANDS

#define OPERATOR_GET1_OPERANDS(def0,mask0,min0,max0)                        \
    OINFO_DECLARE(0,def0,mask0,min0,max0)                                   \
    get_operands(this_bvm,1,&oi0);                                          \

#define OPERATOR_GET2_OPERANDS( def0,mask0,min0,max0,                       \
                                def1,mask1,min1,max1)                       \
    OINFO_DECLARE(0,def0,mask0,min0,max0)                                   \
    OINFO_DECLARE(1,def1,mask1,min1,max1)                                   \
    get_operands(this_bvm,2,&oi0,&oi1);                                     \

#define OPERATOR_GET3_OPERANDS( def0,mask0,min0,max0,                       \
                                def1,mask1,min1,max1,                       \
                                def2,mask2,min2,max2)                       \
    OINFO_DECLARE(0,def0,mask0,min0,max0)                                   \
    OINFO_DECLARE(1,def1,mask1,min1,max1)                                   \
    OINFO_DECLARE(2,def2,mask2,min2,max2)                                   \
    get_operands(this_bvm,3,&oi0,&oi1,&oi2);                                \

#define OPERATOR_GET4_OPERANDS( def0,mask0,min0,max0,                       \
                                def1,mask1,min1,max1,                       \
                                def2,mask2,min2,max2,                       \
                                def3,mask3,min3,max3)                       \
    OINFO_DECLARE(0,def0,mask0,min0,max0)                                   \
    OINFO_DECLARE(1,def1,mask1,min1,max1)                                   \
    OINFO_DECLARE(2,def2,mask2,min2,max2)                                   \
    OINFO_DECLARE(3,def3,mask3,min3,max3)                                   \
    get_operands(this_bvm,4,&oi0,&oi1,&oi2,&oi3);

#define OPERATOR_GET5_OPERANDS( def0,mask0,min0,max0,                       \
                                def1,mask1,min1,max1,                       \
                                def2,mask2,min2,max2,                       \
                                def3,mask3,min3,max3,                       \
                                def4,mask4,min4,max4)                       \
    OINFO_DECLARE(0,def0,mask0,min0,max0)                                   \
    OINFO_DECLARE(1,def1,mask1,min1,max1)                                   \
    OINFO_DECLARE(2,def2,mask2,min2,max2)                                   \
    OINFO_DECLARE(3,def3,mask3,min3,max3)                                   \
    OINFO_DECLARE(4,def4,mask4,min4,max4)                                   \
    get_operands(this_bvm,5,&oi0,&oi1,&oi2,&oi3,&oi4);


#define OPERATOR_REMOVE0_OPERANDS

#define OPERATOR_REMOVE1_OPERANDS                                           \
        stack_pop(this_bvm,rci(this_bvm->dstack_ptr,0));

#define OPERATOR_REMOVE2_OPERANDS                                           \
        OPERATOR_REMOVE1_OPERANDS                                           \
        OPERATOR_REMOVE1_OPERANDS

#define OPERATOR_REMOVE3_OPERANDS                                           \
        OPERATOR_REMOVE2_OPERANDS                                           \
        OPERATOR_REMOVE1_OPERANDS

#define OPERATOR_REMOVE4_OPERANDS                                           \
        OPERATOR_REMOVE3_OPERANDS                                           \
        OPERATOR_REMOVE1_OPERANDS

#define OPERATOR_REMOVE5_OPERANDS                                           \
        OPERATOR_REMOVE4_OPERANDS                                           \
        OPERATOR_REMOVE1_OPERANDS

#define OPERATOR_PUSH_RESULT(num)                                           \
    dstack_push(this_bvm, \
            stack_new_entry(                                                \
                this_bvm,                                                   \
                result##num,                                                \
                nil));

//#define OPERATOR_PUSH_RESULT(num)                                           
//    stack_push(this_bvm,                                                    
//            rci(this_bvm->dstack_ptr,0),                                    
//            stack_new_entry(this_bvm,                                       
//                result##num,                                                
//                nil));

#define OPERATORA_PUSH_RESULT(num) dstack_push(this_bvm, stack_result##num);


#define OPERATOR_ALLOC0_RESULTS

#define OPERATOR_ALLOC1_RESULTS                                             \
    mword *stack_result0 = stack_new_entry(this_bvm, nil, nil);

#define OPERATOR_ALLOC2_RESULTS                                             \
    OPERATOR_ALLOC1_RESULTS                                                 \
    mword *stack_result1 = stack_new_entry(this_bvm, nil, nil);

#define OPERATOR_ALLOC3_RESULTS                                             \
    OPERATOR_ALLOC2_RESULTS                                                 \
    mword *stack_result2 = stack_new_entry(this_bvm, nil, nil);


#define OPERATOR_WRITE_STACK_ENTRY(num) lci(lci(lci(stack_result##num, 0), 1), 0) = result##num;

#define OPERATOR_WRITE0_RESULTS

#define OPERATOR_WRITE1_RESULTS                                             \
    OPERATOR_WRITE_STACK_ENTRY(0)

#define OPERATOR_WRITE2_RESULTS                                             \
    OPERATOR_WRITE1_RESULTS                                                 \
    OPERATOR_WRITE_STACK_ENTRY(1)

#define OPERATOR_WRITE3_RESULTS                                             \
    OPERATOR_WRITE2_RESULTS                                                 \
    OPERATOR_WRITE_STACK_ENTRY(2)


#define OPERATOR_DECLARE0_RESULTS

#define OPERATOR_DECLARE1_RESULTS                                           \
    mword *result0;

#define OPERATOR_DECLARE2_RESULTS                                           \
    OPERATOR_DECLARE1_RESULTS                                               \
    mword *result1;

#define OPERATOR_DECLARE3_RESULTS                                           \
    OPERATOR_DECLARE2_RESULTS                                               \
    mword *result2;


#define OPERATOR_PUSH0_RESULTS

#define OPERATOR_PUSH1_RESULTS                                              \
    OPERATOR_PUSH_RESULT(0)

#define OPERATOR_PUSH2_RESULTS                                              \
    OPERATOR_PUSH1_RESULTS                                                  \
    OPERATOR_PUSH_RESULT(1)

#define OPERATOR_PUSH3_RESULTS                                              \
    OPERATOR_PUSH2_RESULTS                                                  \
    OPERATOR_PUSH_RESULT(2)


#define OPERATORA_PUSH0_RESULTS

#define OPERATORA_PUSH1_RESULTS                                              \
    OPERATORA_PUSH_RESULT(0)

#define OPERATORA_PUSH2_RESULTS                                              \
    OPERATORA_PUSH1_RESULTS                                                  \
    OPERATORA_PUSH_RESULT(1)



/*****************************************************************************
 *                                                                           *
 *                            OPERATOR TYPES                                 *
 *                                                                           *
 ****************************************************************************/


#define OPERATOR_R1_W0_8D(name,ops,def0,mask0,min0,max0)                    \
    OPERATOR(name,                                                          \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)


#define OPERATOR_R1_W0_D(name,ops,def0,mask0,min0,max0)                     \
    OPERATOR(name,                                                          \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)


#define OPERATOR_R1_W0_P(name,ops,def0,mask0,min0,max0)                     \
    OPERATOR(name,                                                          \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)


#define OPERATOR_R2_W1_UOD(name,ops,                                        \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)


#define OPERATOR_R2_W1_IOD(name,ops,                                        \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)


#define OPERATOR_R2_W1_D(name,ops,                                          \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)


#define OPERATOR_R2_W2_D(name,ops,                                          \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE2_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATOR_PUSH2_RESULTS)


#define OPERATOR_R1_W1_ID(name, ops,                                        \
    def0,mask0,min0,max0)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)

#define OPERATOR_R0_W0(name,ops) \
    OPERATOR(name,                                                          \
    OPERATOR_GET0_OPERANDS, \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)


#define OPERATOR_R0_W1(name,ops) \
    OPERATOR(name,                                                          \
    OPERATOR_GET0_OPERANDS, \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)


#define OPERATOR_EVAL_R2_W0_D(name,ops,                                     \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATOR_EVAL(name,                                                     \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)


#define OPERATOR_R2_W0_D(name,ops,                                          \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)


#define OPERATOR_R2_W0_P(name,ops,                                          \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)


#define OPERATOR_R1_W1_8D(name, ops, def0, mask0, min0, max0)               \
        OPERATOR_R1_W1_D( name, ops, def0, mask0, min0, max0)


#define OPERATOR_R1_W1_MD(name, ops, def0, mask0, min0, max0)               \
        OPERATOR_R1_W1_D( name, ops, def0, mask0, min0, max0)


#define OPERATOR_R1_W1_D(name, ops,                                         \
    def0,mask0,min0,max0)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)


#define OPERATOR_R1_W1_P(name, ops,                                         \
    def0,mask0,min0,max0)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)


#define OPERATOR_R1_W2_D(name, ops,                                         \
    def0,mask0,min0,max0)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE2_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATOR_PUSH2_RESULTS)


#define OPERATOR_R3_W0_D(name,ops,                                          \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1,                                                   \
    def2,mask2,min2,max2)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET3_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1,                        \
                            def2, mask2, min2, max2),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE3_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)

#define OPERATOR_R3_W1_D(name,ops,                                          \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1,                                                   \
    def2,mask2,min2,max2)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET3_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1,                        \
                            def2, mask2, min2, max2),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE3_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)

#define OPERATOR_R4_W0_D(name,ops,                                          \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1,                                                   \
    def2,mask2,min2,max2,                                                   \
    def3,mask3,min3,max3)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET4_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1,                        \
                            def2, mask2, min2, max2,                        \
                            def3, mask3, min3, max3),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE4_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)

#define OPERATOR_R5_W0_D(name,ops,                                          \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1,                                                   \
    def2,mask2,min2,max2,                                                   \
    def3,mask3,min3,max3,                                                   \
    def4,mask4,min4,max4)                                                   \
    OPERATOR(name,                                                          \
    OPERATOR_GET5_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1,                        \
                            def2, mask2, min2, max2,                        \
                            def3, mask3, min3, max3,                        \
                            def4, mask4, min4, max4),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE5_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)


/*****************************************************************************
 *                                                                           *
 *                       ALLOCATING OPERATOR TYPES                           *
 *                                                                           *
 ****************************************************************************/

#define OPERATORA_R1_W1_D(name, ops,                                        \
    def0,mask0,min0,max0)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATORA_PUSH1_RESULTS)

#define OPERATORA_R2_W1_FOD(name,ops,                                       \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATORA_PUSH1_RESULTS)

#define OPERATORA_R2_W1_UOD(name,ops,                                       \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATORA_PUSH1_RESULTS)

#define OPERATORA_R2_W1_IOD(name,ops,                                       \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATORA_PUSH1_RESULTS)

#define OPERATORA_R2_W1_D(name,ops,                                         \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATORA_PUSH1_RESULTS)


#define OPERATORA_R1_W0_D(name,ops,def0,mask0,min0,max0)                    \
    OPERATORA(name,                                                         \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE0_RESULTS,                                              \
    OPERATOR_ALLOC0_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE0_RESULTS,                                                \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATORA_PUSH0_RESULTS)


#define OPERATORA_R4_W0_D(name,ops,                                         \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1,                                                   \
    def2,mask2,min2,max2,                                                   \
    def3,mask3,min3,max3)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET4_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1,                        \
                            def2, mask2, min2, max2,                        \
                            def3, mask3, min3, max3),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    OPERATOR_ALLOC0_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE0_RESULTS,                                                \
    OPERATOR_REMOVE4_OPERANDS,                                              \
    OPERATORA_PUSH0_RESULTS)

#define OPERATORA_R2_W0_D(name,ops,                                         \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    OPERATOR_ALLOC0_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE0_RESULTS,                                                \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATORA_PUSH0_RESULTS)

#define OPERATORA_R0_W1(name,ops)                                           \
    OPERATORA(name,                                                         \
    OPERATOR_GET0_OPERANDS,                                                 \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)

#define OPERATORA_R1_W1_P(name, ops,                                        \
    def0,mask0,min0,max0)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)

#define OPERATORA_R3_W0_D(name,ops,                                         \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1,                                                   \
    def2,mask2,min2,max2)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET3_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1,                        \
                            def2, mask2, min2, max2),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    OPERATOR_ALLOC0_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE0_RESULTS,                                                \
    OPERATOR_REMOVE3_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)

#define OPERATORA_R1_W2_D(name, ops,                                        \
    def0,mask0,min0,max0)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE2_RESULTS,                                              \
    OPERATOR_ALLOC2_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE2_RESULTS,                                                \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATOR_PUSH2_RESULTS)

#define OPERATORA_R3_W1_D(name,ops,                                         \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1,                                                   \
    def2,mask2,min2,max2)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET3_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1,                        \
                            def2, mask2, min2, max2),                       \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE3_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)

#define OPERATORA_R2_W2_D(name,ops,                                         \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET2_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1),                       \
    OPERATOR_DECLARE2_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE2_OPERANDS,                                              \
    OPERATOR_PUSH2_RESULTS)

#define OPERATORS_R0_W0(name,ops) \
    OPERATORS(name,                                                         \
    OPERATOR_GET0_OPERANDS,                                                 \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)

#define OPERATORS_R1_W1_P(name, ops,                                        \
    def0,mask0,min0,max0)                                                   \
    OPERATORS(name,                                                         \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE1_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)

#define OPERATORS_R1_W0_P(name,ops,def0,mask0,min0,max0)                    \
    OPERATORS(name,                                                         \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE0_RESULTS,                                              \
    ops,                                                                    \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)

#define OPERATORA_R5_W0_D(name,ops,                                         \
    def0,mask0,min0,max0,                                                   \
    def1,mask1,min1,max1,                                                   \
    def2,mask2,min2,max2,                                                   \
    def3,mask3,min3,max3,                                                   \
    def4,mask4,min4,max4)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET5_OPERANDS( def0, mask0, min0, max0,                        \
                            def1, mask1, min1, max1,                        \
                            def2, mask2, min2, max2,                        \
                            def3, mask3, min3, max3,                        \
                            def4, mask4, min4, max4),                       \
    OPERATOR_DECLARE0_RESULTS,                                              \
    OPERATOR_ALLOC0_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE0_RESULTS,                                                \
    OPERATOR_REMOVE5_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)

#define OPERATORA_R1_W0_P(name,ops,def0,mask0,min0,max0)                    \
    OPERATORA(name,                                                         \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE0_RESULTS,                                              \
    OPERATOR_ALLOC0_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE0_RESULTS,                                                \
    OPERATOR_REMOVE0_OPERANDS,                                              \
    OPERATOR_PUSH0_RESULTS)

#define OPERATORA_R1_W1_ID(name, ops,                                       \
    def0,mask0,min0,max0)                                                   \
    OPERATORA(name,                                                         \
    OPERATOR_GET1_OPERANDS(def0, mask0, min0, max0),                        \
    OPERATOR_DECLARE1_RESULTS,                                              \
    OPERATOR_ALLOC1_RESULTS,                                                \
    ops,                                                                    \
    OPERATOR_WRITE1_RESULTS,                                                \
    OPERATOR_REMOVE1_OPERANDS,                                              \
    OPERATOR_PUSH1_RESULTS)


#endif //OPERATOR_H

// Clayton Bauman 2014

