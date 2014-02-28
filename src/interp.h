// interp.h
//

#include "hash.h"

#ifndef INTERP_H
#define INTERP_H


#define OI_MASK_NONE (1<<0)
#define OI_MASK_LEAF (1<<1)
#define OI_MASK_INTE (1<<2)
#define OI_MASK_TPTR (1<<3)

typedef struct { // operand_info#

    mword *data;
    mword *default;
    mword mask;
    mword min_size;
    mword max_size;

} operand_info;

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
void get_operands(bvm_cache *this_bvm, operand_info **oinfo0, mword num_operands);

#endif //INTERP_H

// Clayton Bauman 2013


