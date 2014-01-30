// load.h
// 

#ifndef LOAD_H
#define LOAD_H

bvm_cache *load(bvm_cache *this_bvm);

bvm_cache *unload(bvm_cache *this_bvm);
mword *_unload(bvm_cache *this_bvm, mword *bs);
mword _runload(bvm_cache *this_bvm, mword *bs, mword *LUT_abs, mword *LUT_rel, mword *dest, mword *offset, mword *LUT_offset);
mword get_rel_offset(bvm_cache *this_bvm, mword *LUT_abs, mword *LUT_rel, mword *entry);
//mword *_newtptr(this_bvm, void);

mword *_load(bvm_cache *this_bvm, mword *bs, mword size);
void _rload(bvm_cache *this_bvm, mword *tree, mword offset);

//void load(void);
////void load_tree(mword *tree, mword offset);
////void _load(this_bvm, mword *tree);//, mword offset);
//void dump(void);
//void dump_tree(mword *tree);
//void arser(void);
//void arser_tree(mword *tree);
////void unload(void);
////void unload_tree(mword base, mword* tree, mword* dest, mword offset);
//void unload(void);
//mword *_unload(this_bvm, mword *tree);
//mword unload_tree(
//        mword *tree, 
//        mword *LUT_abs, 
//        mword *LUT_rel, 
//        mword *dest, 
//        mword *offset,
//        mword *LUT_offset);
//mword get_rel_offset(this_bvm, mword *LUT_abs, mword *LUT_rel, mword *elem);
//

////void _load(this_bvm, mword *tree);
////void load_tree(mword *tree, mword offset);

#endif

// Clayton Bauman 2011

