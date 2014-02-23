-- loops.sp
-- Strange Loop Conference 2013 ELC

((main (code 
    
    (list 
        (list "cart"   w ) 
        (list "times"  x ) 
        (list "loop"   y )
        (list "while"  z ))
    mkhash !

    1 argvn ! hash8
    luha
    ! ))

(w (code 
    (code 
        iter
        (code dup 
            <- %d ' ' . << -> 
            iter %d nl <<) 
        2 argvn !
        $d 
        times)
    2 argvn !
    $d
    times))

(x (code 

    (code 
        (code '*' <<) iter times
        "\n" <<)

    2 argvn !
    $d

    times))

(y (code 

    2 argvn !
    $d

    (code 

    (code dup iter <)
        (code last)
        (code 
            (code '*' <<) iter times
            "\n" <<)
    ifte)

    loop))

(z (code 

    2 argvn !
    $d

    (code dup iter >)
    (code  
        (code '*' <<) iter times
        "\n" <<
        )
    while))

(mkhash (code 
    <- newha ->
    (code 
        <- dup ->
        dup 1 ith
        <- 0 ith hash8 ->
        insha)
    each)))

