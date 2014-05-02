// alloc.c
//

#include "babel.h"
#include "alloc.h"
#include "stack.h"
#include "array.h"
#include "load.h"
#include "bstruct.h"

/* alloc operator
**free**  
> Frees the array on TOS - NOT recursive. See del.  
*/
bvm_cache *free_op(bvm_cache *this_bvm){ // *free_op#

    fatal("stack fix not done");
//    bfree(TOS_0(this_bvm));
//    hard_zap(this_bvm);

    return this_bvm;

}

/* alloc operator
**del**  
> Named after the C++ delete operator  
>
> Frees (de-allocates) a leaf-array or interior array created with
> the newlf or newin operators. Care is required with this operator - 
> everything accessible from TOS will be deleted. For example,  
>
> stack del --OR  
> code del  
>
> ... will crash the interpreter.  
*/
bvm_cache *del(bvm_cache *this_bvm){ // *del#

    fatal("stack fix not done");
//    _del(TOS_0(this_bvm));
//    hard_zap(this_bvm);

    return this_bvm;

}


////_balloc() 
////
//mword *_balloc(mword size){
//
//    // FIXME: Check allocation limit; except if exceeded
//
//    mword *ptr = malloc( MWORDS( size ) ); // 3 = s-field + car + cdr
//
//    if(ptr == NULL){
//        fatal("malloc returned NULL");
//        //FIXME: Exception
//    }
//
//    //FIXME: Put ptr into the bvm's alloc table
//
//    return ptr;
//
//}

/* alloc operator
**destroy**  
> Safer del  
*/
//bvm_cache *destroy(bvm_cache *this_bvm){
//
//    _del((mword*)TOS_0(this_bvm));
//    hard_zap(this_bvm);
//
//    return this_bvm;
//
//}

// Clayton Bauman 2012

