// bpdl2.c
//

#include "babel.h"
#include "bpdl2.h"
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

#define bpdl_label_to_bytes(x)  _lf2by(this_bvm, x)
#define calc_bpdl_label_hash(x) _hash8(this_bvm,bpdl_label_to_bytes(x))

#define bpdl_trie_lookup(x, y) trie_lookup_hash(this_bvm, x, y, nil)
#define bpdl_lookup(x, y) _cp( this_bvm, _ith( this_bvm, bpdl_trie_lookup(x,y), 2) )
#define bpdl_new_entry(x) _cons( this_bvm, x, nil)

#define valeq(x,y) (_arcmp(x, y) == 0)

#define update_list_head                                \
if(is_nil(list_head)){                                  \
    list_head = list_curr = new_entry;                  \
}                                                       \
else{                                                   \
    _append_direct(this_bvm, list_curr, new_entry);     \
    list_curr = (mword*)icdr(list_curr);                \
}                                                       \
                                                        \
sexpr = (mword*)icdr(sexpr);

#define ns_list_entry(x) _ith(this_bvm, ns_list, x)

#define bpdl_ns_lookup(x, y) bpdl_lookup_ns_list(this_bvm, x, y)


//
//
mword *bpdl_lookup_ns_list(bvm_cache *this_bvm, mword *ns_list, mword *tag){

    mword *curr_entry = ns_list;
    mword *result = nil;
    mword ns_hit = 0;

    while(!is_nil(curr_entry)){

        if(trie_exists(this_bvm, rci(curr_entry,0), tag, nil)){
            result = bpdl_lookup(rci(curr_entry,0), tag);
            ns_hit = 1;
            break;
        }

        curr_entry = (mword*)icdr(curr_entry);

    }

    if(!ns_hit){
        result = bpdl_new_entry( tptr_new(this_bvm, tag, nil) );
    }
    else{
        result = bpdl_new_entry( result );
    }

    return result;

}


//
//
mword *bpdl(bvm_cache *this_bvm, mword *sexpr, mword *ns_list){ // bpdl#

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
            return bpdl_list_list(this_bvm, (mword*)icdr(sexpr), ns_list);


        case BPDL_CODE_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_CODE_LIST");
#endif
            return bpdl_code_list(this_bvm, (mword*)icdr(sexpr), ns_list);


        case BPDL_SHORT_VAL_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_SHORT_VAL_LIST");
#endif
            return bpdl_val_list(this_bvm, sexpr, ns_list);


        case BPDL_VAL_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_VAL_LIST");
#endif
            return bpdl_val_list(this_bvm, (mword*)icdr(sexpr), ns_list);


        case BPDL_SHORT_PTR_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_SHORT_PTR_LIST");
#endif
            return bpdl_ptr_list(this_bvm, sexpr, ns_list);


        case BPDL_PTR_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_PTR_LIST");
#endif
            return bpdl_ptr_list(this_bvm, (mword*)icdr(sexpr), ns_list);


        case BPDL_TAG_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_TAG_LIST");
#endif
            return bpdl_tag_list(this_bvm, (mword*)icdr(sexpr), ns_list);


        case BPDL_HASH_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_HASH_LIST");
#endif
            return bpdl_hash_list(this_bvm, (mword*)icdr(sexpr));


        case BPDL_SYM_LIST:
#if(defined BPDL_TRACE || defined INLINE_BPDL_TRACE)
_msg("BPDL_SYM_LIST");
#endif
            //bpdl_sym_list(this_bvm, (mword*)icdr(sexpr));
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
            return bpdl_label_list(this_bvm, sexpr, ns_list);


        default: // BPDL_UNKNOWN_LIST
            _msg("Unrecognized list type");
            _dump(sexpr);
            _die;

    }

    return nil;

}


//
//
mword *bpdl_list_list(bvm_cache *this_bvm, mword *sexpr, mword *ns_list){ // bpdl_list_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword entry_type;
    mword *list_head = nil, *list_curr = nil;
    mword *new_entry, *bpdl_label_hash;

    while(!is_nil(sexpr)){

        entry_type = get_bpdl_list_entry_type((mword*)icar(sexpr));

        switch(entry_type){

            case BPDL_LIST_ENTRY_DNUMBER:
            case BPDL_LIST_ENTRY_HNUMBER:
            case BPDL_LIST_ENTRY_DQUOTE:
            case BPDL_LIST_ENTRY_SQUOTE:

                new_entry = bpdl_new_entry( bpdl_const_to_bstruct( this_bvm, (mword*)icar(sexpr), entry_type) ); 
                break;

            case BPDL_LIST_ENTRY_LABEL:

                if(valeq((mword*)icar(sexpr), SEXPR_NIL_SYMBOL)){
                    new_entry = bpdl_new_entry(nil);
                }
                else{
                    bpdl_label_hash = calc_bpdl_label_hash((mword*)icar(sexpr));
                    new_entry = bpdl_ns_lookup(ns_list, bpdl_label_hash);
                }

                break;

            case BPDL_LIST_ENTRY_INTE:

                new_entry = bpdl_new_entry( bpdl(this_bvm, (mword*)icar(sexpr), ns_list) ); //FIXME: WRONG
                break;

            default:

                _fatal("Unrecognized bpdl list entry"); //FIXME Throw exception

        }

        update_list_head;

    }

    return list_head;

}


//
//
mword *bpdl_code_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table){ // bpdl_code_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword entry_type;
    mword *list_head = nil, *list_curr = nil;
    mword *new_entry, *bpdl_label_hash;

    while(!is_nil(sexpr)){

        entry_type = get_bpdl_list_entry_type((mword*)icar(sexpr));

        switch(entry_type){

            case BPDL_LIST_ENTRY_DNUMBER:
            case BPDL_LIST_ENTRY_HNUMBER:
            case BPDL_LIST_ENTRY_DQUOTE:
            case BPDL_LIST_ENTRY_SQUOTE:

                new_entry = bpdl_new_entry(
                            bpdl_new_entry( bpdl_const_to_bstruct(this_bvm, (mword*)icar(sexpr), entry_type)) );
                break;

            case BPDL_LIST_ENTRY_LABEL:

                if(valeq((mword*)icar(sexpr), SEXPR_NIL_SYMBOL)){
                    new_entry = bpdl_new_entry( bpdl_new_entry(nil) );
                }
                else{

                    bpdl_label_hash = calc_bpdl_label_hash((mword*)icar(sexpr));

                    if(trie_exists(this_bvm, bpdl_opcode_table, bpdl_label_hash, nil)){
                        new_entry = bpdl_new_entry( bpdl_lookup(bpdl_opcode_table, bpdl_label_hash) );
                    }
                    else{

                        if(is_nil(sym_table)){
                            new_entry = bpdl_new_entry( 
                                        bpdl_new_entry( tptr_new(this_bvm, BABEL_TAG_REF_SYM_LOCAL, tptr_new(this_bvm, bpdl_label_hash, nil)) ));
                        }
                        else if(trie_exists(this_bvm, sym_table, bpdl_label_hash, nil)){
                                new_entry = bpdl_new_entry(  
                                            bpdl_new_entry( bpdl_lookup(sym_table, bpdl_label_hash) ));
                        }
                        else{
                            new_entry = bpdl_new_entry( 
                                        bpdl_new_entry( tptr_new(this_bvm, BABEL_TAG_REF_SYM_LOCAL, tptr_new(this_bvm, bpdl_label_hash, nil)) ));
                        }

                    }

                }

                break;

            case BPDL_LIST_ENTRY_INTE:

                new_entry = bpdl_new_entry(
                            bpdl_new_entry( tptr_new(this_bvm, bpdl(this_bvm, (mword*)icar(sexpr), sym_table), nil) ));
                break;

            default:
                _d(entry_type);
                _fatal("Unrecognized bpdl list entry"); //FIXME Throw exception

        }

        update_list_head;

    }

    return list_head;

}


// Only constants or labels (to constants) are valid
//      anything else is _fatal
//
mword *bpdl_val_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table){ // bpdl_val_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword *leaf_list = rbpdl_val_list(this_bvm, sexpr, sym_table);

    return _ls2lf(this_bvm, leaf_list);

}


//
//
mword *rbpdl_val_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table){ // rbpdl_val_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword entry_type;
    mword *list_head = nil, *list_curr = nil;
    mword *new_entry, *bpdl_label_hash;

    while(!is_nil(sexpr)){

        entry_type = get_bpdl_list_entry_type((mword*)icar(sexpr));

        switch(entry_type){

            case BPDL_LIST_ENTRY_DNUMBER:
            case BPDL_LIST_ENTRY_HNUMBER:
            case BPDL_LIST_ENTRY_DQUOTE:
            case BPDL_LIST_ENTRY_SQUOTE:

                new_entry = bpdl_new_entry( bpdl_const_to_bstruct( this_bvm, (mword*)icar(sexpr), entry_type) ); 
                break;

            case BPDL_LIST_ENTRY_LABEL:

                bpdl_label_hash = calc_bpdl_label_hash((mword*)icar(sexpr));
                new_entry = bpdl_new_entry( bpdl_lookup(sym_table, bpdl_label_hash) );
                break;

            case BPDL_LIST_ENTRY_INTE:

                _fatal("Can't nest inside val/leaf array");
                break;

            default:

                _fatal("Unrecognized bpdl val list entry"); //FIXME Throw exception

        }

        update_list_head;

    }

    return list_head;

}


//
//
mword *bpdl_ptr_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table){ // bpdl_ptr_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword *leaf_list = rbpdl_ptr_list(this_bvm, sexpr, sym_table);

    return _bons(this_bvm, leaf_list);

}


//
//
mword *rbpdl_ptr_list(bvm_cache *this_bvm, mword *sexpr, mword *ns_list){ // rbpdl_ptr_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword entry_type;
    mword *list_head = nil, *list_curr = nil;
    mword *new_entry, *bpdl_label_hash;

    while(!is_nil(sexpr)){

        entry_type = get_bpdl_list_entry_type((mword*)icar(sexpr));

        switch(entry_type){

            case BPDL_LIST_ENTRY_DNUMBER:
            case BPDL_LIST_ENTRY_HNUMBER:
            case BPDL_LIST_ENTRY_DQUOTE:
            case BPDL_LIST_ENTRY_SQUOTE:

                new_entry = bpdl_new_entry( bpdl_const_to_bstruct( this_bvm, (mword*)icar(sexpr), entry_type) ); 
                break;

            case BPDL_LIST_ENTRY_LABEL:

                if(valeq((mword*)icar(sexpr), SEXPR_NIL_SYMBOL)){
                    new_entry = bpdl_new_entry(nil);
                }
                else{
                    bpdl_label_hash = calc_bpdl_label_hash((mword*)icar(sexpr));
                    new_entry = bpdl_ns_lookup(ns_list, bpdl_label_hash);
                }

                break;

            case BPDL_LIST_ENTRY_INTE:

                new_entry = bpdl_new_entry( bpdl(this_bvm, (mword*)icar(sexpr), ns_list) );
                break;

            default:

                _fatal("Unrecognized bpdl ptr list entry"); //FIXME Throw exception

        }

        update_list_head;

    }

    return list_head;

}


// XXX: This does not handle [tag "foo"] syntax for vals... 
//
mword *bpdl_tag_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table){ // bpdl_tag_list#

#ifdef BPDL_TRACE
_trace;
#endif

    mword *tag = _hash8( this_bvm, bpdl_quote_to_bstruct( this_bvm, (mword*)icar(sexpr) ) );
    mword *pay = bpdl( this_bvm, (mword*)icdr(sexpr), sym_table);

    return tptr_new(this_bvm, tag, pay);//_bons(this_bvm, leaf_list);

}


//
//
mword *bpdl_hash_list(bvm_cache *this_bvm, mword *sexpr){ // bpdl_hash_list#

#ifdef BPDL_TRACE
_trace;
#endif

    return _hash8( this_bvm, bpdl_quote_to_bstruct( this_bvm, (mword*)icar(sexpr) ) );

}


//
//
void bpdl_sym_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table){ // bpdl_sym_list#

#ifdef BPDL_TRACE
_trace;
#endif

    insym(this_bvm, 
        _hash8( this_bvm, bpdl_quote_to_bstruct( this_bvm, (mword*)icar(sexpr) ) ),
        bpdl( this_bvm, (mword*)icar((mword*)icdr(sexpr)), sym_table));

}


//
//
mword *bpdl_label_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table){ // bpdl_label_list#

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

        bpdl_list = bpdl_list_list(this_bvm, (mword *)icdr(sexpr), sym_table);
        macro_code_list = bpdl_new_entry( bpdl_lookup(bpdl_macro_table, bpdl_label_hash) );
        result = lib_babelcs(this_bvm, (mword*)icar(macro_code_list), bpdl_list, this_bvm->soft_root);

    }
    else{

        _fatal("Unrecognized label-list"); //FIXME Throw exception

    }

    return result;

}


#define BPDL_D_OPERATIONS \
    result0 = bpdl(this_bvm, oi0.data, _cons(this_bvm, oi1.data, nil));

OPERATORA_R2_W1_D(bpdl_d, 
        BPDL_D_OPERATIONS, 
        nil, OI_MASK_ANY, 0, 0,
        nil, OI_MASK_ANY, 0, 0)


// Clayton Bauman 2016

