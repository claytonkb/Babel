// bpdl2.h
//

#ifndef BPDL2_H
#define BPDL2_H

mword *bpdl_lookup_ns_list(bvm_cache *this_bvm, mword *ns_list, mword *tag);
mword *bpdl(bvm_cache *this_bvm, mword *sexpr, mword *ns_list);

mword *bpdl_list_list(bvm_cache *this_bvm, mword *sexpr, mword *ns_list);
mword *bpdl_code_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table);

mword *bpdl_ptr_list(bvm_cache *this_bvm, mword *sexpr, mword *ns_list);
mword *rbpdl_ptr_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table);

mword *bpdl_val_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table);
mword *rbpdl_val_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table);

mword *bpdl_tag_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table);
mword *bpdl_hash_list(bvm_cache *this_bvm, mword *sexpr);
void bpdl_sym_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table);
mword *bpdl_label_list(bvm_cache *this_bvm, mword *sexpr, mword *sym_table);

bvm_cache *bpdl_d(bvm_cache *this_bvm);

#endif //BPDL2_H

// Clayton Bauman 2016

