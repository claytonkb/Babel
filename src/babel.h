// babel.h
//
// Every .c file in the Babel project should include this file

#ifndef BABEL_H
#define BABEL_H

#define BABEL_VERSION ("0.5")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <setjmp.h>

// CONSTANTS
#define MWORD_SIZE sizeof(mword) // MWORD_SIZE#
#define MWORD_BIT_SIZE (MWORD_SIZE << 3) // MWORD_BIT_SIZE#
#define MWORDS(x) ((x)*MWORD_SIZE) // MWORDS#

// The control field consists of the LSBs of the s-field
// The LSB of the s-field is currently defined for use in 
// bstruct-traversal

#define CTL_MASK (MWORD_SIZE-1) // CTL_MASK#
#define STRLEN(s) (sizeof(s)-1) // STRLEN#
#define C2B(x)    (_c2b(this_bvm, x, STRLEN(x))) // C2B#
#define OVERRUN_LIMIT (1<<16)

#define HASH_BIT_SIZE 128 // HASH_BIT_SIZE#
#define HASH_SIZE (HASH_BIT_SIZE/MWORD_BIT_SIZE) // HASH_SIZE#

#define TAG_SIZE (HASH_SIZE*MWORD_SIZE)
#define MIN_TAG_SIZE TAG_SIZE

#define TPTR_SIZE HASH_SIZE+3 // TPTR_SIZE#
                         // 3 = s-field + s-field + car

// IA32 uses 4k pages and multiples...
#define ALLOC_PAGE_SIZE 4096

#define BITS_PER_BYTE 8

#define ROOT_INTERP_THREAD 0

// This includes many reserved opcodes
#define NUM_INTERP_OPCODES 554

#define FLAG_IGN    ((mword)-1)
#define FLAG_SET    1
#define FLAG_CLR    0

// TYPEDEFS
typedef void(*std_fn_ptr)(void);

typedef unsigned mword; // mword#
typedef signed   smword;


typedef struct { // alloc_bank#

    mword *base_ptr;
    mword *alloc_ptr;

} alloc_bank; 


typedef struct { // mem_context#

    alloc_bank *primary;
    alloc_bank *secondary;

} mem_context;


typedef struct { // bvm_flags#

    mword MC_ALLOC_BLOCKING;
    mword MC_GC_BLOCKING;
    mword MC_GC_PENDING;
    mword BVM_INSTR_IN_PROGRESS;
    mword BVM_INCOHERENT;
    mword BVM_CACHE_VALID;
    mword BVM_CACHE_DIRTY;
    mword BVM_CLEAN;
    mword NO_ASYNC;
    mword NO_EXCEPT;

} bvm_flags;


typedef struct { // bvm_cache#

    mword *code_ptr;
    mword *rstack_ptr;

    mword *dstack_ptr;
    mword *ustack_ptr;

    mword *jump_table;
    mword *sym_table;
    mword *self;

    mword *thread_id;
    mword *steps;    
    mword *advance_type;

    //interp ONLY (no save/restore):
    mem_context *mem;
    bvm_flags   *flags;

} bvm_cache;


typedef bvm_cache *(*babel_op)(bvm_cache *); // babel_op#


//bvm_cache *interp_init(int argc, char **argv);
//bvm_cache *interp_init(bvm_cache *root_bvm, int argc, char **argv);
//void init_interp_jump_table(this_bvm, bvm_cache *this_bvm);
//void print_env(char **envp);
void dump_mem(mword *mem, int size);
void temp_rbs2gv(mword *bs);

//MEMORY

#define bfree(x)  free((mword*)(x)-1)

// Memory size is 64MB
#define MEM_SIZE (1<<26)          // MEM_SIZE#

//#define ALLOC_ENTRY_IN_USE 1    // ALLOC_ENTRY_IN_USE#
//#define ALLOC_ENTRY_FREE   0    // ALLOC_ENTRY_FREE#

#define r(x)          (*((mword*)x-2)) // r#

#define mc_alloc_size(x) (x == 0 ? TPTR_SIZE : (abs(x)/MWORD_SIZE))

#define TOP_OF_ALLOC_BANK(x) (x->base_ptr+(MEM_SIZE>>1)-1)

#define newleaf(x) (mc_alloc(this_bvm,  MWORDS(x) ))
#define newinte(x) (mc_alloc(this_bvm,  MWORDS(-1*x) ))
#define newtptr(x) (mc_alloc(this_bvm,  0 ))

#define BIG_ENDIAN    0
#define LITTLE_ENDIAN 1

// GLOBALS
mword *nil; // nil#
mword *empty_string;
mword null_hash[HASH_SIZE];// = { 0x88e9045b, 0x0b7c30af, 0x831422c3, 0x01ab0dc1, };

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
#define is_tptr(x)    ((int)s((mword*)x) == 0) // is_tptr#

#define is_conslike(x) (is_inte(x) && size(x) == 2) // is_conslike#

//FIXME: alloc_size and size are non-optimized... use inline fn?
#define size(x)      (abs(s(x))/MWORD_SIZE) // size#
#define c(x,y)       (*((mword*)x + y)) // c#

#define alloc_size(x) (s(x) == 0 ? TPTR_SIZE : size(x)+1) // alloc_size#

#define tagcmp(x,y,z) ( (is_tptr(x) || (size(x) >= HASH_SIZE)) ? (memcmp((mword*)x, y, z)) : -1 ) // tagcmp#
#define tageq(x,y,z)  ( tagcmp(x,y,z) == 0 ) // tageq#
#define is_nil(x)   ( tageq(x,nil,TAG_SIZE) ) // is_nil#

//#define tagcmp(x,y) ( (is_tptr(x) || (size(x) >= HASH_SIZE)) ? (memcmp((mword*)x, y, HASH_SIZE*MWORD_SIZE)) : -1 ) // tagcmp#
//#define tageq(x,y)  ( tagcmp(x,y) == 0 ) // tageq#
//#define is_nil(x)   ( tageq(x,nil) ) // is_nil#

// XXX UNSAFE; use this only where we know for sure we have a tptr...
#define is_nil_fast(x) ( (memcmp(x, nil, HASH_SIZE*MWORD_SIZE)) == 0 ) // is_nil_fast#

// XXX UNSAFE; use this only where we know for sure we are 
// working with interprer-allocated mem (fastest is_nil)
#define is_nil_interp(x) ( (mword*)x == nil ) // is_nil_interp#

#include "tags.h"
#include "construct.sp.h"

#define is_bvm(x) ( tagcmp((x),BABEL_TAG_BVM,MIN_TAG_SIZE) ) // is_bvm#
#define is_ref(x) ( tageq((x),BABEL_TAG_REF,MIN_TAG_SIZE) ) // is_ref#

//#define is_bvm(x) ( tagcmp((x),BABEL_TAG_BVM) == 0 ) // is_bvm#
//#define is_ref(x) ( tagcmp((x),BABEL_TAG_REF) == 0 ) // is_ref#

//#define car(x)      c((mword*)(x),0) // 
//#define cdr(x)      c((mword*)(x),1) // 
//
//#define _tcar(x)     c((mword*)x,HASH_SIZE+1) // 
//#define _tcdr(x)     c((mword*)x,HASH_SIZE+2) // 
//
////tptr-safe car/cdr:
//#define tcar(x)     (is_tptr(x) ? _tcar(x) : car(x)) // 
//#define tcdr(x)     (is_tptr(x) ? _tcdr(x) : cdr(x)) // 
//
////nil-safe car/cdr:
#define scar(x)     (is_nil(x) ? nil : car(x)) // scar#
#define scdr(x)     (is_nil(x) ? nil : cdr(x)) // scdr#

//car/cdr rework:
//internal car/cdr (not list-safe or tptr-safe):
#define icar(x)     c((mword*)(x),0) // icar#
#define icdr(x)     c((mword*)(x),1) // icdr#

//tptr-safe car/cdr:
#define tcar(x)     c((mword*)x,HASH_SIZE+1) // tcar#
#define tcdr(x)     c((mword*)x,HASH_SIZE+2) // tcdr#

//list-safe car/cdr (not tptr safe):
#define lcar(x)     (is_nil(x) ? nil : icar(x)) // lcar#
#define lcdr(x)     (is_nil(x) ? nil : icdr(x)) // lcdr#

//General-purpose car/cdr:
//#define car(x)      (is_tptr(x) ? tcar(x) : icar(x))
//#define cdr(x)      (is_tptr(x) ? tcdr(x) : icdr(x))
#define car(x)      ( icar(detag(this_bvm, (mword*)x)) ) // car#
#define cdr(x)      ( icdr(detag(this_bvm, (mword*)x)) ) // cdr#

//General-purpose cxr:
//#define tcxr(x,y)     c((mword*)x,HASH_SIZE+1+(y%2)) // txcr#
//#define  cxr(x,y)     (is_tptr(x) ? tcxr(x,y) : c((mword*)(x),y))
#define  cxr(x,y)     ( c((mword*)(detag(this_bvm, x)),y) ) // cxr#

// FIXME: Not tptr-safe
#define is_false(x) (    (is_leaf(x) && icar(x) == 0) || (is_nil(x)) ) // is_false#

//#define is_false(x) (    is_leaf(x) && icar(x) == 0 ||  is_nil(car(x)) ) // is_false#
//                     || !is_leaf(x) && is_nil(car(x)) )

//#define get_sym(x,y)   ( _luha(this_bvm,  (mword*)car(x->sym_table), _hash8(this_bvm, C2B(y))) )   // get_sym#
#define get_sym(x,y)   ( HASH_ENTRY_PAY( x, _luha(x,  get_tptr(x->sym_table), _hash8(x, C2B(y))) ) )   // get_sym#
//#define get_sym(x,y)   ( _luha(x,  get_tptr(x->sym_table), _hash8(x, C2B(y))) )  // get_sym#
#define set_sym(x,y,z) hash_insert( x->sym_table, (y), (z) ) // set_sym#

#define pushd(x,y,z) push_udr_stack(this_bvm, x->dstack_ptr, new_dstack_entry(this_bvm, y,z)) // pushd#
#define popd(x) pop_udr_stack(this_bvm, x->dstack_ptr) // popd#
//#define popd(x) *x->dstack_ptr = _shift(this_bvm, icar(x->dstack_ptr));
#define getd(x,y) get_from_udr_stack(x, x->dstack_ptr, y); remove_from_udr_stack(x, x->dstack_ptr,y);   // getd#

#define pushu(x,y,z) push_udr_stack(this_bvm, x->ustack_ptr, new_dstack_entry(this_bvm, y,z)) // pushu#
#define popu(x) pop_udr_stack(this_bvm, x->ustack_ptr) // popu#

#define pushr(x,y,z) push_udr_stack(this_bvm, x->rstack_ptr, new_rstack_entry(this_bvm, y,z)) // pushr#
#define popr(x) pop_udr_stack(this_bvm, x->rstack_ptr) // popr#

//#define zapd(x) _del( (mword*)icar( popd(x) ) )
#define zapd(x) zap_udr_stack(this_bvm,  x->dstack_ptr )    // zapd#
#define zapu(x) zap_udr_stack(this_bvm,  x->ustack_ptr )    // zapu#
#define zapr(x) zap_udr_stack(this_bvm,  x->rstack_ptr )    // zapr#

#define set_code_ptr(x,y) ((mword*)c(x->code_ptr,0) = y)
#define set_ustack_ptr(x,y) ((mword*)c(x->ustack_ptr,0) = y)
#define set_dstack_ptr(x,y) ((mword*)c(x->dstack_ptr,0) = y)
#define set_rstack_ptr(x,y) ((mword*)c(x->rstack_ptr,0) = y)

//#define mkref(x)  mkref#
#define mkref(x) ( new_tptr(this_bvm,  \
                        _hash8(this_bvm, C2B("/babel/tag/ref")), \
                            consa(this_bvm,  x, nil ) ) )

//#define mktptr(x) mktptr#
#define mktptr(x) ( new_tptr(this_bvm,  \
                        _hash8(this_bvm, C2B(x)), \
                            consa(this_bvm,  nil, nil ) ) )

#define mktptr2(x) ( new_tptr(this_bvm,  \
                        x, \
                        nil ) )

#include "array.h"

//#define MKTMP(n) tmp##n
//
//#define TMPLINE MKTMP(__LINE__)
//
//#define mkartmp(w,x,y,...) \
//    mword x[] = { MWORDS(y), __VA_ARGS__ }; \
//    mword *w = x+1;
//
//#define mkar(x,y,...) mkartmp(x,TMPLINE,y,__VA_ARGS__)
//
//#define mklstmp(w,x,y,...) \
//    mword x[] = { MWORDS(y), __VA_ARGS__ }; \
//    mword *w = _ar2ls( x+1 );
//
//#define mkls(x,y,...) mklstmp(x,TMPLINE,y,__VA_ARGS__)

//    mkar(myarray,3,1,2,3);
//    mkls(mylist, 3,1,2,3);
//
//    _dump(mylist);
//
//    die;

// non-allocating cons
// XXX: Doesn't work with tptr...
#define cons(a,b,c) icar(a) = (mword)(b); icdr(a) = (mword)(c); // cons#

#define new_cons (_newin(this_bvm, 2)) // new_cons#
#define new_atom (_newlfi(this_bvm, 1,0)) // new_atom#

// Stack
#define dstack_get(x,y) get_from_udr_stack(x, x->dstack_ptr, y) // dstack_get#
#define ustack_get(x,y) get_from_udr_stack(x, x->ustack_ptr, y) // ustack_get#
#define rstack_get(x,y) get_from_udr_stack(x, x->rstack_ptr, y) // rstack_get#

#define rstack_get_tag(x,y) get_tag_from_udr_stack(x, x->rstack_ptr, y)

#define ustack_set(x,y,z) set_in_udr_stack(x, x->ustack_ptr, y, z) // ustack_set#
#define dstack_set(x,y,z) set_in_udr_stack(x, x->dstack_ptr, y, z) // dstack_set#
#define rstack_set(x,y,z) set_in_udr_stack(x, x->rstack_ptr, y, z) // rstack_set#

#define ustack_empty(x) is_nil(icar(x->ustack_ptr)) // ustack_empty#
#define dstack_empty(x) is_nil(icar(x->dstack_ptr)) // dstack_empty#
#define rstack_empty(x) is_nil(icar(x->rstack_ptr)) // rstack_empty#
#define code_empty(x) is_nil(icar(x->code_ptr)) // code_empty#

//XXX DEPRECATE XXX
#define TOS_0(x)             (mword*)icar(icar(icar(x->dstack_ptr)))
#define TOS_1(x)         (mword*)icar(icar(icar(icdr(x->dstack_ptr))))
#define TOS_2(x)     (mword*)icar(icar(icar(icdr(icdr(x->dstack_ptr)))))

//#define TOS_0(x) (get_from_udr_stack(x->dstack_ptr, 0))

#define RTOS_0(x)            icar(icar(x->rstack_ptr))

#define STACK_ENTRY_SIZE 2

#define STACK_ENTRY_VAL(x) ((mword*)c(x,0))
#define STACK_ENTRY_LIF(x) ((mword*)c(x,1))

#define IMMORTAL 0  // IMMORTAL#  XXX: TO BE DEPRECATED
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

//Operators
// babel_operator_typeA#
#define babel_operator_typeA(x,y)               \
    mword *op0 = dstack_get(x,0);               \
    mword *result;                              \
                                                \
    y                                           \
                                                \
    popd(x);                                    \
                                                \
    pushd(x, result, IMMORTAL);                 \
                                                \
    return x;


//// babel_operator_typeB#
//#define babel_operator_typeB(x,y)               \
//    mword *op0 = dstack_get(x,0);               \
//    mword *op1 = dstack_get(x,1);               \
//    mword *result;                              \
//                                                \
//    y                                           \
//    popd(x);                                    \
//    popd(x);                                    \
//                                                \
//    pushd(x, result, IMMORTAL);                 \
//                                                \
//    return this_bvm;
//

// babel_operator_typeC#
#define babel_operator_typeC(x,y)               \
    mword *op0 = dstack_get(x,0);               \
                                                \
    y                                           \
                                                \
    popd(x);                                    \
                                                \
    return this_bvm;


// babel_operator_typeD#
#define babel_operator_typeD(x,y)               \
    mword *op0 = dstack_get(x,0);               \
    mword *result;                              \
                                                \
    y                                           \
                                                \
    pushd(x, result, IMMORTAL);                 \
                                                \
    return this_bvm;


// babel_operator_typeE#
#define babel_operator_typeE(x,y)               \
    mword *op0 = dstack_get(x,0);               \
    mword *op1 = dstack_get(x,1);               \
                                                \
    y                                           \
                                                \
    popd(x);                                    \
                                                \
    return this_bvm;


// babel_operator_typeZ#
#define babel_operator_typeB(x,y)               \
    mword *op0 = dstack_get(x,0);               \
    mword *op1 = dstack_get(x,1);               \
    mword *result;                              \
                                                \
    y                                           \
    zapd(x);                                    \
    zapd(x);                                    \
                                                \
    push_udr_stack(this_bvm, x->dstack_ptr,               \
            new_dstack_entry2(x, result,           \
                mktptr2(BABEL_TAG_PACMAN)));    \
                                                \
    return x;

//    pushd(x, result, IMMORTAL);                 \

// BVM
#define BVM_ADVANCE  0
#define BVM_CONTINUE 1
#define BVM_RETURN   2
#define BVM_NEXT     3

// DEBUG

//Uncomment to turn on tracing in bvm_interp():
//#define BVM_TRACE
//#define BVM_OP_TRACE

#define DIE_CODE    1
#define QUOTEME(x)  #x
#define d(x)        printf("%s %08x\n", QUOTEME(x), x);  // d#
#define _mem(x)     int _i; printf("%08x\n", s(x)); for(_i=0; _i<alloc_size(x)-1; _i++){ printf("%08x\n", c(x,_i)); } // _mem#
#define _dump(x)    printf("%s\n", _bs2gv(this_bvm, x));  // _dump#
#define _dumpd(x)   printf("%s\n", _bs2gv(this_bvm, x->dstack_ptr));  // _dumpd#
#define die         fprintf(stderr, "Died at %s line %d\n", __FILE__, __LINE__); exit(DIE_CODE);  // die#
#define warn(x)     fprintf(stderr, "WARNING: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__);  // warn#
#define error(x)    fprintf(stderr, "ERROR: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__); // error#
#define fatal(x)    fprintf(stderr, "FATAL: %s in %s() at %s line %d\n", x, __func__, __FILE__, __LINE__); die;  // fatal#
#define trace       printf("%s in %s line %d\n", __func__, __FILE__, __LINE__);   // trace#
#define err_trace   fprintf(stderr, "%s in %s line %d\n", __func__, __FILE__, __LINE__);   // err_trace#
#define enhance(x)  fprintf(stderr, "ENHANCEMENT: %s in %s at %s line %d\n", x, __func__, __FILE__, __LINE__); // enhance#

#include "interp.h"
#include "bvm.h"
#include "bstruct.h"

#endif //BABEL_H

// Clayton Bauman 2011


