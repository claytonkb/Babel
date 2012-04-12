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

    int i;

    #include "rt.pb.c"

//    _load((mword*)bbl);
//    bvm_init(bbl);
//    init_global_argv(argc, argv);

    _bvm_init(_load((mword*)bbl,sizeof(bbl)/MWORD_SIZE),argc,argv);

    char nil_string[3] = "nil";
//    time( &rawtime );    
//    strcpy( nil_string, ctime(&rawtime) );
    mword *nil_string_key = _c2b(nil_string, 3);

    // FIXME: strcpy and strlen... get rid
    // This needs to be enhanced to look in the hidden section for a 
    // pre-defined seed, it should also save the value it used in the
    // hidden section
    mword *nil_hash = new_hash();
    mword *hash_init = new_hash();
    nil_hash = _pearson16(hash_init, nil_string_key, (mword)strlen((char*)nil_string_key));
    printf("  0x%x ", nil_hash[0]);
    printf("0x%x ", nil_hash[1]);
    printf("0x%x ", nil_hash[2]);
    printf("0x%x ", nil_hash[3]);
//    die

    global_steps = (mword) -1;
    _bvmexec(internal_global_VM);

//    printf("\n");
    return 0;

}

// Clayton Bauman 2011

