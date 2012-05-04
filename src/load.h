// load.h
// 

#ifndef LOAD_H
#define LOAD_H

bvm_cache *load(bvm_cache *this_bvm);
//void clean_tree(mword *tree);
mword *_load(mword *tree, mword tree_size);
mword *load_tree(
        mword *tree, 
        mword offset,
        mword *LUT_abs, 
        mword *LUT_rel, 
        mword *LUT_offset);
mword *get_abs_offset(LUT_rel, LUT_abs, tree);

//void load(void);
////void load_tree(mword *tree, mword offset);
////void _load(mword *tree);//, mword offset);
//void dump(void);
//void dump_tree(mword *tree);
//void arser(void);
//void arser_tree(mword *tree);
////void unload(void);
////void unload_tree(mword base, mword* tree, mword* dest, mword offset);
//void unload(void);
//mword *_unload(mword *tree);
//mword unload_tree(
//        mword *tree, 
//        mword *LUT_abs, 
//        mword *LUT_rel, 
//        mword *dest, 
//        mword *offset,
//        mword *LUT_offset);
//mword get_rel_offset(mword *LUT_abs, mword *LUT_rel, mword *elem);
//

////void _load(mword *tree);
////void load_tree(mword *tree, mword offset);

#endif

// Clayton Bauman 2011

