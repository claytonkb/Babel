// rstack.c
//

#include "babel.h"
#include "rstack.h"
#include "list.h"
#include "cache.h"
#include "introspect.h"


//
//
bvm_cache *rstack_push(bvm_cache *this_bvm, mword *rstack_entry){ // rstack_push#

    _push(this_bvm, rci(this_bvm->rstack_ptr,0), rstack_entry);
    lci(this_bvm->rstack_ptr,0) = rstack_entry;

    return this_bvm;

}


//
//
bvm_cache *rstack_pop(bvm_cache *this_bvm){ // rstack_pop#

    lci(this_bvm->rstack_ptr,0) = _pop(this_bvm, rci(this_bvm->rstack_ptr,0));

    return this_bvm;

}


//
//
mword *rstack_first_looplike_entry(bvm_cache *this_bvm, mword *rstack_entry){ // rstack_first_looplike_entry#

    if(is_nil(rstack_entry)){
        return nil;
    }

    if(rstack_is_entry_looplike(get_rstack_entry_tag(this_bvm,rci(rstack_entry,0)))){

        return rci(rstack_entry,0);

    }
    else{

        return rstack_first_looplike_entry(this_bvm, rci(rstack_entry,1));

    }

    return nil; // quash compiler warning

}


//
//
void rstack_pop_til_looplike_entry(bvm_cache *this_bvm){ // rstack_pop_til_looplike_entry#

    while(!rstack_empty(this_bvm)){


        if(rstack_is_entry_looplike(
                    get_rstack_entry_tag(this_bvm,
                        rci(_ith(this_bvm,this_bvm->rstack_ptr,0),0)))){
            rstack_pop(this_bvm);
            break;
        }
        else{
            rstack_pop(this_bvm);
        }

    }

}


//
//
mword rstack_is_entry_looplike(mword *entry_tag){ // rstack_is_entry_looplike#

    if(     tageq(entry_tag, BABEL_TAG_TIMES,  TAG_SIZE)
        ||  tageq(entry_tag, BABEL_TAG_LOOP,   TAG_SIZE)
        ||  tageq(entry_tag, BABEL_TAG_EACH,   TAG_SIZE)
        ||  tageq(entry_tag, BABEL_TAG_EACHAR, TAG_SIZE)
        ||  tageq(entry_tag, BABEL_TAG_WHILE,  TAG_SIZE)){
        return 1;
    }

    return 0;

}


// Clayton Bauman 2014

