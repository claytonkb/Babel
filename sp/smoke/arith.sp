-- arith.sp
-- This program illustrates Babel's arithmetic operators
-- Use the debugger to explore this demo program
--    % perl babel demo/array.sp
--    % perl babel demo/debug.sp demo/array.sp.bbl
--    type 'h' in the debugger for help

((numbers    (2 3 5 7 11 13 17 19 23))

(main { 

    { numbers
        {   1  +
            1  -
            10 *
            10 /

            500 %}
        each}}
    4 times

    collect !}))


