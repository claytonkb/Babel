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

//void push_alloc(mword *operand, mword alloc_type){

// make hash-entry
mword *new_hash_entry(mword *val, mword *hash){

    mword *temp_consA = new_cons();
    mword *temp_consB = new_cons();

    cons(temp_consA,  hash,  nil);
    cons(temp_consB,  val,   temp_consA);

    return temp_consB;

}


// make hash-entry with key
mword *new_hash_key_entry(mword *val, mword *hash, mword *key){
}

void insha(void){

//    mword *result = new_atom();
//    *result = _cxr1((mword*)TOS_1,car(TOS_0));
//
//    zap();
//    zap();
//    push_alloc(result, INSHA);

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
void luha(void){

    mword *hash_table = (mword*)TOS_1;
    mword *hash       = (mword*)TOS_0;

//    val = new_hash_entry(val, hash);

    mword *result = _luha(hash_table, (char *)hash, 0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
    push_alloc(result, LUHA);

}

// hash_table hash insha
void exha(void){

    mword *hash_table = (mword*)TOS_1;
    mword *hash       = (mword*)TOS_0;

//    val = new_hash_entry(val, hash);

    mword *result = new_atom();
    *result = _exha(hash_table, (char *)hash, 0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
    push_alloc(result, EXHA);

}

void _insha(mword *hash_table, char *hash, mword *val, mword level){
}

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

mword *_luha(mword *hash_table, char *hash, mword level){
}

mword _exha(mword *hash_table, char *hash, mword level){
}

void _inskha(mword *hash_table, mword *val, mword *key){
}

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

