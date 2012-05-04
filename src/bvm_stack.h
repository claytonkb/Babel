// bvm_stack.h

#ifndef BVM_STACK_H
#define BVM_STACK_H

#include "babel.h"
#include "bvm.h"
#include "list.h"

#define zap_switch(x)       \
    switch(x){              \
        case CUADD:         \
        case CUSUB:         \
        case CIADD:         \
        case CISUB:         \
        case CIABS:         \
        case CUMUL:         \
        case CUDIV:         \
        case CUREM:         \
        case CIMUL:         \
        case CIDIV:         \
        case CIREM:         \
        case CUSHL:         \
        case CUSHR:         \
        case CASHR:         \
        case CUROL:         \
        case CUROR:         \
        case F000:          \
        case F001:          \
        case F002:          \
        case F003:          \
        case F004:          \
        case F005:          \
        case F006:          \
        case F007:          \
        case F008:          \
        case F009:          \
        case F00A:          \
        case F00B:          \
        case F00C:          \
        case F00D:          \
        case F00E:          \
        case F00F:          \
        case CNOT:          \
        case ANDOP:         \
        case NOTOP:         \
        case CNE:           \
        case CEQ:           \
        case CULT:          \
        case CULE:          \
        case CUGT:          \
        case CUGE:          \
        case CILT:          \
        case CILE:          \
        case CIGT:          \
        case CIGE:          \
            bfree(val);     \
            break;          \
        case SLURP:         \
        case SLURP8:        \
        case STDINLN:       \
        case SFIELD:        \
        case ARLEN:         \
        case ARLEN8:        \
        case ISLF:          \
        case ISINTE:        \
        case ISHREF:        \
        case CXR:           \
        case NEWREF:        \
        case CUT:           \
        case SLICE:         \
        case SLICE8:        \
        case MU:            \
        case NLF:           \
        case NHREF:         \
        case NIN:           \
        case NVA:           \
        case NPT:           \
        case NEWLF:         \
        case NEWIN:         \
        case CP:            \
        case TRUNC:         \
        case PASTE:         \
        case ARCAT:         \
        case ARCAT8:        \
        case ARCMP:         \
        case TRAV:          \
        case CR:            \
        case STR2AR:        \
        case AR2STR:        \
        case CATOI:         \
        case CU2DEC:        \
        case CI2DEC:        \
        case HEX2CU:        \
        case CU2HEX:        \
        case DEC2CI:        \
        case CARINDEX:      \
        case CDRINDEX:      \
        case ISNIL:         \
        case CONS:          \
        case UNCONS:        \
        case AR2LS:         \
        case LSLEN:         \
        case BONS:          \
        case LS2LF:         \
        case ITH:           \
        case PUSH:          \
        case POP:           \
        case UNSHIFT:       \
        case SHIFT:         \
        case ARGVOP:        \
        case RAND:          \
        case MWORD_SIZEOP:  \
        case SWAP:          \
        case SEL:           \
        case DEPTH:         \
        case CLEAR:         \
        case GIVE:          \
        case TAKE:          \
        case HASH8:         \
        case INSKHA:        \
        case INSHA:         \
        case KEYSHA:        \
        case LUHA:          \
        case EXHA:          \
        case RMHA:          \
        case PHASH8:        \
        case PHASH:         \
        case HASH:          \
        case BBL2GV:        \
        case LOAD:          \
        case BBL2STR:       \
        case UNLOAD:        \
            break;          \
    }                       \

// TODO:   case BVMEXEC:      

#endif // BVM_STACK_H

//Clayton Bauman 2012

