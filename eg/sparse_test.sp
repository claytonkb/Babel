-- Smoke-test for the sparse.pl script

((main (code a bs2gv stdout8))

(a (list a b c d e f g h i j k l $))

(b (val 0 1 2 0x3))

(c (val "\\\"foo\n"))

(d (val 'bar'))

(e (ptr a c e 1 0x2 3))

(f (code "bazinga" 42 fnord fake))

(g (list (list (list '.') (list '.')) (list (list '.') (list '.'))))

(h (ptr (ptr (ptr '.') (ptr '.')) (ptr (ptr '.') (ptr '.'))))

(i (tag 'bebop'  (val 0)))

(j (tag 'doowop' (val '')))

(k (ref nil i j))

(l (hash "foo"))

(fake (oper 0xdecade))

($ (ptr 1 2 3))

(display (code 
    bs2gv 'display.dot' spit8
    cpf86 'Graphviz2.32\bin\dot.exe -Tsvg display.dot -o display.svg' 
        cat8 exec
    cpf86 'Google\Chrome\Application\chrome.exe file:///C:/babel/2013Aug11/Babel/display.svg' 
        cat8 exec))

(cpf86 (val 'C:\Program Files (x86)\'))

(d (code bs2gv stdout8))

(symbol (list nil)))

