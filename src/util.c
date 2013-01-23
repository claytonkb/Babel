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

    //FIXME: Windows-specific!
    Sleep(seconds*1000);

    return this_bvm;

}

#endif

//
// babel_operator
bvm_cache *argvop(bvm_cache *this_bvm){

    mword *result = new_atom;
    (mword *)*result = this_bvm->argv;

    push_alloc(this_bvm, result, IMMORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *mword_sizeop(bvm_cache *this_bvm){

    fatal("stack fix not done");

    mword *result    = new_atom;

    *result = MWORD_SIZE;

    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
// babel_operator
bvm_cache *fnord(bvm_cache *this_bvm){

    printf("fnord fnord\n");

//    mword *result = _luha(this_bvm->sym_table, (mword*)TOS_0(this_bvm));
//    d(result);
//    d(*result);
//
    return this_bvm;

}

// Clayton Bauman 2011

