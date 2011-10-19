// bvm_opcodes.h

#ifndef BVM_OPCODES_H
#define BVM_OPCODES_H

#include "babel.h"

//////////////////////////////////////////////////////////////////////////////
// OPCODES PG0
//////////////////////////////////////////////////////////////////////////////

#define RSVD000     0x000    // 0 is permanently reserved in order to signal an error
#define CUSHL       0x001    //   in case we end up in the weeds
#define RSVD002     0x002
#define CUSHR       0x003
#define RSVD004     0x004
#define CUROL       0x005
#define RSVD006     0x006
#define CUROR       0x007
#define RSVD008     0x008
//#define CASHL       0x009
#define RSVD00A     0x00A
#define CASHR       0x00B
#define CNOT        0x00C
#define RSVD00D     0x00D
#define RSVD00E     0x00E
#define RSVD00F     0x00F

#define CNE         0x010
#define CEQ         0x011
#define CULT        0x012
#define CULE        0x013
#define CUGT        0x014
#define CUGE        0x015
#define CILT        0x01A
#define CILE        0x01B
#define CIGT        0x01C
#define CIGE        0x01D

#define F000        0x020
#define F001        0x021
#define CAND        0x021
#define F002        0x022
#define F003        0x023
#define F004        0x024
#define F005        0x025
#define F006        0x026
#define CXOR        0x026
#define F007        0x027
#define COR         0x027
#define F008        0x028
#define CNOR        0x028
#define F009        0x029
#define CXNOR       0x029
#define F00A        0x02A
#define F00B        0x02B
#define F00C        0x02C
#define F00D        0x02D
#define F00E        0x02E
#define CNAND       0x02E
#define F00F        0x02F

#define CUADD       0x030
#define CUSUB       0x031
#define CUMUL       0x032
#define CUDIV       0x033
#define RSVD034     0x034
#define CUREM       0x035
#define RSVD036     0x036
#define RSVD037     0x037
#define CIADD       0x038
#define CISUB       0x039
#define CIMUL       0x03A
#define CIDIV       0x03B
#define CIABS       0x03C
#define CIREM       0x03D
#define RSVD03E     0x03E
#define RSVD03F     0x03F

#define CFEXP       0x040
#define CFRXP       0x041
#define CLDXP       0x042
#define CLOG        0x043
#define CLOG10      0x044
#define CMODF       0x045
#define CPOW        0x046
#define CSQRT       0x047
#define CFADD       0x048
#define CFSUB       0x049
#define CFMUL       0x04A
#define CFDIV       0x04B
#define CFABS       0x04C
#define CFMOD       0x04D
#define CCEIL       0x04E
#define CFLOOR      0x04F

#define CCOS        0x050
#define CSIN        0x051
#define CTAN        0x052
#define CACOS       0x053
#define CASIN       0x054
#define CATAN       0x055
#define CATAN2      0x056
#define CCOSH       0x058
#define CSINH       0x059
#define CTANH       0x05A

//0060-006F  RSVD

//#ifdef DEBUG
//#define INTERNAL_BVMROOT 0x070 // DEBUG ONLY!!
//#define TOGGLE_STACK_TRACE  0x071
//#endif

#define CATOI       0x080
#define CATOF       0x081
#define CATOL       0x082
#define CRAND       0x083
#define CSRAND      0x084
#define CALLOC      0x085
#define CFREE       0x086
#define CMALLOC     0x087
#define CREALLOC    0x088
#define CEXIT       0x089
#define CGETENV     0x08A
#define CSYSTEM     0x08B

#define CMEMCPY     0x090
#define CMEMMOVE    0x091
#define CSTRNCPY    0x092
#define CSTRNCAT    0x093
#define CMEMCMP     0x094
#define CSTRNCMP    0x095
#define CSTRLEN     0x096

#define CFGETPOS    0x0A0
#define CFTELL      0x0A1
#define CREWIND     0x0A2
#define CFEOF       0x0A3
#define CFSEEK      0x0A4
#define CFSETPOS    0x0A5
#define CFOPEN      0x0A8
#define CFCLOSE     0x0A9
#define CFREOPEN    0x0AA
#define CFSCANF     0x0AB
#define CSCANF      0x0AC
#define CSSCANF     0x0AD

#define CGETCHAR    0x0B0
#define CGETC       0x0B1
#define CGETS       0x0B2
#define CFGETC      0x0B4
#define CFGETS      0x0B5
#define CFREAD      0x0B6
#define CPUTCHAR    0x0B8
#define CPUTC       0x0B9
#define CPUTS       0x0BA
#define CUNGETC     0x0BB
#define CFPUTC      0x0BC
#define CFPUTS      0x0BD
#define CFWRITE     0x0BE

#define CPRINTF     0x0C0
#define CSPRINTF    0x0C1
#define CVPRINTF    0x0C2
#define CFPRINTF    0x0C3
#define CVSPRINTF   0x0C4
#define CVFPRINTF   0x0C5

#define CPRINTS     0x0C8

#define CTMPFILE    0x0D0
#define CTMPNAM     0x0D1
#define CSETBUF     0x0D2
#define CSETVBUF    0x0D3
#define CREMOVE     0x0D4
#define CRENAME     0x0D5
#define CPERROR     0x0D6
#define CCLEARERR   0x0D7
#define CFERROR     0x0D8

//////////////////////////////////////////////////////////////////////////////
// OPCODES PG1
//////////////////////////////////////////////////////////////////////////////

#define FNORD       0x100
#define BVMROOT     0x101
#define BVMEXEC     0x102
#define BVMKILL     0x103
#define BVMBR       0x104
#define BVMSAVE     0x105
#define BVMFORK     0x106
#define MWORD_SIZEOP 0x107
#define ARLEN8      0x108
#define ARLEN16     0x109
#define ARLEN32     0x10a
#define ARLEN       0x10b
#define ARCAT8      0x10c
#define ARCAT16     0x10d
#define ARCAT32     0x10e
#define ARCAT       0x10f
#define MU          0x110
#define NVA         0x111
#define NPT         0x112
#define NLF         0x113
#define NIN         0x114
#define LOAD        0x115
#define UNLOAD      0x116
#define BBL2GV      0x117
#define SLICE8      0x118
#define SLICE16     0x119
#define SLICE32     0x11a
#define SLICE       0x11b
#define ARCMP8      0x11c
#define ARCMP       0x11f
#define SLEEP       0x120
#define DIE         0x121
#define GOTO        0x122
#define CALL        0x123
#define RET         0x124
#define LOOP        0x125
#define LAST        0x126
#define NEXT        0x127
#define W8          0x128
#define W16         0x129
#define W32         0x12a
#define W           0x12b
#define TRUNC       0x12c
#define CXR         0x12d
#define ARDEQ       0x12f
#define EVAL        0x130
#define AREVAL      0x131
#define LSEVAL      0x132
#define MEVAL       0x133
#define EXEC        0x134
#define WHILEOP     0x135
#define TIMES       0x136
#define ISINTE      0x138
#define IN2LF       0x139
#define NEWIN       0x13a
#define AR2LS       0x13b
#define ISLF        0x13c
#define DEL         0x13d
#define NEWLF       0x13e
#define SFIELD      0x13f
//#define PEARSON16   0x140
#define HASH8       0x140
#define RAND        0x141
#define SRAND       0x142
#define ARGVOP      0x143
#define CP          0x148
#define SAVE        0x149
#define ARCHR       0x14B
#define ARMV8       0x14C
#define ARMV        0x14F
#define HASH        0x150
#define LUHA        0x151
#define INSHA       0x152
#define DELHA       0x153
#define EXHA        0x154
#define VALSHA      0x155
#define KEYSHA      0x156
#define CPHA        0x157
#define MKPTR       0x158
#define DEREF       0x159
#define MKSPTR      0x159
#define SDEREF      0x15A
#define INSKHA      0x162

#define ADDOP       0x168
#define MULOP       0x169
#define EXPOP       0x16A
#define MODOP       0x16B
#define ABSOP       0x16C
#define MAXOP       0x16D
#define CEILOP      0x16E
#define SUBOP       0x178
#define DIVOP       0x179
#define LOGOP       0x17A
#define REMOP       0x17B
#define EQOP        0x17C
#define MINOP       0x17D
#define FLOOROP     0x17E

//#define AREQ        0x14c
//#define ARDEQ       0x14d

#define AR2STR      0x180
#define STR2AR      0x181
//#define B2C         0x182
//#define C2B         0x183

#define CU2DEC      0x184
#define CU2HEX      0x185
#define CI2DEC      0x186
#define HEX2CU      0x195
#define DEC2CI      0x196

#define TAKE        0x199
#define GIVE        0x19a

#define LSLEN       0x188
#define LSCAT       0x18c
#define BBL2STR     0x190
#define UNCONS      0x198
#define CDRINDEX    0x199
#define POP         0x19a
#define ZAP         0x19b
#define LS2STK      0x19c
#define PERMSTK     0x19d
#define SWAP        0x19e
#define SEL         0x19f
#define STRINT      0x1a0
#define MACINT      0x1a1
#define INTERPOL    0x1a2
#define MKPEG       0x1a4
#define PEGPAR      0x1a5
#define CONS        0x1a8
#define CARINDEX    0x1a9
#define PUSH        0x1aa
#define DUP         0x1ab
#define STK2LS      0x1ac
#define DUPSTK      0x1ad
#define SHIFT       0x1ae
#define UNSHIFT     0x1af

#define SLURP       0x1b0
#define SPIT        0x1b1
#define JOURNAL     0x1b2

#define ISLS        0x1b8
#define LS2AR       0x1bb
#define ISAT        0x1bc
#define ISNIL       0x1bd

#define SLURP8      0x1c0
#define SPIT8       0x1c1
#define JOURNAL8    0x1c2

#define MKNS        0x1C8
#define LUNS        0x1C9
#define INSNS       0x1CA
#define DELNS       0x1CB
#define EXNS        0x1CC
#define LSNS        0x1CD
#define CPNS        0x1CF

#define RDFILE      0x1d0
#define WRFILE      0x1d1
#define CLFILE      0x1d2
#define CNTFILE     0x1d3
#define STDOUTOP    0x1e0
#define STDERROP    0x1e1
#define STDINOP     0x1e2
#define STDINLN     0x1e3
#define STDINF      0x1e4

