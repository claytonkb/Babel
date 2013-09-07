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

    remove ! ))

(lookup (code (code
        dup
        "Enter a key> " <<
        >> hash8
        luha
        cr <<)
    loop))

(remove (code (code
        dup
        "Enter a key> " <<
        >> hash8
        (code rmha)
            (code "Removed!")
            (code "Nothing removed...")
        ifte
        cr <<)
    loop))

(exists (code (code 
        dup
        "Enter a key> " <<
        >> hash8
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


