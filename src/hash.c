// hash.c
//

#include "babel.h"
#include "hash.h"
#include "list.h"
#include "except.h"
#include "array.h"
#include "stack.h"

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

// val hash_table hash insha
void insha(void){

    mword *val        = (mword*)TOS_2;
    mword *hash_table = (mword*)TOS_1;
    mword *hash       = (mword*)TOS_0;

    val = new_hash_entry(val, hash);

    _insha(hash_table, (char *)hash, val, 0);

//    cons(temp_cons, result, nil);

    zap();
    zap();
    zap();
//    push_alloc(result, SLURP8);

}


void _insha(mword *hash_table, char *hash, mword *val, mword level){

    mword *hash_table_save;

    if(!( is_inte(hash_table) && (size(hash_table) == HASH_LEVEL_SIZE) )){
        except("_insha: Not a hash", __FILE__, __LINE__);
    }

    hash_table = (mword*) *(hash_table+hash[level]);
    
    if(hash_table == (mword*)nil){ //nil
        hash_table = val;
    }
    else if( is_inte(hash_table) && (size(hash_table) == 2) ){ //list
        //Check for collision:
        if( _arcmp( (mword*)car(cdr(hash_table)), (mword*)car(cdr(val))) ) {
            hash_table_save = hash_table;
            hash_table = _newin(HASH_LEVEL_SIZE);
            _insha(hash_table, hash, val,             level+1);
            _insha(hash_table, hash, hash_table_save, level+1);
        }
        else{ // no collision
            car(hash_table) = car(val);
        }
    }
    else if( is_inte(hash_table) && (size(hash_table) == HASH_LEVEL_SIZE) ){ //hash
        _insha(hash_table, hash, val, level+1);
    }
    else{
        except("_insha: Not a hash", __FILE__, __LINE__);
    }

}


mword *_luha(mword *hash_table, char *hash, mword *val, mword level){

    if(hash_table == (mword*)nil){ //nil
        return (mword*)nil;
    }
    else if( is_inte(hash_table) && (size(hash_table) == 2) ){ //list
        return (mword*)car(hash_table);
    }
    else if( is_inte(hash_table) && (size(hash_table) == HASH_LEVEL_SIZE) ){ //hash
        return _luha((mword*)hash_table+hash[level], hash, val, level+1);
    }
    else{
        except("_luha: Not a hash", __FILE__, __LINE__);
    }

}


mword _exha(mword *hash_table, char *hash, mword *val, mword level){

    if(hash_table == (mword*)nil){ //nil
        return 0;
    }
    else if( is_inte(hash_table) && (size(hash_table) == 2) ){ //list
        //Check for collision:
        if( _arcmp( (mword*)car(cdr(hash_table)), (mword*)car(cdr(val))) ) {
            return 1;
        }
        else{ // no collision
            return 0;
        }
    }
    else if( is_inte(hash_table) && (size(hash_table) == HASH_LEVEL_SIZE) ){ //hash
        return _exha((mword*)hash_table+hash[level], hash, val, level+1);
    }
    else{
        except("_exha: Not a hash", __FILE__, __LINE__);
    }

}


void _inskha(mword *hash_table, mword *val, mword *key){
}

// Clayton Bauman 2011

