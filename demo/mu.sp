-- mu.sp
-- This program illustrates Babel's memory operators
-- Use the debugger to explore this demo program
--    % perl babel demo/array.sp
--    % perl babel demo/debug.sp demo/array.sp.bbl
--    type 'h' in the debugger for help

((main (code
        -- Unloaded size of x:                 
        y unload #
        dup

        -- Memory usage of x (words):          
        y mu 

        =  -- same as size of unload? 
        zap

        y nin       -- Number of interior arrays in x
        y nlf +     -- Number of leaf arrays in x
        y nva +     -- Number of values in x
        y npt +     -- Number of pointers in x 

        = )) -- Same as result of mu?

(y 
(ptr
    (ptr
        (ptr
            (ptr 1)
            (ptr 2))

        (ptr
            (ptr 3)
            (ptr 4)))

    (ptr
        (ptr
            (ptr 5)
            (ptr 6))

        (ptr
            (ptr 7)
            (ptr 8))))))

