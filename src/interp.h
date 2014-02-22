// interp.h
//

#include "hash.h"

#ifndef INTERP_H
#define INTERP_H

mword *BABEL_SYM_STEPS;
mword *BABEL_SYM_THREAD_ID;
mword *BABEL_SYM_ADVANCE_TYPE;
mword *BABEL_SYM_SOFT_ROOT;
mword *BABEL_SYM_JUMP_TABLE;

//bvm_cache *interp_init(bvm_cache *this_bvm, int argc, char **argv, char **envp);
//mword *init_interp_jump_table(bvm_cache *this_bvm);
//bvm_cache *endian(bvm_cache *this_bvm);
//void capture_env(bvm_cache *this_bvm, char **envp);
//void init_tags(void);
//void gen_time_string(bvm_cache *this_bvm);
//void init_srand(bvm_cache *this_bvm);
//void init_argv(bvm_cache *this_bvm, int argc, char **argv);
//void init_nil(bvm_cache *this_bvm);
//void flag_init(bvm_cache *this_bvm);
//void init_symbolic_constants(bvm_cache *this_bvm);

int babel_interp(bvm_cache *this_bvm, int argc, char **argv, char **envp);
bvm_cache *interp_init(bvm_cache *this_bvm, int argc, char **argv, char **envp, jmp_buf *cat_ex);
bvm_cache *interp_new(bvm_cache *this_bvm, int argc, char **argv, char **envp, jmp_buf *cat_ex);
bvm_cache *interp_new_jump_table(bvm_cache *this_bvm);
bvm_cache *interp_new_flags(bvm_cache *this_bvm);
bvm_cache *interp_new_nil(bvm_cache *this_bvm);
bvm_cache *interp_new_empty_string(bvm_cache *this_bvm);
bvm_cache *interp_new_null_hash(bvm_cache *this_bvm);
bvm_cache *interp_new_epoch(bvm_cache *this_bvm);
void interp_new_srand(bvm_cache *this_bvm);
void interp_init_symbolic_constants(bvm_cache *this_bvm);
bvm_cache *interp_new_stdin_capture(bvm_cache *this_bvm);
void interp_reset(void);
bvm_cache *interp_capture_argv(bvm_cache *this_bvm);

#endif //INTERP_H

// Clayton Bauman 2013


