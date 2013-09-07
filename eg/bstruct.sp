-- bstruct.sp
-- Strange Loop Conference 2013 ELC

(   (main (code 
        'argv' lusym 1 th
        slurp load 
        display eval))

    (d (code bs2gv stdout8))

    (symbol (list nil))

    (display (code bs2gv 'display.dot' spit8
        cpf86 'Graphviz2.32\bin\dot.exe -Tsvg display.dot -o display.svg' cat8 exec
        cpf86 'Google\Chrome\Application\chrome.exe file:///C:/babel/2013Aug11/Babel/display.svg' cat8 exec))

    (cpf86 (val 'C:\Program Files (x86)\')))

