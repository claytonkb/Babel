-- nest.sp
-- This program illustrates Babel's nest operator
-- Use the debugger to explore this demo program
--    % perl babel demo/nest.sp
--    % perl babel demo/debug.sp demo/nest.sp.bbl
--    type 'h' in the debugger for help

((main (code 
    "The sum is: "
    (list 1 2 3 4 5) 
    sum ! 
    <-> <<
    %d nl <<))

(sum (code 
    (code 
        give
        depth 1 -
        <- (code +) ->
        times) 
    nest)))

