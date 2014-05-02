-- loops.sp
-- Test vectors below

((main 

    {(  ( "cart"   w ) 
        ( "times"  x ) 
        ( "loop"   y )
        ( "while"  z ))
    mkhash !

    1 get_argv ! hash8
    luha 2 ith
    eval })

(w 
    {{iter
        { dup 
            <- '(' << %d << ' ' << -> 
            iter %d << ') ' <<}
        2 get_argv !
        $d 
        times
        "\n" <<}
    2 get_argv !
    $d
    times})

(x 
    {{ 
        { '*' <<} iter times
        "\n" <<}

    2 get_argv !
    $d

    times})

(y 
    {2 get_argv !
    $d

    {{ dup iter cilt}
        {last}
        {{ '*' <<} iter times
            "\n" <<}
    ifte}

    loop})

(z 
    {2 get_argv !
    $d

    {dup iter cigt}
    {{ '*' <<} iter times
        "\n" <<}
    while})

(mkhash 
    { <- newha ->
    { <- dup ->
        dup 1 ith
        <- 0 ith ->
        inskha }
    each })

(get_argv   { <- self 'argv' hash8 luha 2 ith -> ith }))


-- Test vectors:
-- % babel sp/smoke/loops.sp cart 5
-- (5 5) (5 4) (5 3) (5 2) (5 1)
-- (4 5) (4 4) (4 3) (4 2) (4 1)
-- (3 5) (3 4) (3 3) (3 2) (3 1)
-- (2 5) (2 4) (2 3) (2 2) (2 1)
-- (1 5) (1 4) (1 3) (1 2) (1 1)
--
-- % babel sp/smoke/loops.sp loop 5
-- 
-- *
-- **
-- ***
-- ****
-- *****
--
-- % babel sp/smoke/loops.sp times 5
-- *****
-- ****
-- ***
-- **
-- *
--
-- % babel sp/smoke/loops.sp while 5
-- FIXME: Known bug in iter operator inside while loop

