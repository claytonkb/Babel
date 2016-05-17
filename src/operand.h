// operand.h
//

#ifndef OPERAND_H
#define OPERAND_H

#define OI_MASK_NONE 1<<0 // OI_MASK_NONE#
#define OI_MASK_LEAF 1<<1 // OI_MASK_LEAF#
#define OI_MASK_INTE 1<<2 // OI_MASK_INTE#
#define OI_MASK_TPTR 1<<3 // OI_MASK_TPTR#
#define OI_MASK_ANY  (OI_MASK_LEAF|OI_MASK_INTE|OI_MASK_TPTR) // OI_MASK_ANY#

typedef struct { // operand_info#

    mword *data_read_ptr;
    mword *data_overwrite_ptr;

    mword *data; // immediate?
    mword *default_data;
    mword mask;
    mword *required_tag;
    mword min_size;
    mword max_size;

} oinfo;

void get_operands(bvm_cache *this_bvm, int num_operands, ...);

#endif //OPERAND_H

// Clayton Bauman 2014

