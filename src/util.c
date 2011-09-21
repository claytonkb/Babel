// util.c
//

#include "babel.h"
#include "util.h"
#include "bvm_opcodes.h"
#include "array.h"
#include "stack.h"
#include "mt19937ar.h"
#include "string.h"
#ifdef WINDOWS
#include <windows.h>
#endif

void randop(void){

    mword *result    = _newlf(car(TOS_0));

    int i;
    for(i=0;i<car(TOS_0);i++){
        c(result,i) = genrand_int32();
    }

    zap();
    push_alloc(result, RAND);

}

#ifdef WINDOWS

void sleepop(void){

    DWORD seconds = (DWORD)car(TOS_0);
    zap();
    Sleep(seconds*1000);

}

#endif

void init_global_argv(int argc, char **argv){

    #define NUM_BABEL_INTERP_ARGS 1
    if(argc <= NUM_BABEL_INTERP_ARGS){
        global_argv = nil;
        return;
    }

    (mword*)global_argv = _newin(argc-NUM_BABEL_INTERP_ARGS);

    //Note: Each argument can be up to 64KB in size... this is unreasonably large
    //but could never reasonably become a design limitation in the future yet it is
    //still finite to protect against overflow attacks
    #define MAX_ARG_SIZE (1<<16)
    int i;
    for( i = NUM_BABEL_INTERP_ARGS; i < argc; i++ ){
//        (mword*)c((mword*)global_argv, i-NUM_BABEL_INTERP_ARGS) = (mword*)argv[i];
        (mword*)c((mword*)global_argv, i-NUM_BABEL_INTERP_ARGS)
            = _c2b(argv[i], 100);
        
    }

}

void argvop(void){

    push_alloc((mword*)global_argv, ARGVOP);

}

// Clayton Bauman 2011

