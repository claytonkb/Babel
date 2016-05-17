// bpdl.c
//

#include "babel.h"
#include "bpdl.h"
#include "string.h"
#include "array.h"
#include "list.h"
#include "stack.h"
#include "tptr.h"
#include "array.h"
#include "mem.h"
#include "bstruct.h"
#include "trie.h"
#include "bvm.h"
#include "operator.h"
#include "introspect.h"
#include "pearson16.h"
#include "io.h"
#include "logic.h"
#include "hash.h"
#include "lib_babel.h"

//#define BPDL_TRACE
//#define INLINE_BPDL_TRACE

// Differs from _bpdl because no named-sections and assumes all symbols
// pre-exist in symbol-table
//
mword *inline_bpdl(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl#

#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_trace;
#endif

    if(is_nil(sexpr)){
        return nil;
    }

    mword bpdl_list_type = get_bpdl_list_type(sexpr);

    switch(bpdl_list_type){

        case BPDL_LIST_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_LIST_LIST");
#endif
            return inline_bpdl_list_list(this_bvm, (mword*)icdr(sexpr));


        case BPDL_CODE_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_CODE_LIST");
#endif
            return inline_bpdl_code_list(this_bvm, (mword*)icdr(sexpr));


        case BPDL_SHORT_VAL_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_SHORT_VAL_LIST");
#endif
            return inline_bpdl_val_list(this_bvm, sexpr);


        case BPDL_VAL_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_VAL_LIST");
#endif
            return inline_bpdl_val_list(this_bvm, (mword*)icdr(sexpr));


        case BPDL_SHORT_PTR_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_SHORT_PTR_LIST");
#endif
            return inline_bpdl_ptr_list(this_bvm, sexpr);


        case BPDL_PTR_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_PTR_LIST");
#endif
            return inline_bpdl_ptr_list(this_bvm, (mword*)icdr(sexpr));


        case BPDL_TAG_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_TAG_LIST");
#endif
            return inline_bpdl_tag_list(this_bvm, (mword*)icdr(sexpr));


        case BPDL_HASH_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_HASH_LIST");
#endif
            return inline_bpdl_hash_list(this_bvm, (mword*)icdr(sexpr));


        case BPDL_SYM_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_SYM_LIST");
#endif
            //inline_bpdl_sym_list(this_bvm, (mword*)icdr(sexpr));
            return nil;


        case BPDL_BS_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_BS_LIST");
#endif
            return nil; // XXX UNIMPLEMENTED

        case BPDL_QW_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_QW_LIST");
#endif
            return nil; // XXX UNIMPLEMENTED

        case BPDL_SEXPR_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_SEXPR_LIST");
#endif
            return (mword*)icar((mword*)icdr(sexpr));

        case BPDL_LABEL_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_LABEL_LIST");
#endif
// XXX macros go here ... XXX

//            return bpdl_lookup_label(this_bvm, (mword*)icar(sexpr));
            return inline_bpdl_label_list(this_bvm, sexpr);

        default: // BPDL_UNKNOWN_LIST
            _msg("Unrecognized list type");
            _dump(sexpr);
            _die;

    }

    return nil;

}


//
//
mword get_bpdl_list_type(mword *sexpr){ // get_bpdl_list_type#

#ifdef BPDL_TRACE
_trace;
#endif

    if(!is_inte(sexpr) || !is_leaf(icar(sexpr))){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_UNKNOWN_LIST;
    }

#ifdef BPDL_TRACE
_trace;
#endif

    sexpr = (mword*)icar(sexpr);

#ifdef BPDL_TRACE
_trace;
#endif

    if(     _arcmp(sexpr, SEXPR_LIST_SYMBOL) == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_LIST_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_SEXPR_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_SEXPR_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_CODE_SYMBOL) == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_CODE_LIST;
    }
//    else if( bpdl_is_const_like(sexpr) ){
    else if( bpdl_is_number(sexpr) ){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_SHORT_VAL_LIST;
    }
    else if( _arcmp(sexpr, SEXPR_VAL_SYMBOL) == 0 ){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_VAL_LIST;
    }
    else if( bpdl_is_string(sexpr) ){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_SHORT_PTR_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_PTR_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_PTR_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_HASH_SYMBOL) == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_HASH_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_TAG_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_TAG_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_REF_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_REF_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_SYM_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_SYM_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_BS_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_BS_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_QW_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_QW_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_TPTR_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_TPTR_LIST;
    }
    else if(_arcmp(sexpr, SEXPR_BYTES_SYMBOL)  == 0){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_BYTES_LIST;
    }
    else if( !bpdl_is_const_like(sexpr) ){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_LABEL_LIST;
    }

#ifdef BPDL_TRACE
_trace;
#endif

    return BPDL_UNKNOWN_LIST;

}


//
//
mword *inline_bpdl_list_list(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl_list_list#

#ifdef BPDL_TRACE
_trace;
#endif

    // 1. string
    // 2. number
    // 3. label (lookup in sym-table)
    // 4. recurse inline_bpdl
    mword entry_type;
    mword *list_head = nil;
    mword *list_curr = nil;
    mword *new_entry;
    mword *bpdl_label;
    mword *bpdl_label_hash;

    while(!is_nil(sexpr)){

        entry_type = get_bpdl_list_entry_type((mword*)icar(sexpr));

        switch(entry_type){

            case BPDL_LIST_ENTRY_DNUMBER:
            case BPDL_LIST_ENTRY_HNUMBER:
            case BPDL_LIST_ENTRY_DQUOTE:
            case BPDL_LIST_ENTRY_SQUOTE:

                new_entry = 
                    _cons( this_bvm,
                        bpdl_const_to_bstruct(
                            this_bvm, 
                            (mword*)icar(sexpr), 
                            entry_type),
                        nil);

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            case BPDL_LIST_ENTRY_LABEL:

                if(_arcmp((mword*)icar(sexpr), SEXPR_NIL_SYMBOL) == 0){

                        new_entry =
                            _cons( this_bvm, nil, nil);

                }
                else{

//                if(_arcmp((mword*)icar(sexpr), SEXPR_NIL_SYMBOL) == 0){

                    new_entry =
                        _cons( this_bvm, nil, nil);

                    // if built-in:
                    //   substitute opcode-value

                    bpdl_label = _lf2by(this_bvm, (mword*)icar(sexpr));
                    bpdl_label_hash = _hash8(this_bvm,bpdl_label);

                    if(exsym(this_bvm, bpdl_label_hash)){ // XXX PERF (double-lookup)

                        new_entry = 
                            _cons( this_bvm,
                                lusym(this_bvm, bpdl_label_hash),
                                    nil);

                    }
                    else{

                        new_entry =
                            _cons( this_bvm, 
                                tptr_new(this_bvm, BABEL_TAG_REF_SYM_LOCAL, tptr_new(this_bvm, bpdl_label_hash, nil)),
                                nil);

                    }

//                }

                }

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            case BPDL_LIST_ENTRY_INTE:

                new_entry =
                    _cons( this_bvm,
                        inline_bpdl(this_bvm, (mword*)icar(sexpr)),
                        nil);

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            default:

                _fatal("Unrecognized bpdl list entry"); //FIXME Throw exception

        }

        sexpr = (mword*)icdr(sexpr);

    }

    return list_head;

}


//
//
mword get_bpdl_list_entry_type(mword *sexpr){ // get_bpdl_list_entry_type#

#ifdef BPDL_TRACE
_trace;
#endif

    if(is_leaf(sexpr)){
        if(bpdl_is_dnumber(sexpr)){
#ifdef BPDL_TRACE
_trace;
#endif
            return BPDL_LIST_ENTRY_DNUMBER;
        }
        else if(bpdl_is_hnumber(sexpr)){
#ifdef BPDL_TRACE
_trace;
#endif
            return BPDL_LIST_ENTRY_HNUMBER;
        }
        else if(bpdl_is_dquote(sexpr)){
#ifdef BPDL_TRACE
_trace;
#endif
            return BPDL_LIST_ENTRY_DQUOTE;
        }
        else if(bpdl_is_squote(sexpr)){
#ifdef BPDL_TRACE
_trace;
#endif
            return BPDL_LIST_ENTRY_SQUOTE;
        }
//        else if(bpdl_is_label_nil(sexpr)){
//#ifdef BPDL_TRACE
//_trace;
//#endif
//            return BPDL_LIST_ENTRY_NIL;
//        }
        else if(bpdl_is_label_like(sexpr)){
#ifdef BPDL_TRACE
_trace;
#endif
            return BPDL_LIST_ENTRY_LABEL;
        }
    }
    else if(is_inte(sexpr)){
#ifdef BPDL_TRACE
_trace;
#endif
        return BPDL_LIST_ENTRY_INTE;
    }

    return BPDL_LIST_ENTRY_UNKNOWN;

}


//
//
mword *inline_bpdl_code_list(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl_code_list#

#ifdef BPDL_TRACE
_trace;
#endif

    // 1. string
    // 2. number
    // 3. built-in (opcodes)
    // 3. label (lookup in sym-table)
    // 4. recurse inline_bpdl
    mword entry_type;
    mword *list_head = nil;
    mword *list_curr = nil;
    mword *new_entry;
    mword *bpdl_label;
    mword *bpdl_label_hash;

    while(!is_nil(sexpr)){

        entry_type = get_bpdl_list_entry_type((mword*)icar(sexpr));

        switch(entry_type){

            case BPDL_LIST_ENTRY_DNUMBER:
            case BPDL_LIST_ENTRY_HNUMBER:
            case BPDL_LIST_ENTRY_DQUOTE:
            case BPDL_LIST_ENTRY_SQUOTE:

                new_entry = 
                    _cons( this_bvm,
                        _cons( this_bvm,
                            bpdl_const_to_bstruct(
                                this_bvm, 
                                (mword*)icar(sexpr), 
                                entry_type),
                            nil), nil);

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            case BPDL_LIST_ENTRY_LABEL:
                if(_arcmp((mword*)icar(sexpr), SEXPR_NIL_SYMBOL) == 0){
                        new_entry =
                            _cons( this_bvm, 
                                    _cons( this_bvm,
                                        nil,
                                        nil), nil);
                }
                else{

                    // if built-in:
                    //   substitute opcode-value
                    bpdl_label = _lf2by(this_bvm, (mword*)icar(sexpr));
                    bpdl_label_hash = _hash8(this_bvm,bpdl_label);

                    if(trie_exists(this_bvm, bpdl_opcode_table, bpdl_label_hash, nil)){
                        new_entry =
                            _cons( this_bvm, 
                                    _cp( this_bvm, _ith( this_bvm, trie_lookup_hash(this_bvm, bpdl_opcode_table, bpdl_label_hash, nil), 2) ),
                                    nil);
                    }
                    else{

                        new_entry =
                            _cons( this_bvm, 
                                tptr_new(this_bvm, bpdl_label_hash, nil),
                                nil);

                    }

                }

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            case BPDL_LIST_ENTRY_INTE:

                new_entry =
                    _cons( this_bvm,
                        _cons( this_bvm,
                            inline_bpdl(this_bvm, (mword*)icar(sexpr)),
                            nil), nil);

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            default:
                _d(entry_type);
                _fatal("Unrecognized bpdl list entry"); //FIXME Throw exception

        }

        sexpr = (mword*)icdr(sexpr);

    }

    return list_head;

}


// Only constants or labels (to constants) are valid
//      anything else is _fatal
//
mword *inline_bpdl_val_list(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl_val_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword *leaf_list = rinline_bpdl_val_list(this_bvm, sexpr);

    return _ls2lf(this_bvm, leaf_list);

}


//
//
mword *rinline_bpdl_val_list(bvm_cache *this_bvm, mword *sexpr){ // rinline_bpdl_val_list#

#ifdef BPDL_TRACE
_trace;
#endif

    // 1. string
    // 2. number
    // 3. label (lookup in sym-table)
    // 4. recurse inline_bpdl
    mword entry_type;
    mword *list_head = nil;
    mword *list_curr = nil;
    mword *new_entry;

    while(!is_nil(sexpr)){

        entry_type = get_bpdl_list_entry_type((mword*)icar(sexpr));

        switch(entry_type){

            case BPDL_LIST_ENTRY_DNUMBER:
            case BPDL_LIST_ENTRY_HNUMBER:
            case BPDL_LIST_ENTRY_DQUOTE:
            case BPDL_LIST_ENTRY_SQUOTE:

                new_entry = 
                    _cons( this_bvm,
                        bpdl_const_to_bstruct(
                            this_bvm, 
                            (mword*)icar(sexpr), 
                            entry_type),
                        nil);

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            case BPDL_LIST_ENTRY_LABEL:

                new_entry =
                    _cons( this_bvm,
                           lusym2(this_bvm, 
                               _lf2by(this_bvm, 
                               (mword*)icar(sexpr))),
                        nil);

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            case BPDL_LIST_ENTRY_INTE:

                _fatal("Can't nest inside val/leaf array");

                break;

            default:

                _fatal("Unrecognized bpdl val list entry"); //FIXME Throw exception

        }

        sexpr = (mword*)icdr(sexpr);

    }

    return list_head;

}


//
//
mword *inline_bpdl_ptr_list(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl_ptr_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword *leaf_list = rinline_bpdl_ptr_list(this_bvm, sexpr);

    return _bons(this_bvm, leaf_list);

}


//
//
mword *rinline_bpdl_ptr_list(bvm_cache *this_bvm, mword *sexpr){ // rinline_bpdl_ptr_list#

#ifdef BPDL_TRACE
_trace;
#endif

    // 1. string
    // 2. number
    // 3. label (lookup in sym-table)
    // 4. recurse inline_bpdl
    mword entry_type;
    mword *list_head = nil;
    mword *list_curr = nil;
    mword *new_entry;
    mword *bpdl_label;
    mword *bpdl_label_hash;

    while(!is_nil(sexpr)){

        entry_type = get_bpdl_list_entry_type((mword*)icar(sexpr));

        switch(entry_type){

            case BPDL_LIST_ENTRY_DNUMBER:
            case BPDL_LIST_ENTRY_HNUMBER:
            case BPDL_LIST_ENTRY_DQUOTE:
            case BPDL_LIST_ENTRY_SQUOTE:

                new_entry = 
                    _cons( this_bvm,
                        bpdl_const_to_bstruct(
                            this_bvm, 
                            (mword*)icar(sexpr), 
                            entry_type),
                        nil);

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            case BPDL_LIST_ENTRY_LABEL:

                bpdl_label = _lf2by(this_bvm, (mword*)icar(sexpr));
                bpdl_label_hash = _hash8(this_bvm,bpdl_label);

                if(_arcmp((mword*)icar(sexpr), SEXPR_NIL_SYMBOL) == 0){
                    new_entry =
                        _cons( this_bvm,
                               lusym(this_bvm, 
                                   _hash8( this_bvm, bpdl_label )),
                            nil);
                }
                else{

                    if(exsym(this_bvm, bpdl_label_hash)){ // XXX PERF (double-lookup)

                        new_entry =
                            _cons( this_bvm,
                               lusym(this_bvm, 
                                   bpdl_label_hash),
                                       nil);

                    }
                    else{
                        new_entry =
                            _cons( this_bvm, 
                                tptr_new(this_bvm, BABEL_TAG_REF_SYM_LOCAL, tptr_new(this_bvm, bpdl_label_hash, nil)),
                                nil);
                    }

                }

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            case BPDL_LIST_ENTRY_INTE:

                new_entry =
                    _cons( this_bvm,
                        inline_bpdl(this_bvm, (mword*)icar(sexpr)),
                        nil);

                if(is_nil(list_head)){
                    list_head = list_curr = new_entry;
                }
                else{
                    _append_direct(this_bvm, list_curr, new_entry);
                    list_curr = (mword*)icdr(list_curr); // Save unnecessary traversal
                }

                break;

            default:

                _fatal("Unrecognized bpdl ptr list entry"); //FIXME Throw exception

        }

        sexpr = (mword*)icdr(sexpr);

    }

    return list_head;

}


// XXX: This does not handle [tag "foo"] syntax for vals... 
//
mword *inline_bpdl_tag_list(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl_tag_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword *tag = _hash8( this_bvm, bpdl_quote_to_bstruct( this_bvm, (mword*)icar(sexpr) ) );
//    mword *pay = inline_bpdl( this_bvm, (mword*)icar((mword*)icdr(sexpr)) );
    mword *pay = inline_bpdl( this_bvm, (mword*)icdr(sexpr) );
//    mword *pay = inline_bpdl_list_list( this_bvm, (mword*)icdr(sexpr) );

    return tptr_new(this_bvm, tag, pay);//_bons(this_bvm, leaf_list);

}


//
//
mword *inline_bpdl_hash_list(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl_hash_list#

#ifdef BPDL_TRACE
_trace;
#endif

    return _hash8( this_bvm, bpdl_quote_to_bstruct( this_bvm, (mword*)icar(sexpr) ) );

}


//
//
void inline_bpdl_sym_list(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl_sym_list#

#ifdef BPDL_TRACE
_trace;
#endif

    insym(this_bvm, 
        _hash8( this_bvm, bpdl_quote_to_bstruct( this_bvm, (mword*)icar(sexpr) ) ),
        inline_bpdl( this_bvm, (mword*)icar((mword*)icdr(sexpr)) ));

}


//
//
mword *bpdl_lookup_label(bvm_cache *this_bvm, mword *label){ // bpdl_lookup_label#

    if(bpdl_is_label_nil(label)){
        return nil;
    }

    return
        _cons( this_bvm,
           lusym(this_bvm, 
               _hash8( this_bvm, _lf2by(this_bvm, label) )),
            nil);

}


//
//
mword *inline_bpdl_label_list(bvm_cache *this_bvm, mword *sexpr){ // inline_bpdl_label_list#

    mword *sexpr_label = (mword*)icar(sexpr);

    if(bpdl_is_label_nil(sexpr_label)){
        return nil;
    }

    mword *macro_code_list;
    mword *bpdl_list;
    mword *result = nil;
    mword *bpdl_label = _lf2by(this_bvm, sexpr_label);
    mword *bpdl_label_hash = _hash8(this_bvm,bpdl_label);

    if(trie_exists(this_bvm, bpdl_macro_table, bpdl_label_hash, nil)){

        bpdl_list = inline_bpdl_list_list(this_bvm, (mword *)icdr(sexpr));

        macro_code_list =
            _cons( this_bvm, 
                    _cp( this_bvm, _ith( this_bvm, trie_lookup_hash(this_bvm, bpdl_macro_table, bpdl_label_hash, nil), 2) ),
                    nil);

//        result = lib_babelc(this_bvm, (mword*)icar(macro_code_list), bpdl_list); 
        result = lib_babelcs(this_bvm, (mword*)icar(macro_code_list), bpdl_list, this_bvm->soft_root); 

    }
    else{

        _fatal("Unrecognized label-list"); //FIXME Throw exception

    }

    return result;

}


//
//
mword *bpdl_const_to_bstruct(bvm_cache *this_bvm, mword *entry, mword entry_type){ // bpdl_const_to_bstruct#

#ifdef BPDL_TRACE
_trace;
#endif

    switch(entry_type){
        case BPDL_LIST_ENTRY_DNUMBER:
#ifdef BPDL_TRACE
_trace;
#endif
            return bpdl_dnumber_to_bstruct(this_bvm, entry);
        case BPDL_LIST_ENTRY_HNUMBER:
#ifdef BPDL_TRACE
_trace;
#endif
            return bpdl_hnumber_to_bstruct(this_bvm, entry);
        case BPDL_LIST_ENTRY_DQUOTE:
        case BPDL_LIST_ENTRY_SQUOTE:
#ifdef BPDL_TRACE
_trace;
#endif
            return bpdl_quote_to_bstruct(this_bvm, entry);
        default:
            _fatal("Something went wrong"); //FIXME Throw exception
    }

    _fatal("Something went wrong"); //FIXME Throw exception

    return nil;

}


//
//
mword *bpdl_dnumber_to_bstruct(bvm_cache *this_bvm, mword *entry){ // bpdl_dnumber_to_bstruct#

#ifdef BPDL_TRACE
_trace;
#endif

    mword is_negative = 0;

    if(rcl(entry,0) == '-'){ // Negative number
        is_negative = 1;
    }

    if(!is_negative){

        char *str = (char*)_ar2str( this_bvm, entry );

        return _val(this_bvm, (mword)atoi((char*)str));

    }
    else{

        char *str = (char*)_ar2str( this_bvm, entry );

        return _val(this_bvm, (mword)(-1*atoi((char*)(str+1))));

    }

    return 0; // Silence compiler warning

}


//
//
mword *bpdl_hnumber_to_bstruct(bvm_cache *this_bvm, mword *entry){ // bpdl_hnumber_to_bstruct#

#ifdef BPDL_TRACE
_trace;
#endif

    char *str = (char*)_ar2str( this_bvm, entry );

    return _val(this_bvm, (mword)strtoul((char*)(str+2),NULL,16));

}


// This function almost a copy of lf2by() but slightly modified
//
mword *bpdl_quote_to_bstruct(bvm_cache *this_bvm, mword *entry){ // bpdl_quote_to_bstruct#

#ifdef BPDL_TRACE
_trace;
#endif

    mword arr_size  = size(entry)-2; // -2 because of the quote marks
    mword arr8_size = _array8_size(this_bvm, arr_size);
    unsigned char *result = (unsigned char *)_newlfi(this_bvm, arr8_size, 0);

    int i;

    for(i=1; i<arr_size+1; i++){

        result[i-1] = (unsigned char)(entry[i] & 0xff);

    }

    lcl(result, arr8_size-1) = _alignment_word8(this_bvm, arr_size);

    return (mword*)result;

}


//
//
mword *bpdl_squote_to_bstruct(bvm_cache *this_bvm, mword *entry){ // bpdl_squote_to_bstruct#
    return nil;
}


// XXX NOTE: Syntax checking is performed in sexpr.c, so these functions
// are not responsible for checking syntax, only for recognizing the
// type of entry

//
//
mword bpdl_is_const_like(mword *entry){ // bpdl_is_const_like#

#ifdef BPDL_TRACE
_trace;
#endif

    return bpdl_is_number(entry) | bpdl_is_string(entry);

}


//
//
mword bpdl_is_label_nil(mword *entry){ // bpdl_is_label_nil#

#ifdef BPDL_TRACE
_trace;
#endif

    return (_arcmp(entry, SEXPR_NIL_SYMBOL) == 0);

}


//
//
mword bpdl_is_label_like(mword *entry){ // bpdl_is_label_like#

#ifdef BPDL_TRACE
_trace;
#endif

    return 1; // Syntax-checking was already performed in sexpr.c

}


//
//
mword bpdl_is_number(mword *entry){ // bpdl_is_number#

#ifdef BPDL_TRACE
_trace;
#endif

    return bpdl_is_dnumber(entry) | bpdl_is_hnumber(entry) | bpdl_is_bnumber(entry);

}


//
//
mword bpdl_is_dnumber(mword *entry){ // bpdl_is_dnumber#

#ifdef BPDL_TRACE
_trace;
#endif

    mword is_dnumber =
        ((size(entry) == 1) && (rcl(entry,0) == '0'))
            ||
        ((rcl(entry,0) >= '1') && (rcl(entry,0) <= '9'))
            ||
        ((rcl(entry,0) == '-') && 
         (rcl(entry,1) >= '1') && (rcl(entry,1) <= '9'));

    return is_dnumber;

}


//
//
mword bpdl_is_hnumber(mword *entry){ // bpdl_is_hnumber#

#ifdef BPDL_TRACE
_trace;
#endif

    return (rcl(entry,0) == '0') && (rcl(entry,1) == 'x');

}


//
//
mword bpdl_is_bnumber(mword *entry){ // bpdl_is_bnumber#

#ifdef BPDL_TRACE
_trace;
#endif

    return (rcl(entry,0) == '0') && (rcl(entry,1) == 'b');

}


//
//
mword bpdl_is_string(mword *entry){ // bpdl_is_string#

#ifdef BPDL_TRACE
_trace;
#endif

    return bpdl_is_dquote(entry) | bpdl_is_squote(entry);

}


//
//
mword bpdl_is_dquote(mword *entry){ // bpdl_is_dquote#

#ifdef BPDL_TRACE
_trace;
#endif

    return (rcl(entry,0) == '"') && (rcl(entry,size(entry)-1) == '"');

}


//
//
mword bpdl_is_squote(mword *entry){ // bpdl_is_squote#

#ifdef BPDL_TRACE
_trace;
#endif

    return (rcl(entry,0) == '\'') && (rcl(entry,size(entry)-1) == '\'');

}


//
//
void bpdl_init_opcode_table(bvm_cache *this_bvm){ // bpdl_init_opcode_table#

#if(defined BPDL_TRACE || defined BABEL_RESET_TRACE)
_trace;
#endif

this_bvm->flags->MC_USE_MALLOC = FLAG_SET;

    mword *opcode_table = _slurp(this_bvm, C2B("src/opcode_table.bbl"));
    bpdl_opcode_table   = _load(this_bvm, opcode_table, size(opcode_table)); // FIXME trashed after GC !!!!

this_bvm->flags->MC_USE_MALLOC = FLAG_CLR;

    bpdl_return_opcode = _ith(this_bvm, trie_lookup_hash(this_bvm, bpdl_opcode_table, HASH8(this_bvm, "return"), nil), 2);

}


//
//
void bpdl_init_macro_table(bvm_cache *this_bvm){ // bpdl_init_macro_table#

#if(defined BPDL_TRACE || defined BABEL_RESET_TRACE)
_trace;
#endif

this_bvm->flags->MC_USE_MALLOC = FLAG_SET;

    mword *macro_table = _slurp(this_bvm, C2B("src/macro_table.bbl"));
    bpdl_macro_table   = _load(this_bvm, macro_table, size(macro_table));

this_bvm->flags->MC_USE_MALLOC = FLAG_CLR;

}



#define BPDLI_D_OPERATIONS \
    result0 = inline_bpdl(this_bvm, oi0.data);

OPERATORA_R1_W1_D(bpdli_d, 
        BPDLI_D_OPERATIONS, 
        nil, OI_MASK_ANY, 0, 0)



// Clayton Bauman 2015

