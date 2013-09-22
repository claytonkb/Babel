-- libdev.sp

((argv1 (code 'argv' lusym 1 th))

(argvn (code <- 'argv' lusym -> th))

(display (code 
    show 'display.dot' spit8
    cpf86 'Graphviz2.32\bin\dot.exe -Tsvg display.dot -o display.svg' 
        cat8 exec
    3 sleep
    cpf86 'Google\Chrome\Application\chrome.exe file:///C:/babel/2013Aug11/Babel/display.svg' 
        cat8 exec))

(cpf86 (val 'C:\Program Files (x86)\'))

(d (code show stdout8))

(collect (code -1 take))

(vals (code (code 
        keysha 
        (code 2 ith) 
        each 
        collect !)
    nest))

(keys (code (code 
        keysha 
        (code 1 ith) 
        each 
        collect !)
    nest)))

