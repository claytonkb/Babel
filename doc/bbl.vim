" Vim syntax file
" Language: Babel
" Maintainer: Clayton Bauman
" Latest Revision: 29 Sep 2012

if exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword BabelKeywords cushl  cushr  curol  curor  shl  shr  rol  ror  cashr  cnot  cne  ceq  cult  cule  
syn keyword BabelKeywords cugt  cuge  cilt  cile  cigt  cige  F0  F1  cand  F2  F3  F4  F5  F6  cxor  F7  cor  F8  cnor  F9  cxnor  
syn keyword BabelKeywords Fa  Fb  Fc  Fd  Fe  cnand  Ff  cuadd  cusub  cumul  cudiv  curem  ciadd  cisub  cimul  cidiv  ciabs  cirem  
syn keyword BabelKeywords catoi  catof  catol  crand  csrand  calloc  cfree  cmalloc  crealloc  cexit  cgetenv  csystem  cmemcpy  
syn keyword BabelKeywords cmemmove  cstrncpy  cstrncat  cmemcmp  cstrncmp  cstrlen  cfgetpos  cftell  crewind  cfeof  cfseek  cfsetpos  
syn keyword BabelKeywords cfopen  cfclose  cfreopen  cfscanf  cscanf  csscanf  cgetchar  cgetc  cgets  cfgetc  cfgets  cfread  cputchar  
syn keyword BabelKeywords cputc  cputs  cungetc  cfputc  cfputs  cfwrite  cfflush  cprintf  csprintf  cvprintf  cfprintf  cvsprintf  
syn keyword BabelKeywords cvfprintf  cprints  ctmpfile  ctmpnam  csetbuf  csetvbuf  cremove  crename  cperror  clearerr  cferror  fnord  
syn keyword BabelKeywords bvmroot  bvmexec  babel  bvmkill  bvmstep  break  bvmsave  bvmfork  msize  free  rmha  arlen8  arlen16  arlen32  
syn keyword BabelKeywords arlen  size  size8  arcat8  cat8  arcat16  arcat32  arcat  cat  mu  nva  npt  nlf  nin  load  unload  bbl2gv  
syn keyword BabelKeywords dump  slice8  slice16  slice32  slice  arcmp8  arcmp  cmp  cmpar8  sleep  die  goto  loop  last  next  cut  
syn keyword BabelKeywords trunc  cxr  th  ardeq  eval  areval  lseval  meval  exec  while  times  isin  in?  in2lf  newin  ar2ls  islf  
syn keyword BabelKeywords del  newlf  s  hash8  rand  srand  argv  cp  save  paste  archr  armv8  armv  hash  luha  insha  delha  exha  
syn keyword BabelKeywords valsha  keysha  cpha  mkptr  deref  mksptr  sderef  inskha  add  mul  exp  mod  abs  max  ceil  sub  div  log  
syn keyword BabelKeywords rem  eq  min  floor  ar2str  str2ar  cu2dec  cu2hex  ci2dec  len  take  give  lscat  bbl2str  show  hex2cu  
syn keyword BabelKeywords dec2ci  uncons  cdr  pop  zap  ls2stk  permstk  swap  sel  strint  macint  interpol  mkpeg  pegpar  cons  car  
syn keyword BabelKeywords push  dup  stk2ls  dupstk  shift  unshift  slurp  spit  journal  isls  ls2lf  bons  isat  isnil  slurp8  spit8  
syn keyword BabelKeywords journal8  mkns  luns  insns  delns  exns  lsns  cpns  rdfile  wrfile  clfile  cntfile  stdout8  pr  stderr  
syn keyword BabelKeywords stdin  stdinln  stdinf  nhref  and  or  not  ishref  href?  each  newref  span  eachar  phash  phash8  down  up  
syn keyword BabelKeywords take  depth  give  nest  clear  stdout  ls2ar  trav  ith  cr  break  continue  walk  set  perm  stack  code  if  
syn keyword BabelKeywords ifte  ustack  reverse  boilerplate  paste8  iter  unnest  conjure  self  ducp  endian  nhword  deref  if  ord  let   

syn region BabelCodeBlock start="{" end="}" fold transparent
syn region BabelListBlock start="(" end=")" fold transparent
syn region BabelArrayBlock start="\[" end="\]" fold transparent

syntax match BabelString1 /"[^"]*"/
syntax match BabelString2 /'[^']*'/

syntax match BabelHexNumber /\<0x[A-Fa-f0-9]\+\>/
syntax match BabelNumber /\<-*[0-9]*\.*[0-9]\+\>/

syn match BabelIdentifier       /[a-zA-Z_][a-zA-Z_0-9]*[\*&$%!@]\?/

syn match BabelQuote       /q#\|qf#\|doc#\|#/

syn match BabelComment /--.*$/

syn keyword BabelTypes string word pnum


hi link BabelKeywords Keyword
hi link BabelComment Comment
hi link BabelString1 String
hi link BabelString2 String
hi link BabelNumber Number
hi link BabelHexNumber Number
hi link BabelIdentifier Identifier
hi link BabelTypes Type
hi link BabelQuote PreProc

let b:current_syntax = "bbl"

" vim: ts=8 ft=vim
