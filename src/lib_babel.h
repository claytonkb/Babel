// lib.h
// 

#ifndef LIB_H
#define LIB_H

#define lib_init(x, y, z, w) init_interp(x, y, z, w)

#define LIB_UNLOADED_BVM 0
#define LIB_LOADED_BVM   1

mword *lib_bpdl(bvm_cache *this_bvm, mword *code_sexpr, mword *sym_table);
mword *lib_babel(bvm_cache *this_bvm, mword *bvm, mword *arg_stack, mword load_state);
mword *lib_babelc(bvm_cache *this_bvm, mword *code_list, mword *arg_stack);
mword *lib_babelcs(bvm_cache *this_bvm, mword *code_list, mword *arg_stack, mword *sym_table);
mword *lib_code_to_bvm(bvm_cache *this_bvm, mword *code_list, mword *sym_table);
mword *lib_babelr(bvm_cache *this_bvm, mword *bvm, mword *arg_stack);

#endif //LIB_H

// Clayton Bauman 2016

