// babel.c
//

#include "babel.h"
#include "interp.h"

//main
//
int main(int argc, char **argv, char **envp){ // main#

    bvm_cache root;
    bvm_cache *this_bvm = &root;

    interp_babel(this_bvm, argc, argv, envp);

    return 0;

}


// Clayton Bauman 2014

