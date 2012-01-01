code: 
   ([[1 2 3 [1 2 3]]] unload
    ["out.bbl"] spit

    ["out.bbl"] slurp8
    dup arlen [1] cusub trunc
    load
    bbl2str stdout)

-- od -t x4 out.bbl

