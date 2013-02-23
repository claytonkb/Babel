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

typedef unsigned mword; // mword#
typedef signed   smword;

typedef struct {

    mword *code_ptr;
    mword *rstack_ptr;

    mword *dstack_ptr;
    mword *ustack_ptr;

    mword *jump_table;
    mword *sym_table;
    mword *self;

    mword *thread_id;
    mword *argv;
    mword *steps;
    mword *advance_type;

} bvm_cache;

typedef bvm_cache *(*babel_op)(bvm_cache *); // bvm_cache#

//bvm_cache *interp_init(int argc, char **argv);
bvm_cache *interp_init(bvm_cache *root_bvm, int argc, char **argv);
void init_interp_jump_table(bvm_cache *this_bvm);
bvm_cache *endian(bvm_cache *this_bvm);
void print_env(void);
void dump_mem(mword *mem, int size);
void init_nil(void);
void temp_rbs2gv(mword *bs);
void init_tags(void);

#define BIG_ENDIAN    0
#define LITTLE_ENDIAN 1

// GLOBALS
mword *nil; // nil#
mword *empty_string;

//mword*      internal_global_VM; //Interpreter-visible machine pointer
//mword*      global_VM;          //Machine pointer
////mword       global_machine_page_size;
//jmp_buf     exception_env;
//int         exception_type;

// CONSTANTS
#define MWORD_SIZE sizeof(mword) // MWORD_SIZE#
#define MWORD_BIT_SIZE (MWORD_SIZE << 3) // MWORD_BIT_SIZE#
#define MWORDS(x) ((x)*MWORD_SIZE) // MWORDS#

// The control field consists of the LSBs of the s-field
// The LSB of the s-field is currently defined for use in 
// bstruct-traversal


#define CTL_MASK (MWORD_SIZE-1) // CTL_MASK#
#define STRLEN(s) (sizeof(s)-1) // STRLEN#
#define C2B(x)    (_c2b(x, STRLEN(x))) // C2B#

#define HASH_BIT_SIZE 128 // HASH_BIT_SIZE#
#define HASH_SIZE (HASH_BIT_SIZE/MWORD_BIT_SIZE) // HASH_SIZE#

// IA32 uses 4k pages and multiples...
#define ALLOC_PAGE_SIZE 4096

#define BITS_PER_BYTE 8

#define ROOT_INTERP_THREAD 0

// This includes many reserved opcodes
#define NUM_INTERP_OPCODES 533

// Operating-system compatibility
#define WINDOWS
//#define STAR_NIX

// XXX This should be set to 3 for 64-bit compilation...
// I can't find a simple/direct way to connect this to MWORD_SIZE
// so it has to be defined independently
#define LOG_MWORD_SIZE 2

// UTILITIES
#define s(x)          (*((mword*)x-1)) // s#

#define is_leaf(x)    ((int)s((mword*)x) >  0) // is_leaf#
#define is_inte(x)    ((int)s((mword*)x) <  0) // is_inte#
#define is_tlist(x)   ((int)s((mword*)x) == 0) // is_tlist#

#define is_conslike(x) (is_inte(x) && size(x) == 2) // is_conslike#

#define size(x)      (abs(s(x))/MWORD_SIZE) // size#
#define c(x,y)       (*((mword*)x + y)) // c#

#define tagcmp(x,y) ( (is_tlist(x) || (size(x) >= HASH_SIZE)) ? (memcmp((mword*)x, y, HASH_SIZE*MWORD_SIZE)) : -1 ) // tagcmp#
#define tageq(x,y)  ( tagcmp(x,y) == 0 ) // tageq#
#define is_nil(x)   ( tageq(x,nil) ) // is_nil#

// XXX UNSAFE; use this only where we know for sure we have a tlist...
#define is_nil_fast(x) ( (memcmp(x, nil, HASH_SIZE*MWORD_SIZE)) == 0 ) // is_nil_fast#

// XXX UNSAFE; use this only where we know for sure we are 
// working with interprer-allocated mem (fastest is_nil)
#define is_nil_interp(x) ( (mword*)x == nil ) // is_nil_interp#

#include "tags.h"

#define is_bvm(x) ( tagcmp((x),BABEL_TAG_BVM) == 0 ) // is_bvm#
#define is_ref(x) ( tagcmp((x),BABEL_TAG_REF) == 0 ) // is_ref#

//#define car(x)      c((mword*)(x),0) // 
//#define cdr(x)      c((mword*)(x),1) // 
//
//#define _tcar(x)     c((mword*)x,HASH_SIZE+1) // 
//#define _tcdr(x)     c((mword*)x,HASH_SIZE+2) // 
//
////tlist-safe car/cdr:
//#define tcar(x)     (is_tlist(x) ? _tcar(x) : car(x)) // 
//#define tcdr(x)     (is_tlist(x) ? _tcdr(x) : cdr(x)) // 
//
////nil-safe car/cdr:
#define scar(x)     (is_nil(x) ? nil : car(x)) // scar#
#define scdr(x)     (is_nil(x) ? nil : cdr(x)) // scdr#


//car/cdr rework:
//internal car/cdr (not list-safe or tlist-safe):
#define icar(x)     c((mword*)(x),0) // icar#
#define icdr(x)     c((mword*)(x),1) // icdr#

//tlist-safe car/cdr:
#define tcar(x)     c((mword*)x,HASH_SIZE+1) // tcar#
#define tcdr(x)     c((mword*)x,HASH_SIZE+2) // tcdr#

//list-safe car/cdr (not tlist safe):
#define lcar(x)     (is_nil(x) ? nil : icar(x)) // lcar#
#define lcdr(x)     (is_nil(x) ? nil : icdr(x)) // lcdr#

//General-purpose car/cdr:
#define car(x)      (is_tlist(x) ? tcar(x) : icar(x)) // car#
#define cdr(x)      (is_tlist(x) ? tcdr(x) : icdr(x)) // cdr#

//General-purpose cxr:
#define tcxr(x,y)     c((mword*)x,HASH_SIZE+1+(y%2)) // txcr#
#define  cxr(x,y)     (is_tlist(x) ? tcxr(x,y) : c((mword*)(x),y)) // cxr#

#define is_false(x) (    is_leaf(x) && icar(x) == 0 ||  is_nil(car(x)) ) // is_false#
//                     || !is_leaf(x) && is_nil(car(x)) )

#define get_sym(x,y)   ( _luha( (mword*)car(x->sym_table), _hash8(C2B(y))) )   // get_sym#
#define set_sym(x,y,z) hash_insert( x->sym_table, (y), (z) ) // set_sym#

#define pushd(x,y,z) push_udr_stack(x->dstack_ptr, new_dstack_entry(y,z)) // pushd#
#define popd(x) pop_udr_stack(x->dstack_ptr) // popd#

#define pushu(x,y,z) push_udr_stack(x->ustack_ptr, new_dstack_entry(y,z)) // pushu#
#define popu(x) pop_udr_stack(x->ustack_ptr) // popu#

#define pushr(x,y,z) push_udr_stack(x->rstack_ptr, new_rstack_entry(y,z)) // pushr#
#define popr(x) pop_udr_stack(x->rstack_ptr) // popr#

#define mkref(x,y) ( new_tlist( \
                        _hash8(C2B("/babel/tag/ref")), \
                        consa( _hash8(C2B(x)), \
                            consa( y, nil ) ) ) )

// non-allocating cons
// XXX: Doesn't work with tlists...
#define cons(a,b,c) icar(a) = (mword)(b); icdr(a) = (mword)(c); // cons#

#define new_cons (_newin(2)) // new_cons#
#define new_atom (_newlfi(1,0)) // new_atom#

// Stack
//#define TOS_0(x)             car(car(x->dstack_ptr))
//#define TOS_1(x)         car(car(cdr(x->dstack_ptr)))
//#define TOS_2(x)     car(car(cdr(cdr(x->dstack_ptr))))

#define TOS_0(x)             (mword*)icar(icar(icar(x->dstack_ptr)))
#define TOS_1(x)         (mword*)icar(icar(icar(icdr(x->dstack_ptr))))
#define TOS_2(x)     (mword*)icar(icar(icar(icdr(icdr(x->dstack_ptr)))))

#define RTOS_0(x)            icar(icar(x->rstack_ptr))

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
#define d(x) printf("%s %08x\n", QUOTEME(x), x);  // d#
#define _dump(x) printf("%s\n", _bs2gv(x));  // _dump#
#define die      fprintf(stderr, "Died at %s line %d\n", __FILE__, __LINE__); exit(DIE_CODE);  // die#
#define warn(x)  fprintf(stderr, "WARNING: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__);  // warn#
#define error(x) fprintf(stderr, "ERROR: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__); // error#
#define trace    fprintf(stderr, "%s in %s line %d\n", __func__, __FILE__, __LINE__);   // trace#
#define fatal(x) fprintf(stderr, "FATAL: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__); die;  // fatal#
#define enhance(x) fprintf(stderr, "ENHANCEMENT: %s in %s at %s line %d\n", x, __func__, __FILE__, __LINE__); // enhance#

#endif //BABEL_H

// Clayton Bauman 2011


