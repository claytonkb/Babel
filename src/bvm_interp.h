// bvm_interp.h

#ifndef BVM_INTERP_H
#define BVM_INTERP_H

#include "babel.h"
#include "bvm.h"
#include "list.h"

#define opcode_switch(x)    \
    switch(x){              \
        arith_ops           \
        io_ops              \
        array_ops           \
        bstruct_ops         \
        list_ops            \
        stack_ops           \
        eval_ops            \
        hash_ops            \
        bvm_ops             \
        cmp_ops             \
        string_ops          \
        shift_ops           \
        logic_ops           \
        util_ops            \
        default:                    \
            fprintf(stderr, "opcode %x\n", x);                        \
            error("bvm_interp: unrecognized opcode");  \
            break;          \
    }                       \

#define arith_ops       \
    case CUADD:         \
        cuadd(this_bvm);        \
        break;          \
    case CUSUB:         \
        cusub(this_bvm);        \
        break;          \
    case CIADD:         \
        ciadd(this_bvm);        \
        break;          \
    case CISUB:         \
        cisub(this_bvm);        \
        break;          \
    case CIABS:         \
        ciabs(this_bvm);        \
        break;          \
    case CUMUL:         \
        cumul(this_bvm);        \
        break;          \
    case CUDIV:         \
        cudiv(this_bvm);        \
        break;          \
    case CUREM:         \
        curem(this_bvm);        \
        break;          \
    case CIMUL:         \
        cimul(this_bvm);        \
        break;          \
    case CIDIV:         \
        cidiv(this_bvm);        \
        break;          \
    case CIREM:         \
        cirem(this_bvm);        \
        break;          \

#define shift_ops       \
      case CUSHL:       \
        cushl(this_bvm);        \
        break;          \
      case CUSHR:       \
        cushr(this_bvm);        \
        break;          \
      case CASHR:       \
        cashr(this_bvm);        \
        break;          \
      case CUROL:       \
        curol(this_bvm);        \
        break;          \
      case CUROR:       \
        curor(this_bvm);        \
        break;          \

#define logic_ops       \
      case F000:        \
        logicF0(this_bvm);      \
        break;          \
      case F001:        \
        logicF1(this_bvm);      \
        break;          \
      case F002:        \
        logicF2(this_bvm);      \
        break;          \
      case F003:        \
        logicF3(this_bvm);      \
        break;          \
      case F004:        \
        logicF4(this_bvm);      \
        break;          \
      case F005:        \
        logicF5(this_bvm);      \
        break;          \
      case F006:        \
        logicF6(this_bvm);      \
        break;          \
      case F007:        \
        logicF7(this_bvm);      \
        break;          \
      case F008:        \
        logicF8(this_bvm);      \
        break;          \
      case F009:        \
        logicF9(this_bvm);      \
        break;          \
      case F00A:        \
        logicFA(this_bvm);      \
        break;          \
      case F00B:        \
        logicFB(this_bvm);      \
        break;          \
      case F00C:        \
        logicFC(this_bvm);      \
        break;          \
      case F00D:        \
        logicFD(this_bvm);      \
        break;          \
      case F00E:        \
        logicFE(this_bvm);      \
        break;          \
      case F00F:        \
        logicFF(this_bvm);      \
        break;          \
      case CNOT:        \
        cnot(this_bvm);         \
        break;          \
      case ANDOP:       \
        andop(this_bvm);        \
        break;          \
      case OROP:        \
        orop(this_bvm);         \
        break;          \
      case NOTOP:       \
        notop(this_bvm);        \
        break;          \

#define cmp_ops         \
      case CNE:         \
        cne(this_bvm);          \
        break;          \
      case CEQ:         \
        ceq(this_bvm);          \
        break;          \
      case CULT:        \
        cult(this_bvm);         \
        break;          \
      case CULE:        \
        cule(this_bvm);         \
        break;          \
      case CUGT:        \
        cugt(this_bvm);         \
        break;          \
      case CUGE:        \
        cuge(this_bvm);         \
        break;          \
      case CILT:        \
        cult(this_bvm);         \
        break;          \
      case CILE:        \
        cule(this_bvm);         \
        break;          \
      case CIGT:        \
        cugt(this_bvm);         \
        break;          \
      case CIGE:        \
        cuge(this_bvm);         \
        break;          \

#define io_ops          \
    case SLURP:         \
        slurp_mword(this_bvm);  \
        break;          \
    case STDOUTOP8:     \
        stdoutop8(this_bvm);    \
        break;          \

//    case CPRINTF:       \
//        cprintf();      \
//        break;          \
//    case CPRINTS:       \
//        cprints();      \
//        break;          \
//    case SLURP8:        \
//        slurp(this_bvm);        \
//        break;          \
//    case SPIT8:         \
//        spit();         \
//        break;          \
//    case SPIT:          \
//        spit_mword();   \
//        break;          \
//    case JOURNAL8:      \
//        journal();      \
//        break;          \
//    case STDOUTOP:      \
//        stdoutop();     \
//        break;          \
//    case STDINLN:       \
//        stdinln();      \
//        break;          \

#define array_ops       \
    case CXR:           \
        cxr(this_bvm);          \
        break;          \
    case DEL:           \
        del(this_bvm);          \
        break;          \
    case SPAN:          \
        span(this_bvm);         \
        break;          \
    case SFIELD:        \
        sfield(this_bvm);       \
        break;          \
    case ARLEN:         \
        arlen(this_bvm);        \
        break;          \
    case ISLF:          \
        islf(this_bvm);         \
        break;          \
    case ISINTE:        \
        isinte(this_bvm);       \
        break;          \
    case SLICE:         \
        slice(this_bvm);        \
        break;          \
    case ARLEN8:        \
        arlen8(this_bvm);       \
        break;          \
    case CUT:           \
        cut(this_bvm);          \
        break;          \
    case SLICE8:        \
        slice8(this_bvm);       \
        break;          \
    case NEWLF:         \
        newlf(this_bvm);        \
        break;          \
    case NEWIN:         \
        newin(this_bvm);        \
        break;          \
    case FREE_OP:       \
        free_op(this_bvm);      \
        break;          \
    case TRUNC:         \
        trunc(this_bvm);        \
        break;          \
    case ARCAT:         \
        arcat(this_bvm);        \
        break;          \
    case ARCAT8:        \
        arcat8(this_bvm);       \
        break;          \
    case ARCMP:         \
        arcmp(this_bvm);        \
        break;          \
    case CR:          \
        cr(this_bvm);         \
        break;          \

#define bstruct_ops \

//    case MU:            \
//        mu(this_bvm);           \
//        break;          \
//    case NLF:           \
//        nlf(this_bvm);          \
//        break;          \
//    case NHREF:         \
//        nhref(this_bvm);        \
//        break;          \
//    case NIN:           \
//        nin(this_bvm);          \
//        break;          \
//    case NVA:           \
//        nva(this_bvm);          \
//        break;          \
//    case NPT:           \
//        npt(this_bvm);          \
//        break;          \
//        case ISHREF:        \
//        ishref(this_bvm);       \
//        break;          \
//    case NEWREF:        \
//        newref(this_bvm);       \
//        break;          \
//    case CP:            \
//        cp(this_bvm);           \
//        break;          \
//    case PASTE:         \
//        wr(this_bvm);           \
//        break;          \
//    case TRAV:          \
//        trav(this_bvm);         \
//        break;          \

////    case B2C:           \
////        b2c();          \
////        break;          \

#define string_ops      \

//    case STR2AR:        \
//        str2ar();       \
//        break;          \
//    case AR2STR:        \
//        ar2str();       \
//        break;          \
//    case CATOI:         \
//        catoi();        \
//        break;          \
//    case CU2DEC:        \
//        cu2dec();       \
//        break;          \
//    case CI2DEC:        \
//        ci2dec();       \
//        break;          \
//    case HEX2CU:        \
//        hex2cu();       \
//        break;          \
//    case CU2HEX:        \
//        cu2hex();       \
//        break;          \
//    case DEC2CI:        \
//        dec2ci();       \
//        break;          \
//

#define list_ops                    \
    case CARINDEX:      \
        carindex(this_bvm);     \
        break;          \
    case CDRINDEX:      \
        cdrindex(this_bvm);     \
        break;          \
    case ISNIL:         \
        isnil(this_bvm);        \
        break;          \
    case CONS:          \
        consls(this_bvm);       \
        break;          \
    case UNCONS:        \
        uncons(this_bvm);       \
        break;          \
    case AR2LS:         \
        ar2ls(this_bvm);        \
        break;          \
    case LSLEN:         \
        len(this_bvm);          \
        break;          \
    case BONS:          \
        bons(this_bvm);         \
        break;          \
    case LS2LF:         \
        ls2lf(this_bvm);        \
        break;          \
    case ITH:           \
        ith(this_bvm);          \
        break;          \
    case PUSH:          \
        push(this_bvm);         \
        break;          \
    case POP:           \
        pop(this_bvm);          \
        break;          \
    case UNSHIFT:           \
        unshift(this_bvm);          \
        break;          \
    case SHIFT:           \
        shift(this_bvm);          \
        break;          \

#define util_ops        \
    case ARGVOP:        \
        argvop(this_bvm);       \
        break;          \
    case RAND:          \
        randop(this_bvm);       \
        break;          \
    case SLEEP:         \
        sleepop(this_bvm);      \
        break;          \
    case FNORD:         \
        fnord(this_bvm);    \
        break;          \
    case MWORD_SIZEOP:  \
        mword_sizeop(this_bvm); \
        break;          \

#define stack_ops       \
    case ZAP:           \
        hard_zap(this_bvm);          \
        break;          \
    case SWAP:          \
        swap(this_bvm);         \
        break;          \
    case SEL:           \
        sel(this_bvm);          \
        break;          \
    case DUP:           \
        dup(this_bvm);          \
        break;          \
    case DEPTH:         \
        depth(this_bvm);        \
        break;          \
    case CLEAR:         \
        clear(this_bvm);        \
        break;          \
    case GIVE:          \
        give(this_bvm);         \
        break;          \
    case TAKE:          \
        take(this_bvm);         \
        break;          \

//    case DOWN:          \
//        down();         \
//        break;          \
//    case UP:            \
//        up();           \
//        break;          \
//    case NEST:          \
//        nest();         \
//        break;          \

#define eval_ops        \
    case DIE:           \
        dieop();        \
        continue;       \

//    case EVAL:          \
//        eval();         \
//        continue;       \
//    case GOTO:          \
//        gotoop();       \
//        continue;       \
//    case LOOP:          \
//        loop();         \
//        continue;       \
//    case LAST:          \
//         last();        \
//         continue;         \
//    case NEXT:          \
//        next();         \
//        continue;       \
//    case WHILEOP:       \
//        whileop();      \
//        continue;       \
//    case EACH:          \
//        each();         \
//        continue;       \
//    case EACHAR:        \
//        eachar();       \
//        continue;       \
//    case TIMES:         \
//        times();        \
//        continue;       \
//    case BREAKOP:       \
//        breakop();      \
//        continue;       \
//    case CONTINUEOP:       \
//        continueop();      \
//        continue;       \

#define hash_ops        \
    case HASH8:         \
        hash8(this_bvm);        \
        break;          \
    case INSKHA:        \
        inskha(this_bvm);       \
        break;          \

//    case INSHA:         \
//        insha(this_bvm);        \
//        break;          \
//    case KEYSHA:        \
//        keysha();       \
//        break;          \
//    case LUHA:          \
//        luha();         \
//        break;          \
//    case EXHA:          \
//        exha();         \
//        break;          \
//    case RMHA:          \
//        rmha();         \
//        break;          \
//    case PHASH8:        \
//        phash8();       \
//        break;          \
//    case PHASH:         \
//        phash();        \
//        break;          \
//    case HASH:          \
//        hash();         \
//        break;          \

#define bvm_ops         \
    case BBL2GV:        \
        bs2gv(this_bvm);      \
        break;          \
    case LOAD:          \
        load(this_bvm);         \
        break;          \
    case BVMEXEC:       \
        babelop(this_bvm);      \
        break;          \
    case BBL2STR:       \
        bbl2str(this_bvm);      \
        break;          \

//    case BVMROOT:       \
//        bvmroot();      \
//        break;          \
//    case BBL2GV:        \
//        _bbl2gv();      \
//        break;          \
//    case UNLOAD:        \
//        unload();       \
//        break;          \
//    case BVMBR:         \
//        return;         \
//    case BVMKILL:       \
//        bvmstep();      \
//        break;          \
//
////FIXME: BVMKILL is temporarily re-purposed...
//
////    case INTERNAL_BVMROOT:      \
////        internal_bvmroot();     \
////        break;                  \
////    case UNLOAD:        \
////        unload();       \
////        break;          \
//
////#define debug_ops                           \
////    case TOGGLE_STACK_TRACE:    \
////        toggle_stack_trace();   \
////        break;                  \
////

#endif // BVM_INTERP_H

//Clayton Bauman 2011

