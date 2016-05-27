// babel.c
//

#include "babel.h"
#include "interp.h"

//main
//
int main(int argc, char **argv, char **envp){ // main#

    bvm_cache root;
    bvm_cache *this_bvm = &root;


#ifdef PROF_MODE
    int wall_clock_time = interp_babel(this_bvm, argc, argv, envp);
    _dd(wall_clock_time);
#else
    interp_babel(this_bvm, argc, argv, envp);
#endif

    return 0;

}


// Clayton Bauman 2014

