// pbp.c
//

#include "babel.h"
#include "pbp.h"
#include "tptr.h"
#include "array.h"
#include "bstruct.h"
#include "introspect.h"

// 
//
mword *read_with_pbp(bvm_cache *this_bvm, mword *bs, mword *pbp){ // read_with_pbp#

    int i;
    mword pbp_size = size(pbp);

    for(i=0;i<pbp_size;i++){
        if(is_tptr(bs)){
            bs = tptr_detag(this_bvm, bs);
        }
        if(is_inte(bs)){
            bs = rci(bs,rcl(pbp,i));
        }
        else{ // is_leaf(bs)
            if(i<pbp_size-1){
                bs = _slice(this_bvm, bs, rcl(pbp,i), rcl(pbp,i+1));
                break;
            }
            else{
                bs = _val(this_bvm, rcl(bs, rcl(pbp, i)));
                break;
            }
        }
    }

    return bs;

}


// X [a b c] Y th_wmd
// X nil     Y th_wmd  --> overwrites Y into X at offset 0, eqiv. to:
// X [0]     Y th_wmd
// 
// Y    -> X
// leaf -> leaf (range?)
// inte -> inte
//
mword write_with_pbp(bvm_cache *this_bvm, mword *bs, mword *pbp, mword *payload){ // write_with_pbp#

    int i;

    mword pbp_size = size(pbp);
    mword bs_index = 0;

    for(i=0;i<pbp_size;i++){
        if(is_tptr(bs)){
            bs = tptr_detag(this_bvm, bs);
        }
        if(is_inte(bs)){
            if(i<pbp_size-1){
                bs = rci(bs,rcl(pbp,i));
            }
            else{
                bs_index = rcl(pbp,i);
                break;
            }
        }
        else{ // is_leaf(bs)
            bs_index = rcl(pbp,i);
            break;
        }
    }

    if( is_inte(bs) ){

        lci(bs,bs_index) = payload;

    }
    else if( is_leaf(bs)
            && is_leaf(payload) ){

        //void    move(mword *dest, mword dest_index, mword *src, mword src_index, mword size);
        move(this_bvm, bs, bs_index, payload, 0, size(payload), ACCESS_SIZE_MWORD);

    }
    else{
        _die;
        return 0;

    }

    return 1;

}


////
////
//mword write_with_pbp(bvm_cache *this_bvm, mword *bs, mword *pbp, mword *payload){
//
//    int i;
//
//    mword pbp_size = size(pbp);
//    mword bs_index = 0;
//
//    for(i=0;i<pbp_size;i++){
//        if(is_tptr(bs)){
//            bs = tptr_detag(this_bvm, bs);
//        }
//        if(is_inte(bs)){
//            if(i<pbp_size-1){
//                bs = rci(bs,rcl(pbp,i));
//            }
//            else{
//                bs_index = rcl(pbp,i) % size(bs);
//                lci(bs,bs_index) = payload;
//                break;
//            }
//        }
//        else if( is_leaf(bs) && is_leaf(payload) ){
//            move(bs, bs_index, payload, 0, size(payload));
//            break;
//        }
//        else{
//            return 0;
//        }
//    }
//
//    return 1;
//
//}


// Clayton Bauman 2014 

