// fixed_opcodes.h
//

#ifndef FIXED_OPCODES_H
#define FIXED_OPCODES_H

#include "bvm.h"
#include "load.h"
#include "stack.h"
#include "bvm_opcodes.h"
#include "bvm_interp.h"
#include "arith.h"
#include "io.h"
#include "array.h"
#include "eval.h"
#include "list.h"
#include "debug.h"
#include "pearson16.h"
#include "except.h"
#include "cmp.h"
#include "string.h"
#include "shift.h"
#include "logic.h"
#include "util.h"
#include <time.h>
#include "mt19937ar.h"
#include "hash.h"
#include "bstruct.h"
#include "alloc.h"
#include "babel.h"

babel_op interp_fixed_opcodes[NUM_INTERP_OPCODES] = {

    //000
    rsvd,
    cushl,
    rsvd,
    cushr,
    rsvd,
    curol,
    cashr,
    curor,
    rsvd,
    rsvd,
    rsvd,
    cashr,
    cnot,
    rsvd,
    rsvd,
    rsvd,



    //010
    cne,
    ceq,
    cult,
    cule,
    cugt,
    cuge,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    cult,
    cule,
    cugt,
    cuge,
    rsvd,
    rsvd,



    //020
    logicF0,
    logicF1,
    logicF2,
    logicF3,
    logicF4,
    logicF5,
    logicF6,
    logicF7,
    logicF8,
    logicF9,
    logicFA,
    logicFB,
    logicFC,
    logicFD,
    logicFE,
    logicFF,



    //030
    cuadd,
    cusub,
    cumul,
    cudiv,
    rsvd,
    curem,
    rsvd,
    rsvd,
    ciadd,
    cisub,
    cimul,
    cidiv,
    ciabs,
    cirem,
    rsvd,
    rsvd,



    //040
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //050
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //060
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //070
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //080
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //090
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //0a0
    catoi,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //0b0
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //0c0
    cprintf,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    cprints,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,

 
 
    //0d0
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //0e0
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //0f0
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //100
    fnord,
    rsvd,
    babelop,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    mword_sizeop,
    arlen8,
    free_op,
    rsvd, //    rmha,
    arlen,
    arcat8,
    rsvd,
    rsvd,
    arcat,



    //110
    mu,
    nva,
    npt,
    nlf,
    nin,
    load,
    unload, //    unload,
    bs2gv,
    slice8,
    rsvd,
    rsvd,
    slice,
    rsvd,
    rsvd,
    rsvd,
    arcmp,



    //120
    sleepop,
    dieop,
    gotoop, //    gotoop,
    rsvd,
    rsvd,
    loop, //    loop,
    last, //    last,
    rsvd, //    next,
    rsvd,
    rsvd,
    rsvd,
    cut,
    trunc,
    //cxr,
    rsvd,
    rsvd,
    rsvd,



    //130
    eval, //    eval,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    whileop, //    whileop,
    times, //    times,
    rsvd,
    isinte,
    rsvd,
    newin,
    ar2ls,
    islf,
    del,
    newlf,
    sfield,



    //140
    hash8,
    randop,
    rsvd,
    argvop,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    cp, //    cp,
    paste,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //150
    rsvd, //    hash,
    rsvd,//luha, //    luha,
    rsvd, //    insha,
    rsvd,
    rsvd,//exha, //    exha,
    rsvd,
    rsvd, //    keysha,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //160
    rsvd,
    rsvd,
    rsvd,//inskha,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //170
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //180
    ar2str,
    str2ar,
    rsvd,
    rsvd,
    cu2dec,
    cu2hex,
    ci2dec,
    rsvd,
    len,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //190
    bbl2str,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    hex2cu,
    dec2ci,
    rsvd,
    uncons,
    cdrindex,
    pop,
    zap,
    rsvd,
    rsvd,
    swap,
    sel,



    //1a0
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    consls,
    carindex,
    push,
    dup,
    rsvd,
    rsvd,
    shift,
    unshift,



    //1b0
    slurp_mword,
    spit_mword,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    ls2lf,
    bons,
    rsvd,
    isnil,
    rsvd,
    rsvd,



    //1c0
    slurp,
    spit,
    journal,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //1d0
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,
    rsvd,



    //1e0 
    stdoutop8,
    rsvd,
    rsvd,
    stdinln,
    rsvd,
    rsvd,
    andop,
    orop,
    notop,
    //ishref, //    ishref,
    rsvd,
    each, //    each,
    rsvd, //    newref,
    span,
    eachar, //    eachar,
    rsvd, //    phash,
    rsvd, //    phash8,



    //1f0
    down, //    down,
    up, //    up,
    take, //    take,
    depth, //    depth,
    give, //    give,
    nest, //    nest,
    clear, //    clear,
    stdoutop,
    rsvd,
    trav,
    ith,
    cr,
    rsvd, //    breakop,
    rsvd, //    continueop,
    walk,
    set,



    //200
    perm,
    ciadd_assign,
    bvmstack,
    bvmcode,
    ifte,
    bvmustack,
    reverse,
    boilerplate,
    paste8,
    iter,
    rsvd,//unnest
    conjure,
    self,
    cisub_assign,
    ducp,
    endian,



    //210
    rsvd,
    //deref,
    rsvd,
    ifop,
    ordop,
    let,
    split,
    lusym,
    th,
    move,
    move8,
    insls,

};

#endif //FIXED_OPCODES_H

