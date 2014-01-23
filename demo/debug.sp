-- debug.sp
--        (code dup 'h' streq !) (code zap help_msg strcat ! <<))
--
-- TODO:
-- Pointer lookup
-- udr-stack injection
-- code injection

((main (code 
    load_program !
    debugger     !))

(debugger (code (code

        dup
        dup

        prompt !

        (list  -- multi-letter strings work...
        (code dup 'b' streq !) (code zap save_bbl2str      !)
        (code dup 'c' streq !) (code zap dump_code         !)
        (code dup 'd' streq !) (code zap dump_dstack       !)
        (code dup 'h' streq !) (code zap show_help_msg     !)
        (code dup 'i' streq !) (code zap save_bvm          !)
        (code dup 'l' streq !) (code zap show_listing      !)
        (code dup 'm' streq !) (code zap show_mem          !)
        (code dup 'q' streq !) (code zap last               )
        (code dup 'r' streq !) (code zap dump_rstack       !)
        (code dup 's' streq !) (code zap bvm_step           )
        (code dup 't' streq !) (code zap bvm_step 
                                         dump_dstack       !)
        (code dup 'u' streq !) (code zap dump_ustack       !)
        (code 1              ) (code zap                    ))
        cond)

    loop))


(strcat (code <- '' ->  (code .) each))
(strne  (code arcmp))
(streq  (code strne ! not))


(prompt (code 
    "\n> " <<
    >>))


(load_program (code 
    1 argvn ! 
    slurp 
    load))


(bvm_code_ptr   (code car car car))
(bvm_dstack_ptr (code cdr car car))
(bvm_ustack_ptr (code cdr car cdr car))
(bvm_rstack_ptr (code car cdr ))
(bvm_sym_table  (code cdr cdr cdr car car))


-- FIXME: jump_table needs to be set to nil before unload...
(save_bvm (code 
    unload
    "debug.bbl"
    spit ))


(save_bbl2str (code 
    bvm_dstack_ptr !
--    car car
    car
    bs2gv
    "debug.dot"
    spit8 ))


(dump_bvm (code 
    dup
    dump_opcode !

    dup
    dump_dstack !))


(dump_ustack (code 
    bvm_ustack_ptr  ! 
    dump_stack      ! ))


(dump_rstack (code 
    bvm_rstack_ptr  ! 
    dump_stack      ! ))


(dump_stack (code 

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


(dump_code (code 

    bvm_code_ptr !
    dup

    (code isnil)
        (code zap "  nil\n" <<)
        (code (code 
            --car dup
            dup
            (code isleaf)
                (code 
                    0 th 
                    opcodes <-> 
                    ith
                    '  ' <<
                    <<)
                (code zap ' () ' <<)
            ifte)
            ... 
            "\n" <<)
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


(show_listing (code 
    bvm_dstack_ptr !
    stack_val !
    listing ! 
    print_listing !))

(stack_val (code car 0 ith car))


(show_mem (code 
    bvm_dstack_ptr !
    stack_val !
    -- rawmem
    hex08 !
    << ))


(print_listing (code (code 
        dup 
        0 ith   <<
        ' '     <<
        1 ith   << 
        -- (code <<)
        -- ...
        "\n"  <<) 
    ... ))


(hex08 (code 

    %x
    <- zero_pad cp dup ->
    
    dup 

    arlen8 dup

    <- 
        8 sw- !
        <->
        0 
    ->

    move8))


(sw- (code <-> -))
(zero_pad (val "00000000"))
-- (ducp (code cp <-> zap))


(listing (code 
    unload
    2 newin <-
    (code 
        quad_iter ! hex08 ! mkcell !
        <- hex08 ! mkcell ! ->  
        push mkcell !
        -> push <-)
    eachar
    -> 
    pop zap ))


(quad_iter (code iter 4 *))
(mkcell (code (ptr nil) car cons)) -- FIXME SUPER YUCKY!


(show_help_msg (code help_msg strcat ! <<))


(opcodes (list 
    "rsvd" "cushl" "rsvd" "cushr" "rsvd" "curol" "cashr" "curor" "rsvd" 
    "rsvd" "rsvd" "cashr" "cnot" "rsvd" "rsvd" "rsvd" "cne" "ceq" "cult" "cule"
    "cugt" "cuge" "rsvd" "rsvd" "rsvd" "rsvd" "cilt" "cile" "cigt" "cige" 
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
    "bvmrstack" "bvmsym" "hibernate" "bvm_step"))


(help_msg (list 
    "b      Dump the dstack to 'debug.dot'\n"
    "c      Dump the code list\n"
    "d      Dump the dstack\n"
    "l      Show listing of TOS\n"
    "h      Show this message\n"
    "i      Save BVM to file 'debug.bbl'\n"
    "q      Quit the debugger\n"
    "r      Dump the rstack\n"
    "s      Step the program\n"
    "t      s d\n"
    "u      Dump the ustack\n")))

