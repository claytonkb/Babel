// hash.c
//

#include "babel.h"
#include "hash.h"
#include "list.h"
#include "except.h"
#include "array.h"
#include "stack.h"
#include "pearson16.h"
#include "bvm.h"
#include "bvm_opcodes.h"
#include "ref.h"
#include "bstruct.h"

#define opposite_side(x) ((x==0)?1:0)

#define HASH_ENTRY_SIZE 5

#define HASH_ENTRY_REF(x) ((mword*)c(x,0))
#define HASH_ENTRY_VAL(x) ((mword*)c(x,1))
#define HASH_ENTRY_KEY(x) ((mword*)c(x,2))
#define HASH_ENTRY_CNT(x) ((mword*)c(x,3))
#define HASH_ENTRY_BND(x) ((mword*)c(x,4))

//
mword *new_hash_entry(mword *hash, mword *key, mword *val, mword ref_count, mword bounding){

    mword *href = _newref(hash);

    mword *ref_count_ptr = new_atom;
    *ref_count_ptr = ref_count;

    mword *bounding_ptr = new_atom;
    *bounding_ptr = bounding;

    mword *hash_entry = _newin(HASH_ENTRY_SIZE);

//    (mword*)c(hash_entry,0) = href;
//    (mword*)c(hash_entry,1) = val;
//    (mword*)c(hash_entry,2) = key;
//    (mword*)c(hash_entry,3) = ref_count_ptr;
//    (mword*)c(hash_entry,4) = bounding_ptr;

    HASH_ENTRY_REF(hash_entry) = href;
    HASH_ENTRY_VAL(hash_entry) = val;
    HASH_ENTRY_KEY(hash_entry) = key;
    HASH_ENTRY_CNT(hash_entry) = ref_count_ptr;
    HASH_ENTRY_BND(hash_entry) = bounding_ptr;

    return hash_entry;

}

// inskha - insert with key into hash
bvm_cache *inskha(bvm_cache *this_bvm){

    mword *hash_table = (mword*)TOS_2(this_bvm);
    mword *key        = (mword*)TOS_1(this_bvm);
    mword *val        = (mword*)TOS_0(this_bvm);

    mword *result = _inskha(hash_table, key, val);

    hard_zap(this_bvm);
    hard_zap(this_bvm);
//    hard_zap(this_bvm);

    push_alloc(this_bvm, result, INSKHA);

//printf("%s\n\n",_bs2gv(this_bvm->stack_ptr));

}

//
mword *_inskha(mword *hash_table, mword *key, mword *val){

    mword *result;
    mword *temp;
    mword cons_side;

    mword *hash = _hash8(key);

    if(is_nil(hash_table)){
        cons_side = _cxr1(hash,0);
        temp = new_hash_entry(hash, key, val, (mword)-1, 0);
        result = new_cons;
        c(result,cons_side) = (mword)temp;
//printf("%s",_bs2gv(temp));
    }
    else{
        _rinskha(hash_table, hash, key, val, 0);
        result = hash_table;
    }

    return result;

}

//
void _rinskha(mword *hash_table, mword *hash, mword *key, mword *val, mword level){

    mword *temp;
    mword cons_side   = _cxr1(hash,level);
    mword *next_level = (mword*)c(hash_table,cons_side);

    // 1. cons_side = nil
    //      insert
    // 2. cons_side is inte AND size = 2
    //      recurse
    // 3. cons_side is inte AND size = HASH_ENTRY_SIZE
    //      split and insert

//    printf("%x\n",s(next_level));

    if(is_nil(next_level)){

        (mword*)c(hash_table,cons_side) 
            = new_hash_entry(hash, key, val, (mword)-1, 0);

    }
    else if(is_inte(next_level) && size(next_level) == 2){

        _rinskha((mword*)c(hash_table,cons_side), hash, key, val, level+1);

    }
    else if(is_inte(next_level) && size(next_level) == HASH_ENTRY_SIZE){

        mword *colliding_hash_ref           = HASH_ENTRY_REF(next_level);
        mword colliding_next_cons_side      = _cxr1(colliding_hash_ref,level+1);

        temp = new_cons;
        (mword *)c(temp,colliding_next_cons_side) = next_level;

        (mword*)c(hash_table,cons_side)     = temp;

        _rinskha((mword*)c(hash_table,cons_side), hash, key, val, level+1);

    }
    else{
        //printf("%s",_bs2gv(next_level));
        //die
        error("_rinskha: Error inserting into hash");
    }

}

//void inskha(void){
//
//    mword *hash_table = (mword*)TOS_2;
//    mword *key        = (mword*)TOS_1;
//    mword *val        = (mword*)TOS_0;
//
////    mword *hash_init = new_hash();
////    mword *hash = _pearson16(hash_init, key, (mword)strlen((char*)key));
////
//
//    mword *result = _inskha(hash_table, key, val);
//
//    zap();
//    zap();
//    zap();
//
//    push_alloc(result, INSHA);
//
//}
//
//mword *_inskha(mword *hash_table, mword *key, mword *val){
//
//    mword *hash_init = new_hash();
//    mword *hash = _pearson16(hash_init, key, (mword)strlen((char*)key));
//
////    char *buffer = (char*)_newlf(1000);
////    mword discard = tree_bbl2str(hash,buffer);
////    printf("%s\n",buffer);
////
//    mword *result;
//    mword *temp;
//    mword cons_side;
//
//    if(hash_table == (mword*)nil){
//        cons_side = _cxr1(hash,0);
//        temp = new_hash_entry(hash, val, key, (mword)-1);
//        result = _newin(2);
//        c(result,cons_side) = (mword)temp;
//    }
//    else{
//        _inskha_tree(hash_table, hash, key, val, 0);
//        result = hash_table;
//    }
//
//    return result;
//
//}
//
//
//void _inskha_tree(mword *hash_table, mword *hash, mword *key, mword *val, mword level){
//
//    mword cons_side = _cxr1(hash,level);
//    mword next_cons_side;
//    mword occupant_cons_side;
//    mword *temp;
//
//    if(is_href((mword*)car(c(hash_table,cons_side)))){
//        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
//            car(cdr(c(hash_table,cons_side))) = (mword)val;
//        }
//        else{
//            temp = (mword*)c(hash_table,cons_side);
//            c(hash_table,cons_side) = (mword)_newin(2);
//            next_cons_side = _cxr1((mword*)car(temp),level+1);
//            c((mword*)c(hash_table,cons_side),next_cons_side) = (mword)temp;
//            _inskha_tree((mword*)c(hash_table,cons_side), hash, key, val, level+1);
//        }
//    }
//    else if(c(hash_table,cons_side) == nil){
//        temp = new_hash_entry(hash, val, key, (mword)-1);
//        c(hash_table,cons_side) = (mword)temp;
//    }
//    else{
//        _inskha_tree((mword*)c(hash_table,cons_side), hash, key, val, level+1);
//    }
//
//}
//
//
//// hash_table hash insha
//void exha(void){
//
//    mword *hash_table = (mword*)TOS_1;
//    mword *hash       = (mword*)TOS_0;
//
//    mword *result = _exha(hash_table,hash);
//
//    zap();
//
//    push_alloc(result, EXHA);
//
//}
//
//mword *_exha(mword *hash_table, mword *hash){
//
//    mword *result = new_atom();
//
//    if(hash_table == (mword*)nil){
//        *result = 0;
//    }
//    else{
//        *result = _exha_tree(hash_table, hash, 0);
//    }
//
//    return result;
//
//}
//
//mword _exha_tree(mword *hash_table, mword *hash, mword level){
//
//    mword cons_side = _cxr1(hash,level);
//    mword next_cons_side;
//    mword occupant_cons_side;
//    mword *temp;
//
//    if(is_href((mword*)car(c(hash_table,cons_side)))){
//        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
//            return 1;
//        }
//        else{
//            return 0;
//        }
//    }
//    else if(c(hash_table,cons_side) == nil){
//        return 0;
//    }
//    else{
//        return _exha_tree((mword*)c(hash_table,cons_side), hash, level+1);
//    }
//
//}
//
//void luha(void){
//
//    mword *hash_table = (mword*)TOS_1;
//    mword *hash       = (mword*)TOS_0;
//
//    mword *result = _luha(hash_table,hash);
//
//    zap();
//
//    push_alloc(result, LUHA);
//
//}
//
//mword *_luha(mword *hash_table, mword *hash){
//
//    mword *result;
//
//    if(hash_table == (mword*)nil){
//        result = (mword*)nil;
//    }
//    else{
//        result = _luha_tree(hash_table, hash, 0);
//    }
//
//    return result;
//
//}
//
//
//mword *_luha_tree(mword *hash_table, mword *hash, mword level){
//
//    mword cons_side = _cxr1(hash,level);
////    mword *tempB;
//
//    if(is_href((mword*)car(c(hash_table,cons_side)))){
//        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
//            return (mword*)car(cdr(c(hash_table,cons_side)));
//        }
//        else{
//            return (mword*)nil;
//        }
//    }
//    else if(c(hash_table,cons_side) == nil){
//        return (mword*)nil;
//    }
//    else{
//        return _luha_tree((mword*)c(hash_table,cons_side), hash, level+1);
//    }
//
//}
//
//void rmha(void){
//
//    mword *hash_table = (mword*)TOS_1;
//    mword *hash       = (mword*)TOS_0;
//
//    _rmha(hash_table,hash);
//
//    zap();
//
//}
//
//void _rmha(mword *hash_table,mword *hash){
//
//    mword *sink;
//
//    if(hash_table == (mword*)nil){
//        return;
//    }
//    else{
//        sink = _rmha_tree(hash_table, hash, 0);
//    }
//
//}
//
//mword *_rmha_tree(mword *hash_table, mword *hash, mword level){
//
//    mword cons_side = _cxr1(hash,level);
//    mword *temp;
//
//    if(is_href((mword*)car(c(hash_table,cons_side)))){
//        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
//            c(hash_table,cons_side) = nil;
//            return (mword*)c(hash_table,opposite_side(cons_side));
//        }
//        else{
//            return (mword*)nil;
//        }
//    }
//    else if(c(hash_table,cons_side) == nil){
//        return (mword*)nil;
//    }
//    else{
//        temp = _rmha_tree((mword*)c(hash_table,cons_side), hash, level+1);
//        if(temp == (mword*)nil){
//            return (mword*)nil;
//        }
//        else{
//            if((c(hash_table,opposite_side(cons_side)) == nil) && (level > 0)){
//                return temp;
//            }
//            else{
//                (mword*)c(hash_table,cons_side) = temp;
//                return (mword*)nil;
//            }
//        }
//    }
//}
//
//// hash_table hash insha
//void keysha(void){
//
//    mword *hash_table = (mword*)TOS_0;
//
//    mword **result = (mword**)_newin(1);
//    *result = (mword*)nil;
//
//    _keysha(hash_table, result);
//
//    zap();
//
//    push_alloc(*result, KEYSHA);
//
//}
//
//void _keysha(mword *hash_table, mword **list){
//
//    mword *temp;
//
//    if(hash_table==(mword*)nil){
//        return;
//    }
//    else if(is_href((mword*)car(hash_table))){
////        printf("B");
//        temp = new_cons();
//        cons(temp,(mword*)car(hash_table),*list);
//        *list = temp;
//        return;
//    }
//    else{
////        printf("C");
//        _keysha((mword*)car(hash_table),list);
//        _keysha((mword*)cdr(hash_table),list);
//        return;
//    }
////        printf("D");
//
//}

// Clayton Bauman 2011

