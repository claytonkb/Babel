// XXX STACK FIX DONE
// list.c
//

#include "babel.h"
#include "list.h"
#include "stack.h"
#include "bvm_opcodes.h"
#include "except.h"
#include "array.h"

//
//
inline mword *new_cons(void){

    mword *ptr = malloc( MWORDS(3) );
    if(ptr == NULL){
        except("new_cons: malloc returned NULL", __FILE__, __LINE__);
    }

    ptr[0] = (int)(-2 * MWORD_SIZE);
    return (ptr+1);

}

//
//
inline mword *new_atom(void){

    mword *ptr = malloc( MWORDS(2) );
    if(ptr == NULL){
        except("new_atom: malloc returned NULL", __FILE__, __LINE__);
    }

    ptr[0] = MWORD_SIZE;
    return ptr+1;

}

void carindex(void){
    
//    cons(temp_cons, (mword*)car(TOS_0), nil);

    mword *car_TOS_0 = (mword*)car(TOS_0);

    zap();
    push_alloc(car_TOS_0, CARINDEX);

}

void cdrindex(void){



//    cons(temp_cons, (mword*)cdr(car(car(stack_ptr))), nil);
//    cons(temp_cons, (mword*)cdr(TOS_0), nil);
    mword *cdr_TOS_0 = (mword*)cdr(TOS_0);

    zap();
    push_alloc(cdr_TOS_0, CDRINDEX);

}

void isnil(void){

//    printf(((char*)TOS_0), c(0,(mword*)TOS_1));


    mword *result    = new_atom();
    
    *result = ( (mword)car(TOS_0) == (mword)nil );


    zap();
    push_alloc(result, ISNIL);


}

//consls
//#define cons(a,b,c) car(a) = (mword)(b); cdr(a) = (mword)(c);
void consls(void){

//    mword *temp_cons = new_cons();
//
//    cons(temp_cons, (mword*)TOS_1, (mword*)TOS_0);

    mword *result = _consls((mword*)TOS_1, (mword*)TOS_0);

    zap();
    zap();
    push_alloc(result, CONS);

}

mword *_consls(mword *car_field, mword *cdr_field){

    mword *temp_cons = new_cons();

    cons(temp_cons, car_field, cdr_field);

    return temp_cons;

}


//
//
void uncons(void){

    //FIXME: No checking...
    mword *temp_car = (mword*)car(TOS_0);
    mword *temp_cdr = (mword*)cdr(TOS_0);

    zap();
    push_alloc(temp_car, UNCONS);
    push_alloc(temp_cdr, UNCONS);

}

////push
//// equivalent to Joy's swons
//void push(void){
//
//    mword *temp_cons = new_cons();
//
//    cons(temp_cons, (mword*)TOS_0, (mword*)TOS_1);
//
//    zap();
//    zap();
//    push_alloc(temp_cons, PUSH);
//
//}
//
////pop
////
//void pop(void){
//
//    //FIXME: No checking...
//    mword *temp_car = (mword*)car(TOS_0);
//    TOS_0 = cdr(TOS_0);
//    push_alloc(temp_car, POP);
//
//}

void len(void){
    mword *result = _newlf(1);
    *result = _len((mword*)TOS_0);
    zap();
    push_alloc(result, LSLEN);

}

mword _len(mword *list){
    mword length = 0;
    while(list != (mword*)nil){
        length++;
        list = (mword*)cdr(list);
    }
    return length;
}


void bons(void){

    mword *result = _bons((mword*)TOS_0);
    zap();
    push_alloc(result, BONS);

}

mword *_bons(mword *list){

    mword list_length = _len(list);
    mword *arr = _newin(list_length);

    int i=0;
    while(list != (mword*)nil){
        c(arr,i) = car(list);
        i++;
        list = (mword*)cdr(list);
    }

    return arr;

}

void ls2lf(void){

    mword *result = _ls2lf((mword*)TOS_0);
    zap();
    push_alloc(result, LS2LF);

}

mword *_ls2lf(mword *list){

    mword list_length = _len(list);
    mword *arr = _newlf(list_length);

    int i=0;
    while(list != (mword*)nil){
        if(!is_leaf((mword*)car(list))){
            die //FIXME: Exception
        }
        c(arr,i) = car(car(list));
        i++;
        list = (mword*)cdr(list);
    }

    return arr;

}


// Clayton Bauman 2011

