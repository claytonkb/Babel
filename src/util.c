// util.c
//

#include "babel.h"
#include "util.h"
#include "bvm_opcodes.h"
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

#ifdef WINDOWS
#include <windows.h>
#endif

//
// babel_operator
bvm_cache *randop(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword num_mwords = car(TOS_0(this_bvm));
    mword *result    = _newlf(num_mwords);

    int i;
    for(i=0;i<num_mwords;i++){
        c(result,i) = genrand_int32();
    }

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

#ifdef WINDOWS

//
// babel_operator
bvm_cache *sleepop(bvm_cache *this_bvm){

    fatal("stack fix not done");
    DWORD seconds = (DWORD)car(TOS_0(this_bvm));
    zap(this_bvm);

    Sleep(seconds*1000);

    return this_bvm;

}

//#elsif STAR_NIX ...
#endif

//
// babel_operator
bvm_cache *argvop(bvm_cache *this_bvm){

    mword *result = new_atom;
//    (mword *)*result = this_bvm->argv;

    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

// FIXME: move to bstruct.c ...
// babel_operator
bvm_cache *mword_sizeop(bvm_cache *this_bvm){

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
//            (mword*)get_sym(this_bvm, (char*)_b2c(symbol)), 
            _luha( get_tptr(this_bvm->sym_table), _hash8(symbol)),
            IMMORTAL);

    return this_bvm;

}


//
//
bvm_cache *fnord(bvm_cache *this_bvm){ // fnord#

    _dump(this_bvm->ustack_ptr);
    die;

//    _dump(dstack_get(this_bvm,0));
//    die;

//    printf("fnord fnord\n");

//    mword *op0 = dstack_get(this_bvm,0);               
//
//    mword *temp = _shift(op0);
//
//    _dump(temp);
//    die;
//
//    mword *temp = _shift(op0);

    return this_bvm;

}

// Clayton Bauman 2011

