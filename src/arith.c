// XXX STACK FIX DONE
// arith.c
//

#include "babel.h"
#include "arith.h"
#include "stack.h"
#include "list.h"
#include "bstruct.h"
#include "bvm_opcodes.h"
#include "except.h"
#include "array.h"
#include "bvm.h"

//cuadd
//#define CUADD      0x030
void cuadd(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = (mword)car(TOS_0) + (mword)car(TOS_1);

    // Detect overflow
    if((mword)*result < (mword)car(TOS_0)){
        except("cuadd: overflow", __FILE__, __LINE__);
    }

//    cons(temp_cons, result, nil);
    // FIXME Overflow exception

    zap();
    zap();
//    push_alloc(temp_cons, CUADD);
    push_alloc(result, CUADD);

}

//cusub
//#define CUSUB      0x031
void cusub(void){

    // Detect underflow
    if((mword)car(TOS_1) < (mword)car(TOS_0)){
        except("cusub: underflow", __FILE__, __LINE__);
    }

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    *result = (mword)car(TOS_1) - (mword)car(TOS_0);
//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(temp_cons, CUSUB);
    push_alloc(result, CUSUB);

}

//ciadd
//#define CIADD      0x038
void ciadd(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    (int)*result = (int)car(TOS_0) + (int)car(TOS_1);
//    cons(temp_cons, result, nil);

    // Detect underflow/overflow
    if( ((int)*result <  0) && ((int)car(TOS_0) > 0) && ((int)car(TOS_1) > 0) ){
        except("ciadd: overflow", __FILE__, __LINE__);
    }
    if( ((int)*result >= 0) && ((int)car(TOS_0) < 0) && ((int)car(TOS_1) < 0) ){
        except("ciadd: underflow", __FILE__, __LINE__);
    }

    zap();
    zap();
//    push_alloc(temp_cons, CIADD);
    push_alloc(result, CIADD);

}

//cisub
//#define CISUB      0x039
void cisub(void){

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    (int)*result = (int)car(TOS_1) - (int)car(TOS_0);
//    cons(temp_cons, result, nil);

    // Detect underflow/overflow
    if( ((int)*result <  0) && ((int)car(TOS_0) < 0) && ((int)car(TOS_1) > 0) ){
        except("cisub: overflow", __FILE__, __LINE__);
    }
    if( ((int)*result >= 0) && ((int)car(TOS_0) > 0) && ((int)car(TOS_1) < 0) ){
        except("cisub: underflow", __FILE__, __LINE__);
    }

    zap();
    zap();
//    push_alloc(temp_cons, CISUB);
    push_alloc(result, CISUB);

}

//ciabs
//#define CIABS      0x03C
void ciabs(void){

    // The most negative number in 2's complement cannot be abs()'d
    if( ((int)car(TOS_0) - 1) > 0 ){
        except("ciabs: overflow", __FILE__, __LINE__);
    }

//    mword *temp_cons = new_cons();
    mword *result    = new_atom();

    (int)*result = abs((int)car(TOS_0));
//    cons(temp_cons, result, nil);

    zap();
//    push_alloc(temp_cons, CIABS);
    push_alloc(result, CIABS);

}

//cumul
//#define CUMUL      0x032
void cumul(void){

//    mword *temp_cons = new_cons();
    mword *result = new_atom();

    *result = car(TOS_0) * car(TOS_1);
//    cons(temp_cons, result, nil);

    // Argh... There must be a better way!
    if( *result / car(TOS_0) != car(TOS_1) ){
        except("cumul: overflow", __FILE__, __LINE__);
    }

    zap();
    zap();
//    push_alloc(temp_cons, CUMUL);
    push_alloc(result, CUMUL);

}

//cudiv 
//#define CUDIV      0x033
void cudiv(void){

    if( car(TOS_0) == 0 ){
        except("cudiv: zero divisor", __FILE__, __LINE__);
    }

//    mword *temp_cons = new_cons();
    mword *result = new_atom();

    *result = car(TOS_1) / car(TOS_0);
//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(temp_cons, CUDIV);
    push_alloc(result, CUDIV);

}

//curem
//#define CUREM      0x035
void curem(void){

    if( car(TOS_0) == 0 ){
        except("curem: zero modulus", __FILE__, __LINE__);
    }

//    mword *temp_cons = new_cons();
    mword *result = new_atom();

    *result = car(TOS_1) % car(TOS_0);
//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(temp_cons, CUREM);
    push_alloc(result, CUREM);

}

//cimul
//#define CIMUL      0x03A
void cimul(void){

//    mword *temp_cons = new_cons();
    mword *result = new_atom();

    (int)*result = (int)car(TOS_0) * (int)car(TOS_1);
//    cons(temp_cons, result, nil);

    // Argh... There must be a better way!
    if( (int)*result / (int)car(TOS_0) != (int)car(TOS_1) ){
        except("cimul: overflow", __FILE__, __LINE__);
    }

    zap();
    zap();
//    push_alloc(temp_cons, CIMUL);
    push_alloc(result, CIMUL);

}


//cidiv 
//#define CIDIV      0x03B
void cidiv(void){

    if( car(TOS_0) == 0 ){
        except("cidiv: zero divisor", __FILE__, __LINE__);
    }

//    mword *temp_cons = new_cons();
    mword *result = new_atom();

    (int)*result = (int)car(TOS_1) / (int)car(TOS_0);
//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(temp_cons, CIDIV);
    push_alloc(result, CIDIV);

}

//curem
//#define CIREM      0x03D
void cirem(void){

    if( car(TOS_0) == 0 ){
        except("cirem: zero modulus", __FILE__, __LINE__);
    }

//    mword *temp_cons = new_cons();
    mword *result = new_atom();

    (int)*result = (int)car(TOS_1) % (int)car(TOS_0);
//    cons(temp_cons, result, nil);

    zap();
    zap();
//    push_alloc(temp_cons, CIREM);
    push_alloc(result, CIREM);

}

// Clayton Bauman 2011

