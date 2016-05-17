// pbp.h
// 

#ifndef PBP_H
#define PBP_H

mword *read_with_pbp( bvm_cache *this_bvm, mword *bs, mword *pbp);
mword  write_with_pbp(bvm_cache *this_bvm, mword *bs, mword *pbp, mword *payload);

#endif //PBP_H

// Clayton Bauman 2014

