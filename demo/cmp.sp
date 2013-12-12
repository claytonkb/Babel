-- cmp.sp
-- This program illustrates Babel's arithmetic comparison 
-- operators
-- Use the debugger to explore this demo program
--    % perl babel demo/cmp.sp
--    % perl babel demo/debug.sp demo/cmp.sp.bbl
--    type 'h' in the debugger for help

((numbers (list 
    (list 4 5)
    (list 5 5)
    (list 5 4)))

(ops (list 
    (code ~= )
    (code =  )
    (code u< )
    (code u<=)
    (code u> )
    (code u>=)))

(signed_numbers (list 
    (list -4 -5)
    (list -5 -5)
    (list -5 -4)))

(signed_ops (list
    (code ~= )
    (code =  )
    (code <  )
    (code <= )
    (code >  )
    (code >= )))

(main (code

    ops
    (code 
        <- 
        numbers
        (code 
            give
            -> dup <-
            !)
        each
        ->
        clear)
    each

    signed_ops
    (code 
        <- 
        signed_numbers
        (code 
            give
            -> dup <-
            !)
        each
        ->
        clear)
    each)))

