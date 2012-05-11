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
    mword *alloc_atom;
    mword *alloc_cons;
    mword *alloc_hash;
    mword *alloc_stack;
    mword *alloc_rstack;
    mword *alloc_unmanaged;
} alloc;

typedef struct {
    mword *hidden;
    mword *sym_table;
    mword *code_ptr;
    mword *stack_ptr;
    mword *rstack_ptr;
    mword *jump_table;
    mword thread_id;
    mword *argv;
    mword steps;
    mword advance_type;
    alloc machine;
} bvm_cache;

typedef bvm_cache *(*babel_op)(bvm_cache *);

mword*      nil;

bvm_cache *interp_init(bvm_cache *root_bvm, int argc, char **argv);
void init_interp_jump_table(bvm_cache *this_bvm);

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

#define NUM_INTERP_OPCODES 521

#define WINDOWS
//#define STAR_NIX

// XXX This should be set to 3 for 64-bit compilation...
// I can't find a simple/direct way to connect this to MWORD_SIZE
// so it has to be defined independently
#define LOG_MWORD_SIZE 2

// UTILITIES
#define s(x)         (*((mword*)x-1))
#define is_leaf(x)   ((int)s((mword*)x) >  0)
#define is_inte(x)   ((int)s((mword*)x) <  0)
#define is_href(x)   ((int)s((mword*)x) == 0)

#define size(x)      (is_href(x)?HASH_SIZE:(abs(s(x))/MWORD_SIZE))
#define c(x,y)       (*(y + x))

#define is_nil(x)   ( is_href(x) ? (memcmp((x), nil, HASH_SIZE) == 0) : 0 )

#define is_false(x) (    is_leaf(x) && car(x) == 0 \
                     || !is_leaf(x) && is_nil(scar(x)) )

#define car(x)      c((mword*)x,0)
#define cdr(x)      c((mword*)x,1)

//nil-safe car/cdr:
#define scar(x)     (is_nil(x) ? nil : car(x))
#define scdr(x)     (is_nil(x) ? nil : cdr(x))

#define cons(a,b,c) car(a) = (mword)(b); cdr(a) = (mword)(c);

#define new_cons (_newin(2))
#define new_atom (_newlf(1))

//XXX we may yet need a generally hash-ref-safe car/cdr

#define TOS_0(x)             car(car(x->stack_ptr))
#define TOS_1(x)         car(car(cdr(x->stack_ptr)))
#define TOS_2(x)     car(car(cdr(cdr(x->stack_ptr))))

#define RTOS_0(x)            car(car(x->rstack_ptr))

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
#define trace fprintf(stderr, "%s in %s line %d\n", __FILE__, __LINE__);

#endif //BABEL_H

// Clayton Bauman 2011

