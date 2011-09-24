// babel.c
//

#include "babel.h"
#include "io.h"
#include "bvm.h"
#include "pearson16.h"
#include "debug.h"
#include <time.h>
#include "mt19937ar.h"
#include "array.h"
#include "string.h"
#include "util.h"
#include "load.h"

//main
//
int main(int argc, char **argv){

    pearson16_init();

    time_t rawtime;
    char time_string[30];
    time( &rawtime );    
    strcpy( time_string, ctime(&rawtime) );
    mword *time_string_key = _c2b(time_string, 30);

//    printf("Initializing...\n");
//    Sleep(3000);
//    printf("Initialized...\n");
    
    mword *time_hash = new_hash();
    mword *hash_init = new_hash();
    time_hash = _pearson16(hash_init, time_string_key);
    init_by_array(time_hash, HASH_SIZE*(sizeof(mword)/sizeof(unsigned long)));
//    printf("%x", genrand_int32());
//    die

    #include "rt.pb.c"
//    printf("%08x", *(bbl+1));
//    die

//    global_VM = (_slurp(argv[1])+1);
//    global_machine_page_size = *(global_VM-2);
//    bvm_init();
//    setjmp(exception_env); // XXX for use by except() ONLY!! XXX
//    bvm_interp();

    global_machine_page_size = BBL_SIZE;
    _load((mword*)bbl);

//    printf("%08x", *(bbl+1));
//    die
//    printf("OK");
    internal_global_VM = bbl+1;
    global_VM = (mword *)cdr(internal_global_VM);

    init_global_argv(argc, argv);

    _bvmexec(internal_global_VM);

    printf("\n");
    return 0;

}

//// NEW main:
//// #include "rt.c"
//int main(int argc, char **argv){
//
//    global_VM = babelrt; // #define'd in rt.c
//
//    _bvmexec(global_VM,argc, argv);
//
//    return 0;
//
//}

// Clayton Bauman 2011

