// bvm_interp.h

#ifndef BVM_INTERP_H
#define BVM_INTERP_H

#include "babel.h"
#include "bvm.h"

#define THEY_WISH //FNORD FNORD

#define opcode_switch(x)    \
    switch(x){              \
        arith_ops           \
        io_ops              \
        array_ops           \
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
            except("bvm_interp: unrecognized opcode", __FILE__, __LINE__);  \
            break;          \
    }                       \

//FIXME: SWITCH DEFAULT ABOVE SEEMS TO BE BROKEN...

#define arith_ops       \
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

#define shift_ops       \
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

#define logic_ops       \
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
        cnot();         \
        break;          \
      case ANDOP:       \
        andop();        \
        break;          \
      case OROP:        \
        orop();         \
        break;          \
      case NOTOP:       \
        notop();        \
        break;          \


#define cmp_ops         \
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

#define io_ops          \
    case CPRINTF:       \
        cprintf();      \
        break;          \
    case CPRINTS:       \
        cprints();      \
        break;          \
    case SLURP8:        \
        slurp();        \
        break;          \
    case SLURP:         \
        slurp_mword();  \
        break;          \
    case SPIT8:         \
        spit();         \
        break;          \
    case SPIT:          \
        spit_mword();   \
        break;          \
    case JOURNAL8:      \
        journal();      \
        break;          \
    case STDOUTOP:      \
        stdoutop();     \
        break;          \
    case STDOUTOP8:     \
        stdoutop8();    \
        break;          \
    case STDINLN:       \
        stdinln();      \
        break;          \


#define array_ops       \
    case SFIELD:        \
        sfield();       \
        break;          \
    case ARLEN:         \
        arlen();        \
        break;          \
    case ARLEN8:        \
        arlen8();       \
        break;          \
    case ISLF:          \
        islf();         \
        break;          \
    case ISINTE:        \
        isinte();       \
        break;          \
    case ISHREF:        \
        ishref();       \
        break;          \
    case NEWREF:        \
        newref();       \
        break;          \
    case CUT:           \
        cut();          \
        break;          \
    case SLICE:         \
        slice();        \
        break;          \
    case SLICE8:        \
        slice8();       \
        break;          \
    case MU:            \
        mu();           \
        break;          \
    case NLF:           \
        nlf();          \
        break;          \
    case NHREF:         \
        nhref();        \
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
    case FREE_OP:       \
        free_op();      \
        break;          \
    case DEL:           \
        del();          \
        break;          \
    case CP:            \
        cp();           \
        break;          \
    case SPAN:          \
        span();         \
        break;          \
    case TRUNC:         \
        trunc();        \
        break;          \
    case CXR:           \
        cxr();          \
        break;          \
    case PASTE:         \
        wr();           \
        break;          \
    case ARCAT:         \
        arcat();        \
        break;          \
    case ARCAT8:        \
        arcat8();       \
        break;          \
    case ARCMP:         \
        arcmp();        \
        break;          \
    case TRAV:          \
        trav();         \
        break;          \
    case CR:          \
        cr();         \
        break;          \

//    case B2C:           \
//        b2c();          \
//        break;          \


#define string_ops      \
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
    case AR2LS:         \
        ar2ls();        \
        break;          \
    case LSLEN:         \
        len();          \
        break;          \
    case BONS:          \
        bons();         \
        break;          \
    case LS2LF:         \
        ls2lf();        \
        break;          \
    case ITH:           \
        ith();          \
        break;          \
    case PUSH:          \
        push();         \
        break;          \
    case POP:           \
        pop();          \
        break;          \
    case UNSHIFT:           \
        unshift();          \
        break;          \
    case SHIFT:           \
        shift();          \
        break;          \

#define util_ops        \
    case RAND:          \
        randop();       \
        break;          \
    case SLEEP:         \
        sleepop();      \
        break;          \
    case ARGVOP:        \
        argvop();       \
        break;          \
    case FNORD:         \
        NSA_BACKDOOR    \
        break;          \
    case MWORD_SIZEOP:  \
        mword_sizeop(); \
        break;          \

#define stack_ops       \
    case ZAP:           \
        zap();          \
        break;          \
    case SEL:           \
        sel();          \
        break;          \
    case DUP:           \
        dup();          \
        break;          \
    case SWAP:          \
        swap();         \
        break;          \
    case DOWN:          \
        down();         \
        break;          \
    case UP:            \
        up();           \
        break;          \
    case TAKE:          \
        take();         \
        break;          \
    case GIVE:          \
        give();         \
        break;          \
    case DEPTH:         \
        depth();        \
        break;          \
    case NEST:          \
        nest();         \
        break;          \
    case CLEAR:         \
        clear();        \
        break;          \


#define eval_ops        \
    case EVAL:          \
        eval();         \
        continue;       \
    case GOTO:          \
        gotoop();       \
        continue;       \
    case LOOP:          \
        loop();         \
        continue;       \
    case LAST:          \
         last();        \
         continue;         \
    case NEXT:          \
        next();         \
        continue;       \
    case WHILEOP:       \
        whileop();      \
        continue;       \
    case EACH:          \
        each();         \
        continue;       \
    case EACHAR:        \
        eachar();       \
        continue;       \
    case TIMES:         \
        times();        \
        continue;       \
    case DIE:           \
        dieop();        \
        continue;       \
    case BREAKOP:       \
        breakop();      \
        continue;       \
    case CONTINUEOP:       \
        continueop();      \
        continue;       \

#define hash_ops        \
    case INSHA:         \
        insha();        \
        break;          \
    case KEYSHA:        \
        keysha();       \
        break;          \
    case INSKHA:        \
        inskha();       \
        break;          \
    case LUHA:          \
        luha();         \
        break;          \
    case EXHA:          \
        exha();         \
        break;          \
    case RMHA:          \
        rmha();         \
        break;          \
    case PHASH8:        \
        phash8();       \
        break;          \
    case PHASH:         \
        phash();        \
        break;          \
    case HASH8:         \
        hash8();        \
        break;          \
    case HASH:          \
        hash();         \
        break;          \

#define bvm_ops         \
    case BVMROOT:       \
        bvmroot();      \
        break;          \
    case BBL2GV:        \
        _bbl2gv();      \
        break;          \
    case BBL2STR:       \
        bbl2str();      \
        break;          \
    case LOAD:          \
        load();         \
        break;          \
    case UNLOAD:        \
        unload();       \
        break;          \
    case BVMBR:         \
        return;         \
    case BVMEXEC:       \
        bvmexec();      \
        break;          \
    case BVMKILL:       \
        bvmstep();      \
        break;          \

//FIXME: BVMKILL is temporarily re-purposed...

//    case INTERNAL_BVMROOT:      \
//        internal_bvmroot();     \
//        break;                  \
//    case UNLOAD:        \
//        unload();       \
//        break;          \

//#define debug_ops                           \
//    case TOGGLE_STACK_TRACE:    \
//        toggle_stack_trace();   \
//        break;                  \
//
//#endif // BVM_INTERP_H

//Clayton Bauman 2011

