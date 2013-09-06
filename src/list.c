// list.c
//

#include "babel.h"
#include "list.h"
#include "stack.h"
#include "bvm_opcodes.h"
#include "except.h"
#include "array.h"
#include "bstruct.h"
#include "tptr.h"


/* list operator
**ins**  
> Inserts one list into another
> `X Y ins| -> X with Y inserted after first list element`
*/
bvm_cache *insls(bvm_cache *this_bvm){ // insls#

    mword *src_list  = dstack_get(this_bvm,0);
    mword *dest_list = dstack_get(this_bvm,1); // XXX: User-dependent
    popd(this_bvm);

    mword *end_src_list   = _list_end(src_list);
    mword *next_dest_list = (mword*)icdr(dest_list);

    (mword*)c(dest_list,1) = src_list;
    (mword*)c(end_src_list,1)  = next_dest_list;

    return this_bvm;

}


//
//
inline mword *_unshift(mword *list, mword *bs){ // _unshift#

    mword *endls = _list_end(list);
    (mword*)c(endls,1) = consa( bs, nil );

    return list;

}


//
//
inline mword *_shift(mword *list){ // _shift#

    if(is_nil(list)) return nil;

    mword *endls = _list_next_to_end(list);

    if(is_nil(endls)) return nil;

    mword *temp = (mword*)c(endls,1);
    (mword*)c(endls,1) = nil;

    return temp;

}


//
//
inline mword *_push(mword *list, mword *bs){ // _push#

    return consa( bs, list );

}


// FIXME: icar doesn't work with tptrs...
//
inline mword *_pop(mword *list){ // _pop#

    if(is_nil(list)) return nil;

    mword *temp = (mword*)icdr(list);
    (mword*)icdr(list) = nil;

    return temp;

//    if(is_nil(temp)) return nil;
//    
//    icar(list) = icdr(icar(list));
//    (mword*)icdr(temp) = nil;
//
//    return temp;

}


/* list operator
**car**  
> Named after the venerable Lisp operator  
> Returns the left side of a cons  
>  
> `[cons(X,Y)]| -> {X}|`    
> `[cons(X,Y)]| -> [X]|`    
>  
> Equivalent to: 0 cxr (see above)  
*/
bvm_cache *carindex(bvm_cache *this_bvm){ // carindex#
    
    //FIXME: Exception if used on leaf-array
    mword *result = (mword*)car(dstack_get(this_bvm,0));
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* list operator
**cdr**  
> Named after the venerable Lisp operator  
> Returns the right side of a cons  
>  
> `[cons(X,Y)]| -> {Y}|`    
> `[cons(X,Y)]| -> [Y]|`    
*/
bvm_cache *cdrindex(bvm_cache *this_bvm){ // cdrindex#

    //FIXME: Exception if used on leaf-array
    mword *result = (mword*)cdr(dstack_get(this_bvm,0));
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* list operator
**isnil**  
> Tests if TOS is nil  
> `[nil]| -> {1}|`    
> `{X}|   -> {0}|`    
> `[X]|   -> {0}|`    
>  
> For all X != nil  
*/
bvm_cache *isnil(bvm_cache *this_bvm){ // isnil#

    mword *result = _newva(is_nil(dstack_get(this_bvm,0)));
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* list operator
**cons**  
> Named after the venerable Lisp operator  
> Constructs one list from two  
> `[X] [Y]| -> [cons(X, Y)]|`    
>
> Note:   
> `[X] [Y] cons car`     Leaves `[X]` on TOS  
> `[X] [Y] cons cdr`     Leaves `[Y]` on TOS  
*/
bvm_cache *consls(bvm_cache *this_bvm){ // consls#

    mword *result = consa(
                        dstack_get(this_bvm,1),
                        dstack_get(this_bvm,0));
    popd(this_bvm);
    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

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



/* list operator
**uncons**  
> Undoes cons  
> `[cons(X, Y)]| -> [X] [Y]|`    
*/
bvm_cache *uncons(bvm_cache *this_bvm){ // uncons#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *temp_car = (mword*)car(op0);
    mword *temp_cdr = (mword*)cdr(op0);

    pushd(this_bvm, temp_car, IMMORTAL);
    pushd(this_bvm, temp_cdr, IMMORTAL);

    return this_bvm;


}


//
//
bvm_cache *list_end(bvm_cache *this_bvm){

}


//
//
mword *_list_end(mword *list){ // _list_end#

    while(!is_nil(cdr(list))){
        list = (mword*)cdr(list);
    }
    return list;
    
}

mword *_list_next_to_end(mword *list){

    while(!is_nil(cdr(cdr(list)))){
        list = (mword*)cdr(list);
    }
    return list;
    
}


/* list operator
**unshift**  
> Like Perl's `pop` function   
> Undoes shift  
>  
> `(a b) (c) unshift --> leaves (a b c) on TOS`    
>  
> `A B unshift <--> A B swap push`    
*/
bvm_cache *unshift(bvm_cache *this_bvm){ // unshift#

    mword *dest_list = get_from_udr_stack(this_bvm, this_bvm->dstack_ptr, 1);
    mword *push_list = (mword*)icar(popd(this_bvm));

//    _dump(dest_list);
//    die;

    mword *endls = _list_end(dest_list);
    (mword*)c(endls,1) = push_list;

    //zap(this_bvm);
    // XXX Might get alloc-type bugs here...
//    push_alloc(this_bvm, temp_cons, PUSH);

    return this_bvm;

}

/* list operator
**pop**  
> Like Perl's `shift` function   
> Undoes push of length-1 list  
>  
> `(a b c) pop --> leaves (b c) (a) on TOS`    
*/
bvm_cache *pop(bvm_cache *this_bvm){ // pop#

    mword *list = get_from_udr_stack(this_bvm, this_bvm->dstack_ptr, 0);

    set_in_udr_stack(this_bvm, this_bvm->dstack_ptr, 0, (mword*)icdr(list));

    (mword*)c(list,1) = nil;

    pushd(this_bvm, list, IMMORTAL);

    return this_bvm;

}

/* list operator
**push**  
> Like Perl's `unshift` operator  
> Undoes pop  
>  
> `(b c) (a) push --> leaves (a b c) on TOS`    
*/
bvm_cache *push(bvm_cache *this_bvm){ // push#

    mword *dest_list = get_from_udr_stack(this_bvm, this_bvm->dstack_ptr, 0);
    mword *push_list = getd(this_bvm, 1);

    mword *endls = _list_end(dest_list);
    (mword*)c(endls,1) = push_list;

    return this_bvm;

}

/* list operator
**shift**  
> Like the Perl operator  
> Undoes unshift of length 1 list  
>  
> `(a b c) shift --> leaves (a b) (c) on TOS`    
*/
bvm_cache *shift(bvm_cache *this_bvm){ // shift#

    mword *list = get_from_udr_stack(this_bvm, this_bvm->dstack_ptr, 0);
    mword *endls = _list_next_to_end(list);

    mword *shifted = (mword*)c(endls,1);
    (mword*)c(endls,1) = nil;

    pushd(this_bvm, shifted, IMMORTAL);

    return this_bvm;

}


/* list operator
**len** (##)  
> List length  
*/
bvm_cache *len(bvm_cache *this_bvm){ // len#

    mword *list = getd(this_bvm,0);
    mword *result = _newva( _len( list ) );

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
mword _len(mword *list){ // _len#

    mword length = 0;

    while(!is_nil(list)){
        length++;
        list = (mword*)cdr(list);
    }

    return length;

}


/* list operator
**bons**  
> Undoes `ar2ls` on an interior-array.  
>  
> Mnemonic: "Babel cons"... since it's like consing together an entire list 
> into an "oversize" cons box.  
*/
bvm_cache *bons(bvm_cache *this_bvm){ // bons#

    mword *list = getd(this_bvm,0);
    mword *result = _bons(list);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
mword *_bons(mword *list){ // _bons#

    mword *arr = _newin(_len(list));

    int i=0;

    while(!is_nil(list)){
        c(arr,i) = car(list);
        i++;
        list = (mword*)cdr(list);
    }

    return arr;

}


/* list operator
**ls2lf**  
> Undoes `ar2ls` on a leaf-array  
*/
bvm_cache *ls2lf(bvm_cache *this_bvm){ // ls2lf#

    mword *list = dstack_get(this_bvm,0);
    mword *result = _ls2lf(list);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
mword *_ls2lf(mword *list){ // _ls2lf#

    mword *arr = _newlf(_len(list));

    int i=0;
    while(!is_nil(list)){
        if(!is_leaf((mword*)icar(list))){
            die; //FIXME: Exception
        }
        c(arr,i) = car(car(list));
        i++;
        list = (mword*)cdr(list);
    }

    return arr;

}


/* list operator
**ith**  
> Returns the ith element of a list.  Same as th operator, except for 
> lists.  
> Mnemonic: lIst -> Ith  
>
> `('a' 'b' 'c' 'd') 2 ith --> leaves 'c' on TOS`    
*/
bvm_cache *ith(bvm_cache *this_bvm){ // ith#

    mword *list  = getd(this_bvm,1);
    mword *index = getd(this_bvm,0);

    mword *result = _ith(list, c(index,0));

    pushd(this_bvm, result, IMMORTAL);

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


// XXX Consider deprecating as this is easily done in pure Babel code...
/* list operator
**walk**  
> Walks a list  
>  
> `(X) (Y) walk`    
>  
> Traverses list X according to the sequence of offsets specified
> in list Y. Performs an iterated ith.  
*/
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

/* list operator
**rev**  
> `(1 2 3) rev -> leaves (3 2 1) on TOS`    
*/
bvm_cache *reverse(bvm_cache *this_bvm){ // reverse#

    mword *list = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword *result = _reverse(list,nil);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


//
//
mword *_reverse(mword *list, mword *new_cdr){ // _reverse#

    mword *temp = (mword*)cdr(list);

    (mword*)icdr(list) = new_cdr;

    if(is_nil(temp))
        return list;

    return _reverse(temp, list);

}


/* list operator
**part**  
> Partitions a list - opposite of `append`  
> `( 'a' 'b' 'c' 'd' 'e' 'f' 'g') (1 3 5) part`    
> -> leaves `( ('a') ('b' 'c') ('d' 'e') ('f' 'g') ) on TOS`    
*/
bvm_cache *split(bvm_cache *this_bvm){ // split#

    mword *list     = getd(this_bvm,1);
    mword *indices  = getd(this_bvm,0);

    mword *result = _split(list, indices);

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


//
//
mword *_list_cut(mword *list, mword index){ // _list_cut#

    mword *temp;

    if(is_nil(list)) return nil;

    if(index == 0) return list;

    if(index == 1){

        temp = (mword*)icdr(list);
        (mword*)icdr(list) = nil;

        return temp;

    }

    return _list_cut( (mword*)icdr(list), index-1 );

}


/* list operator
**append**  
> Appends a list of lists. Opposite of `part`.  
> `( ('a') ('b' 'c') ('d' 'e') ('f' 'g') ) append`    
> --> leaves `('a' 'b' 'c' 'd' 'e' 'f' 'g') on TOS`    
*/
bvm_cache *append(bvm_cache *this_bvm){ // append#

    mword *list = dstack_get(this_bvm,0);

    //mword *result = _append_direct(_ith(list,0), _ith(list,1));
    mword *result = _append(list);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


mword *_append(mword *lists){ // _append#

    if(is_nil(icdr(lists)))
        return (mword*)icar(lists);
        //return lists;

    return _append_direct(_ith(lists,0),_append((mword*)icdr(lists)));

}


//
//
mword *_append_direct(mword *head_list, mword *tail_list){ // _append_direct#

    mword *endls = _list_end(head_list);

    (mword *)icdr(endls) = tail_list;

    return head_list;

}


/* list operator
**strint**  
**macint**  
**interpol**  
**cart**  
> Example:  
> `((1 2 3 ) ( 4 5 6 )) cart`  
> ` TOS: (((1 4) (1 5) (1 6)) ((2 4) (2 5) (2 6)) ((3 4) (3 5) (3 6)))`  
>
> Works for all dimensions  

**transpose**  
> `(( 1  2  3  4 ) (  5  6  7  8  9 ) (  10 11 12 13 14 15 )) transpose`  
> `TOS: ((1 5 10) (2 6 11) (3 7 12) (4 8 13) (nil 9 13) (nil nil 15))`  

**sort**  
**uniq**  
**filter**
> Equivalent to Perl's grep
> `( 2 cugt ) ( 1 2 3 4 5 ) filter`  
> Leaves ( 3 4 5 ) on the stack
*/

// Clayton Bauman 2011

