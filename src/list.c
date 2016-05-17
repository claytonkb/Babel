// list.c
//

#include "babel.h"
#include "list.h"
#include "bstruct.h"
#include "tptr.h"
#include "operator.h"
#include "array.h"
#include "string.h"


// creates a new list of given size
// note that the list is created in REVERSE order:
// _newls(this_bvm, 3, _val(this_bvm,3), _val(this_bvm,2), _val(this_bvm,1)) --> (1 2 3)
//
mword *_mkls(bvm_cache *this_bvm, mword list_size, ...){ // _mkls#

    va_list vl;
    va_start(vl,list_size);

    int i;

    mword *last_cons = nil;

    for(i=0;i<list_size;i++){
        last_cons = (void*)_cons(this_bvm, va_arg(vl,mword*), last_cons);
    }

    va_end(vl);

    return last_cons;

}


// creates a new circular, doubly-linked list of given size
// note that the list is created in REVERSE order
//
mword *_mkdls(bvm_cache *this_bvm, mword list_size, ...){ // _mkdls#

    va_list vl;
    va_start(vl,list_size);

    int i;

    mword *last_dcons  = nil;
    mword *this_dcons  = va_arg(vl,mword*);
           last_dcons  = (void*)_dcons(this_bvm, this_dcons, last_dcons, nil);
    mword *first_dcons = last_dcons;

    for(i=1;i<list_size;i++){
        this_dcons = va_arg(vl,mword*);
        this_dcons = (void*)_dcons(this_bvm, this_dcons, last_dcons, nil);
        lci(last_dcons,2) = this_dcons;
        last_dcons = this_dcons;
    }

    va_end(vl);

    // close the ends to form a circle
    lci(last_dcons,2)  = first_dcons;
    lci(first_dcons,1) = last_dcons;

    return last_dcons;

}


//
//
mword *_cons(bvm_cache *this_bvm, mword *car, mword *cdr){ // _cons#

    mword **cons_cell = (mword**)_new_cons(this_bvm);
    lci(cons_cell,0) = car;
    lci(cons_cell,1) = cdr;

    return (mword*)cons_cell;

}


// Make a double-linked list cons cell (three entries instead of two)
//
mword *_dcons(bvm_cache *this_bvm, mword *car, mword *cdr, mword *cpr){ // _dcons#

    mword **dcons_cell = (mword**)_new_dcons(this_bvm);
    lci(dcons_cell,0) = car;
    lci(dcons_cell,1) = cdr;
    lci(dcons_cell,2) = cpr;

    return (mword*)dcons_cell;

}


// insert into double-list
//
mword *_insdls(bvm_cache *this_bvm, mword *src_list, mword *dest_list){ // _insdls#

    if(is_nil(dest_list)){
        return src_list;
    }
    
    if(is_nil(src_list)){
        return dest_list;
    }

    mword *next_dest_list = (mword*)icdr(dest_list);
    mword *end_src_list   = _dlist_end(this_bvm, src_list);

    lci(dest_list,    1) = src_list;
    lci(end_src_list, 1) = next_dest_list;
    lci(src_list,     2) = dest_list;

    if(is_nil(next_dest_list)){
        return dest_list;
    }

    lci(next_dest_list,2) = end_src_list;

    return dest_list;

}


// insert into double-list
//
mword *_insdls_prev(bvm_cache *this_bvm, mword *src_list, mword *dest_list){ // _insdls_prev#

    if(is_nil(dest_list)){
        return src_list;
    }

    if(is_nil(src_list)){
        return dest_list;
    }

    mword *prev_dest_list = (mword*)icpr(dest_list);
    mword *end_src_list   = _dlist_end(this_bvm, src_list);

    lci(end_src_list,  1) = dest_list;
    lci(dest_list,     2) = end_src_list;
    lci(src_list,      2) = prev_dest_list;

    lci(prev_dest_list,1) = src_list;

    return dest_list;

}


//
//
mword *_insls(bvm_cache *this_bvm, mword *src_list, mword *dest_list){ // _insls#

    if(is_nil(dest_list)){
        return src_list;
    }
    
    mword *next_dest_list = (mword*)icdr(dest_list);
    mword *end_src_list   = _list_end(this_bvm, src_list);

    lci(dest_list,1)      = src_list;
    lci(end_src_list,1)   = next_dest_list;

    return dest_list;

}


//
//
mword *_unshift(bvm_cache *this_bvm, mword *list, mword *bs){ // _unshift#

    mword *endls = _list_end(this_bvm, list);
    lci(endls,1) = _cons(this_bvm,  bs, nil );

    return list;

}


//
//
mword *_unshift_op(bvm_cache *this_bvm, mword *list, mword *bs){ // _unshift_op#

    mword *endls = _list_end(this_bvm, list);
    lci(endls,1) = bs;

    return list;

}


//
//
mword *_shift(bvm_cache *this_bvm, mword *list){ // _shift#

    if(is_nil(list)) return nil;

    if(_len(this_bvm,list) < 2) return list;

    mword *endls = _list_next_to_end(this_bvm, list);

    if(is_nil(endls)) return nil;

    mword *temp = (mword*)rci(endls,1);
    lci(endls,1) = nil;

    return temp;

}


// NOTE: _push does not update list to point to the new head, it
// is the caller's responsibility to maintain this
//
void _push(bvm_cache *this_bvm, mword *list, mword *bs){ // _push#

    lci(bs,1) = list;

}


//
//
mword *_pop(bvm_cache *this_bvm, mword *list){ // _pop#

    if(is_nil(list)) return nil;

    mword *temp = (mword*)icdr(list);
    lci(list,1) = nil;

    return temp;

}


//
//
mword *_dlist_end(bvm_cache *this_bvm, mword *list){ // _dlist_end#

    mword *head = list;
    
    while(!is_nil(cdr(list)) && head != cdr(list)){
        list = (mword*)cdr(list);
    }

    return list;
    
}


//
//
mword *_list_end(bvm_cache *this_bvm, mword *list){ // _list_end#

    while(!is_nil(cdr(list))){
        list = (mword*)cdr(list);
    }
    return list;
    
}


//
//
mword *_list_next_to_end(bvm_cache *this_bvm, mword *list){ // *_list_next_to_end#

    while(!is_nil(cdr(cdr(list)))){
        list = (mword*)cdr(list);
    }
    return list;
    
}


//
//
mword _len(bvm_cache *this_bvm, mword *list){ // _len#

    mword length = 0;

    while(!is_nil(list)){
        length++;
        list = (mword*)cdr(list);
    }

    return length;

}


//
//
mword _len_dlist(bvm_cache *this_bvm, mword *list){ // _len_dlist#

    mword length = 0;
    mword *head = list;

    while(!is_nil(list)){
        length++;
        if(head == cdr(list)){
            break; }
        list = (mword*)cdr(list);
    }

    return length;

}


//
//
mword *_bons(bvm_cache *this_bvm, mword *list){ // _bons#

    mword *head = list;
    mword *arr = (mword*)_newin(this_bvm, _len(this_bvm, list));
    mword *curr_node;

    int i=0;

    while(!is_nil(list)){
        curr_node = (mword*)car(list);
        if(curr_node == head){
            lci(arr,i) = arr;
        }
        else{
            lci(arr,i) = curr_node;
        }        
        i++;
        list = (mword*)cdr(list);
    }

    return arr;

}

//FIXME (CLEANUP): _ls2lf should use len() instead of re-writing it
//
mword *_ls2lf(bvm_cache *this_bvm, mword *list){ // _ls2lf#

    mword *head = list;
    int total_size=0;
    int list_length=0; // We don't use _len because we're already traversing the list

    while(!is_nil(list)){
        if(!is_leaf((mword*)icar(list))){
            _fatal("can't cat leaf and inte arrays"); //FIXME: Exception
        }
        total_size += size(car(list));
        list_length++;
        list = (mword*)cdr(list);        
    }

    if(!list_length){
        return (mword*)_newlfi(this_bvm, 1, 0);
    }

    mword *arr = (mword*)_newlfi(this_bvm, total_size, 0);

    list = head;

    int i=0;
    int element_size;
    while(!is_nil(list)){

        element_size = size(car(list));
        memmove(arr+i, (mword*)car(list), (size_t)BYTE_SIZE(element_size));
        i += element_size;
        list = (mword*)cdr(list);

    }

    return arr;

}


//
//
mword *_lscat8(bvm_cache *this_bvm, mword *list){ // _lscat8#

    mword *head = list;
    int total_size=0;
    int list_length=0; // We don't use _len because we're already traversing the list

    while(!is_nil(list)){
        if(!is_leaf((mword*)icar(list))){
            _fatal("can't cat leaf and inte arrays"); //FIXME: Exception
        }
        total_size += _arlen8(this_bvm, (mword*)icar(list));
        list_length++;
        list = (mword*)cdr(list);        
    }

    if(!list_length){
        return (mword*)_newlfi(this_bvm, 1, 0);
    }

    char *arr = (char*)_newstr(this_bvm, total_size, ' ');

    list = head;

    int i=0;
    int element_size;
    while(!is_nil(list)){

        element_size = _arlen8(this_bvm, (mword*)icar(list));
        memmove(arr+i, (char*)icar(list), (size_t)element_size);
        i += element_size;
        list = (mword*)cdr(list);

    }

    return (mword*)arr;

}


//
//
mword *_cdri(bvm_cache *this_bvm, mword *list, mword i){ // _cdri#

    while(i > 0){
        i--;
        list = (mword*)cdr(list);
    }

    return list;

}


//
//
mword *_ith(bvm_cache *this_bvm, mword *list, mword i){ // _ith#

    list = _cdri(this_bvm, list, i);

    return (mword*)car(list);

}


// This function assumes the dlist is well-formed
//
mword *_reverse_dlist(bvm_cache *this_bvm, mword *list, mword *head, mword direction){ // _reverse_dlist#

    if(is_nil(list))
        return nil;

    mword *next = rci(list,1+direction);
    mword *prev = rci(list,2-direction);

    lci(list,2-direction) = next;
    lci(list,1+direction) = prev;

//    mword *next = rci(list,1);
//    mword *prev = rci(list,2);
//
//    lci(list,2) = next;
//    lci(list,1) = prev;

    if(next == head || is_nil(next))
        return list;

    return _reverse_dlist(this_bvm, next, head, direction);

}


//
//
mword *_reverse(bvm_cache *this_bvm, mword *list, mword *new_cdr){ // _reverse#

    mword *temp = (mword*)cdr(list);

    lci(list,1) = new_cdr;

    if(is_nil(temp))
        return list;

    return _reverse(this_bvm, temp, list);

}


//
//
mword *_split(bvm_cache *this_bvm, mword *list, mword *indices){ // _split#

    return _rsplit(this_bvm, list,indices,0);

}


//
//
mword *_rsplit(bvm_cache *this_bvm, mword *list, mword *indices, mword count){ // _rsplit#

    mword *orig_list = list;

    if (is_nil(indices)) return _cons(this_bvm,  orig_list, nil );

    if (is_nil(list)) return nil;// 

    mword curr_index = icar(icar(indices));

    if (curr_index < count) return _cons(this_bvm,  orig_list, nil );

    if (curr_index == 0) return _cons(this_bvm,  nil, _cons(this_bvm,  orig_list, nil ) );

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
        lci(prev_list,1) = nil;
    }

    return _cons(this_bvm,  orig_list, _rsplit(this_bvm, list, indices, count) );

}


//
//
mword *_list_cut(bvm_cache *this_bvm, mword *list, mword index){ // _list_cut#

    mword *temp;

    if(is_nil(list)) return nil;

    if(index == 0) return list;

    if(index == 1){

        temp = (mword*)icdr(list);
        lci(list,1) = nil;

        return temp;

    }

    return _list_cut(this_bvm,  (mword*)icdr(list), index-1 );

}


//
//
mword *_dlist_cut(bvm_cache *this_bvm, mword *list, mword index, mword direction){ // _dlist_cut#

    mword *temp;

    if(is_nil(list)) return nil;

    if(index == 0) return list;

    if(direction == CDR_DIRECTION){
        temp = rci(list,1);
    }
    else{ //if(direction == CPR_DIRECTION)
        temp = rci(list,2);
    }

    if(index == 1){

        lci(list,1+direction) = nil;
        lci(temp,2-direction) = nil;

        return temp;

    }

    return _dlist_cut(this_bvm, temp, index-1, direction);

}


//
//
mword *_append(bvm_cache *this_bvm, mword *lists){ // _append#

    if(is_nil(icdr(lists)))
        return (mword*)icar(lists);
        //return lists;

    return _append_direct(this_bvm, _ith(this_bvm, lists,0),_append(this_bvm, (mword*)icdr(lists)));

}


///
//
mword *_append_direct(bvm_cache *this_bvm, mword *head_list, mword *tail_list){ // _append_direct#

    mword *endls = _list_end(this_bvm, head_list);

    lci(endls,1) = tail_list;

    return head_list;

}


//
//
mword *_append_direct_dlist(bvm_cache *this_bvm, mword *head_list, mword *tail_list){ // _append_direct_dlist#

    mword *endls = _dlist_end(this_bvm, head_list);

    lci(endls,     1) = tail_list;
    lci(tail_list, 2) = endls;

    return head_list;

}


//
//
mword *_ar2ls(bvm_cache *this_bvm, mword *arr){ // _ar2ls#

    mword *last_cons = (mword*)nil;
    int i;
    mword *entry;

    if(is_inte(arr)){
        for(i=size(arr)-1;i>=0;i--){
            last_cons = _cons(this_bvm, (mword*)rci(arr,i),last_cons);
        }
    }
    else{
        for(i=size(arr)-1;i>=0;i--){
            entry = _newlfi(this_bvm, 1, 0);
            *entry = rcl(arr,i);
            last_cons = _cons(this_bvm, entry,last_cons);
        }
    }

    return last_cons;

}


/*****************************************************************************
 *                                                                           *
 *                              LIST OPERATORS                               *
 *                                                                           *
 ****************************************************************************/


#define ITH_RD_OPERATIONS \
    result0 = _ith(this_bvm, oi1.data, *oi0.data);

OPERATORA_R2_W1_D(ith_rd, ITH_RD_OPERATIONS, 
        nil, OI_MASK_LEAF, 0, 0,
        nil, OI_MASK_INTE, 0, 0)


#define LEN_D_OPERATIONS \
    result0 = _val(this_bvm, _len_dlist(this_bvm, oi0.data));

OPERATORA_R1_W1_D(len_d, LEN_D_OPERATIONS, nil, OI_MASK_INTE, 0, 0)


#define AR2LS_D_OPERATIONS \
    result0 = _ar2ls(this_bvm, oi0.data);

OPERATORA_R1_W1_D(ar2ls_d, AR2LS_D_OPERATIONS, nil, OI_MASK_INTE|OI_MASK_LEAF, 0, 0)


#define BONS_D_OPERATIONS \
    result0 = _bons(this_bvm, oi0.data);

OPERATORA_R1_W1_D(bons_d, BONS_D_OPERATIONS, nil, OI_MASK_INTE, 0, 0)


#define CAR_RD_OPERATIONS \
    result0 = lcar(oi0.data);

OPERATORA_R1_W1_D(car_rd, CAR_RD_OPERATIONS, nil, OI_MASK_INTE, 0, 0)


#define CDR_RD_OPERATIONS \
    result0 = lcdr(oi0.data);

OPERATORA_R1_W1_D(cdr_rd, CDR_RD_OPERATIONS, nil, OI_MASK_INTE, 0, 0)


#define CONS_D_OPERATIONS \
    result0 = _cons(this_bvm, oi1.data, oi0.data);

OPERATORA_R2_W1_D(cons_d, CONS_D_OPERATIONS, 
        nil, OI_MASK_ANY, 0, 0,
        nil, OI_MASK_ANY, 0, 0)


#define UNCONS_D_OPERATIONS \
    result0 = lcar(oi0.data); \
    result1 = lcdr(oi0.data);

OPERATORA_R1_W2_D(uncons_d, UNCONS_D_OPERATIONS, 
        nil, OI_MASK_INTE, 0, 0)


#define SHIFT_D_OPERATIONS \
    result0 = oi0.data; \
    result1 = _shift(this_bvm, oi0.data);

OPERATORA_R1_W2_D(shift_d, SHIFT_D_OPERATIONS, 
        nil, OI_MASK_INTE, 0, 0)


#define UNSHIFT_D_OPERATIONS \
    result0 = _unshift_op(this_bvm, oi1.data, oi0.data);

//    result0 = _unshift(this_bvm, oi1.data, rci(oi0.data,0));

OPERATORA_R2_W1_D(unshift_d, UNSHIFT_D_OPERATIONS, 
        nil, OI_MASK_INTE, 0, 0,
        nil, OI_MASK_ANY, 0, 0)


//#define UNSHIFT_RD_OPERATIONS 
//    result0 = _unshift(this_bvm, oi1.data, rci(oi0.data,0));
//
//OPERATOR_R2_W1_D(unshift_d, UNSHIFT_RD_OPERATIONS, 
//        nil, OI_MASK_INTE, 0, 0,
//        nil, OI_MASK_ANY, 0, 0)


#define POP_D_OPERATIONS \
    result0 = _pop(this_bvm, oi0.data); \
    result1 = oi0.data; 

OPERATORA_R1_W2_D(pop_d, POP_D_OPERATIONS, 
        nil, OI_MASK_INTE, 0, 0)


#define PUSH_D_OPERATIONS \
    _push(this_bvm, oi1.data, oi0.data); \
    result0 = oi0.data;

OPERATORA_R2_W1_D(push_d, PUSH_D_OPERATIONS, 
        nil, OI_MASK_INTE, 0, 0,
        nil, OI_MASK_INTE, 0, 0)


#define LS2LF_D_OPERATIONS \
    result0 = _ls2lf(this_bvm, oi0.data);

OPERATORA_R1_W1_D(ls2lf_d, LS2LF_D_OPERATIONS, nil, OI_MASK_INTE, 0, 0)


#define SPLIT_D_OPERATIONS \
    result0 = _split(this_bvm, oi1.data, oi0.data);

OPERATORA_R2_W1_D(split_d, SPLIT_D_OPERATIONS, 
        nil, OI_MASK_INTE, 0, 0,
        nil, OI_MASK_INTE, 0, 0)


#define APPEND_D_OPERATIONS \
    result0 = _append(this_bvm, oi0.data);

OPERATORA_R1_W1_D(append_d, APPEND_D_OPERATIONS, nil, OI_MASK_INTE, 0, 0)


#define INS_D_OPERATIONS \
    result0 = _insls(this_bvm, oi0.data, oi1.data);

OPERATORA_R2_W1_D(ins_d, INS_D_OPERATIONS, 
        nil, OI_MASK_INTE, 0, 0,
        nil, OI_MASK_INTE, 0, 0)


#define REVERSE_D_OPERATIONS \
    result0 = _reverse(this_bvm, oi0.data, nil);

OPERATORA_R1_W1_D(rev_d, REVERSE_D_OPERATIONS, nil, OI_MASK_INTE, 0, 0)


//mword *_reverse(bvm_cache *this_bvm, mword *list, mword *new_cdr){ // _reverse#

//list operators
//--------------
//
//(a) (b c d)   append  --> (a b c d)
//(a b c d) (1) part    --> (a) (b c d)
//(a) (b c d)   cons    --> [(a) (b c d)]
//(b c d) (a)   push    --> (a b c d)
//(a b c d)     pop     --> (b c d) (a)
//(a b c) (d)   unshift --> (a b c d)
//(a b c d)     shift   --> (a b c) (d)


// Clayton Bauman 2014

