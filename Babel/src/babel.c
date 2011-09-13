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

    global_VM = (_slurp(argv[1])+1);
    global_machine_page_size = *(global_VM-2);

    bvm_init();

    setjmp(exception_env); // XXX for use by except() ONLY!! XXX

    init_global_argv(argc, argv);

    bvm_interp();

//    tree_dot();

    printf("\n");
    return 0;

}

// Clayton Bauman 2011

