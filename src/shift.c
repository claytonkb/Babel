// XXX STACK FIX DONE
// shift.c
//

#include "babel.h"
#include "shift.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"

//void cushl(void){
//
//    mword *result    = new_atom();
//
//    *result = (mword)car(TOS_1) << (mword)car(TOS_0);
//
//    zap();
//    zap();
//    push_alloc(result, CUSHL);
//
//}
//
//void cushr(void){
//
//    mword *result    = new_atom();
//
//    *result = (mword)car(TOS_1) >> (mword)car(TOS_0);
//
//    zap();
//    zap();
//    push_alloc(result, CUSHR);
//
//}
//
//void cashr(void){
//
//    mword *result    = new_atom();
//
//    (int)*result = (int)car(TOS_1) >> (int)car(TOS_0);
//
//    zap();
//    zap();
//    push_alloc(result, CASHR);
//
//}
//
//void curol(void){
//
//    mword *result    = new_atom();
//
//    mword tempA = (mword)car(TOS_1) << (mword)car(TOS_0);
//    mword tempB = (mword)car(TOS_1) >> (MWORD_BIT_SIZE - (mword)car(TOS_0));
//    *result = tempA | tempB;
//
//    zap();
//    zap();
//    push_alloc(result, CUROL);
//
//}
//
//void curor(void){
//
//    mword *result    = new_atom();
//
//    mword tempA = (mword)car(TOS_1) >> (mword)car(TOS_0);
//    mword tempB = (mword)car(TOS_1) << (MWORD_BIT_SIZE - (mword)car(TOS_0));
//    *result = tempA | tempB;
//
//    zap();
//    zap();
//    push_alloc(result, CUROR);
//
//}
//

// Clayton Bauman 2011

