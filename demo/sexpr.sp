--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                           BPDL S-EXPR PARSER                             --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

((opcode_table  (ptr nil nil))

(main (code 

    create_opcode_table !

    self 'argv' 
    hash8 luha 
    2 ith 
    cdr

    <- 
        ''
        construct
        (code .) each
    ->

    (code >>> "\n" . .) each

    <- "( " -> .
    "\n)" .

    dos_clean !
    balanced_parse !

--    (code depth 1 >)
--        (code 
--            collect !
--            append)
--        (code fnord)
--    ifte

    cdr

    (code 
        cdr
        (code 
            dup
            <- 1 ith detag ".parse_tree" . ->
            2 ith
            set_sym ! ) 
        each)
    each

    "nil.encoded"
    nil
    set_sym !

    "root" encode_section !

--    self
--    entsha
--    (code 1 ith nl <<) each
--    die

--    "root" get_encoded ! bs2gv << die
--    eval

    nil
    "root" get_encoded !
    babel))

(load_file (code 1 get_argv ! >>> ))
(get_argv  (code <- self 'argv' hash8 luha 2 ith -> ith))

(set_sym   (code <- <- self -> -> inskha))
(get_sym   (code <- self -> hash8 luha 2 ith))
(ex_sym    (code <- self -> hash8 exha))

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                      BALANCED EXPRESSION PARSER                          --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

(balanced_parse (code 
    (code
        give_str !
--        trim_leading_ws !
        trim_comments_and_ws !
        (list
            (code dup lparen =) (code <- "list " give_str ! ->)
            (code dup lcurly =) (code <- "code " give_str ! ->))
        cond
        begin_balanced !)
    nest))

(give_str (code str2ar ar2ls rev give ))

(space          (val 0x20))
(dquote         (val 0x22))
(squote         (val 0x27))
(lbracket       (val 0x5b))
(rbracket       (val 0x5d))
(lparen         (val 0x28))
(rparen         (val 0x29))
(lcurly         (val 0x7b))
(rcurly         (val 0x7d))
(dos_newline    (val 0x0d))
(newline        (val 0x0a))
(dash           (val 0x2d))
(backslash      (val 0x5c))
(slash          (val 0x2f))
(lowercase_n    (val 0x6e))
(lowercase_t    (val 0x74))
(lowercase_x    (val 0x78))

(is_close_bracket (code 
    dup dup
    rcurly   = <-
    rparen   = <-
    rbracket = -> ->
    or or))

(is_open_bracket (code 
    dup dup
    lbracket = <-
    lparen   = <-
    lcurly   = -> ->
-- dump_stack ! die
    or or))

(begin_balanced (code
    (code is_open_bracket !)
        (code fnord)
        (code "blech\n" << die)
    ifte

    nil <-

    (code depth 0 >)
    (code
--        trim_leading_ws !
        trim_comments_and_ws !
        (list 
            (code dup lbracket          =) (code begin_balanced !     -> cons <-)
            (code dup lparen            =) (code 
                                                 <- "list " give_str ! ->
                                                 begin_balanced !     -> cons <-)
            (code dup lcurly            =) (code 
                                                 <- "code " give_str ! ->
                                                 begin_balanced !     -> cons <-)
            (code dup is_close_bracket  !) (code zap                        last)
            (code dup dquote            =) (code zap get_dquote !     -> cons <-)
            (code dup squote            =) (code zap get_squote !     -> cons <-)
            (code 1                      ) (code get_non_quote  !     -> cons <-))
        cond)
    while
    -> rev ))

(is_babel_whitespace (code 
    dup
    space   = <-
    newline = ->
    or))

(is_babel_comment (code 
    2 take 
    dup 
    <- give   -> 
       give
    <- dash = ->
       dash =
    and))

(trim_comments_and_ws (code
    (code dup is_babel_whitespace !
          <-  is_babel_comment    ! ->
          cor)
        (code 
            trim_leading_ws !
            trim_comment    !)
    while))

(trim_comment (code 
    (code is_babel_comment !)
        (code 
            (code dup newline ~=)
                (code zap)
            while)
        (code fnord)
    ifte))

(trim_leading_ws (code 
    (code dup is_babel_whitespace !)
        (code zap)
    while))

-- NOTE: This will cause problems when quoting 
-- DOS newline characters
(dos_clean (code 
    (code
        give_str !
        (code 
            (code dup dos_newline =)
                (code zap)
                (code down)
            ifte)
        depth 1 -
        times
        flip
        collect !
        ls2lf ar2str)
    nest))

--(gather_non_squote (code
--    nil <-
--    (code dup squote ~=)
--        (code -> cons <-)
--    while
--    zap
--    -> rev))

--(gather_non_dquote (code
--    nil <-
--    (code dup dquote ~=)
--        (code -> cons <-)
--    while
--    zap
--    -> rev))

(gather_non_squote (code
    nil <-
    (code depth 0 >) -- Potential bug as we lookahead two chars
        (code 
        (code <- dup squote    = -> <->
              <- dup slash     = ->
              and)
            (code (code -> cons <-) 2 times)
            (code 
                (code dup squote ~=)
                    (code -> cons <-)
                    (code last)
                ifte)
        ifte)
    while
    zap
    -> rev))

(gather_non_dquote (code
    nil <-
    (code depth 0 >) -- Potential bug as we lookahead two chars
        (code 
        (code <- dup dquote    = -> <->
              <- dup backslash = ->
              and)
            (code (code -> cons <-) 2 times)
            (code 
                (code dup dquote ~=)
                    (code -> cons <-)
                    (code last)
                ifte)
        ifte)
    while
    zap
    -> rev))

(gather_non_whitespace (code
    nil <-
    (code 
        dup dup dup dup dup
        rcurly     ~= <-
        rparen     ~= <-
        space      ~= <-
        lbracket   ~= <-
        rbracket   ~= 
        -> -> -> ->
        and and and and)
        (code -> cons <-)
    while
    -> rev))

(get_dquote (code

    gather_non_dquote !

    ls2str !
    unescape_dquote !
    <- "string" hash8 -> newtptr ))

(get_squote (code

    gather_non_squote !

    ls2str !
    unescape_squote !
    <- "string" hash8 -> newtptr ))

(ls2str (code ls2lf ar2str))

(get_non_quote (code 

    gather_non_whitespace !

    ls2str !

    (code dup dup is_hex !)
        (code $x <- "number" hash8 -> newtptr)
        (code 
        (code is_numeric !)
            (code $d <- "number" hash8 -> newtptr)
            (code    <- "ident"  hash8 -> newtptr)
        ifte)
    ifte))

(is_generic_non_whitespace (code 
    0 <-
    (list rcurly rparen space lbracket rbracket)
    (code 
        <- dup -> 
        = 
        -> cor <- )
    each
    not))

(is_ident_token  (code "ident"  hash8 tageq))
(is_number_token (code "number" hash8 tageq))
(is_string_token (code "string" hash8 tageq))

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                               ENCODER                                    --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

(encode_section (code 
    dup
    get_parse_tree !
    encode         !
    dup
    <- set_encoded ! ->))

(encode (code
    pop
    (code car dup is_ident_token !)
        (code 
            detag
            (list 
                (code dup "val"  streq !) (code zap encode_val  !)
                (code dup "ptr"  streq !) (code zap encode_ptr  !)
                (code dup "tag"  streq !) (code zap encode_tag  !)
                (code dup "list" streq !) (code zap encode_list !)
                (code dup "hash" streq !) (code zap encode_hash !)
                (code dup "oper" streq !) (code zap encode_oper !)
                (code dup "ref"  streq !) (code zap encode_ref  !)
                (code dup "code" streq !) (code zap encode_code !)
                (code 1                 ) (code die              ))
            cond)
        (code die)
    ifte))

-- FIXME: tag checking
(encode_val (code 
    (code
        (code detag) each
        (code depth 1 >)
            (code 
                depth 1 - take
                (code cat) each) -- or ls2lf
            (code fnord)
        ifte)
    nest))

(encode_ptr (code
    (code
        (code 
            (code dup istptr)
                (code 
                    (code dup is_ident_token ! )
                        (code 
                            detag
                            (code dup is_encoded     !)
                                (code get_encoded    !)
                                (code encode_section !)
                            ifte)
                        (code detag)
                    ifte)
                (code encode !)
            ifte)
        each
        collect !
        bons)
    nest))

(encode_list (code 
    (code
        (code 
            (code dup istptr)
                (code 
                    (code dup is_ident_token ! )
                        (code 
                            detag
                            (code dup is_encoded     !)
                                (code get_encoded    !)
                                (code encode_section !)
                            ifte)
                        (code detag)
                    ifte)
                (code encode !)
            ifte)
        each
        collect !)
    nest))

(encode_tag (code 
    give

    (code dup istptr)
        (code 
            (code dup is_ident_token ! )
                (code 
                    detag
                    (code dup is_encoded     !)
                        (code get_encoded    !)
                        (code encode_section !)
                    ifte)
                (code detag)
            ifte)
        (code encode !)
    ifte

--dump_stack ! die
--    encode !

    <- detag hash8 ->
    newtptr))

(encode_code (code 
    (code
        (code 
            (code dup istptr)
                (code 
                    (code dup is_ident_token      !)
                        (code 
                            (code detag dup is_opcode !)
                                (code lookup_opcode   !)
                                (code 
                                    (code dup is_encoded     !)
                                        (code get_encoded    !)
                                        (code encode_section !)
                                    ifte
                                    1 take bons)
                            ifte)
                        (code detag 1 take bons)
                    ifte)
                (code encode ! 1 take bons)
            ifte)
        each
        collect !)
    nest))

(encode_hash (code die))
(encode_oper (code die))
(encode_ref  (code die))

(unescape_dquote (code 
    (code
        give_str !
        (code depth 0 >)
            (code 
                (code dup backslash =)
                    (code 
                        zap
                        (list 
                            (code dup lowercase_n =)(code zap "\n" down)
                            (code dup lowercase_t =)(code zap "\t" down)
                            (code 1                )(code down         ))
                        cond)
                    (code down)
                ifte)
        while
        flip
        collect !
        ls2lf ar2str)
    nest))

(unescape_squote (code 
    (code
        give_str !
        (code depth 0 >)
            (code 
                (code dup slash =)
                    (code 
                        zap
                        (list 
                            (code dup lowercase_n =)(code zap "\n" down)
                            (code dup lowercase_t =)(code zap "\t" down)
                            (code dup lowercase_t =)(code zap "\t" down)
                            (code 1                )(code down         ))
                        cond)
                    (code down)
                ifte)
        while
        flip
        collect !
        ls2lf ar2str)
    nest))

(is_encoded (code
    ".encoded" .
    ex_sym !))

(get_encoded (code
    ".encoded" .
    get_sym !))

(set_encoded (code
    <- ".encoded" . ->
    set_sym !))

(get_parse_tree (code
    ".parse_tree" .
    get_sym !))

(set_parse_tree (code
    <- ".parse_tree" . ->
    set_sym !))

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                              OPCODE TABLE                                --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

(is_opcode (code 
    hash8 
    <- opcode_table car ->
    exha))

(lookup_opcode (code 
    hash8 
    <- opcode_table car ->
    luha detag
    2 ith))

(create_opcode_table (code 
    opcode_table
    opcode_list
    mkhash ! 1 take
    set ))

(opcode_list (list
    (list "cushl"          0x001)        (list "cu2hex"         0x185)
    (list "cushr"          0x003)        (list "ci2dec"         0x186)
    (list "curol"          0x005)        (list "len"            0x188)
    (list "cashr"          0x006)        (list "bbl2str"        0x190)
    (list "curor"          0x007)        (list "hex2cu"         0x195)
    (list "cashr"          0x00b)        (list "dec2ci"         0x196)
    (list "cnot"           0x00c)        (list "uncons"         0x198)
    (list "cne"            0x010)        (list "cdr"            0x199)
    (list "ceq"            0x011)        (list "pop"            0x19a)
    (list "cult"           0x012)        (list "zap"            0x19b)
    (list "cule"           0x013)        (list "swap"           0x19e)
    (list "cugt"           0x014)        (list "sel"            0x19f)
    (list "cuge"           0x015)        (list "cons"           0x1a8)
    (list "cilt"           0x01a)        (list "car"            0x1a9)
    (list "cile"           0x01b)        (list "push"           0x1aa)
    (list "cigt"           0x01c)        (list "dup"            0x1ab)
    (list "cige"           0x01d)        (list "shift"          0x1ae)
    (list "logicF0"        0x020)        (list "unshift"        0x1af)
    (list "logicF1"        0x021)        (list "slurp"          0x1b0)
    (list "cand"           0x021)        (list "spit"           0x1b1)
    (list "logicF2"        0x022)        (list "ls2lf"          0x1ba)
    (list "logicF3"        0x023)        (list "bons"           0x1bb)
    (list "logicF4"        0x024)        (list "isnil"          0x1bd)
    (list "logicF5"        0x025)        (list "slurp8"         0x1c0)
    (list "logicF6"        0x026)        (list "spit8"          0x1c1)
    (list "cxor"           0x026)        (list "journal8"       0x1c2)
    (list "logicF7"        0x027)        (list "stdout8"        0x1e0)
    (list "cor"            0x027)        (list "stdinln"        0x1e3)
    (list "logicF8"        0x028)        (list "nhref"          0x1e5)
    (list "cnor"           0x028)        (list "and"            0x1e6)
    (list "logicF9"        0x029)        (list "or"             0x1e7)
    (list "cxnor"          0x029)        (list "not"            0x1e8)
    (list "logicFa"        0x02a)        (list "each"           0x1ea)
    (list "logicFb"        0x02b)        (list "span"           0x1ec)
    (list "logicFc"        0x02c)        (list "eachar"         0x1ed)
    (list "logicFd"        0x02d)        (list "phash"          0x1ee)
    (list "logicFe"        0x02e)        (list "phash8"         0x1ef)
    (list "cnand"          0x02e)        (list "down"           0x1f0)
    (list "logicFf"        0x02f)        (list "up"             0x1f1)
    (list "cuadd"          0x030)        (list "take"           0x1f2)
    (list "cusub"          0x031)        (list "depth"          0x1f3)
    (list "cumul"          0x032)        (list "give"           0x1f4)
    (list "cudiv"          0x033)        (list "nest"           0x1f5)
    (list "curem"          0x035)        (list "clear"          0x1f6)
    (list "ciadd"          0x038)        (list "stdout"         0x1f7)
    (list "cisub"          0x039)        (list "trav"           0x1f9)
    (list "cimul"          0x03a)        (list "ith"            0x1fa)
    (list "cidiv"          0x03b)        (list "cr"             0x1fb)
    (list "ciabs"          0x03c)        (list "nl"             0x1fb)
    (list "cirem"          0x03d)        (list "walk"           0x1fe)
    (list "catoi"          0x0a0)        (list "set"            0x1ff)
    (list "cprintf"        0x0c0)        (list "perm"           0x200)
    (list "cprints"        0x0a8)        (list "add_assign"     0x201)
    (list "fnord"          0x100)        (list "bvmstack"       0x202)
    (list "babel"          0x102)        (list "stack"          0x202)
    (list "msize"          0x107)        (list "bvmcode"        0x203)
    (list "arlen8"         0x108)        (list "ifte"           0x204)
    (list "free"           0x109)        (list "bvmustack"      0x205)
    (list "rmha"           0x10a)        (list "rev"            0x206)
    (list "arlen"          0x10b)        (list "boilerplate"    0x207)
    (list "cat8"           0x10c)        (list "paste8"         0x208)
    (list "cat"            0x10f)        (list "iter"           0x209)
    (list "mu"             0x110)        (list "conjure"        0x20b)
    (list "nva"            0x111)        (list "self"           0x20c)
    (list "npt"            0x112)        (list "sub_assign"     0x20d)
    (list "nlf"            0x113)        (list "endian"         0x20f)
    (list "nin"            0x114)        (list "nhword"         0x210)
    (list "load"           0x115)        (list "deref"          0x211)
    (list "unload"         0x116)        (list "if"             0x212)
    (list "bs2gv"          0x117)        (list "ord"            0x213)
    (list "show"           0x117)        (list "let"            0x214)
    (list "slice8"         0x118)        (list "split"          0x215)
    (list "slice"          0x11b)        (list "lusym"          0x216)
    (list "arcmp"          0x11f)        (list "th"             0x217)
    (list "sleep"          0x120)        (list "move"           0x218)
    (list "die"            0x121)        (list "move8"          0x219)
    (list "goto"           0x122)        (list "ins"            0x21a)
    (list "loop"           0x125)        (list "exec"           0x21b)
    (list "last"           0x126)        (list "dev"            0x21c)
    (list "next"           0x127)        (list "cond"           0x21d)
    (list "cut"            0x12b)        (list "append"         0x21e)
    (list "trunc"          0x12c)        (list "newha"          0x21f)
    (list "eval"           0x130)        (list "bvmrstack"      0x220)
    (list "while"          0x135)        (list "bvmsym"         0x221)
    (list "times"          0x136)        (list "hibernate"      0x222)
    (list "isinte"         0x138)        (list "bvm_step"       0x223)
    (list "newin"          0x13a)        (list "rawmem"         0x224)
    (list "ar2ls"          0x13b)        (list "set_ith"        0x225)
    (list "isleaf"         0x13c)        (list "set_car"        0x226)
    (list "del"            0x13d)        (list "set_cdr"        0x227)
    (list "newlf"          0x13e)        (list "detag"          0x228)
    (list "sfield"         0x13f)        (list "flush_cache"    0x229)
    (list "hash8"          0x140)        (list "update_cache"   0x22a)
    (list "rand"           0x141)        (list "copy_collect"   0x22b)
    (list "argv"           0x143)        (list "bvm_root"       0x22c)
    (list "cp"             0x148)        (list "dump_cache"     0x22d)
    (list "paste"          0x149)        (list "sortlf"         0x22e)
    (list "hash"           0x150)        (list "shuf"           0x22f)
    (list "luha"           0x151)        (list "pass"           0x230)
    (list "insha"          0x152)        (list "fail"           0x231)
    (list "exha"           0x154)        (list "newtptr"        0x232)
    (list "keysha"         0x156)        (list "istptr"         0x233)
    (list "entsha"         0x156)        (list "get_tag"        0x234)
    (list "inskha"         0x162)        (list "seq"            0x235)
    (list "ar2str"         0x180)        (list "alt"            0x236)
    (list "str2ar"         0x181)        (list "tageq"          0x237)
    (list "cu2dec"         0x184)        (list "exit"           0x238)))

(construct (list 
'((root      [tag "/babel/tag/sparse_bvm" '
'(bvm_code bvm_stack jump_table sym_table) ])'

'(bvm_code [tag "/babel/tag/bvm_code" ((main) nil)])'

'(bvm_stack [tag "/babel/tag/bvm_stack" (dstack ustack)])'

'(dstack     (nil))'
'(ustack     (nil))'

'(jump_table [tag "/babel/tag/jump_table" nil])'

'(sym_table [tag "/babel/tag/sym_table" sym_table_hash_table])'

'(sym_table_hash_table [tag "/babel/tag/hash_table" symbol ])'

'(symbol (nil)))')))

