// XXX STACK FIX DONE
// list.c
//

#include "babel.h"
#include "list.h"
#include "stack.h"
#include "bvm_opcodes.h"
#include "except.h"
#include "array.h"

//mword *car(mword *x){
//
//    if(is_nil(x)){
//        return global_nil;
//    }
//    else{
//        return fast_car(x);
//    }
//
//}
//
//mword *car(mword *x){
//
//    if(is_nil(x)){
//        return global_nil;
//    }
//    else{
//        return fast_cdr(x);
//    }
//
//}
//
////
////
inline mword *new_cons(void){

    mword *ptr = malloc( MWORDS(3) );
    if(ptr == NULL){
        error("new_cons: malloc returned NULL");
    }

    ptr[0] = (int)(-2 * MWORD_SIZE);
    return (ptr+1);

}

////
////
inline mword *new_atom(void){

    mword *ptr = malloc( MWORDS(2) );
    if(ptr == NULL){
        error("new_atom: malloc returned NULL");
    }

    ptr[0] = MWORD_SIZE;
    return ptr+1;

}

//void carindex(void){
//    
////    cons(temp_cons, (mword*)car(TOS_0), nil);
//
//    mword *car_TOS_0 = (mword*)car(TOS_0);
//
//    zap();
//    push_alloc(car_TOS_0, CARINDEX);
//
//}
//
//void cdrindex(void){
//
//
//
////    cons(temp_cons, (mword*)cdr(car(car(stack_ptr))), nil);
////    cons(temp_cons, (mword*)cdr(TOS_0), nil);
//    mword *cdr_TOS_0 = (mword*)cdr(TOS_0);
//
//    zap();
//    push_alloc(cdr_TOS_0, CDRINDEX);
//
//}
//
////FIXME - This operator originally took the car of TOS to determine
////if this was nil, rather than looking at TOS directly. The change
////may result in OBO errors in lists. The root issue here is that
////car() and cdr() can't handle hash-refs. This will have to be fixed.
//void isnil(void){
//
////    printf(((char*)TOS_0), c(0,(mword*)TOS_1));
//
//    mword *result    = new_atom();
//    
////    *result = ( (mword)car(TOS_0) == (mword)nil );
////    *result = is_nil((mword*)car(TOS_0));
//    *result = is_nil((mword*)TOS_0);
//
//
//    zap();
//    push_alloc(result, ISNIL);
//
//
//}
//
////consls
////#define cons(a,b,c) car(a) = (mword)(b); cdr(a) = (mword)(c);
//void consls(void){
//
////    mword *temp_cons = new_cons();
////
////    cons(temp_cons, (mword*)TOS_1, (mword*)TOS_0);
//
//    mword *result = _consls((mword*)TOS_1, (mword*)TOS_0);
//
//    zap();
//    zap();
//    push_alloc(result, CONS);
//
//}
//
//mword *_consls(mword *car_field, mword *cdr_field){
//
//    mword *temp_cons = new_cons();
//
//    cons(temp_cons, car_field, cdr_field);
//
//    return temp_cons;
//
//}
//
//
////
////
//void uncons(void){
//
//    //FIXME: No checking...
//    mword *temp_car = (mword*)car(TOS_0);
//    mword *temp_cdr = (mword*)cdr(TOS_0);
//
//    zap();
//    push_alloc(temp_car, UNCONS);
//    push_alloc(temp_cdr, UNCONS);
//
//}
//
//void list_end(void){
//
//}
//
//mword *_list_end(mword *list){
//
//    while(cdr(list) != nil){ //Breaks due to car/cdr can't handle hash-refs
//        list = (mword*)cdr(list);
//    }
//    return list;
//    
//}
//
//mword *_list_next_to_end(mword *list){
//
//    while(cdr(cdr(list)) != nil){
//        list = (mword*)cdr(list);
//    }
//    return list;
//    
//}
//
////push
//// Like Perl's
//// ((a b c)) ((d e f)) push --> (a b c d e f)
//void push(void){
//
//    mword *endls = _list_end((mword*)TOS_1);
//    c(endls,1) = TOS_0;
//
//    zap();
//    // XXX Might get alloc-type bugs here...
////    push_alloc(temp_cons, PUSH);
//
//}
//
////pop
//// Like Perl's
//void pop(void){
//
//    mword *endls = _list_next_to_end((mword*)TOS_0);
//
//    mword *temp = (mword*)c(endls,1);
//    c(endls,1) = nil;
//
//    push_alloc(temp, POP);
//
//}
//
////unshift
//// Like Perl's
//// ((a b c)) ((d e f)) push --> (d e f a b c)
//// A B unshift <--> A B swap push
//void unshift(void){
//
//    swap();
//
//    mword *endls = _list_end((mword*)TOS_1);
//    c(endls,1) = TOS_0;
//
//    zap();
//    // XXX Might get alloc-type bugs here...
////    push_alloc(temp_cons, PUSH);
//
//}
//
////shift
//// Like Perl's
//void shift(void){
//
//    mword *temp = (mword*)TOS_0;
//    TOS_0 = cdr(TOS_0);
//
//    c(temp,1) = nil;
//
//    push_alloc(temp, SHIFT);
//
//}
//
////NIL-CONVERSION DONE
//void len(void){ 
//    mword *result = _newlf(1);
//    *result = _len((mword*)TOS_0);
//    zap();
//    push_alloc(result, LSLEN);
//
//}
//

//NIL-CONVERSION DONE
mword _len(mword *list){
    mword length = 0;
//    while(list != (mword*)nil){
    while(!is_nil(list)){
        length++;
        list = (mword*)cdr(list);
    }
    return length;
}

//
//void bons(void){
//
//    mword *result = _bons((mword*)TOS_0);
//    zap();
//    push_alloc(result, BONS);
//
//}
//
//mword *_bons(mword *list){
//
//    mword list_length = _len(list);
//    mword *arr = _newin(list_length);
//
//    int i=0;
//    while(list != (mword*)nil){
//        c(arr,i) = car(list);
//        i++;
//        list = (mword*)cdr(list);
//    }
//
//    return arr;
//
//}
//
//void ls2lf(void){
//
//    mword *result = _ls2lf((mword*)TOS_0);
//    zap();
//    push_alloc(result, LS2LF);
//
//}
//
//mword *_ls2lf(mword *list){
//
//    mword list_length = _len(list);
//    mword *arr = _newlf(list_length);
//
//    int i=0;
//    while(list != (mword*)nil){
//        if(!is_leaf((mword*)car(list))){
//            die //FIXME: Exception
//        }
//        c(arr,i) = car(car(list));
//        i++;
//        list = (mword*)cdr(list);
//    }
//
//    return arr;
//
//}
//
//void ith(void){
//
//    mword *result = _ith((mword*)TOS_1, car(TOS_0));
//    zap();
//    zap();
//    push_alloc(result, ITH);
//
//}
//
//mword *_ith(mword *list, mword i){
//
//    while(i > 0){
//        i--;
//        list = (mword*)cdr(list);
//    }
//    return (mword*)car(list);
////    return list;
//
//}
//

// Clayton Bauman 2011

