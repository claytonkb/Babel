// bpdl.h
//

#ifndef BPDL_H
#define BPDL_H

// XXX Change to enum:
#define BPDL_CODE_LIST          0
#define BPDL_LIST_LIST          1
#define BPDL_HASH_LIST          2
#define BPDL_OPER_LIST          3
#define BPDL_VAL_LIST           4
#define BPDL_PTR_LIST           5
#define BPDL_TAG_LIST           6
#define BPDL_REF_LIST           7
#define BPDL_SHORT_VAL_LIST     8
#define BPDL_SYM_LIST           9
#define BPDL_BS_LIST            10
#define BPDL_SHORT_PTR_LIST     11
#define BPDL_LABEL_LIST         12
#define BPDL_TPTR_LIST          13
#define BPDL_BYTES_LIST         14
#define BPDL_QUOTE_LIST         15
#define BPDL_SEXPR_LIST         16
#define BPDL_QW_LIST            17
#define BPDL_NOT_BUILT_IN       18
#define BPDL_BITS_LIST          19
#define BPDL_NIL_LIST           20
#define BPDL_UNKNOWN_LIST       (mword)(-1)

#define BPDL_LIST_ENTRY_DNUMBER 1
#define BPDL_LIST_ENTRY_HNUMBER 2
#define BPDL_LIST_ENTRY_NUMBER  (BPDL_LIST_ENTRY_DNUMBER|BPDL_LIST_ENTRY_HNUMBER) 
#define BPDL_LIST_ENTRY_DQUOTE  4
#define BPDL_LIST_ENTRY_SQUOTE  8
#define BPDL_LIST_ENTRY_QUOTE   (BPDL_LIST_ENTRY_DQUOTE|BPDL_LIST_ENTRY_SQUOTE)
#define BPDL_LIST_ENTRY_CONST   (BPDL_LIST_ENTRY_DNUMBER|BPDL_LIST_ENTRY_HNUMBER|BPDL_LIST_ENTRY_DQUOTE|BPDL_LIST_ENTRY_SQUOTE)
#define BPDL_LIST_ENTRY_LABEL   16
#define BPDL_LIST_ENTRY_INTE    32
#define BPDL_LIST_ENTRY_NIL     64
#define BPDL_LIST_ENTRY_UNKNOWN (mword)(-1)

#define add_to_flat_bs_list(bs) lci(flat_bs_list, 0) = _cons(this_bvm, bs, rci(flat_bs_list,0)); 


mword *inline_bpdl(bvm_cache *this_bvm, mword *sexpr);
mword get_bpdl_list_type(mword *sexpr);
mword *inline_bpdl_list_list(bvm_cache *this_bvm, mword *sexpr);
mword get_bpdl_list_entry_type(mword *sexpr);
mword *inline_bpdl_code_list(bvm_cache *this_bvm, mword *sexpr);
mword *rinline_bpdl_val_list(bvm_cache *this_bvm, mword *sexpr);
mword *inline_bpdl_val_list(bvm_cache *this_bvm, mword *sexpr);


mword *rinline_bpdl_ptr_list(bvm_cache *this_bvm, mword *sexpr);
mword *inline_bpdl_ptr_list(bvm_cache *this_bvm, mword *sexpr);
mword *inline_bpdl_tag_list(bvm_cache *this_bvm, mword *sexpr);
mword *inline_bpdl_hash_list(bvm_cache *this_bvm, mword *sexpr);
void   inline_bpdl_sym_list(bvm_cache *this_bvm, mword *sexpr);

mword *bpdl_lookup_label(bvm_cache *this_bvm, mword *label);
//mword *bpdl_label_list(bvm_cache *this_bvm, mword *sexpr);
mword *inline_bpdl_label_list(bvm_cache *this_bvm, mword *sexpr);

mword bpdl_is_const_like(mword *entry);
mword bpdl_is_label_like(mword *entry);
mword bpdl_is_number(mword *entry);
mword bpdl_is_dnumber(mword *entry);
mword bpdl_is_hnumber(mword *entry);
mword bpdl_is_bnumber(mword *entry);
mword bpdl_is_string(mword *entry);
mword bpdl_is_dquote(mword *entry);
mword bpdl_is_squote(mword *entry);
mword bpdl_is_label_nil(mword *entry);

mword *bpdl_const_to_bstruct(bvm_cache *this_bvm, mword *entry, mword entry_type);
mword *bpdl_dnumber_to_bstruct(bvm_cache *this_bvm, mword *entry);
mword *bpdl_hnumber_to_bstruct(bvm_cache *this_bvm, mword *entry);
mword *bpdl_quote_to_bstruct(bvm_cache *this_bvm, mword *entry);

void bpdl_init_opcode_table(bvm_cache *this_bvm);
void bpdl_init_macro_table(bvm_cache *this_bvm);

bvm_cache *bpdli_d(bvm_cache *this_bvm);

// Clayton Bauman 2015


#endif //BPDL_H

// Clayton Bauman 2015

