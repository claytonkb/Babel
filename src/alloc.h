// alloc.h
// 

#ifndef ALLOC_H
#define ALLOC_H

void free_op(void);
void _free_op(mword *mem);
void del(void);
void _del(mword *tree);
void del_tree(mword *tree, mword *arr_list, mword *offset);
mword check_address(mword *arr_list, mword *address);

#endif //ALLOC_H

// Clayton Bauman 2012

