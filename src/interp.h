// interp.h
//

#ifndef INTERP_H
#define INTERP_H

bvm_cache *interp_init(bvm_cache *this_bvm, int argc, char **argv, char **envp);
mword *init_interp_jump_table(bvm_cache *this_bvm);
bvm_cache *endian(bvm_cache *this_bvm);
void capture_env(bvm_cache *this_bvm, char **envp);
void init_tags(void);
void gen_time_string(bvm_cache *this_bvm);
void init_srand(bvm_cache *this_bvm);
void init_argv(bvm_cache *this_bvm, int argc, char **argv);
void init_nil(bvm_cache *this_bvm);

#endif //INTERP_H

// Clayton Bauman 2013


