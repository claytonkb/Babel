-- dragon.sp
-- Strange Loop Conference 2013 ELC

-- 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 
--  0   1   0   1   0   1   0   1   0   1   0   
--    0       1       0       1       0       
--        0               1               0
--                0               1   
-- 
-- 0010011000110110001001110011011100100110001 ...

((main (code 
    '0010011000110110001001110011011100100110001...' nl <<
    argv1 ! $d 
    '0' dragon !
    "\n" << ))

(dragon (code
    <- 1 - dup 
    (code 0 >)
        (code 
            dup
            '0' dragon !
            -> <<
            '1' dragon !)
        (code -> << zap)
    ifte)))

