-- lib.sp

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                            BABEL STD LIB                                 --
--                                                                          --
--      HASHES                                                              --
--      STRINGS                                                             --
--      LISTS                                                               --
--      MISC                                                                --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

((nothing {fnord})

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                               HASHES                                     --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

(mkhash 
    { <- newha ->
    { <- dup ->
        dup 1 ith
        <- 0 ith ->
        inskha }
    each })

-- pays
-- Extracts all payloads from a hash table
(pays 
    {{entsha 
        {2 ith}
        each 
        collect !}
    nest})

-- keys
-- Extracts all string keys from a hash table
(keys 
    {{entsha 
        {1 ith}
        each 
        collect !}
    nest})

-- hkeys
-- Extracts all hashed keys from a hash table
(hkeys 
    {{entsha 
        {0 ith}
        each 
        collect !}
    nest})

-- "foo" bar set_sym
-- "foo" get_sym
-- "foo" ex_sym
(set_sym    { <- <- self -> ->       inskha       })
(get_sym    { <-    self    -> hash8 luha   2 ith })
(ex_sym     { <-    self    -> hash8 exha         })

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                               STRINGS                                    --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

(is_upper 
    { dup 
    <- 0x40 cugt ->
    0x5b cult 
    cand })

(is_lower
    { dup 
    <- 0x60 cugt ->
    0x7b cult 
    cand })

(strins 
    { <->
    <- <- str2ls ! dup ->
    dup
    {0 =}
        {zap zap
            -> str2ls ! 
            <-> 
            2 take append }
        {1 - cdri ! 
            -> str2ls ! 
            ins}
    ifte
    ls2lf ar2str})

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                                LISTS                                     --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

(collect    { -1 take })

(cdri       { <- {cdr} -> times })

(str2ls     {str2ar ar2ls})

(ls2str     {ls2lf ar2str})

-- (a b c) d N insi
(insi
    {{dup}
        {zap push}
        {<->
            <- <- {cdr} -> 
            1 - 
            times ->
            ins}
    ifte})

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--                                                                          --
--                                MISC                                      --
--                                                                          --
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

(dump_stack { 
    {depth}
        {collect ! dup
            <- give ->
            rev 
            {bbl2str nl <<} each}
        {"nil\n" <<}
    ifte})

(get_argv   { <- self 'argv' hash8 luha 2 ith -> ith })

(conjunct   {1 <- { <- dup -> eval -> and <- } each zap ->})

(disjunct   {0 <- { <- dup -> eval -> or  <- } each zap ->}))



