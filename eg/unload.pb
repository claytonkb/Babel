code: 
   ([[1 2 3 [1 2 3] "Hello, world"]] unload
    ["out.bbl"] spit

    ["out.bbl"] slurp8
    dup arlen [1] cusub trunc
    load
    bbl2str stdout)

lst:
    [(dup size ci2dec stdout ret)]

-- od -t x4 out.bbl

