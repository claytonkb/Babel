-- array.sp
-- This program illustrates Babel's array operators
-- Use the debugger to explore this demo program
--    perl babel demo/array.sp
--    perl babel demo/debug.sp demo/array.sp.bbl
--    type 'h' in the debugger for help

((my_leaf       (val 0 1 2 3 4 5 6 7 8 9))

(my_interior    (ptr 0 1 2 3 4 5 6 7 8 9))

(main (code

    -- Converting an array to a list
    my_interior
    ar2ls zap

    -- Array length
    my_interior 
    arlen zap

    -- array-8 length
    "Hello, world" arlen8 zap

    -- Cutting/catting an array
    my_leaf 
    3 cut cat 
    zap

    -- leaf-array s-field, isleaf, etc.
    my_leaf dup dup
    sfield zap
    isleaf zap
    isinte zap

    -- interior-array s-field, isleaf, etc.
    my_interior dup dup
    sfield zap
    isleaf zap
    isinte zap

    -- Allocating a new leaf-array
    7 newlf 
    zap

    -- Allocating a new interior-array
    7 newin 
    zap

    -- Comparing two leaf-arrays
    a b == zap
    b a == zap 
    a a == zap ))

(a (val 1 3 5))
(b (val 2 4 6))


