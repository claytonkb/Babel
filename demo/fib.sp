-- fib.sp

((main (code 
    'argv' lusym 
    1 th $d
    fib ! 
    %d nl <<))

(fib (code 
    (code is_zero !)
        (code zap 1)
        (code 
            (code is_one !)
                (code fnord)
                (code 
                    cp 
                    <- 2 - fib ! -> 
                    1 - fib ! 
                    + )
            ifte)
    ifte ))

(is_zero (code dup 0 ceq))
(is_one  (code dup 1 ceq)))

