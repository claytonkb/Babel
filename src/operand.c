// operand.c
//

#include "babel.h"
#include "stack.h"
#include "operand.h"
#include "list.h"
#include "introspect.h"
#include "array.h"
#include "bvm.h"
#include "trie.h"
#include "tptr.h"
#include "bstruct.h"
#include "pearson16.h"
#include "string.h"

//
//
void get_operands(bvm_cache *this_bvm, int num_operands, ...){ // get_operands#

    va_list vl;
    va_start(vl,num_operands);

    mword *dstack = rci(this_bvm->dstack_ptr,0);

    mword stack_direction = stack_dir(this_bvm, dstack);

    mword *current_stack_entry = rci(stack_TOS(this_bvm, dstack), stack_direction);

    int i;
    oinfo *current_operand;
    mword *symbol_name;

//data_read_ptr
//data_overwrite_ptr

    for(i=0;i<num_operands;i++){

        current_operand = va_arg(vl,oinfo*);

//        if(is_stack_sentinel(this_bvm, current_stack_entry)){
//            if(!(current_operand->mask && OI_MASK_NONE)){
//                _fatal("operand required"); // FIXME: EXCEPTION
//            }
//            break;
//        }

        current_operand->data = rci(rci(rci(current_stack_entry,0),1),0);

        if(is_tptr(current_operand->data)){

            if(tageq(current_operand->data, BABEL_TAG_REF_SYM_LOCAL, TAG_SIZE)){

                symbol_name = tptr_simple_detag(current_operand->data);

                if(!exsym(this_bvm, symbol_name)){
                    insym(this_bvm, symbol_name, nil);
//                    insym2(this_bvm, HASH8(this_bvm,"foo"), nil);
//                    insym2(this_bvm, _hash8(this_bvm,symbol_name), nil);
                }

//                current_operand->data               = lusym(this_bvm, symbol_name);
//                current_operand->data_read_ptr      = current_operand->data;
//                current_operand->data_overwrite_ptr = _cdri(this_bvm, lusym2(this_bvm, symbol_name), 2);

                current_operand->data_overwrite_ptr = _cdri(this_bvm, lusym2(this_bvm, symbol_name), 2);
                current_operand->data_read_ptr      = rci(current_operand->data_overwrite_ptr,0);
                current_operand->data               = current_operand->data_read_ptr; //lusym(this_bvm, symbol_name);

            }
            else{
                if(current_operand->mask & OI_MASK_TPTR){ // XXX priority-conflict between this and the else-clause

                    if(!is_nil(current_operand->required_tag)
                            && !tageq(
                                current_operand->data, 
                                current_operand->required_tag,
                                TAG_SIZE )){
                        _fatal("wrong operand tag detected"); //FIXME: EXCEPTION
                    }

                } //else do nothing
            }

        }
        else{

            current_operand->data_overwrite_ptr = current_operand->data;
            current_operand->data_read_ptr      = current_operand->data;

            if(!((is_inte(current_operand->data) 
                    && (current_operand->mask & OI_MASK_INTE))
                ||
                (is_leaf(current_operand->data) 
                    && (current_operand->mask & OI_MASK_LEAF)))){
                _msg("OPCODE");
                _d(rcl(rci(_ith(this_bvm, this_bvm->code_ptr, 0),0),0));
                _msg("EXPECTED OPERAND");
                _d(current_operand->mask);
                _msg("ACTUAL OPERAND");
                //_memi(current_operand->data);
                _d(s(current_operand->data));
                _d(size(current_operand->data));
                _dumpf(current_operand->data);
                _msg("_dumpf(current_operand->data)");
                //_dump(current_operand->data);
                _fatal("wrong operand type detected"); //FIXME: EXCEPTION
            }

        }

        current_stack_entry = rci(current_stack_entry, stack_direction);

    }

    for(;i<num_operands;i++){
        if(!(current_operand->mask && OI_MASK_NONE)){
            _fatal("operand required"); // FIXME: EXCEPTION
        }
        else{
            current_operand->data = current_operand->default_data;
        }
    }

    va_end(vl);

}

// Clayton Bauman 2014

