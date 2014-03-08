-- libdev.sp

((argv1 (code 'argv' lusym 1 th))

(argvn (code <- 'argv' lusym -> th))

(dump_stack (code 
        (code depth)
        (code depth take rev (code bbl2str nl <<) each)
        (code "nil\n" <<)
    ifte))

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

-- pays
-- Extracts all payloads from a hash table
(pays (code (code 
        entsha 
        (code 2 ith) 
        each 
        collect !)
    nest))

-- keys
-- Extracts all string keys from a hash table
(keys (code (code 
        entsha 
        (code 1 ith) 
        each 
        collect !)
    nest))

-- hkeys
-- Extracts all hashed keys from a hash table
(hkeys (code (code 
        entsha 
        (code 0 ith) 
        each 
        collect !)
    nest))

-- mkhash
-- Constructs a hash from a list-of-lists:
--
--      (list   (list "key0" payload0)
--              (list "key1" payload1)
--              (list "key2" payload2))
--
(mkhash (code 
    <- newha ->
    (code 
        <- dup ->
        dup 1 ith
        <- 0 ith ->
        inskha)
    each))

-- mkhash_hkey
-- Same as mkhash but discards the string key
(mkhash_hkey (code 
    <- newha ->
    (code 
        <- dup ->
        dup 1 ith
        <- 0 ith hash8 ->
        insha)
    each))

(ins_ith (code 
    <->
    <- <- (code cdr) -> 
    1 - 
    times ->
    ins)))

