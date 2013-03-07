// list.c
//

#include "babel.h"
#include "list.h"
#include "stack.h"
#include "bvm_opcodes.h"
#include "except.h"
#include "array.h"
#include "bstruct.h"

//
//
inline mword *_push(mword *list, mword *bs){ // _push#

    mword *endls = _list_end(list);
    (mword*)c(endls,1) = consa( bs, nil );

    return list;

}


//
//
inline mword *_pop(mword *list){ // _pop#

    if(is_nil(list)) return nil;

    mword *endls = _list_next_to_end(list);

    if(is_nil(endls)) return nil;

    mword *temp = (mword*)c(endls,1);
    (mword*)c(endls,1) = nil;

    return temp;

}


//
//
inline mword *_unshift(mword *list, mword *bs){ // _unshift#

    return consa( bs, list );

}


// FIXME: icar doesn't work with tlists...
//
inline mword *_shift(mword *list){ // _shift#

    if(is_nil(list)) return nil;

    mword *temp = (mword*)icar(list);

    if(is_nil(temp)) return nil;
    
    icar(list) = icdr(icar(list));
    (mword*)icdr(temp) = nil;

    return temp;

}


//
// babel_operator
bvm_cache *carindex(bvm_cache *this_bvm){
    
    fatal("stack fix not done");
    mword *car_TOS_0 = (mword*)scar(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, car_TOS_0, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
// babel_operator
bvm_cache *cdrindex(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *cdr_TOS_0 = (mword*)scdr(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, cdr_TOS_0, IMMORTAL); // FIXME: Depends

    return this_bvm;

}

//
// babel_operator
bvm_cache *isnil(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result    = new_atom;
    
    *result = is_nil(TOS_0(this_bvm));

//    d(car((mword*)TOS_0(this_bvm)))

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//consls
//#define cons(a,b,c) car(a) = (mword)(b); cdr(a) = (mword)(c);
// babel_operator
bvm_cache *consls(bvm_cache *this_bvm){

    mword *result = new_atom;
    (mword*)*result = _consls(TOS_1(this_bvm), TOS_0(this_bvm));

    hard_zap(this_bvm);
    hard_zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
//
mword *_consls(mword *car_field, mword *cdr_field){

    mword *temp_cons = new_cons;

    cons(temp_cons, car_field, cdr_field);

    return temp_cons;

}


// Allocating cons
//
mword *consa(mword *car_field, mword *cdr_field){ // consa#

    mword *temp_cons = new_cons;

    cons(temp_cons, car_field, cdr_field);

    return temp_cons;

}



//
// babel_operator
bvm_cache *uncons(bvm_cache *this_bvm){

    fatal("stack fix not done");
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

    while(!is_nil(scdr(scdr(list)))){
        list = (mword*)cdr(list);
    }
    return list;
    
}

//push
// Like Perl's
// ((a b c)) ((d e f)) push --> (a b c d e f)
// babel_operator
bvm_cache *push(bvm_cache *this_bvm){

    error("stack ordering fix not done");
    fatal("stack fix not done");
    mword *endls = _list_end(TOS_1(this_bvm));
    (mword*)c(endls,1) = TOS_0(this_bvm);

    zap(this_bvm);
    // XXX Might get alloc-type bugs here...
//    push_alloc(this_bvm, temp_cons, PUSH);

    return this_bvm;

}

//pop
// Like Perl's
// babel_operator
bvm_cache *pop(bvm_cache *this_bvm){

    error("stack ordering fix not done");
    fatal("stack fix not done");
    mword *endls = _list_next_to_end(TOS_0(this_bvm));

    mword *temp = (mword*)c(endls,1);
    (mword*)c(endls,1) = nil;

    push_alloc(this_bvm, temp, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//unshift
// Like Perl's
// ((a b c)) ((d e f)) push --> (d e f a b c)
// A B unshift <--> A B swap push
// babel_operator
bvm_cache *unshift(bvm_cache *this_bvm){

    error("stack ordering fix not done");
    fatal("stack fix not done");
    swap(this_bvm);

    mword *endls = _list_end(TOS_1(this_bvm));
    (mword*)c(endls,1) = TOS_0(this_bvm);

    zap(this_bvm);
    // XXX Might get alloc-type bugs here...
//    push_alloc(this_bvm, temp_cons, PUSH);

    return this_bvm;

}

//shift
// Like Perl's
// babel_operator
bvm_cache *shift(bvm_cache *this_bvm){

    error("stack ordering fix not done");
    fatal("stack fix not done");
    mword *temp = TOS_0(this_bvm);
    TOS_0(this_bvm) = (mword*)cdr(TOS_0(this_bvm));

    (mword*)c(temp,1) = nil;

    push_alloc(this_bvm, temp, IMMORTAL); //FIXME: Depends

    return this_bvm;

}


//
// babel_operator
bvm_cache *len(bvm_cache *this_bvm){ 

    fatal("stack fix not done");
    mword *result = new_atom;

    *result = _len(TOS_0(this_bvm));

    zap(this_bvm);
    push_alloc(this_bvm, result, MORTAL);

    return this_bvm;

}

//
mword _len(mword *list){

    mword length = 0;

    while(!is_nil(list)){
        length++;
        list = (mword*)cdr(list);
    }

    return length;
}

//
// babel_operator
bvm_cache *bons(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _bons(TOS_0(this_bvm));

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
// babel_operator
bvm_cache *ls2lf(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _ls2lf(TOS_0(this_bvm));

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
// babel_operator
bvm_cache *ith(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _ith(TOS_1(this_bvm), car(TOS_0(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}

//
//
mword *_ith(mword *list, mword i){ // _ith#

    while(i > 0){
        i--;
        list = (mword*)cdr(list);
    }

    return (mword*)car(list);

}

//
// babel_operator
bvm_cache *walk(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *result = _walk(TOS_1(this_bvm),TOS_0(this_bvm));

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
// babel_operator
bvm_cache *reverse(bvm_cache *this_bvm){

    fatal("stack fix not done");
    mword *list = TOS_0(this_bvm);

    hard_zap(this_bvm);

    mword *result = _reverse(this_bvm,(mword*)list,nil);

    push_alloc(this_bvm, result, IMMORTAL); //FIXME: Depends

    return this_bvm;

}


//
mword *_reverse(bvm_cache *this_bvm, mword *list, mword *new_cdr){

    mword *temp = (mword*)cdr(list);

    (mword*)icdr(list) = new_cdr;

    if(is_nil(temp))
        return list;

    return _reverse(this_bvm, temp, list);

}


//
//
bvm_cache *split(bvm_cache *this_bvm){ // split#

    mword *indices  = dstack_get(this_bvm,0);
    mword *list     = dstack_get(this_bvm,1);

    mword *result = _split(list, indices);

    popd(this_bvm);
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
mword *_split(mword *list, mword *indices){ // _split#

    return _rsplit(list,indices,0);

}


//
//
mword *_rsplit(mword *list, mword *indices, mword count){ // _rsplit#

    mword *orig_list = list;
    mword *temp_list;

    if (is_nil(indices)) return consa( orig_list, nil );

    if (is_nil(list)) return nil;// 

    mword curr_index = car(car(indices));

    if (curr_index < count) return consa( orig_list, nil );

    if (curr_index == 0) return consa( nil, consa( orig_list, nil ) );

    indices = (mword*)cdr(indices);

    //if (is_nil(list)) return list;
    
    mword *prev_list = list;

    while(!is_nil(list)){

//        if(count++ > 10){
//            _dump(list);
//            die;
//        }

        if(curr_index == count){
            break;
        }

        count++;

        prev_list = list;
        list = (mword*)cdr(list);

    }

    if(!is_nil(cdr(prev_list))){
        (mword*)icdr(prev_list) = nil;
    }

    return consa( orig_list, _rsplit(list, indices, count) );

}

// Clayton Bauman 2011

