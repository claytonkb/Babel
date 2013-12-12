-- logic.sp
-- This program demonstrates Babel's C-style logic operators
-- Use the debugger to explore this demo program
--    % perl babel demo/logic.sp
--    % perl babel demo/debug.sp demo/logic.sp.bbl
--    type 'h' in the debugger for help

((opsA (list
    (code cand)
    (code cor)
    (code cxor)
    (code cxnor)
    (code cnor)
    (code cnand)))

(opsB (list 
    (code cnot)
    (code 4 cushl)
    (code 4 cushr)
    (code 4 curol)
    (code 4 curor)
    (code 4 cashr)))

(x (list 0x12345678 z))
(z (val 0x87654321))

(main (code 
    opsA
    (code 
        <- x give -> 
        !
        zap)
    each

    opsB
    (code 
        <- z -> 
        !
        zap)
    each)))

