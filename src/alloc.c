// alloc.c
//

#include "babel.h"
#include "alloc.h"
#include "stack.h"
#include "array.h"
#include "load.h"
#include "bstruct.h"

//void free_op(void){
//
//    _free_op((mword*)TOS_0);
//    zap();
//
//}
//
//void _free_op(mword *mem){
//
//    free(mem-1);
//
//}
//
//void del(void){
//
//    _del((mword*)TOS_0);
//    zap();
//
//}
//
//void _del(mword *tree){
//
//    int i;
//
//    mword *del_list = _bs2ar(tree);
//
//    for(i=0;i<size(del_list);i++){
//        _free_op((mword*)c(del_list,i));
//    }
//
//}

// Clayton Bauman 2012

