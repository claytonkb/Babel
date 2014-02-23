// util.c
//

#include "babel.h"
#include "util.h"
//#include "bvm_opcodes.h"
#include "array.h"
#include "stack.h"
#include "mt19937ar.h"
#include "string.h"
#include "list.h"
#include "bstruct.h"
#include "hash.h"
#include "alloc.h"
#include "tptr.h"
#include "pearson16.h"
#include "mem.h"

#ifdef WINDOWS
#include <windows.h>
#endif


//
// babel_operator
bvm_cache *bvm_root(bvm_cache *this_bvm){ // *bvm_root#

    pushd(this_bvm, this_bvm->self, IMMORTAL);

    return this_bvm;

}


//
//
bvm_cache *endian(bvm_cache *this_bvm){ // *endian#

    fatal("unimplemented");
    mword test = 1;
    char *test_addr = (char *)&test;
    mword *result = new_atom;

    if(*test_addr){
        *result = LITTLE_ENDIAN;
    }
    else{
        *result = BIG_ENDIAN;
    }

    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
// babel_operator
bvm_cache *randop(bvm_cache *this_bvm){ // *randop#

    mword num_mwords = icar(dstack_get(this_bvm,0));
    mword *result    = _newlf(this_bvm, num_mwords);

    int i;
    for(i=0;i<num_mwords;i++){
        c(result,i) = genrand_int32();
    }

    popd(this_bvm);
    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

#ifdef WINDOWS

//
// babel_operator
bvm_cache *sleepop(bvm_cache *this_bvm){ // *sleepop#

//    fatal("stack fix not done");
    DWORD seconds = (DWORD)car(dstack_get(this_bvm,0));
    popd(this_bvm);

    Sleep(seconds*1000);

    return this_bvm;

}

//#elsif STAR_NIX ...
#endif

//
// babel_operator
bvm_cache *argvop(bvm_cache *this_bvm){ // *argvop#

    mword *result = new_atom;
//    (mword *)*result = this_bvm->argv;

    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// FIXME: move to bstruct.c ...
// babel_operator
bvm_cache *mword_sizeop(bvm_cache *this_bvm){ // *mword_sizeop#

    fatal("stack fix not done");

    mword *result    = new_atom;

    *result = MWORD_SIZE;

    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}


//XXX Temporary operator... DEPRECATE
//
bvm_cache *lusym(bvm_cache *this_bvm){ // lusym#

    mword *symbol = dstack_get(this_bvm,0);
    popd(this_bvm);

    pushd(this_bvm, 
//            (mword*)get_sym(this_bvm, (char*)_b2c(this_bvm, symbol)), 
            HASH_ENTRY_PAY( this_bvm, _luha(this_bvm,  get_tptr(this_bvm->sym_table), _hash8(this_bvm, symbol)) ),
            IMMORTAL);

    return this_bvm;

}


//
//
bvm_cache *bvm_dump_cache(bvm_cache *this_bvm){ // bvm_dump_cache#

    d(this_bvm->self);
    _mema(this_bvm->self);

    d(this_bvm->code_ptr);
    _mema(this_bvm->code_ptr);

    d(this_bvm->rstack_ptr);
    _mema(this_bvm->rstack_ptr);

    d(this_bvm->dstack_ptr);
    _mema(this_bvm->rstack_ptr);

    d(this_bvm->ustack_ptr);
    _mema(this_bvm->rstack_ptr);

    d(this_bvm->sym_table);
    _mema(this_bvm->rstack_ptr);

    d(this_bvm->soft_root);

    d(this_bvm->thread_id);
    d(this_bvm->steps);
    d(this_bvm->advance_type);

    d(this_bvm->mask_table);

    d(this_bvm->flags);

    d(this_bvm->flags->MC_ALLOC_BLOCKING);
    d(this_bvm->flags->MC_GC_BLOCKING);
    d(this_bvm->flags->MC_GC_PENDING);
    d(this_bvm->flags->BVM_INSTR_IN_PROGRESS);
    d(this_bvm->flags->BVM_INCOHERENT);
    d(this_bvm->flags->BVM_CACHE_INVALID);
    d(this_bvm->flags->BVM_CACHE_DIRTY);
    d(this_bvm->flags->BVM_CACHE_ONLY);
    d(this_bvm->flags->BVM_CACHE_BLOCKING);
    d(this_bvm->flags->BVM_SYMBOLS_DEFINED);
    d(this_bvm->flags->BVM_CLEAN);
    d(this_bvm->flags->NO_ASYNC);
    d(this_bvm->flags->NO_EXCEPT);

    d(this_bvm->interp);

    d(this_bvm->interp->cat_ex);
    d(this_bvm->interp->argc);
    d(this_bvm->interp->argv);
    d(this_bvm->interp->envp);
    d(this_bvm->interp->interp_argv);
    d(this_bvm->interp->mem);
    d(this_bvm->interp->nil);
    d(this_bvm->interp->jump_table);
    d(this_bvm->interp->empty_string);
    d(this_bvm->interp->utc_epoch);
    d(this_bvm->interp->srand);
    d(this_bvm->interp->null_hash);
    d(this_bvm->interp->thread_counter);
    d(this_bvm->interp->global_tick_count);

    return this_bvm;

}


//
//
bvm_cache *fnord(bvm_cache *this_bvm){ // fnord#

    return this_bvm;

}


bvm_cache *dev(bvm_cache *this_bvm){ // dev#

//    int i;
//    int mem_bank_size = TOP_OF_ALLOC_BANK(mem->primary) 
//                            - mem->primary->alloc_ptr;
//
//    for(
//            i=0;
//            i < mem_bank_size;
//            i++
//            ){
//
//        printf("%08x\n", c(mem->primary->alloc_ptr,i));
//        //printf("%08x\n", c((this_bvm->self-1),i));
//
//    }
//
//    die;

//    mword *temp = _bvm_init(this_bvm, dstack_get(this_bvm,0));
//    _dump(temp);
//    die;

//    _dump(this_bvm->rstack_ptr);
//    die;

//    _dump(this_bvm->dstack_ptr);
//    die;

//    _dump(dstack_get(this_bvm,0));
//    die;

//    _dump(this_bvm->self);
//    die;

//    printf("fnord fnord\n");

//    mword *op0 = dstack_get(this_bvm,0);               
//
//    mword *temp = _shift(this_bvm, op0);
//
//    _dump(temp);
//    die;
//
//    mword *temp = _shift(this_bvm, op0);

    return this_bvm;

}


bvm_cache *readmem(bvm_cache *this_bvm){ // readmem#

    mword *ptr = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *result = _newlfi(this_bvm, 1,*ptr);

    pushd(this_bvm, 
            result,
            IMMORTAL);

    return this_bvm;

}


// Clayton Bauman 2011

