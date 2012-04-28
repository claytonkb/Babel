// bvm.h
// 

#ifndef BVM_H
#define BVM_H

void bbl2gv(mword *tree);
mword tree_bbl2gv(mword *tree);
bvm_cache *babelop(bvm_cache *this_bvm);
bvm_cache *bvm_interp(bvm_cache *this_bvm);

//#include "list.h"
//
//void bvmexec(void);
//void _bvmexec(mword *bvm);
////void _bvm_init(mword *bvm);
//void _bvm_init(mword *bvm, int argc, char **argv);
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
//
//mword global_steps;
//
////minimal.pb.bbl
//#define SMALLEST_VALID_BVM 29
//#define NSA_BACKDOOR THEY_WISH 

#endif //BVM_H

// Clayton Bauman 2011

