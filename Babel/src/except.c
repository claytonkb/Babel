// except.c
//

#include "babel.h"
#include "except.h"

// except
//
// This function NEVER RETURNS
//
// There are only one of two ways to get out of this function
// a) If there is no exception handler:
//      print error message and die
// b) If there is an exception handler:
//      call it, then longjmp to restart the interpreter
void except(char *message, char *file, int line){

    // 1. hash the message to generate exception handler ID
    // 2. look up in the sym_table
    // 3. if it exists, call it then longjmp
    //if( exception_handler_exists ){
    //    exception_handler();
    //    longjmp(exception_env, 0);
    //}
    // 4. else, print error message and die
    //else{

    fprintf(stderr, "EXCEPTION: %s at %s line %d\n", message, file, line);
    die

    //}

}

// Clayton Bauman 2011

