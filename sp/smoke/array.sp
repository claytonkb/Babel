-- array.sp
-- See below for test vectors

((my_leaf       [val 0 1 2 3 4 5 6 7 8 9])

(my_interior    [ptr 0 1 2 3 4 5 6 7 8 9])

(main {

    -- Converting an array to a list
    my_interior
    ar2ls

    -- Array length
    my_interior 
    arlen

    -- array-8 length
    "Hello, world" arlen8

    -- Cutting/catting an array
    my_leaf 
    3 cut cat 

    -- leaf-array s-field, isleaf, etc.
    my_leaf dup dup
    sfield 
    isleaf 
    isinte 

    -- interior-array s-field, isleaf, etc.
    my_interior dup dup
    sfield 
    isleaf 
    isinte 

    -- Allocating a new leaf-array
    7 newlf 

    -- Allocating a new interior-array
    7 newin 
    

    -- Comparing two leaf-arrays
    a b == 
    b a ==  
    a a ==  

    dump_stack !})

(dump_stack { 
        { depth }
        { depth take rev { bbl2str nl << } each }
        { "nil\n" << }
    ifte})

(a [val 1 3 5])
(b [val 2 4 6]))

-- Test vectors:
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0xffffffff ]
-- [ptr  nil  nil  nil  nil  nil  nil  nil ]
-- [val 0x0 0x0 0x0 0x0 0x0 0x0 0x0 ]
-- [val 0x0 ]
-- [ptr [val 0x0 ] [val 0x1 ] [val 0x2 ] [val 0x3 ] [val 0x4 ] [val 0x5 ] [val 0x6 ] [val 0x7 ] [val 0x8 ] [val 0x9 ] ]
-- [ptr [val 0x0 ] [val 0x1 ] [val 0x2 ] [val 0x3 ] [val 0x4 ] [val 0x5 ] [val 0x6 ] [val 0x7 ] [val 0x8 ] [val 0x9 ] ]
-- [val 0x0 ]
-- [val 0x0 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 ]
-- [val 0x0 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 ]
-- [val 0x0 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 ]
-- [val 0xa ]
-- [ptr [val 0x0 ] [ptr [val 0x1 ] [ptr [val 0x2 ] [ptr [val 0x3 ] [ptr [val 0x4 ] [ptr [val 0x5 ] [ptr [val 0x6 ] [ptr [val
-- 0x7 ] [ptr [val 0x8 ] [ptr [val 0x9 ]  nil ] ] ] ] ] ] ] ] ] ]

