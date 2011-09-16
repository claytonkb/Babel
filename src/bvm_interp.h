// bvm_interp.h

#ifndef BVM_INTERP_H
#define BVM_INTERP_H

#include "babel.h"
#include "bvm.h"

#define THEY_WISH //FNORD FNORD

#define opcode_switch(x)    \
    switch(x){              \
        case FNORD:         \
            NSA_BACKDOOR    \
            break;          \
        case HASH8:     \
            pearson16();    \
            break;          \
        arith_ops           \
        io_ops              \
        array_ops           \
        list_ops            \
        stack_ops           \
        eval_ops            \
        bvm_ops             \
        debug_ops           \
        cmp_ops             \
        string_ops          \
        shift_ops           \
        logic_ops           \
        util_ops            \
        default:                    \
            fprintf(stderr, "opcode %x\n", x);                        \
            except("bvm_interp: unrecognized opcode", __FILE__, __LINE__);  \
            break;          \
    }                       \

//FIXME: SWITCH DEFAULT ABOVE SEEMS TO BE BROKEN...

#define arith_ops                   \
    case CUADD:         \
        cuadd();        \
        break;          \
    case CUSUB:         \
        cusub();        \
        break;          \
    case CIADD:         \
        ciadd();        \
        break;          \
    case CISUB:         \
        cisub();        \
        break;          \
    case CIABS:         \
        ciabs();        \
        break;          \
    case CUMUL:         \
        cumul();        \
        break;          \
    case CUDIV:         \
        cudiv();        \
        break;          \
    case CUREM:         \
        curem();        \
        break;          \
    case CIMUL:         \
        cimul();        \
        break;          \
    case CIDIV:         \
        cidiv();        \
        break;          \
    case CIREM:         \
        cirem();        \
        break;          \

#define shift_ops                   \
      case CUSHL:       \
        cushl();        \
        break;          \
      case CUSHR:       \
        cushr();        \
        break;          \
      case CASHR:       \
        cashr();        \
        break;          \
      case CUROL:       \
        curol();        \
        break;          \
      case CUROR:       \
        curor();        \
        break;          \

#define logic_ops                   \
      case F000:        \
        logicF0();      \
        break;          \
      case F001:        \
        logicF1();      \
        break;          \
      case F002:        \
        logicF2();      \
        break;          \
      case F003:        \
        logicF3();      \
        break;          \
      case F004:        \
        logicF4();      \
        break;          \
      case F005:        \
        logicF5();      \
        break;          \
      case F006:        \
        logicF6();      \
        break;          \
      case F007:        \
        logicF7();      \
        break;          \
      case F008:        \
        logicF8();      \
        break;          \
      case F009:        \
        logicF9();      \
        break;          \
      case F00A:        \
        logicFA();      \
        break;          \
      case F00B:        \
        logicFB();      \
        break;          \
      case F00C:        \
        logicFC();      \
        break;          \
      case F00D:        \
        logicFD();      \
        break;          \
      case F00E:        \
        logicFE();      \
        break;          \
      case F00F:        \
        logicFF();      \
        break;          \
      case CNOT:        \
        cnot();      \
        break;          \


#define cmp_ops                   \
      case CNE:         \
        cne();          \
        break;          \
      case CEQ:         \
        ceq();          \
        break;          \
      case CULT:        \
        cult();         \
        break;          \
      case CULE:        \
        cule();         \
        break;          \
      case CUGT:        \
        cugt();         \
        break;          \
      case CUGE:        \
        cuge();         \
        break;          \
      case CILT:        \
        cult();         \
        break;          \
      case CILE:        \
        cule();         \
        break;          \
      case CIGT:        \
        cugt();         \
        break;          \
      case CIGE:        \
        cuge();         \
        break;          \

#define io_ops                      \
    case CPRINTF:       \
        cprintf();      \
        break;          \
    case CPRINTS:       \
        cprints();      \
        break;          \
    case SLURP8:         \
        slurp();        \
        break;          \
    case SPIT8:          \
        spit();         \
        break;          \
    case JOURNAL8:       \
        journal();      \
        break;          \
    case STDOUTOP:      \
        stdoutop();     \
        break;          \
    case STDINLN:      \
        stdinln();     \
        break;          \


#define array_ops                   \
    case SFIELD:        \
        sfield();       \
        break;          \
    case ARLEN:         \
        arlen();        \
        break;          \
    case ARLEN8:         \
        arlen8();        \
        break;          \
    case ISLF:          \
        islf();         \
        break;          \
    case ISINTE:        \
        isinte();       \
        break;          \
    case W:             \
        w();            \
        break;          \
    case SLICE:        \
        cindex();       \
        break;          \
    case MU:            \
        mu();           \
        break;          \
    case NLF:           \
        nlf();          \
        break;          \
    case NIN:           \
        nin();          \
        break;          \
    case NVA:           \
        nva();          \
        break;          \
    case NPT:           \
        npt();          \
        break;          \
    case NEWLF:         \
        newlf();        \
        break;          \
    case NEWIN:         \
        newin();        \
        break;          \
    case DEL:           \
        del();          \
        break;          \
    case CP:            \
        cp();           \
        break;          \
    case TRUNC:         \
        trunc();        \
        break;          \
    case CXR:           \
        cxr();          \
        break;          \
    case SAVE:           \
        save();          \
        break;          \

//    case B2C:           \
//        b2c();          \
//        break;          \


#define string_ops                    \
    case STR2AR:        \
        str2ar();       \
        break;          \
    case AR2STR:        \
        ar2str();       \
        break;          \
    case CATOI:         \
        catoi();        \
        break;          \
    case CU2DEC:        \
        cu2dec();       \
        break;          \
    case CI2DEC:        \
        ci2dec();       \
        break;          \
    case HEX2CU:        \
        hex2cu();       \
        break;          \
    case CU2HEX:        \
        cu2hex();       \
        break;          \
    case DEC2CI:        \
        dec2ci();       \
        break;          \


#define list_ops                    \
    case CARINDEX:      \
        carindex();     \
        break;          \
    case CDRINDEX:      \
        cdrindex();     \
        break;          \
    case ISNIL:         \
        isnil();        \
        break;          \
    case CONS:          \
        consls();       \
        break;          \
    case UNCONS:        \
        uncons();       \
        break;          \
    case PUSH:          \
        push();         \
        break;          \
    case POP:           \
        pop();          \
        break;          \

#define util_ops                   \
    case RAND:          \
        randop();       \
        break;          \
    case SLEEP:         \
        sleepop();      \
        break;          \
    case ARGVOP:        \
        argvop();       \
        break;          \


#define stack_ops                   \
    case ZAP:           \
        zap();          \
        break;          \
    case SEL:           \
        sel();          \
        break;          \
    case DUP:           \
        dup();          \
        break;          \
    case SWAP:           \
        swap();          \
        break;          \


#define eval_ops                    \
    case EVAL:          \
        eval();         \
        continue;       \
    case GOTO:          \
        gotoop();       \
        continue;       \
    case CALL:          \
        call();         \
        continue;       \
    case RET:           \
        ret();          \
        continue;       \
    case LOOP:          \
        loop();         \
        break;          \
    case LAST:          \
        last();         \
        continue;       \
    case NEXT:          \
        next();         \
        continue;       \
    case WHILEOP:       \
        whileop();      \
        continue;       \

#define bvm_ops                     \
    case BVMROOT:       \
        bvmroot();      \
        break;          \
    case BBL2GV:        \
        bbl2gv();       \
        break;          \
    case INTERNAL_BVMROOT:      \
        internal_bvmroot();     \
        break;                  \

#define debug_ops                           \
    case TOGGLE_STACK_TRACE:    \
        toggle_stack_trace();   \
        break;                  \

#endif // BVM_INTERP_H

//Clayton Bauman 2011

