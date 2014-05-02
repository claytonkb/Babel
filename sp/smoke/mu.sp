-- mu.sp
-- Test vectors below

((main {
        -- Unloaded size of x:                 
        y unload #

        -- Memory usage of x (words):          
        y mu 

        y nin               -- Number of interior arrays in x
        y nlf  +            -- Number of leaf arrays in x
        y nva  +            -- Number of values in x
        y npt  +            -- Number of pointers in x  
        y ntag 5 * +        -- Number of tagged-pointers in x  

    dump_stack !})

(y 
[tag "foo" [ptr
    [ptr
        [ptr
            [ptr 1]
            [ptr 2]]

        [ptr
            [ptr 3]
            [ptr 4]]]

    [ptr
        [ptr
            [ptr 5]
            [ptr 6]]

        [ptr
            [ptr 7]
            [ptr 8]]]]])

(dump_stack { 
    {depth}
        {collect ! dup
            <- give ->
            rev 
            {bbl2str nl <<} each}
        {"nil\n" <<}
    ifte})

(collect    { -1 take }))

-- Test Vectors:
-- [val 0x3c ]
-- [val 0x3c ]
-- [val 0x3c ]

