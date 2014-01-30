// bvm.h
// 

#ifndef BVM_H
#define BVM_H

// XXX UNSAFE; Gets the code-pointer from a bvm hard-root
#define bvm_code_ptr(x)     car(car(x)) // bvm_code_ptr#
#define bvm_rstack_ptr(x)   cdr(car(x)) // bvm_rstack_ptr#
#define bvm_dstack_ptr(x)   car(icar(cdr(x))) // bvm_dstack_ptr#
#define bvm_ustack_ptr(x)   car(cdr(icar(cdr(x)))) // bvm_ustack_ptr#
#define bvm_jump_table(x)   car(icar(icdr(cdr(x)))) // bvm_jump_table#
#define bvm_sym_table(x)    (mword*)get_tptr((mword*)icar(icdr(icdr(cdr(x))))) // bvm_sym_table#

#define eval_return(x) _ith(this_bvm, (mword*)icar(x),0) // eval_return#
#define eval_type(x)   _ith(this_bvm, (mword*)icar(x),1) // eval_type#

void bbl2gv(mword *tree);
mword tree_bbl2gv(mword *tree);
bvm_cache *babelop(bvm_cache *this_bvm);
bvm_cache *bvm_interp(bvm_cache *this_bvm);
bvm_cache *bvmcode(bvm_cache *this_bvm);
bvm_cache *bvmstack(bvm_cache *this_bvm);
bvm_cache *rsvd(bvm_cache *this_bvm);
bvm_cache *bvmustack(bvm_cache *this_bvm);
bvm_cache *boilerplate(bvm_cache *this_bvm);
bvm_cache *self(bvm_cache *this_bvm);
bvm_cache *update_bvm_cache(bvm_cache *this_bvm);
bvm_cache *flush_bvm_cache(bvm_cache *this_bvm);
bvm_cache *exec(bvm_cache *this_bvm);
bvm_cache *bvmrstack(bvm_cache *this_bvm);
bvm_cache *bvmsym(bvm_cache *this_bvm);
bvm_cache *hibernate(bvm_cache *this_bvm);
mword *_bvm_init(bvm_cache *this_bvm, mword *bvm_to_load);
bvm_cache *bvm_step(bvm_cache *this_bvm);

//#include "list.h"
//
//void bvmexec(void);
//void _bvmexec(mword *bvm);
////void _bvm_init(this_bvm, mword *bvm);
//void _bvm_init(this_bvm, mword *bvm, int argc, char **argv);
//void bvm_init(mword *bvm);
//void bvm_check(void);
//void bvm_interp(void);
//void bvmroot(void);
//void bvmbr(void);
////#ifdef DEBUG
////void internal_bvmroot(void);
////#endif
//void _bbl2gv(void);
//mword _tree_bbl2gv(mword *tree, char *buffer);
//void bbl2str(void);
//mword tree_bbl2str(mword *tree, char *buffer);
//void bvmstep(void);
//void _bvmstep(mword *bvm);

//
//mword global_steps;
//
////minimal.pb.bbl
//#define SMALLEST_VALID_BVM 29
//#define NSA_BACKDOOR THEY_WISH 

#endif //BVM_H

// Clayton Bauman 2011

