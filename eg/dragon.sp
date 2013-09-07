-- dragon.sp
-- Strange Loop Conference 2013 ELC

((main (code dragon1 eval ))

-- 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 
--  0   1   0   1   0   1   0   1   0   1   0   
--    0       1       0       1       0       
--        0               1               0
--                0               1   
-- 
-- 0010011000110110001001110011011100100110001 ...

(dragon1 (code 
     argv1 ! $d
    '0' osc !
    "\n" << ))

(osc (code
    <- 1 cusub dup 
    (code 0 >)
        (code 
            dup
            '0' osc !
            -> <<
            '1' osc !)
        (code -> << zap)
    ifte))


(argv1 (code 'argv' lusym 1 th))

(d (code bs2gv stdout8))

(display (code bs2gv 'test.dot' spit8
    cpf86 'Graphviz2.32\bin\dot.exe -Tsvg test.dot -o test.svg' cat8 exec
    cpf86 'Google\Chrome\Application\chrome.exe file:///C:/babel/2013Aug11/Babel/test.svg' cat8 exec))

(cpf86 (val 'C:\Program Files (x86)\'))

(symbol (list nil)))

