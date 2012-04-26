// XXX STACK FIX DONE
// cmp.c
//

#include "babel.h"
#include "cmp.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"

//void cne(void){
//
//
//    mword *result    = new_atom();
//
//    *result = (mword)car(TOS_0) != (mword)car(TOS_1);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CNE);
//
//}
//
//void ceq(void){
//
//
//    mword *result    = new_atom();
//
//    *result = (mword)car(TOS_0) == (mword)car(TOS_1);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CEQ);
//
//}
//
//void cult(void){
//
//
//    mword *result    = new_atom();
//
//    *result = (mword)car(TOS_1) < (mword)car(TOS_0);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CULT);
//
//}
//
//void cule(void){
//
//
//    mword *result    = new_atom();
//
//    *result = (mword)car(TOS_1) <= (mword)car(TOS_0);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CULE);
//
//}
//
//void cugt(void){
//
//
//    mword *result    = new_atom();
//
//    *result = (mword)car(TOS_1) > (mword)car(TOS_0);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CUGT);
//
//}
//
//void cuge(void){
//
//
//    mword *result    = new_atom();
//
//    *result = (mword)car(TOS_1) >= (mword)car(TOS_0);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CUGE);
//
//}
//
//void cilt(void){
//
//
//    mword *result    = new_atom();
//
//    (int)*result = (int)car(TOS_1) < (int)car(TOS_0);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CILT);
//
//}
//
//void cile(void){
//
//
//    mword *result    = new_atom();
//
//    (int)*result = (int)car(TOS_1) <= (int)car(TOS_0);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CILE);
//
//}
//
//void cigt(void){
//
//
//    mword *result    = new_atom();
//
//    (int)*result = (int)car(TOS_1) > (int)car(TOS_0);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CIGT);
//
//}
//
//void cige(void){
//
//
//    mword *result    = new_atom();
//
//    (int)*result = (int)car(TOS_1) >= (int)car(TOS_0);
//
//
//
//    zap();
//    zap();
//    push_alloc(result, CIGE);
//
//}
//

// Clayton Bauman 2011

