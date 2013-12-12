-- load.sp
-- Strange Loop Conference 2013 ELC

((main (code 

    "demo/unload.bbl" slurp
    load

    lookup ! ))

(prompt (code 
        dup
        "Enter a key> " <<
        >> hash8 ))

(lookup (code (code
        prompt !
        luha
        cr <<)
    loop))

(remove (code (code
        prompt !
        (code rmha)
            (code "Removed!")
            (code "Nothing removed...")
        ifte
        cr <<)
    loop))

(exists (code (code 
        prompt !
        (code exha)
            (code "Exists!")
            (code "Does not exist")
        ifte
        cr <<)
    loop))

(mkhash (code 
    <- newha ->
    (code 
        <- dup ->
        dup 1 ith
        <- 0 ith hash8 ->
        insha)
    each)))


