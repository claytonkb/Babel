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

    #include "rt.pb.c"
    _load((mword*)bbl);

    _bvm_init(bbl);

    //Need to figure out what to do with this...
    init_global_argv(argc, argv);

    _bvmexec(internal_global_VM);

    printf("\n");
    return 0;

}

// Clayton Bauman 2011

