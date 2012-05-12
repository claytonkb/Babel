// list.c
//

#include "babel.h"
#include "list.h"
#include "stack.h"
#include "bvm_opcodes.h"
#include "except.h"
#include "array.h"

//
bvm_cache *carindex(bvm_cache *this_bvm){
    
    mword *car_TOS_0 = (mword*)car(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, car_TOS_0, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
bvm_cache *cdrindex(bvm_cache *this_bvm){

    mword *cdr_TOS_0 = (mword*)cdr(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, cdr_TOS_0, IMMORTAL); // FIXME: Depends

    return this_bvm;

}

//
bvm_cache *isnil(bvm_cache *this_bvm){

    mword *result    = new_atom;
    
    *result = is_nil((mword*)TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//consls
//#define cons(a,b,c) car(a) = (mword)(b); cdr(a) = (mword)(c);
bvm_cache *consls(bvm_cache *this_bvm){

    mword *result = _consls((mword*)TOS_1(this_bvm), (mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
mword *_consls(mword *car_field, mword *cdr_field){

    mword *temp_cons = new_cons;

    cons(temp_cons, car_field, cdr_field);

    return temp_cons;

}

//
bvm_cache *uncons(bvm_cache *this_bvm){

    //FIXME: No checking...
    mword *temp_car = (mword*)car(TOS_0(this_bvm));
    mword *temp_cdr = (mword*)cdr(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, temp_car, IMMORTAL); //FIXME: Depends
    push_alloc(this_bvm, temp_cdr, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

bvm_cache *list_end(bvm_cache *this_bvm){

}

mword *_list_end(mword *list){

    while(!is_nil(scdr(list))){ //FIXME Breaks due to car/cdr can't handle hash-refs
        list = (mword*)cdr(list);
    }
    return list;
    
}

mword *_list_next_to_end(mword *list){

    while(is_nil(scdr(scdr(list)))){
        list = (mword*)cdr(list);
    }
    return list;
    
}

//push
// Like Perl's
// ((a b c)) ((d e f)) push --> (a b c d e f)
bvm_cache *push(bvm_cache *this_bvm){

    mword *endls = _list_end((mword*)TOS_1(this_bvm));
    c(endls,1) = TOS_0(this_bvm);

    zap(this_bvm);
    // XXX Might get alloc-type bugs here...
//    push_alloc(this_bvm, temp_cons, PUSH);

    return this_bvm;

}

//pop
// Like Perl's
bvm_cache *pop(bvm_cache *this_bvm){

    mword *endls = _list_next_to_end((mword*)TOS_0(this_bvm));

    mword *temp = (mword*)c(endls,1);
    (mword*)c(endls,1) = nil;

    push_alloc(this_bvm, temp, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//unshift
// Like Perl's
// ((a b c)) ((d e f)) push --> (d e f a b c)
// A B unshift <--> A B swap push
bvm_cache *unshift(bvm_cache *this_bvm){

    swap(this_bvm);

    mword *endls = _list_end((mword*)TOS_1(this_bvm));
    c(endls,1) = TOS_0(this_bvm);

    zap(this_bvm);
    // XXX Might get alloc-type bugs here...
//    push_alloc(this_bvm, temp_cons, PUSH);

    return this_bvm;

}

//shift
// Like Perl's
bvm_cache *shift(bvm_cache *this_bvm){

    mword *temp = (mword*)TOS_0(this_bvm);
    TOS_0(this_bvm) = cdr(TOS_0(this_bvm));

    (mword*)c(temp,1) = nil;

    push_alloc(this_bvm, temp, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
bvm_cache *len(bvm_cache *this_bvm){ 

    mword *result = new_atom;

    *result = _len((mword*)TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}


mword _len(mword *list){

    mword length = 0;

    while(!is_nil(list)){
        length++;
        list = (mword*)cdr(list);
    }

    return length;
}

//
bvm_cache *bons(bvm_cache *this_bvm){

    mword *result = _bons((mword*)TOS_0(this_bvm));

    zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
mword *_bons(mword *list){

    mword *arr = _newin(_len(list));

    int i=0;

    while(!is_nil(list)){
        c(arr,i) = car(list);
        i++;
        list = (mword*)cdr(list);
    }

    return arr;

}

//
bvm_cache *ls2lf(bvm_cache *this_bvm){

    mword *result = _ls2lf((mword*)TOS_0(this_bvm));

    zap(this_bvm);

    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
mword *_ls2lf(mword *list){

    mword *arr = _newlf(_len(list));

    int i=0;
    while(!is_nil(list)){
        if(!is_leaf((mword*)scar(list))){
            die //FIXME: Exception
        }
        c(arr,i) = car(car(list));
        i++;
        list = (mword*)cdr(list);
    }

    return arr;

}

//
bvm_cache *ith(bvm_cache *this_bvm){

    mword *result = _ith((mword*)TOS_1(this_bvm), car(TOS_0(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
mword *_ith(mword *list, mword i){

    while(i > 0){
        i--;
        list = (mword*)cdr(list);
    }

    return (mword*)car(list);

}

//
bvm_cache *walk(bvm_cache *this_bvm){

    mword *result = _walk((mword*)TOS_1(this_bvm),(mword*)TOS_0(this_bvm));

    hard_zap(this_bvm);
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
mword *_walk(mword *bs, mword *walk_list){

    if (is_nil(walk_list)) return bs;

    if (!is_inte(bs)) error("_walk: Can't walk non-interior array");

    return _walk(_ith(bs,(mword)car(car(walk_list))),(mword*)cdr(walk_list));

}

//
bvm_cache *reverse(bvm_cache *this_bvm){

    mword *list = (mword*)TOS_0(this_bvm);

    hard_zap(this_bvm);

    mword *result = _reverse(this_bvm,(mword*)list,nil);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}


//
mword *_reverse(bvm_cache *this_bvm, mword *list, mword *new_cdr){

    mword *temp = (mword*)cdr(list);

    (mword*)cdr(list) = new_cdr;

    if(is_nil(temp))
        return list;

    return _reverse(this_bvm, temp, list);

}

// Clayton Bauman 2011

