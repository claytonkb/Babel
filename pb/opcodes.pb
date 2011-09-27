--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--
-- BABEL OPCODE DEF'NS
--
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--


------------------------------------------------------------------------------
--
-- SHIFTS
--
------------------------------------------------------------------------------

cushl:
{0x001}

cushr:
{0x003}

curol:
{0x005}

curor:
{0x007}

cashr:
{0x00b}

cnot:
{0x00c}

------------------------------------------------------------------------------
--
-- COMPARES
--
------------------------------------------------------------------------------

cne:
{0x010}

ceq:
{0x011}

cult:
{0x012}

cule:
{0x013}

cugt:
{0x014}

cuge:
{0x015}

cilt:
{0x01a}

cile:
{0x01b}

cigt:
{0x01c}

cige:
{0x01d}

------------------------------------------------------------------------------
--
-- LOGIC
--
------------------------------------------------------------------------------
F0:
{0x020}

F1:
{0x021}

cand:
{0x021}

F2:
{0x022}

F3:
{0x023}

F4:
{0x024}

F5:
{0x025}

F6:
{0x026}

cxor:
{0x026}

F7:
{0x027}

cor:
{0x027}

F8:
{0x028}

cnor:  
{0x028}

F9:
{0x029}

cxnor:
{0x029}

Fa:
{0x02a}

Fb:
{0x02b}

Fc:
{0x02c}

Fd:
{0x02d}

Fe:
{0x02e}

cnand:
{0x02e}

Ff:
{0x02f}

------------------------------------------------------------------------------
--
-- ARITHMETIC
--
------------------------------------------------------------------------------

cuadd:
{0x030}

cusub:
{0x031}

cumul:
{0x032}

cudiv:
{0x033}

curem:
{0x035}

ciadd:
{0x038}

cisub:
{0x039}

cimul:
{0x03a}

cidiv:
{0x03b}

ciabs:
{0x03c}

cirem:
{0x03d}

------------------------------------------------------------------------------
--
-- FLOATING POINT
--
------------------------------------------------------------------------------
-- exp frxp ldxp log log10 modf pow sqrt add  sub  mul  div  abs  mod  ceil floor
-- cos  sin  tan acos asin atan atan2 cosh sinh tanh

------------------------------------------------------------------------------
--
-- STDLIB
--
------------------------------------------------------------------------------
catoi:
{0x080}

catof:
{0x080}

catol:
{0x081}

crand:
{0x082}

csrand:
{0x083}

calloc:
{0x084}

cfree:
{0x085}

cmalloc:
{0x086}

crealloc:
{0x088}

cexit:
{0x089}

cgetenv:
{0x08a}

csystem:
{0x08b}

------------------------------------------------------------------------------
--
-- STRING
--
------------------------------------------------------------------------------
cmemcpy:
{0x090}

cmemmove:
{0x091}

cstrncpy:
{0x092}

cstrncat:
{0x093}

cmemcmp:
{0x094}

cstrncmp:
{0x095}

cstrlen:
{0x096}

------------------------------------------------------------------------------
--
-- STDIO
--
------------------------------------------------------------------------------
cfgetpos:
{0x0a1}

cftell:
{0x0a2}

crewind:
{0x0a3}

cfeof:
{0x0a4}

cfseek:
{0x0a5}

cfsetpos:
{0x0a6}

cfopen:
{0x0a8}

cfclose:
{0x0a9}

cfreopen:
{0x0aa}

cfscanf:
{0x0ab}

cscanf:
{0x0ac}

csscanf:
{0x0ad}

cgetchar:
{0x0b0}

cgetc:
{0x0b1}

cgets:
{0x0b2}

cfgetc:
{0x0b4}

cfgets:
{0x0b5}

cfread:
{0x0b6}

cputchar:
{0x0b8}

cputc:
{0x0b9}

cputs:
{0x0ba}

cungetc:
{0x0bb}

cfputc:
{0x0bc}

cfputs:
{0x0bd}

cfwrite:
{0x0be}

cfflush:
{0x0bf}

cprintf:
{0x0c0}

csprintf:
{0x0c1}

cvprintf:
{0x0c2}

cfprintf:
{0x0c3}

cvsprintf:
{0x0c4}

cvfprintf:
{0x0c5}

cprints:
{0x0c8}

ctmpfile:
{0x0d0}

ctmpnam:
{0x0d1}

csetbuf:
{0x0d2}

csetvbuf:
{0x0d3}

cremove:
{0x0d4}

crename:
{0x0d5}

cperror:
{0x0d6}

clearerr:
{0x0d7}

cferror:
{0x0d8}


------------------------------------------------------------------------------
--
-- BVM
--
------------------------------------------------------------------------------

fnord:
{0x100}

bvmroot:
{0x101}

bvmexec:
{0x102}

bvmkill:
{0x103}

bvmdb:
{0x104}

bvmsave:
{0x105}

bvmfork:
{0x106}

arlen8:
{0x108}

arlen16:
{0x109}

arlen32:
{0x10a}

arlen:
{0x10b}

arcat8:
{0x10c}

arcat16:
{0x10d}

arcat32:
{0x10e}

arcat:
{0x10f}

mu:
{0x110}

nva:
{0x111}

npt:
{0x112}

nlf:
{0x113}

nin:
{0x114}

load:
{0x115}

unload:
{0x116}

bbl2gv:
{0x117}

slice8:
{0x118}

slice16:
{0x119}

slice32:
{0x11a}

slice:
{0x11b}

arcmp8:
{0x11c}

arcmp:
{0x11f}

sleep:
{0x120}

die:
{0x121}

goto:
{0x122}

call:
{0x123}

ret:
{0x124}

loop:
{0x125}

last:
{0x126}

next:
{0x127}

w8:
{0x128}

w16:
{0x129}

w32:
{0x12a}

w:
{0x12b}

trunc:
{0x12c}

cxr:
{0x12d}

ardeq:
{0x12f}

eval:
{0x130}

areval:
{0x131}

lseval:
{0x132}

meval:
{0x133}

exec:
{0x134}

while:
{0x135}

isin:
{0x138}

in2lf:
{0x139}

newin:
{0x13a}

ar2ls:
{0x13b}

islf:
{0x13c}

del:
{0x13d}

newlf:
{0x13e}

s:
{0x13f}

hash8:
{0x140}

rand:
{0x141}

srand:
{0x142}

argv:
{0x143}

cp:
{0x148}

save:
{0x149}

archr:
{0x14b}

armv8:
{0x14c}

armv:
{0x14f}

hash:
{0x150}

luha:
{0x151}

insha:
{0x152}

delha:
{0x153}

exha:
{0x154}

valsha:
{0x155}

keysha:
{0x156}

cpha:
{0x157}

mkptr:
{0x158}

deref:
{0x159}

mksptr:
{0x159}

sderef:
{0x15a}

inskha:
{0x162}

add:
{0x168}

mul:
{0x169}

exp:
{0x16a}

mod:
{0x16b}

abs:
{0x16c}

max:
{0x16d}

ceil:
{0x16e}

sub:
{0x178}

div:
{0x179}

log:
{0x17a}

rem:
{0x17b}

eq:
{0x17c}

min:
{0x17d}

floor:
{0x17e}

ar2str:
{0x180}

str2ar:
{0x181}


cu2dec:
{0x184}

cu2hex:
{0x185}

ci2dec:
{0x186}

hex2cu:
{0x195}

dec2ci:
{0x196}

take:
{0x199}

give:
{0x19a}

lslen:
{0x188}

lscat:
{0x18c}

bbl2str:
{0x190}

uncons:
{0x198}

cdr:
{0x199}

pop:
{0x19a}

zap:
{0x19b}

ls2stk:
{0x19c}

permstk:
{0x19d}

swap:
{0x19e}

sel:
{0x19f}

strint:
{0x1a0}

macint:
{0x1a1}

interpol:
{0x1a2}

mkpeg:
{0x1a4}

pegpar:
{0x1a5}

cons:
{0x1a8}

car:
{0x1a9}

push:
{0x1aa}

dup:
{0x1ab}

stk2ls:
{0x1ac}

dupstk:
{0x1ad}

shift:
{0x1ae}

unshift:
{0x1af}

slurp:
{0x1b0}

spit:
{0x1b1}

journal:
{0x1b2}

isls:
{0x1b8}

ls2ar:
{0x1bb}

isat:
{0x1bc}

isnil:
{0x1bd}

slurp8:
{0x1c0}

spit8:
{0x1c1}

journal8:
{0x1c2}

mkns:
{0x1c8}

luns:
{0x1c9}

insns:
{0x1ca}

delns:
{0x1cb}

exns:
{0x1cc}

lsns:
{0x1cd}

cpns:
{0x1cf}

rdfile:
{0x1d0}

wrfile:
{0x1d1}

clfile:
{0x1d2}

cntfile:
{0x1d3}

stdout:
{0x1e0}

stderr:
{0x1e1}

stdin:
{0x1e2}

stdinln:
{0x1e3}

stdinf:
{0x1e4}


