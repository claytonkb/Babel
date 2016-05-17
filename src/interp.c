// interp.c
//

#include "babel.h"
#include "interp.h"
#include "init.h"
#include "bvm.h"
#include "stack.h"
#include "list.h"
#include "mem.h"
#include "cache.h"
#include "opcode.h"
#include "introspect.h"
#include "rstack.h"
#include "eval.h"
//#include "construct.sp.h"
#include "trie.h"
#include "string.h"
#include "array.h"
#include "tptr.h"
#include "access.h"
#include "stack.h"
#include "bpdl.h"
#include "pearson16.h"

#define X(a, b, c, d) b,
static char **interp_opcode_names = (char *[]){
OPCODE_LIST
""
};
#undef X

//#define INTERP_CORE_TRACE 

//
//
int interp_babel(bvm_cache *this_bvm, int argc, char **argv, char **envp){ // interp_babel#

#ifdef BABEL_RESET_TRACE
_trace;
#endif

    clock_t wall_clock_time;
    wall_clock_time = clock();

    jmp_buf cat_ex;

    int val;
    val = setjmp(cat_ex);

    if(val==CAT_EXCEPT){
        fprintf(stderr,"CATASTROPHIC EXCEPTION: babel\n");
        _die;
    }
    else if(val==INTERP_RESET){
        fprintf(stderr,"INTERP_RESET: babel\n");
    }

    init_interp(this_bvm, argc, argv, envp, &cat_ex);

    _pre_interp(this_bvm);

    _babel_root(this_bvm, this_bvm->self);

    _post_interp(this_bvm);

    wall_clock_time = (clock() - wall_clock_time) / CLOCKS_PER_SEC;

    return wall_clock_time;

}


//
//
bvm_cache *interp_core(bvm_cache *this_bvm){ // interp_core#

    jmp_buf op_restart;
    int val;

    // FIXME: Each fresh instance of interp_core must have its own op_restart
    this_bvm->interp->op_restart = &op_restart;

#if(defined INTERP_CORE_TRACE || defined BABEL_RESET_TRACE)
_trace;
#endif

    if(this_bvm->flags->BVM_CACHE_INVALID == FLAG_SET){
        _cat_except(this_bvm);
    }

#ifdef INTERP_CORE_TRACE
_trace;
#endif

    while( interp_update_steps(this_bvm) ){

#ifdef INTERP_CORE_TRACE
_trace;
#endif

        val = setjmp(op_restart);

        if(val==OP_RESTART){
            if(this_bvm->flags->MC_GC_OP_RESTART == FLAG_SET){ // non-reentrant
                _cat_except(this_bvm);
            }
            else{

                if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
                    _msg("MC_GC_OP_RESTART");
                }

                // FIXME Add livelock detect FIXME
                this_bvm->flags->MC_GC_OP_RESTART = FLAG_SET;
                this_bvm->flags->BVM_INSTR_IN_PROGRESS = FLAG_CLR;
                mem_copy_collect(this_bvm);
                this_bvm->flags->MC_GC_OP_RESTART = FLAG_CLR;
                val = 0;
#ifdef INTERP_CORE_TRACE
_trace;
#endif
            }
        }

#ifdef INTERP_CORE_TRACE
_trace;
#endif

        this_bvm->flags->BVM_INSTR_IN_PROGRESS = FLAG_SET;

        if(code_empty(this_bvm)){
#ifdef INTERP_CORE_TRACE
_trace;
#endif
            if(!rstack_empty(this_bvm)){
#ifdef INTERP_CORE_TRACE
_trace;
#endif

//static mword count=0;
//count++;
//
//if(count == 0x696){
//_dumpf(this_bvm->rstack_ptr);
//_msg("_dumpf(this_bvm->rstack_ptr);");
//}

                _next(this_bvm);
                continue;
            }

            // Exiting due to code-list empty
            this_bvm->flags->BVM_CODE_LIST_EMPTY = FLAG_SET;
            break;

        }

        mword *next_entry = interp_get_next_code_entry(this_bvm);

        if( is_inte(next_entry) ){

#ifdef INTERP_CORE_TRACE
_memi(rci(next_entry,0));
#endif

            if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
//                fprintf(stderr, " --> %08x     push\n", this_bvm->interp->global_tick_count);
//                _memi(rci(next_entry,0));
                mword *operand = rci(next_entry,0);
                fprintf(stderr, "%08x     push ", this_bvm->interp->global_tick_count);
                if(is_leaf(operand)){
                    fprintf(stderr, "%x", rcl(operand,0));
                    if(size(operand)>1){
                        fprintf(stderr, " ...");
                    }
                    fprintf(stderr, "\n");
                }
                else if(is_inte(rci(next_entry,0))){
                    fprintf(stderr, "[]\n");
                }
                else{
                    fprintf(stderr, "tag\n");
                }
            }

            interp_push_operand(this_bvm, rci(next_entry,0));

        }
        else if( is_leaf(next_entry) ){

#ifdef INTERP_CORE_TRACE
_d(rcl(next_entry,0))
#endif

            if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){ // FIXME: Block this out with #ifdef DEV_MODE
                _opcode_trace(rcl(next_entry,0));
            }

#ifdef INTERP_CORE_TRACE
_trace;
#endif

            interp_op_exec(this_bvm, rcl(next_entry,0));

        }
        else{ // is_tptr(next_entry)

#ifdef INTERP_CORE_TRACE
_trace;
#endif

            //built-ins
            //  nil
            //  true/false
            //  accept/reject (pass/fail)
            //  known types
            //      list/code/ptr/val/str/bvm/etc.
            //  macros
            //  soft exceptions
            //lusym

            interp_push_operand(this_bvm, tptr_new(this_bvm, BABEL_TAG_REF_SYM_LOCAL, next_entry));

//            if(exsym(this_bvm, next_entry)){
//                interp_push_operand(this_bvm, lusym(this_bvm, next_entry));
//            }
//            else{
//                interp_push_operand(this_bvm, nil);
//            }

        }

        this_bvm->flags->BVM_INSTR_IN_PROGRESS = FLAG_CLR;

#ifdef INTERP_CORE_TRACE
_trace;
#endif

        if(this_bvm->flags->MC_GC_PENDING == FLAG_SET){

#ifdef INTERP_CORE_TRACE
_trace;
#endif

            mem_copy_collect(this_bvm);

        }

        mword advance_type = get_advance_type(this_bvm);

        if(advance_type == BVM_ADVANCE){

#ifdef INTERP_CORE_TRACE
_trace;
#endif

            interp_advance(this_bvm);

        }
        else if(advance_type == BVM_RETURN){

#ifdef INTERP_CORE_TRACE
_trace;
#endif
            if(this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET){
                fprintf(stderr, " --> BVM_RETURN\n");
            }

            break;

        }
        else{

#ifdef INTERP_CORE_TRACE
_trace;
#endif

            // do nothing...
            set_advance_type(this_bvm, BVM_ADVANCE);

        }

#ifdef INTERP_CORE_TRACE
_trace;
#endif
        this_bvm->interp->global_tick_count++;
        this_bvm->flags->MC_GC_PNR = FLAG_CLR;

    }

    cache_flush(this_bvm);

#ifdef INTERP_CORE_TRACE
_trace;
#endif

    return this_bvm;

}


//
//
bvm_cache *interp_advance(bvm_cache *this_bvm){ // interp_advance#

#if 0 // Might interfere with OP_RESTART

    static mword *code_ptr_livelock_detect = NULL;

    if( rci(this_bvm->code_ptr,0) == code_ptr_livelock_detect ){
        _dumpf(this_bvm->code_ptr);
        _fatal("code_ptr_livelock_detect; test.dot updated");
    }

    code_ptr_livelock_detect = rci(this_bvm->code_ptr,0);

#endif

    lci(this_bvm->code_ptr,0) = rci(rci(this_bvm->code_ptr,0),1);

    return this_bvm;

}


//
//
mword *interp_get_next_code_entry(bvm_cache *this_bvm){ // interp_get_next_code_entry#

    return rci(rci(this_bvm->code_ptr,0),0);

}


//
//
bvm_cache *interp_op_exec(bvm_cache *this_bvm, mword opcode){ // interp_op_exec#

    bvm_cache *discard;
    babel_op op_ptr;

    op_ptr = (babel_op)this_bvm->interp->jump_table[ opcode % NUM_INTERP_OPCODES ];

    discard = op_ptr(this_bvm);

    discard = discard; // suppress compiler warning, gets optimized away

    return this_bvm;

}


//
//
mword interp_update_steps(bvm_cache *this_bvm){ // interp_update_steps#

    mword *steps = rci(this_bvm->steps,0);
    mword return_val = rcl(steps,0);

    lcl(steps,0) = return_val-1;

    if(this_bvm->interp->global_tick_count 
            > this_bvm->interp->limits->max_num_steps){
        return_val = 0;
    }

#ifdef PROF_MODE
    if((this_bvm->flags->BVM_INTERP_OP_TRACE == FLAG_SET)
            && ((return_val % 1024) == 0)){
        fprintf(stderr, "%d time (ms)\n", (time_ms() - this_bvm->interp->epoch_ms));
    }
#endif

    return return_val;

}


//
//
bvm_cache *interp_push_operand(bvm_cache *this_bvm, mword *operand){ // interp_push_operand#

//    stack_push(this_bvm,
//            rci(this_bvm->dstack_ptr,0),
//            stack_new_entry(
//                this_bvm,
//                operand,
//                nil));

    dstack_push(this_bvm,
            stack_new_entry(
                this_bvm,
                operand,
                nil));

    return this_bvm;

}


// OPERATOR
//
bvm_cache *rsvd(bvm_cache *this_bvm){ // rsvd#

    _error("an attempt was made to execute a reserved opcode");

    return this_bvm;

}


// XXX This is a DEBUG FUNCTION... DO NOT USE XXX 
//
bvm_cache *_pre_interp(bvm_cache *this_bvm){ // _pre_interp#

//    char **values = (char *[]){"abc", "def", "ghi"};
//
//    fprintf(stderr, "%s\n", values[1]);

//    mword i=0;
//
//    for(i=0; i<20; i++){
//        printf("%d:%d ", i, ((i+2)/3)*4);
//    }
//_die;
#ifdef PROF_MODE
this_bvm->interp->profile->BVM_PROFILE_ENABLE = FLAG_SET;
#endif

//_dumpf(bpdl_macro_table);
//_msg("_dumpf(bpdl_macro_table);");

//PROFILE_ALLOC_SIZE_8 = 0;
//PROFILE_ALLOC_SIZE_2 = 0;
//PROFILE_ALLOC_SIZE_3 = 0;
//PROFILE_ALLOC_SIZE_4 = 0;
//PROFILE_ALLOC_SIZE_5 = 0;
//PROFILE_ALLOC_SIZE_6 = 0;
//PROFILE_ALLOC_SIZE_7 = 0;
//

//    _dumpf(this_bvm->self);

//    MEMORYSTATUSEX statex;
//    statex.dwLength = sizeof(statex);
//    GlobalMemoryStatusEx(&statex);
//
//    #define DIV 1024
//
//    printf("There is  %d percent of memory in use.\n",    (int)statex.dwMemoryLoad);
//    printf("There are %d total KB of physical memory.\n", (int)statex.ullTotalPhys/DIV);
//    printf("There are %d free  KB of physical memory.\n", (int)statex.ullAvailPhys/DIV);
//    printf("There are %d total KB of paging file.\n",     (int)statex.ullTotalPageFile/DIV);
//    printf("There are %d free  KB of paging file.\n",     (int)statex.ullAvailPageFile/DIV);
//    printf("There are %d total KB of virtual memory.\n",  (int)statex.ullTotalVirtual/DIV);
//    printf("There are %d free  KB of virtual memory.\n",  (int)statex.ullAvailVirtual/DIV);
//    printf("There are %d free  KB of extended memory.\n", (int)statex.ullAvailExtendedVirtual/DIV);

//    insym(this_bvm, HASH8(this_bvm,"foo"), _val(this_bvm, 0xabaddeed));
//    mword *my_sym = lusym(this_bvm, HASH8(this_bvm,"foo"));
//    mword *my_sym = bvm_sym(this_bvm,BABEL_SYM_SOFT_ROOT);
//    mword *my_sym = trie_lookup_hash(this_bvm, bvm_sym(this_bvm,BABEL_SYM_SOFT_ROOT), HASH8(this_bvm,"foo"), nil);

//    _dump(my_sym);
//    _die;

//    #include "opcodes.bbl.h"
//
//    mword *load_opcodes = _newlfcp(this_bvm, opcodes_bbl, BYTE_SIZE(OPCODES_BBL_SIZE));
//    memcpy(load_opcodes, opcodes_bbl, BYTE_SIZE(OPCODES_BBL_SIZE)); // WAIVER required
//    mword *opcode_table = _load(this_bvm, (mword*)load_opcodes,OPCODES_BBL_SIZE);

//    _dump(bpdl_opcode_table);
//    _die;

    //'c', 'a', 'e', 's', 'a', 'r'
//    mword *test = _mklf(this_bvm, 8, '"', 'c', 'a', 'e', 's', 'a', 'r', '"');
//    mword *test1 = bpdl_quote_to_bstruct(this_bvm, test);

//    mword *test = _mklf(this_bvm, 5, '-', '9', '7', '5', '3');
//    mword *test1 = bpdl_dnumber_to_bstruct(this_bvm, test);

//    mword *test = _mklf(this_bvm, 8, '0', 'x', '0', '2', '4', 'c', 'e', 'f');
//    mword *test1 = bpdl_hnumber_to_bstruct(this_bvm, test);

//    mword index  = 0;
//    mword *test  = _str2ar(this_bvm, C2B("[list 123 'abc' 456]"));
//    mword *test1 = _pre_sexpr(this_bvm, test, &index);
//    mword *test2 = inline_bpdl(this_bvm, test1);
////    mword type   = get_bpdl_list_type(test1);
//
//    //test1 = (mword*)icdr(test1);
////    mword type = get_bpdl_list_entry_type((mword*)icar(test1));
////    _d(type);
////    _dump(test);
////    _dump(test1);
//    _dump(test2);
//    _die;

//    mword *foo = _mkdls(this_bvm, 1,
//                    _val(this_bvm, 1));

//    mword *foo = _mkdls(this_bvm, 6,
//                    _val(this_bvm, 6), 
//                    _val(this_bvm, 5), 
//                    _val(this_bvm, 4),
//                    _val(this_bvm, 3), 
//                    _val(this_bvm, 2), 
//                    _val(this_bvm, 1));
//
//foo = _reverse_dlist(this_bvm,foo,foo);
//_dump(foo);
//_die;

//    mword *bar = _mkdls(this_bvm, 3,
//                    _val(this_bvm, 6), 
//                    _val(this_bvm, 5), 
//                    _val(this_bvm, 4));
//
//    foo = _insdls(this_bvm, bar, foo);
//
//_dump(foo);
//_die;

//
//stack_push(
//        this_bvm, 
//        my_stack, 
//        stack_new_entry(
//            this_bvm,
//            _val(this_bvm, 0xdead),
//            nil));
//
//stack_push(
//        this_bvm, 
//        my_stack, 
//        stack_new_entry(
//            this_bvm,
//            _val(this_bvm, 0xbeef),
//            nil));
//
//stack_rot(this_bvm, my_stack); 
//stack_flip(this_bvm, my_stack); 
//
//stack_push(
//        this_bvm, 
//        my_stack, 
//        stack_new_entry(
//            this_bvm,
//            _val(this_bvm, 0xbabe),
//            nil));
//
//stack_push(
//        this_bvm, 
//        my_stack, 
//        stack_new_entry(
//            this_bvm,
//            _val(this_bvm, 0xface),
//            nil));

//stack_rot(this_bvm, my_stack); 

//stack_pop(this_bvm, my_stack);
//stack_pop(this_bvm, my_stack);

//mword foo = is_dstack_empty(this_bvm, my_stack);
//_d(foo);


//stack_twist(this_bvm, my_stack);

//stack_bottom(this_bvm, my_stack);
//
//stack_rot(this_bvm, my_stack); 
//stack_flip(this_bvm, my_stack); 
//
//stack_dump(this_bvm, my_stack);

//_dump(my_stack);
//_die;

//mword *bar = stack_bottom(this_bvm, my_stack);
//
//_dump(bar);
//_die;
//
//mword depth = stack_depth(this_bvm, my_stack);
//_d(depth);
//
//mword dia = stack_dia(this_bvm, my_stack);
//_d(dia);
//_die;

//mword *y;
//y = stack_new_entry(this_bvm, _val(this_bvm, 0xdead), nil);
//stack_push(this_bvm, my_stack, y);
//

//mword x = is_dstack_clear(this_bvm, my_stack);
//_d(x);
//
//mword z = is_ustack_clear(this_bvm, my_stack);
//_d(z);
//
//_die;


//    mword *temp  = _mklf(this_bvm, 5, 0, 1, 1, 0, 2);
//    mword *temp3 = _mklf(this_bvm, 3, 0, 1, 1);
//    mword *temp4 = _mklf(this_bvm, 4, 0, 1, 1, 1);
//    mword *temp = _mklf(this_bvm, 4, 0, 1, 0, 1);

//    trav_return temp2 = _trav(this_bvm, this_bvm->self, temp);
//    _dump(temp2.bs);

//    _set(this_bvm, this_bvm->self, temp,  _val(this_bvm, 0xdeadbeef));
//    _set(this_bvm, this_bvm->self, temp4, _ptr(this_bvm, _val(this_bvm, 0xdeadbeef) ));
//
//    mword *temp2 = _get(this_bvm, this_bvm->self, temp3);
//    _dump(temp2);

//    _dump(this_bvm->self);
//   _die;

//    mword *test = trie_lookup_hash(this_bvm, 
//                            tptr_detag(this_bvm, this_bvm->self), 
//                            nil, 
//                            string_c2b(this_bvm, "parent_bvm", 100));
//
//    _dump(test);
//    _die;
//

//    mword ntags = _ntag(this_bvm, this_bvm->self);
//    mword ninte = _nin(this_bvm, this_bvm->self);
//    mword nleaf = _nlf(this_bvm, this_bvm->self);
//    mword mu    = _mu(this_bvm, this_bvm->self);
//    _d(ntags);
//    _d(ninte);
//    _d(nleaf);
//    _d(mu);
//    _die;


//    mword *test = _bs2ar(this_bvm, this_bvm->self);
//    _msort(this_bvm, test);
//    _dump(test);
//    _die;

//    mword *foo = _mkls(this_bvm, 3,
//                    _val(this_bvm, 3), 
//                    _val(this_bvm, 2), 
//                    _val(this_bvm, 1));

//    mword *test  = _unload_fast(this_bvm, this_bvm->self);
//    mword *test  = _cp_fast(this_bvm, this_bvm->self);
//    mword *test = _unload(this_bvm, this_bvm->self);
//_d(size(test));
//_d(size(test2));
//_die;
//    mword *test = _unload(this_bvm, foo);
//_memi(test);
//    _dump(test);

//    mword *test = trie_new(this_bvm);
//    trie_insert(this_bvm, test, nil, string_c2b(this_bvm, "soft_root", 100), _val(this_bvm, 0xabaddeed));
//    trie_insert(this_bvm, test, nil, string_c2b(this_bvm, "parent_bvm", 100), _val(this_bvm, 0xfadedcab));
//    trie_insert(this_bvm, test, nil, string_c2b(this_bvm, "ustack_ptr", 100), _val(this_bvm, 0xdeadbeef));
//    trie_insert(this_bvm, test, nil, string_c2b(this_bvm, "code_ptr", 100), _val(this_bvm, 0xbabeface));
//    trie_insert(this_bvm, test, nil, string_c2b(this_bvm, "baz", 100), _val(this_bvm, 0xcafefeed));
//
//    _dump(test);
//    _die;

//    mword *test2 = trie_lookup_hash(this_bvm, test, nil, string_c2b(this_bvm, "foo", 100));
//    _dump(test2);
//    _die;
//
//    mword removed = trie_remove(this_bvm, test, nil, string_c2b(this_bvm, "baz", 100));
//    _d(removed);
////    removed = trie_remove(this_bvm, test, nil, string_c2b(this_bvm, "code_ptr", 100));
////    _d(removed);
//    removed = trie_remove(this_bvm, test, nil, string_c2b(this_bvm, "ustack_ptr", 100));
//    _d(removed);
//    _dump(test);
//    _die;

    return this_bvm;

}


//
//
void _interp_exit(bvm_cache *this_bvm){ // interp_exit#

    set_advance_type(this_bvm, BVM_RETURN);

    return;

}


// XXX This is a DEBUG FUNCTION... DO NOT USE XXX 
//
bvm_cache *_post_interp(bvm_cache *this_bvm){ // _post_interp#

//_d(this_bvm->interp->profile->mem_alloc_count);

//_d(PROFILE_ALLOC_SIZE_8);
//_d(PROFILE_ALLOC_SIZE_2);
//_d(PROFILE_ALLOC_SIZE_3);
//_d(PROFILE_ALLOC_SIZE_4);
//_d(PROFILE_ALLOC_SIZE_5);
//_d(PROFILE_ALLOC_SIZE_6);
//_d(PROFILE_ALLOC_SIZE_7);

//    stack_dump(this_bvm, this_bvm->dstack_ptr);
//    _dump(this_bvm->self);

    return this_bvm;

}


/*****************************************************************************
 *                                                                           *
 *                            INTERP OPERATORS                               *
 *                                                                           *
 ****************************************************************************/


#define EXIT_OPS \
    _interp_exit(this_bvm);

OPERATOR_R0_W0(interp_exit,EXIT_OPS)




// Clayton Bauman 2014

