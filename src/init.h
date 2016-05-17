// init.h
// 

#ifndef INIT_H
#define INIT_H

bvm_cache *init_interp(bvm_cache *this_bvm, int argc, char **argv, char **envp, jmp_buf *cat_ex);
bvm_cache *init_interp_flags(bvm_cache *this_bvm);
bvm_cache *init_interp_limits(bvm_cache *this_bvm);
bvm_cache *init_interp_privileges(bvm_cache *this_bvm);
bvm_cache *init_jump_table(bvm_cache *this_bvm);
void init_global_consts(bvm_cache *this_bvm);
bvm_cache *init_nil(bvm_cache *this_bvm);
void init_interp_consts(bvm_cache *this_bvm);
bvm_cache *init_new_epoch(bvm_cache *this_bvm);
bvm_cache *init_new_epoch(bvm_cache *this_bvm);
void init_new_srand(bvm_cache *this_bvm);
bvm_cache *init_capture_argv(bvm_cache *this_bvm);
bvm_cache *init_load_root_bvm(bvm_cache *this_bvm);

#ifdef PROF_MODE
void init_bvm_profile(bvm_cache *this_bvm);
#endif

#endif //INIT_H

// Clayton Bauman 2014

