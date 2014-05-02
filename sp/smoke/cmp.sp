-- cmp.sp
-- See below for test vectors

((numbers        (( 4  5) ( 5  5) ( 5  4)))
 (signed_numbers ((-4 -5) (-5 -5) (-5 -4)))

(ops   ({~=  } {=   }
        {cult} {cule}
        {cugt} {cuge}))

(signed_ops 
       ({~=  } {=   } 
        {cilt} {cile}
        {cigt} {cige}))

(dump_stack {
    {depth}
        {collect ! dup
            <- give ->
            rev 
            {bbl2str nl <<} each}
        {"nil\n" <<}
    ifte})

(collect    { -1 take })

(main 
    {ops

    {<- numbers
        {give -> dup <- !}
        each
        -> zap}
    each

    signed_ops
    {<- signed_numbers
        {give -> dup <- !}
        each
        -> zap}
    each

    dump_stack !}))

-- Test vectors:
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x1 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x0 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x1 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x0 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x1 ]
-- [val 0x0 ]
-- [val 0x1 ]
 
