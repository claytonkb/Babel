// babel.h
// Every .c file in the Babel project should include this file

#ifndef BABEL_H
#define BABEL_H

#define BABEL_VERSION ("0.4")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <setjmp.h>

// TYPEDEFS
typedef void(*std_fn_ptr)(void);
typedef unsigned mword;
typedef signed   smword;

typedef struct {
    mword *self;
    mword *code_ptr;
    mword *stack_ptr;
    mword *ustack_ptr;
    mword *rstack_ptr;
    mword *jump_table;
    mword *sym_table;
    mword *thread_id;
    mword *argv;
    mword *steps;
    mword *advance_type;
} bvm_cache;

typedef bvm_cache *(*babel_op)(bvm_cache *);

mword *nil;
mword *empty_string;

//bvm_cache *interp_init(int argc, char **argv);
bvm_cache *interp_init(bvm_cache *root_bvm, int argc, char **argv);
void init_interp_jump_table(bvm_cache *this_bvm);
bvm_cache *endian(bvm_cache *this_bvm);
void print_env(void);
void dump_mem(mword *mem, int size);
void init_nil(void);
void temp_rbs2gv(mword *bs);

#define BIG_ENDIAN    0
#define LITTLE_ENDIAN 1

// GLOBALS
//mword*      internal_global_VM; //Interpreter-visible machine pointer
//mword*      global_VM;          //Machine pointer
////mword       global_machine_page_size;
//jmp_buf     exception_env;
//int         exception_type;

// CONSTANTS
#define MWORD_SIZE sizeof(mword)
#define MWORD_BIT_SIZE (MWORD_SIZE << 3)
#define MWORDS(x) ((x)*MWORD_SIZE)

// The control field consists of the LSBs of the s-field
// The LSB of the s-field is currently defined for use in 
// bstruct-traversal
#define CTL_MASK (MWORD_SIZE-1)

#define STRLEN(s) (sizeof(s)-1)
#define C2B(x)    (_c2b(x, STRLEN(x)))

#define HASH_BIT_SIZE 128
#define HASH_SIZE (HASH_BIT_SIZE/MWORD_BIT_SIZE)

// IA32 uses 4k pages and multiples...
#define ALLOC_PAGE_SIZE 4096

#define BITS_PER_BYTE 8

#define ROOT_INTERP_THREAD 0

// This includes many reserved opcodes
#define NUM_INTERP_OPCODES 533

#define WINDOWS
//#define STAR_NIX

// XXX This should be set to 3 for 64-bit compilation...
// I can't find a simple/direct way to connect this to MWORD_SIZE
// so it has to be defined independently
#define LOG_MWORD_SIZE 2

// UTILITIES
#define s(x)          (*((mword*)x-1))
#define is_leaf(x)    ((int)s((mword*)x) >  0)
#define is_inte(x)    ((int)s((mword*)x) <  0)
#define is_tlist(x)   ((int)s((mword*)x) == 0)

//#define is_href(x)   ((int)s((mword*)x) == 0)

//#define size(x)      (is_href(x)?HASH_SIZE:(abs(s(x))/MWORD_SIZE))
#define size(x)      (abs(s(x))/MWORD_SIZE)
#define c(x,y)       (*(y + x))

//#define is_nil(x)   ( is_href(x) ? (memcmp((x), nil, HASH_SIZE) == 0) : 0 )
//#define is_nil(x)   ( (mword)(x) == (mword)nil )

#define eqtag(x,y) (memcmp(x, y, HASH_SIZE*MWORD_SIZE))
#define is_nil(x) ( eqtag(x,nil) == 0 )

#define is_false(x) (    is_leaf(x) && car(x) == 0 \
                     || !is_leaf(x) && is_nil(scar(x)) )

#define car(x)      c((mword*)(x),0)
#define cdr(x)      c((mword*)(x),1)

#define ttag(x,y)   c((mword*)x,y)
#define tcar(x)     c((mword*)x,HASH_SIZE)
#define tcdr(x)     c((mword*)x,HASH_SIZE+1)

//nil-safe car/cdr:
#define scar(x)     (is_nil(x) ? nil : car(x))
#define scdr(x)     (is_nil(x) ? nil : cdr(x))

#define cons(a,b,c) car(a) = (mword)(b); cdr(a) = (mword)(c);

#define new_cons (_newin(2))
#define new_atom (_newlf(1))

// Stack
//#define TOS_0(x)             car(car(x->stack_ptr))
//#define TOS_1(x)         car(car(cdr(x->stack_ptr)))
//#define TOS_2(x)     car(car(cdr(cdr(x->stack_ptr))))

#define TOS_0(x)             (mword*)car(car(car(x->stack_ptr)))
#define TOS_1(x)         (mword*)car(car(car(cdr(x->stack_ptr))))
#define TOS_2(x)     (mword*)car(car(car(cdr(cdr(x->stack_ptr)))))

#define RTOS_0(x)            car(car(x->rstack_ptr))

#define STACK_ENTRY_SIZE 2

#define STACK_ENTRY_VAL(x) ((mword*)c(x,0))
#define STACK_ENTRY_LIF(x) ((mword*)c(x,1))

#define IMMORTAL 0
#define MORTAL   1
#define is_mortal(x) ( STACK_ENTRY_LIF(x) != IMMORTAL )

#define REQUIRE_LEAF_LEAF(x) \
    if(!is_leaf(TOS_0(x)) || !is_leaf(TOS_1(x))) { \
        error("One or more operands not a leaf-array"); \
        die \
    }

#define REQUIRE_LEAF(x) \
    if(!is_leaf(TOS_0(x))) { \
        error("Top-of-stack not a leaf-array"); \
        die \
    }

#define alloc_type(x) car(car(cdr(x))) 
#define return_type(x) car(cdr(car(x))) 

// BVM
#define BVM_ADVANCE  0
#define BVM_CONTINUE 1
#define BVM_RETURN   2
#define BVM_NEXT     3

// DEBUG
#define DIE_CODE 1
#define QUOTEME(x) #x
#define d(x) printf("%s %08x\n", QUOTEME(x), x);
#define _dump(x) printf("%s\n", _bs2gv(x));
#define die  fprintf(stderr, "Died at %s line %d\n", __FILE__, __LINE__); exit(DIE_CODE);
#define warn(x) fprintf(stderr, "WARNING: %s at %s line %d\n", x, __FILE__, __LINE__);
#define error(x) fprintf(stderr, "ERROR: %s in %s at %s line %d\n", x, __func__, __FILE__, __LINE__);
#define trace fprintf(stderr, "%s in %s line %d\n", __func__, __FILE__, __LINE__);
#define fatal(x) fprintf(stderr, "ERROR: %s in %s at %s line %d\n", x, __func__, __FILE__, __LINE__); die;

#endif //BABEL_H

// Clayton Bauman 2011

