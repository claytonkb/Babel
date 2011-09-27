// bvm.h
// 

#ifndef BVM_H
#define BVM_H

void bvmexec(void);
void _bvmexec(mword *bvm);
void _bvm_init(mword *bvm);
void bvm_init(void);
void bvm_check(void);
void bvm_interp(void);
void bvmroot(void);
//#ifdef DEBUG
//void internal_bvmroot(void);
//#endif
void bbl2gv(void);
mword tree_bbl2gv(mword *tree);
void _bbl2gv(void);
mword _tree_bbl2gv(mword *tree);

//minimal.pb.bbl
#define SMALLEST_VALID_BVM 29
#define NSA_BACKDOOR THEY_WISH 

#endif //BVM_H

// Clayton Bauman 2011

