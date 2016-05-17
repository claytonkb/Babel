// lib.c
//

#include "babel.h"
#include "lib_babel.h"
#include "sexpr.h"
#include "bpdl.h"
#include "string.h"
#include "bstruct.h"
#include "null.sp.h"
#include "trie.h"
#include "tptr.h"
#include "list.h"
#include "introspect.h"

//lib_init   Initializes the interpreter for library use
//lib_babel  Standard semantics for _babel(); must call lib_init once before calling
//lib_bpdl   Pass in string + sym_table (optional), get back bstruct
//lib_babelv Always returns a value (unloads if necessary)
//lib_babelc Standard semantics for _babel(); must call lib_init once before calling

// Add a lib_sexpr?

//
//
mword *lib_babel(bvm_cache *this_bvm, mword *bvm, mword *arg_stack, mword load_state){ // lib_babel#

    mword *loaded_bvm;

    if(load_state == LIB_UNLOADED_BVM){
        loaded_bvm = _load(this_bvm, bvm, NULL_BVM_SIZE); // XXX _cp before _load?? XXX
    }
    else{
        loaded_bvm = bvm;
    }

    return _babel(this_bvm, loaded_bvm, arg_stack, nil);

}


//
//
mword *lib_bpdl(bvm_cache *this_bvm, mword *code_sexpr, mword *sym_table){ // lib_bpdl#

    mword *bstring = _str2ar(this_bvm, code_sexpr);

    return inline_bpdl(this_bvm, sexpr_op2(this_bvm, bstring));

}


//
//
mword *lib_code_to_bvm(bvm_cache *this_bvm, mword *code_list, mword *sym_table){ // lib_code_to_bvm#

    mword *null_BVM_copy = _cp(this_bvm, this_bvm->interp->cold_null_BVM);

    mword *cons_code_list = _cons(this_bvm, code_list, nil);

    trie_insert( this_bvm, 
            tptr_detag(this_bvm, null_BVM_copy), 
            BABEL_SYM_CODE_PTR, 
            nil, 
            cons_code_list);

    trie_insert( this_bvm, 
            tptr_detag(this_bvm, null_BVM_copy), 
            BABEL_SYM_CODE_RESTART_POINT, 
            nil, 
            code_list);

    if(!is_nil(sym_table)){
        trie_insert( this_bvm, 
                tptr_detag(this_bvm, null_BVM_copy), 
                BABEL_SYM_SOFT_ROOT, 
                nil, 
                sym_table);
    }

    return null_BVM_copy;

}


// re-startable lib_babelc
//
mword *lib_babelr(bvm_cache *this_bvm, mword *bvm, mword *arg_stack){ // lib_babelr#

    mword *code_restart_point = _ith( this_bvm, trie_lookup_hash( this_bvm, tptr_detag( this_bvm, bvm ), BABEL_SYM_CODE_RESTART_POINT, nil), 2);

    trie_insert( this_bvm, 
            tptr_detag(this_bvm, bvm), 
            BABEL_SYM_CODE_PTR, 
            nil, 
            _cons(this_bvm, code_restart_point, nil));

    return lib_babel(this_bvm, bvm, arg_stack, LIB_LOADED_BVM);

}


//
//
mword *lib_babelc(bvm_cache *this_bvm, mword *code_list, mword *arg_stack){ // lib_babelc#

    return lib_babel(this_bvm, lib_code_to_bvm(this_bvm, code_list, nil), arg_stack, LIB_LOADED_BVM);

}


////
////
//mword *lib_babelc(bvm_cache *this_bvm, mword *code_list, mword *arg_stack){ // lib_babelc#
//
//    mword *null_BVM_copy = _cp(this_bvm, this_bvm->interp->cold_null_BVM);
//
//    trie_insert( this_bvm, 
//            tptr_detag(this_bvm, null_BVM_copy), 
//            BABEL_SYM_CODE_PTR, 
//            nil, 
//            _cons(this_bvm, code_list, nil));
//
//    return lib_babel(this_bvm, null_BVM_copy, arg_stack, LIB_LOADED_BVM);
//
//}


//
//
mword *lib_babelcs(bvm_cache *this_bvm, mword *code_list, mword *arg_stack, mword *sym_table){ // lib_babelcs#

//    mword *null_BVM_copy = _cp(this_bvm, this_bvm->interp->cold_null_BVM);
//
//    trie_insert( this_bvm, 
//            tptr_detag(this_bvm, null_BVM_copy), 
//            BABEL_SYM_CODE_PTR, 
//            nil, 
//            _cons(this_bvm, code_list, nil));

    mword *null_BVM_copy = lib_code_to_bvm(this_bvm, code_list, nil);

    trie_insert( this_bvm, 
            tptr_detag(this_bvm, null_BVM_copy), 
            BABEL_SYM_SOFT_ROOT, 
            nil, 
            sym_table);

    return lib_babel(this_bvm, null_BVM_copy, arg_stack, LIB_LOADED_BVM);

}



// Clayton Bauman 2016

