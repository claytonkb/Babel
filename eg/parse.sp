-- parse.sp
-- Strange Loop Conference 2013 ELC

((main (code 
    argv1 ! >>>
    str2ar ar2ls
    parser ! ))

(argv1 (code 'argv' lusym 1 th))

(parser (code (code
    ar2str 

--    (code "-" arcmp not)
--        (code "match!\n" stdout8)
--        (code "no match:(\n" stdout8)
--    ifte)

    (list
        (code dup "(" arcmp not ) (code zap "OPEN_PAREN"  cr stdout8)
        (code dup ")" arcmp not ) (code zap "CLOSE_PAREN"  cr stdout8)
        (code     "-" arcmp not ) (code     "DASH" cr stdout8))
    cond)

    each dev))

(streq (code arcmp not))

(newline_split (code 
    dup
    0xa char_match eval 
    split ))

(char_match (code 
    match_list 2 newin 0 paste
    compare_char swap set
    (code 
        (code compare_char ceq) 
            (code 
                match_list iter 
                    -- dup cu2dec cr stdout8 
                1 take unshift zap) 
            (code fnord) 
        ifte)
    each
    match_list cdr))

(compare_char (val 0))

(match_list (ptr nil nil))

(tons (code take bons))

(display (code bs2gv 'display.dot' spit8
    cpf86 'Graphviz2.32\bin\dot.exe -Tsvg display.dot -o display.svg' cat8 exec
    cpf86 'Google\Chrome\Application\chrome.exe file:///C:/babel/2013Aug11/Babel/display.svg' cat8 exec))

(cpf86 (val 'C:\Program Files (x86)\'))

(d (code bs2gv stdout8))

(symbol (list nil)))

