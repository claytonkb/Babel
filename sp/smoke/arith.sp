-- arith.sp
-- See below for test vectors

((numbers    (2 3 5 7 11 13 17 19 23))

(main 
    { numbers 
    { 1     u+
        1   u-
        10  u*
        10  u/
         
        -1  +
        -1  -
        -10 *
        -10 /

        500 %}
    each

    dump_stack ! })

(dump_stack { 
        { depth }
        { depth take rev { bbl2str nl << } each }
        { "nil\n" << }
    ifte}))

-- Test vectors:
-- [val 0x17 ]
-- [val 0x13 ]
-- [val 0x11 ]
-- [val 0xd ]
-- [val 0xb ]
-- [val 0x7 ]
-- [val 0x5 ]
-- [val 0x3 ]
-- [val 0x2 ]

