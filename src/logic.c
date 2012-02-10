// XXX STACK FIX DONE
// logic.c
//

#include "babel.h"
#include "logic.h"
#include "stack.h"
#include "list.h"
#include "bvm_opcodes.h"

void logicF0(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = 0;

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F000);
    push_alloc(result, F000);

}

void logicF1(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = (mword)car(TOS_1) & (mword)car(TOS_0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F001);
    push_alloc(result, F001);

}

void logicF2(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = (mword)car(TOS_1) & ~((mword)car(TOS_0));

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F002);
    push_alloc(result, F002);

}

void logicF3(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = (mword)car(TOS_0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F003);
    push_alloc(result, F003);

}

void logicF4(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~((mword)car(TOS_1)) & (mword)car(TOS_0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F004);
    push_alloc(result, F004);

}

void logicF5(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = (mword)car(TOS_1);

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F005);
    push_alloc(result, F005);

}

void logicF6(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = (mword)car(TOS_1) ^ (mword)car(TOS_0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
//   push_alloc(result, F006);
    push_alloc(result, F006);

}

void logicF7(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = (mword)car(TOS_1) | (mword)car(TOS_0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F007);
    push_alloc(result, F007);

}

void logicF8(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~((mword)car(TOS_1) | (mword)car(TOS_0));

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F008);
    push_alloc(result, F008);

}

void logicF9(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~((mword)car(TOS_1) ^ (mword)car(TOS_0));

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F009);
    push_alloc(result, F009);

}

void logicFA(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~((mword)car(TOS_1));

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F00A);
    push_alloc(result, F00A);

}

void logicFB(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~( ~(mword)car(TOS_1) & (mword)car(TOS_0) );

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F00B);
    push_alloc(result, F00B);

}

void logicFC(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~(mword)car(TOS_0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
//   push_alloc(result, F00C);
    push_alloc(result, F00C);

}

void cnot(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~(mword)car(TOS_0);

//    cons(temp_cons, result, nil);

    zap();
//    zap();
//    push_alloc(temp_cons, CNOT);
    push_alloc(result, CNOT);

}


void logicFD(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~( (mword)car(TOS_1) & ~(mword)car(TOS_0) );

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F00D);
    push_alloc(result, F00D);

}

void logicFE(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = ~( (mword)car(TOS_1) & (mword)car(TOS_0) );

//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(result, F00E);
    push_alloc(result, F00E);

}

void logicFF(void){

    mword *result    = new_atom();

    *result = (mword)(-1);

    zap();
    zap();
    push_alloc(result, F00F);

}

void andop(void){

    mword *result    = new_atom();

    *result = ( !is_false((mword*)TOS_1) && !is_false((mword*)TOS_0) );
    
    zap();
    zap();

    push_alloc(result, ANDOP);

}

void orop(void){

    mword *result    = new_atom();

    *result = ( !is_false((mword*)TOS_1) || !is_false((mword*)TOS_0) );
    
    zap();
    zap();
    push_alloc(result, OROP);

}

void notop(void){

    mword *result    = new_atom();

    *result = is_false((mword*)TOS_0);

    zap();
    push_alloc(result, NOTOP);

}

// Clayton Bauman 2011

