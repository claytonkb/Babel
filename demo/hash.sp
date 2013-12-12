-- hash.sp
-- Strange Loop Conference 2013 ELC

((main (code 

    (list 
        (list "cattle"  "herd"      ) 
        (list "eagles"  "aerie"     ) 
        (list "fish"    "school"    ) 
        (list "geese"   "gaggle"    ) 
        (list "gulls"   "screech"   ) 
        (list "trout"   "hover"     ))
    mkhash !

    lookup ! ))

(prompt (code 
        dup
        "Enter a key> " <<
        >> hash8 ))

(lookup (code (code
        prompt !
        luha
        nl <<)
    loop))

(remove (code (code
        prompt !
        (code rmha)
            (code "Removed!")
            (code "Nothing removed...")
        ifte
        nl <<)
    loop))

(exists (code (code 
        prompt !
        (code exha)
            (code "Exists!")
            (code "Does not exist")
        ifte
        nl <<)
    loop))

(mkhash (code 
    <- newha ->
    (code 
        <- dup ->
        dup 1 ith
        <- 0 ith hash8 ->
        insha)
    each)))


