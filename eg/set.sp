-- demo.sp
-- Strange Loop Conference 2013 ELC

(   (main   
    (code 10 cu2dec cr stdout8))

    (symbol (list nil))

    (fib (code 
        (code is_zero eval)
            (code zap 1)
            (code 
                (code is_one eval)
                    (code fnord)
                    (code cp down 2 cusub fib eval up 1 cusub fib eval cuadd )
                ifte)
        ifte ))

    (is_zero (code dup 0 ceq))
    (is_one  (code dup 1 ceq))

    (d (code bs2gv stdout8))

    (display (code bs2gv 'test.dot' spit8
        cpf86 'Graphviz2.32\bin\dot.exe -Tsvg test.dot -o test.svg' cat8 exec
        cpf86 'Google\Chrome\Application\chrome.exe file:///C:/babel/2013Aug11/Babel/test.svg' cat8 exec))

    (cpf86 (val 'C:\Program Files (x86)\'))

    (t3 (code "Hello" cr stdout8))

    (t2 (code (list 1 2 3) (list 4 5 6) ins d eval ))

    (t1 (code 
            (ptr (ptr 0 1) (ptr 2 3)) 
            (list 1) 
            --trav
            d eval))

    (t0 
        (code 
            "Babel" str2ar ar2ls 
            rev 
            ls2lf ar2str 
            cr stdout8)))

