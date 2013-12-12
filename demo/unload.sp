-- unload.sp
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

    unload 

    dup
    "demo/unload.bbl" spit

    display !))

(mkhash (code 
    <- newha ->
    (code 
        <- dup ->
        dup 1 ith
        <- 0 ith hash8 ->
        insha)
    each)))


