// babel.c
//

#include "babel.h"
#include "array.h"

//main
//
int main(int argc, char **argv, char **envp){ // main#

    bvm_cache root_bvm;
    bvm_cache *this_bvm = &root_bvm;

    int running_time = babel_interp(this_bvm, argc, argv, envp);

    //printf("babel finished %ds\n", running_time);

    return 0;

}

// Clayton Bauman 2011

