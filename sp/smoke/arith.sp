-- arith.sp
-- This program illustrates Babel's arithmetic operators
-- Use the debugger to explore this demo program
--    % perl babel demo/array.sp
--    % perl babel demo/debug.sp demo/array.sp.bbl
--    type 'h' in the debugger for help

((numbers    (list 2 3 5 7 11 13 17 19 23))

(main (code 

    (code 
    numbers
    (code
        1   u+
        1   u-
        10  u*
        10  u/
        500 u%
         
        -1  +
        -1  -
        -10 *
        -10 /

        500 %)
    each)
    4 times

    -- dev

    -- "Done!\n" <<

    collect !)))

--    bbl2str << )))


