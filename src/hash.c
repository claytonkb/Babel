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

#define opposite_side(x) ((x==0)?1:0)

//void push_alloc(mword *operand, mword alloc_type){

// make hash-entry
mword *new_hash_entry(mword *hash, mword *val, mword *key, mword ref_count){

    mword *href = _newref(hash);
    mword *ref_count_ptr = new_atom();
    *ref_count_ptr = ref_count;

    mword *temp_consA = new_cons();
    mword *temp_consB = new_cons();
    mword *temp_consC = new_cons();
    mword *temp_consD = new_cons();

    cons(temp_consA,  ref_count_ptr,  nil       );
    cons(temp_consB,  key,            temp_consA);
    cons(temp_consC,  val,            temp_consB);
    cons(temp_consD,  href,           temp_consC);

    return temp_consD;

}


void insha(void){

    mword *hash_table = (mword*)TOS_2;
    mword *hash       = (mword*)TOS_1;
    mword *val        = (mword*)TOS_0;

    mword *result = _insha(hash_table, hash, val);

    zap();
    zap();
    zap();

    push_alloc(result, INSHA);

}

mword *_insha(mword *hash_table, mword *hash, mword *val){

    mword *result;
    mword *temp;
    mword cons_side;

    if(hash_table == (mword*)nil){
        cons_side = _cxr1(hash,0);
        temp = new_hash_entry(hash, val, (mword*)nil, (mword)-1);
        result = _newin(2);
        c(result,cons_side) = (mword)temp;
    }
    else{
        _insha_tree(hash_table, hash, val, 0);
        result = hash_table;
    }

    return result;

}

void _insha_tree(mword *hash_table, mword *hash, mword *val, mword level){

    mword cons_side = _cxr1(hash,level);
    mword next_cons_side;
    mword occupant_cons_side;
    mword *temp;

    if(is_href((mword*)car(c(hash_table,cons_side)))){
        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
            car(cdr(c(hash_table,cons_side))) = (mword)val;
        }
        else{
            temp = (mword*)c(hash_table,cons_side);
            c(hash_table,cons_side) = (mword)_newin(2);
            next_cons_side = _cxr1((mword*)car(temp),level+1);
            c((mword*)c(hash_table,cons_side),next_cons_side) = (mword)temp;
            _insha_tree((mword*)c(hash_table,cons_side), hash, val, level+1);
        }
    }
    else if(c(hash_table,cons_side) == nil){
        temp = new_hash_entry(hash, val, (mword*)nil, (mword)-1);
        c(hash_table,cons_side) = (mword)temp;
    }
    else{
        _insha_tree((mword*)c(hash_table,cons_side), hash, val, level+1);
    }

}

void inskha(void){

    mword *hash_table = (mword*)TOS_2;
    mword *key        = (mword*)TOS_1;
    mword *val        = (mword*)TOS_0;

//    mword *hash_init = new_hash();
//    mword *hash = _pearson16(hash_init, key, (mword)strlen((char*)key));
//

    mword *result = _inskha(hash_table, key, val);

    zap();
    zap();
    zap();

    push_alloc(result, INSHA);

}

mword *_inskha(mword *hash_table, mword *key, mword *val){

    mword *hash_init = new_hash();
    mword *hash = _pearson16(hash_init, key, (mword)strlen((char*)key));

//    char *buffer = (char*)_newlf(1000);
//    mword discard = tree_bbl2str(hash,buffer);
//    printf("%s\n",buffer);
//
    mword *result;
    mword *temp;
    mword cons_side;

    if(hash_table == (mword*)nil){
        cons_side = _cxr1(hash,0);
        temp = new_hash_entry(hash, val, key, (mword)-1);
        result = _newin(2);
        c(result,cons_side) = (mword)temp;
    }
    else{
        _inskha_tree(hash_table, hash, key, val, 0);
        result = hash_table;
    }

    return result;

}


void _inskha_tree(mword *hash_table, mword *hash, mword *key, mword *val, mword level){

    mword cons_side = _cxr1(hash,level);
    mword next_cons_side;
    mword occupant_cons_side;
    mword *temp;

    if(is_href((mword*)car(c(hash_table,cons_side)))){
        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
            car(cdr(c(hash_table,cons_side))) = (mword)val;
        }
        else{
            temp = (mword*)c(hash_table,cons_side);
            c(hash_table,cons_side) = (mword)_newin(2);
            next_cons_side = _cxr1((mword*)car(temp),level+1);
            c((mword*)c(hash_table,cons_side),next_cons_side) = (mword)temp;
            _inskha_tree((mword*)c(hash_table,cons_side), hash, key, val, level+1);
        }
    }
    else if(c(hash_table,cons_side) == nil){
        temp = new_hash_entry(hash, val, key, (mword)-1);
        c(hash_table,cons_side) = (mword)temp;
    }
    else{
        _inskha_tree((mword*)c(hash_table,cons_side), hash, key, val, level+1);
    }

}


// hash_table hash insha
void exha(void){

    mword *hash_table = (mword*)TOS_1;
    mword *hash       = (mword*)TOS_0;

    mword *result = _exha(hash_table,hash);

    zap();

    push_alloc(result, EXHA);
    
}

mword *_exha(mword *hash_table, mword *hash){

    mword *result = new_atom();

    if(hash_table == (mword*)nil){
        *result = 0;
    }
    else{
        *result = _exha_tree(hash_table, hash, 0);
    }

    return result;

}

mword _exha_tree(mword *hash_table, mword *hash, mword level){

    mword cons_side = _cxr1(hash,level);
    mword next_cons_side;
    mword occupant_cons_side;
    mword *temp;

    if(is_href((mword*)car(c(hash_table,cons_side)))){
        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(c(hash_table,cons_side) == nil){
        return 0;
    }
    else{
        return _exha_tree((mword*)c(hash_table,cons_side), hash, level+1);
    }

}

void luha(void){

    mword *hash_table = (mword*)TOS_1;
    mword *hash       = (mword*)TOS_0;

    mword *result = _luha(hash_table,hash);

    zap();

    push_alloc(result, LUHA);

}

mword *_luha(mword *hash_table, mword *hash){

    mword *result;

    if(hash_table == (mword*)nil){
        result = (mword*)nil;
    }
    else{
        result = _luha_tree(hash_table, hash, 0);
    }

    return result;

}


mword *_luha_tree(mword *hash_table, mword *hash, mword level){

    mword cons_side = _cxr1(hash,level);
//    mword *tempB;

    if(is_href((mword*)car(c(hash_table,cons_side)))){
        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
            return (mword*)car(cdr(c(hash_table,cons_side)));
        }
        else{
            return (mword*)nil;
        }
    }
    else if(c(hash_table,cons_side) == nil){
        return (mword*)nil;
    }
    else{
        return _luha_tree((mword*)c(hash_table,cons_side), hash, level+1);
    }

}

void rmha(void){

    mword *hash_table = (mword*)TOS_1;
    mword *hash       = (mword*)TOS_0;

    _rmha(hash_table,hash);

    zap();

}

void _rmha(mword *hash_table,mword *hash){

    mword *sink;

    if(hash_table == (mword*)nil){
        return;
    }
    else{
        sink = _rmha_tree(hash_table, hash, 0);
    }

}

mword *_rmha_tree(mword *hash_table, mword *hash, mword level){

    mword cons_side = _cxr1(hash,level);
    mword *temp;

    if(is_href((mword*)car(c(hash_table,cons_side)))){
        if(!_arcmp((mword*)car(c(hash_table,cons_side)),hash)){
            c(hash_table,cons_side) = nil;
            return (mword*)c(hash_table,opposite_side(cons_side));
        }
        else{
            return (mword*)nil;
        }
    }
    else if(c(hash_table,cons_side) == nil){
        return (mword*)nil;
    }
    else{
        temp = _rmha_tree((mword*)c(hash_table,cons_side), hash, level+1);
        if(temp == (mword*)nil){
            return (mword*)nil;
        }
        else{
            if((c(hash_table,opposite_side(cons_side)) == nil) && (level > 0)){
                return temp;
            }
            else{
                (mword*)c(hash_table,cons_side) = temp;
                return (mword*)nil;
            }
        }
    }
}

// hash_table hash insha
void keysha(void){

    mword *hash_table = (mword*)TOS_0;

    mword **result = (mword**)_newin(1);
    *result = (mword*)nil;

    _keysha(hash_table, result);

    zap();

    push_alloc(*result, KEYSHA);

}

void _keysha(mword *hash_table, mword **list){

    mword *temp;

    if(hash_table==(mword*)nil){
        return;
    }
    else if(is_href((mword*)car(hash_table))){
//        printf("B");
        temp = new_cons();
        cons(temp,(mword*)car(hash_table),*list);
        *list = temp;
        return;
    }
    else{
//        printf("C");
        _keysha((mword*)car(hash_table),list);
        _keysha((mword*)cdr(hash_table),list);
        return;
    }
//        printf("D");

}


//// val hash_table hash insha
//void insha(void){
//
//    mword *val        = (mword*)TOS_2;
//    mword *hash_table = (mword*)TOS_1;
//    mword *hash       = (mword*)TOS_0;
//
//    val = new_hash_entry(val, hash);
//
//    _insha(hash_table, (char *)hash, val, 0);
//
////    cons(temp_cons, result, nil);
//
//    zap();
//    swap();
//    zap();
////    push_alloc(result, SLURP8);
//
//}

// hash_table hash insha


//void select_bit_test(void){
//
//    mword *result = new_atom();
//    *result = select_bit((mword*)TOS_1,car(TOS_0));
//
//    zap(); zap();
//    push_alloc(result, FNORD);
//
//}
//
//mword select_bit(mword *val, mword bit){
//    mword mword_select = bit/(MWORD_BIT_SIZE);
//    mword bit_offset = bit % MWORD_BIT_SIZE;
//    if(mword_select > size(val)-1)
//         except("select_bit error:", __FILE__, __LINE__);
//    return (c(val,mword_select) & (1<<bit_offset)) >> bit_offset;
//}


//void _insha(mword *hash_table, char *hash, mword *val, mword level){
//
//    mword *new_hash_table_level;
//    mword *next_level;
//
//    if(level == HASH_SIZE*MWORD_SIZE){
//        except("_insha: Something went wrong", __FILE__, __LINE__);
//    }
//
//    if(!( is_inte(hash_table) && (size(hash_table) == HASH_LEVEL_SIZE) )){
//        except("_insha: Not a hash", __FILE__, __LINE__);
//    }
//
//    next_level = (mword*)c(hash_table,(unsigned char)hash[level]);
//
//    if((mword)next_level == (mword)nil){ //nil
//        (mword*)c(hash_table,(unsigned char)hash[level]) = val;
//        return;
//    }
//    else if( is_inte((mword*)next_level) ){
//        if( size((mword*)next_level) == 2 ){
//            if( _arcmp( (mword*)car(cdr(next_level)), (mword*)car(cdr(val))) ) {
//                new_hash_table_level = _newin(HASH_LEVEL_SIZE);
//                (mword*)c(hash_table,(unsigned char)hash[level]) = new_hash_table_level;
//                _insha((mword*)c(hash_table,(unsigned char)hash[level]), hash, val, level+1);
//                _insha((mword*)c(hash_table,(unsigned char)hash[level]), (char*)car(cdr(next_level)), next_level, level+1);
//            }
//            else{ // no collision
//                (mword*)c(hash_table,(unsigned char)hash[level]) = val;
//            }
//        }
//        else if( size((mword*)next_level) == HASH_LEVEL_SIZE ){ //hash
//            _insha((mword*)c(hash_table,(unsigned char)hash[level]), hash, val, level+1);
//        }
//        else{
//            except("_insha: Not a hash(2)", __FILE__, __LINE__);
//        }
//    }
//    else{
//        except("_insha: Not a hash(3)", __FILE__, __LINE__);
//    }
//
//}
//
//mword *_luha(mword *hash_table, char *hash, mword level){
//
//    mword *next_level;
//
//    if(level == HASH_SIZE*MWORD_SIZE){
//        except("_luha: Something went wrong", __FILE__, __LINE__);
//    }
//
//    if(!( is_inte(hash_table) && (size(hash_table) == HASH_LEVEL_SIZE) )){
//        except("_luha: Not a hash", __FILE__, __LINE__);
//    }
//
//    next_level = (mword*)c(hash_table,(unsigned char)hash[level]);
//
//    if((mword)next_level == (mword)nil){ //nil
//        return (mword*)nil;
//    }
//    else if( is_inte((mword*)next_level) ){
//        if( size((mword*)next_level) == 2 ){
//            if( _arcmp( (mword*)car(cdr(next_level)), (mword*)hash ) ) {
//                return (mword*)nil;
//            }
//            else{ // no collision
//                return (mword*)car((mword*)c(hash_table,(unsigned char)hash[level]));
//            }
//        }
//        else if( size((mword*)next_level) == HASH_LEVEL_SIZE ){ //hash
//            return _luha((mword*)c(hash_table,(unsigned char)hash[level]), hash, level+1);
//        }
//        else{
//            except("_luha: Not a hash(2)", __FILE__, __LINE__);
//        }
//    }
//    else{
//        except("_luha: Not a hash(3)", __FILE__, __LINE__);
//    }
//
//}
//
//
//mword _exha(mword *hash_table, char *hash, mword level){
//
//    mword *next_level;
//
//    if(level == HASH_SIZE*MWORD_SIZE){
//        except("_exha: Something went wrong", __FILE__, __LINE__);
//    }
//
//    if(!( is_inte(hash_table) && (size(hash_table) == HASH_LEVEL_SIZE) )){
//        except("_exha: Not a hash", __FILE__, __LINE__);
//    }
//
//    next_level = (mword*)c(hash_table,(unsigned char)hash[level]);
//
//    if((mword)next_level == (mword)nil){ //nil
//        return 0;
//    }
//    else if( is_inte((mword*)next_level) ){
//        if( size((mword*)next_level) == 2 ){
//            if( _arcmp( (mword*)car(cdr(next_level)), (mword*)hash ) ) {
//                return 0;
//            }
//            else{ // no collision
//                return 1;
//            }
//        }
//        else if( size((mword*)next_level) == HASH_LEVEL_SIZE ){ //hash
//            return _exha((mword*)c(hash_table,(unsigned char)hash[level]), hash, level+1);
//        }
//        else{
//            except("_exha: Not a hash(2)", __FILE__, __LINE__);
//        }
//    }
//    else{
//        except("_exha: Not a hash(3)", __FILE__, __LINE__);
//    }
//
//}
//
//

// Clayton Bauman 2011

