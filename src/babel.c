// babel.c
//

#include "babel.h"
#include "array.h"

//main
//
int main(int argc, char **argv, char **envp){

    bvm_cache root_bvm;
    bvm_cache *this_bvm = &root_bvm;

    interp_init(this_bvm, argc, argv, envp);

    bvm_interp(this_bvm);

    return 0;

}

// Clayton Bauman 2011

