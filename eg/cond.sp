-- cond.sp

((main (code (code z eval) 3 times))

(y (code 
    2
    (list 
        (code dup 1 ceq)(code zap "One!"   cr stdout8)
        (code dup 2 ceq)(code zap "Two!"   cr stdout8)
        (code     3 ceq)(code     "Three!" cr stdout8))
    cond))

(z (code (list 
        (code 1)(code "One!"   cr stdout8)
        (code 0)(code "Two!"   cr stdout8)
        (code 0)(code "Three!" cr stdout8))
    cond "Done\n" stdout8))

(transpose (code 
    len cu2dec stdout8))

(lscat (code 2 take append))

(conds (code 
    (code ) 
    each))

(display (code 
    bs2gv 'display.dot' spit8
    cpf86 'Graphviz2.32\bin\dot.exe -Tsvg display.dot -o display.svg' 
        cat8 exec
    cpf86 'Google\Chrome\Application\chrome.exe file:///C:/babel/2013Aug11/Babel/display.svg' 
        cat8 exec))

(cpf86 (val 'C:\Program Files (x86)\'))

(d (code bs2gv stdout8))

(symbol (list nil)))

