-- debug.sp

((main (code 
    load_program !
    debugger !))

(debugger (code (code

        dup

        prompt !

        (list
        (code 's' streq) (code bvm_step)
        (code 'd' streq) (code dump_bvm !)
        (code 'q' streq) (code last))
        cond)

    loop))

(strne (code arcmp))
(streq (code strne ! not))

(prompt (code "\n> " <<))

(load_program (code 
    1 argvn ! 
    slurp 
    load))

(bvm_code_ptr   (code car car car))
(bvm_dstack_ptr (code cdr car car))
(bvm_ustack_ptr (code cdr car cdr car))
(bvm_rstack_ptr (code cdr cdr car))
(bvm_sym_table  (code cdr cdr cdr car car))


(dump_bvm (code 
        dup
        dump_opcode !

        dup
        dump_dstack !))


(dump_dstack (code 

    bvm_dstack_ptr !
    dup

    (code car isnil)
        (code zap "  nil" <<)
        (code 
        car
        (code 
            "  " << 
            car
            bbl2str << "\n" <<) 
        each)
    ifte))


(dump_opcode (code 

    bvm_code_ptr !
    dup

    (code isnil)
        (code zap)
        (code 
            car dup
            (code isleaf)
                (code 
                    0 th 
                    opcodes <-> 
                    ith
                    '  ' << nl <<)
                (code zap)
            ifte)
    ifte))


(opcodes (list 
"rsvd" "cushl" "rsvd" "cushr" "rsvd" "curol" "cashr" "curor" "rsvd" 
"rsvd" "rsvd" "cashr" "cnot" "rsvd" "rsvd" "rsvd" "cne" "ceq" "cult" "cule"
"cugt" "cuge" "rsvd" "rsvd" "rsvd" "rsvd" "cult" "cule" "cugt" "cuge" 
"rsvd" "rsvd" "logicF0" "logicF1" "logicF2" "logicF3" "logicF4" "logicF5" 
"logicF6" "logicF7" "logicF8" "logicF9" "logicFA" "logicFB" "logicFC" 
"logicFD" "logicFE" "logicFF" "cuadd" "cusub" "cumul" "cudiv" "rsvd" 
"curem" "rsvd" "rsvd" "ciadd" "cisub" "cimul" "cidiv" "ciabs" "cirem" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "catoi" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"cprintf" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "cprints" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "fnord" "rsvd" "babelop" "rsvd" 
"rsvd" "rsvd" "rsvd" "mword_sizeop" "arlen8" "free_op" "rmha" "arlen" 
"arcat8" "rsvd" "rsvd" "arcat" "mu" "nva" "npt" "nlf" "nin" "load" 
"unload" "bs2gv" "slice8" "rsvd" "rsvd" "slice" "rsvd" "rsvd" "rsvd" 
"arcmp" "sleepop" "dieop" "gotoop" "rsvd" "rsvd" "loop" "last" "next" 
"rsvd" "rsvd" "rsvd" "cut" "trunc" "rsvd" "rsvd" "rsvd" "eval" "rsvd" 
"rsvd" "rsvd" "rsvd" "whileop" "times" "rsvd" "isinte" "rsvd" "newin" 
"ar2ls" "islf" "del" "newlf" "sfield" "hash8" "randop" "rsvd" "argvop" 
"rsvd" "rsvd" "rsvd" "rsvd" "cp" "paste" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "luha" "insha" "rsvd" "exha" "rsvd" "keysha" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"inskha" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"ar2str" "str2ar" "rsvd" "rsvd" "cu2dec" "cu2hex" "ci2dec" "rsvd" "len" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "bbl2str" "rsvd" "rsvd" 
"rsvd" "rsvd" "hex2cu" "dec2ci" "rsvd" "uncons" "cdrindex" "pop" "zap" 
"rsvd" "rsvd" "swap" "sel" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "consls" "carindex" "push" "dup" "rsvd" "rsvd" "shift" "unshift" 
"slurp_mword" "spit_mword" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "ls2lf" "bons" "rsvd" "isnil" "rsvd" "rsvd" "slurp" "spit" "journal" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" 
"rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "rsvd" "stdoutop8" 
"rsvd" "rsvd" "stdinln" "rsvd" "rsvd" "andop" "orop" "notop" "rsvd" "each" 
"rsvd" "span" "eachar" "rsvd" "rsvd" "down" "up" "take" "depth" "give" 
"nest" "clear" "stdoutop" "rsvd" "trav" "ith" "cr" "rsvd" "rsvd" "walk" 
"set" "perm" "ciadd_assign" "bvmstack" "bvmcode" "ifte" "bvmustack" 
"reverse" "boilerplate" "paste8" "iter" "rsvd" "conjure" "self" 
"cisub_assign" "ducp" "endian" "rsvd" "deref" "ifop" "ordop" "let" "split" 
"lusym" "th" "move" "move8" "insls" "exec" "dev" "cond" "append" "newha" 
"bvmrstack" "bvmsym" "hibernate" "bvm_step")))
        
--    bbl2str 
--    << )))

--    bvm_dstack_ptr !
--    dup
--    (code isnil)
--        (code fnord)
--        (code 
--            (code dup car bbl2str << "\n" <<)
--            each)
--    ifte)))

