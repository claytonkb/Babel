// tptr.c
//

#include "babel.h"
#include "tptr.h"
#include "mem.h"
#include "bstruct.h"
#include "operator.h"
#include "introspect.h"

//
//
mword *tptr_new(bvm_cache *this_bvm, const mword *hash, mword *bs){ // tptr_new#

    mword *ptr = mem_alloc( this_bvm, TPTR_SFIELD );

    int i;
    for(i=0; i<HASH_SIZE; i++){ // FIXME: PERF... use memcpy
        ptr[i] = hash[i];
    }

    lcl(ptr,HASH_SIZE)   = INTE_SFIELD*MWORD_SIZE;

    set_tptr(ptr,bs);

    return ptr;

}


//
//
mword *tptr_extract_hash(bvm_cache *this_bvm, mword *tptr){ // tptr_extract_hash#

    mword *ptr = _newlfi(this_bvm, HASH_SIZE, 0);
    int i;

    for(i=0; i<HASH_SIZE; i++){// FIXME: PERF... use memcpy
        ptr[i] = rcl(tptr,i);
    }

    return ptr;

}


//
//
void tptr_update_hash(bvm_cache *this_bvm, mword *tptr, mword *hash){ // tptr_update_hash#

    int i;

    for(i=0; i<HASH_SIZE; i++){// FIXME: PERF... use memcpy
        tptr[i] = rcl(hash,i);
    }

}


//
//
mword *tptr_extract_ptr(bvm_cache *this_bvm, mword *tptr){ // tptr_extract_ptr#

    mword* temp = (mword*)(tptr+TPTR_PTR_OFFSET);

    if(is_tptr(temp)){
        return tptr_extract_ptr(this_bvm, temp);
    }
    else{
        return temp;
    }

}


#define MAX_DETAG_DEPTH 1024

// Safely, recursively de-references a tag
mword *tptr_detag(bvm_cache *this_bvm, mword *tptr){ // tptr_detag#

    static int livelock_detect=0;

    if(is_nil(tptr)){
        return nil;
    }

    if(is_tptr(tptr)){
        if(livelock_detect++ > MAX_DETAG_DEPTH){
            //cat_except(this_bvm);
            _fatal("FIXME: this should have been a cat_except...");
        }
        return tptr_detag(this_bvm, get_tptr(tptr));
    }
    else{
        livelock_detect=0;
        return tptr;
    }

}


// Hard de-references a tag (recursive)
//
mword *tptr_hard_detag(bvm_cache *this_bvm, mword *tptr){ // tptr_hard_detag#

    mword *temp = get_tptr(tptr);

    if(is_tptr(temp)){
        return tptr_hard_detag(this_bvm, temp);
    }
    else{
        return temp;
    }

}


// XXX The return-value from this function contains unsafe pointers!!! XXX
// XXX internal interp use ONLY                                        XXX
// XXX If you pass tag=nil, returns ALL tags in bs                     XXX
// XXX PERF: A _tags2ar (like _bs2ar) would be more efficient          XXX
//
mword *tptr_find_tag_unsafe(bvm_cache *this_bvm, mword *bs, mword *tag){ // tptr_find_tag_unsafe#

    mword *span_array     = _bs2ar(this_bvm, bs);
    mword size_span_array = size(span_array);
    mword size_inte;
    mword *tag_list       = nil;
    mword *curr_span_elem;
    mword *curr_inte_elem;


//_dump(span_array);

    int i,j;
    for(i=0; i<size_span_array; i++){

        curr_span_elem = rci(span_array,i);

        if(is_inte(curr_span_elem)){ // check each element

            size_inte = size(curr_span_elem);

            for(j=0;j<size_inte;j++){

                curr_inte_elem = rci(curr_span_elem,j);

                if(is_nil(curr_inte_elem)){
                    continue;
                }

                if(is_tptr(curr_inte_elem)){

                    if(is_nil(tag)){

                        // push onto tag_list
                        if(is_nil(tag_list)){
                            tag_list = _cons(this_bvm, (curr_span_elem+j), nil);
                        }
                        else{
                            _unshift(this_bvm, tag_list, (curr_span_elem+j));
                        }

                    }
                    else{

                        if( tageq(curr_inte_elem, tag, TAG_SIZE) ){

                            // push onto tag_list
                            if(is_nil(tag_list)){
                                tag_list = _cons(this_bvm, (curr_span_elem+j), nil);
                            }
                            else{
                                _unshift(this_bvm, tag_list, (curr_span_elem+j));
                            }

                        }

                    }


                }

            }

        }

    }

    return tag_list;

}


//
//
mword *tptr_find_tag(bvm_cache *this_bvm, mword *search_bs, mword *tag){  // tptr_find_tag#

    if(is_nil(search_bs) || is_nil(tag)){
        return nil;
    }

    mword *search_tag;

    if(is_tptr(tag)){
        search_tag = tag;
    }
    else{
        search_tag = tptr_new(this_bvm, tag, nil);
    }

    mword *sub_list = bstruct_find(this_bvm, search_bs, search_tag);
    mword *result = nil;

    while(!is_nil(sub_list)){
        result = _cons(this_bvm, rci(rci(sub_list,0),0), result);
        sub_list = (mword*)icdr(sub_list);
    }

    return result;

}



/*****************************************************************************
 *                                                                           *
 *                              TPTR OPERATORS                               *
 *                                                                           *
 ****************************************************************************/


#define FIND_TAG_D_OPERATIONS \
    result0 = tptr_find_tag(this_bvm, oi1.data, oi0.data);

OPERATORA_R2_W1_D(find_tag_d, 
        FIND_TAG_D_OPERATIONS, 
        0, OI_MASK_ANY, 0, 0,
        0, OI_MASK_ANY, 0, 0)


#define DETAG_OPERATIONS \
    result0 = get_tptr(oi0.data);

OPERATORA_R1_W1_D(detag_d, DETAG_OPERATIONS, nil, OI_MASK_TPTR, 0, 0)


// Clayton Bauman 2014

