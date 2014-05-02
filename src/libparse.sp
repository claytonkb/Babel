-- libparse.sp

-- Tokens:
--   [ ]
--   'foo'
-- 

((is_squote_string (code 

    (code 

        str2chars ! 

        pop car
        is_squote !
        <->

        dump_stack ! die

        shift car
        is_squote !
        <->

        <- cand ->

        (code 
            is_squote ! not)
        each

        collect !
        <- 1 -> (code cand) each)

    nest))

(is_escape (code 
    (code dup #8 2 =)
    (code 
        str2ar 1 cut zap
        ar2str
        is_backslash !)
    (code zap 0)
    ifte))

(is_ident (code 
    (code dup #8)
    (code
        str2chars !
        pop car
        is_c_first_ident !
        <->
        (code 
            is_c_ident !
            cand )
        each)
    (code zap 0)
    ifte))

(is_hex (code 
    str2ar dup
    (code 
    dup 0 th 0x30 = 
    <-  1 th 0x78 = ->
    and) 
        (code 2 cut <-> zap ar2str is_hex_numeric !)
        (code zap 0)
    ifte))

(is_hex_numeric (code 
    (code dup #8)
        (code
            <- 1 ->
            str2chars !
            (code 
                is_hex_digit !
                cand )
            each)
       (code zap 0)
    ifte))

-- string -> bool
(is_signed_numeric (code 
    str2ar ar2ls dup
    (code 0 ith 0x2d =)
        (code cdr )
        (code fnord )
    ifte
    ls2lf ar2str 
    is_numeric !))

-- string -> bool
(is_numeric (code 
    (code dup #8)
        (code
            (code dup '0' streq !)
                (code zap 1)
                (code
                    str2chars !
                    pop car
                    is_nonzero !
                    <->
                    (code 
                        is_digit !
                        cand )
                    each)
            ifte)
        (code zap 0)
    ifte))

(str2chars (code str2ar ar2chars !))

(ar2chars (code 
    (code 
        (code ar2str) 
        eachar 
        collect !) 
    nest))

(streq  (code == not))

(is_digit (code 
    str2ar dup 
    <- 0x30 >= ->
    0x3a < 
    cand ))

(is_hex_digit (code 
    str2ar 

    dup dup
    <- 0x30 >= ->
    0x3a < 
    cand 
    <->

    dup dup
    <- 0x41 >= ->
    0x46 <= 
    cand 
    <->

    dup
    <- 0x61 >= ->
    0x66 <= 
    cand

    cor cor))

(is_nonzero (code 
    str2ar dup 
    <- 0x30 > ->
    0x3a < 
    cand ))

(is_upper (code
    str2ar dup 
    <- 0x40 > ->
    0x5b < 
    cand))

(is_lower (code
    str2ar dup 
    <- 0x60 > ->
    0x7b < 
    cand))

(is_alpha (code
    dup 
    <- is_upper ! ->
    is_lower ! 
    cor ))

(is_alphanum (code
    dup 
    <- is_alpha ! ->
    is_digit ! 
    cor ))

(is_ascii (code str2ar 0x80 < ))

(is_printable (code
    str2ar dup 
    <- 0x1f > ->
    0x7f < 
    cand))

(is_ex_ascii (code
    str2ar dup
    <- 0x80 > ->
    0xff <=
    cand))

(is_c_first_ident (code 
    dup
    <- is_alpha ! ->
    is_underscore !
    cor ))

(is_c_ident (code 
    dup
    <- is_c_first_ident ! ->
    is_digit !
    cor ))

(is_list_delimiter (code 
    dup
    <- is_lparen ! ->
    is_rparen !
    cor ))

(is_code_delimiter (code 
    dup
    <- is_lcurly ! ->
    is_rcurly !
    cor ))

(is_sexpr_delimiter (code 
    dup
    <- is_lbracket ! ->
    is_rbracket !
    cor ))

-- This is very inefficient and clunky...
(is_sigil (code 

    dup
    <- is_bang ! ->

    dup
    <- is_at ! ->

    dup
    <- is_hash ! ->

    dup
    <- is_dollar ! ->

    dup
    <- is_percent ! ->

    dup
    <- is_aster ! ->

    dup
    <- is_carat ! ->

    is_amper !

    (code cor) 7 times ))

(is_space       (code str2ar 0x20 =))
(is_bang        (code str2ar 0x21 =))
(is_dquote      (code str2ar 0x22 =))
(is_hash        (code str2ar 0x23 =))
(is_dollar      (code str2ar 0x24 =))
(is_percent     (code str2ar 0x25 =))
(is_amper       (code str2ar 0x26 =))
(is_squote      (code str2ar 0x27 =))
(is_lparen      (code str2ar 0x28 =))
(is_rparen      (code str2ar 0x29 =))
(is_aster       (code str2ar 0x2a =))
(is_plus        (code str2ar 0x2b =))
(is_comma       (code str2ar 0x2c =))
(is_dash        (code str2ar 0x2d =))
(is_period      (code str2ar 0x2e =))
(is_slash       (code str2ar 0x2f =))

(is_colon       (code str2ar 0x3a =))
(is_scolon      (code str2ar 0x3b =))
(is_lt          (code str2ar 0x3c =))
(is_equal       (code str2ar 0x3d =))
(is_gt          (code str2ar 0x3e =))
(is_question    (code str2ar 0x3f =))

(is_at          (code str2ar 0x40 =))

(is_lbracket    (code str2ar 0x5b =))
(is_backslash   (code str2ar 0x5c =))
(is_rbracket    (code str2ar 0x5d =))
(is_carat       (code str2ar 0x5e =))
(is_underscore  (code str2ar 0x5f =))

(is_backquote   (code str2ar 0x60 =))

(is_lcurly      (code str2ar 0x7b =))
(is_vert_bar    (code str2ar 0x7c =))
(is_rcurly      (code str2ar 0x7d =))
(is_tilde       (code str2ar 0x7e =)))

