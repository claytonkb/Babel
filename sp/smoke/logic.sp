-- logic.sp
-- Test vectors below

((opsA 
    ({cand}
    {cor}
    {cxor}
    {cxnor}
    {cnor}
    {cnand}))

(opsB 
    ({cnot}
    {4 cushl}
    {4 cushr}
    {4 curol}
    {4 curor}
    {4 cashr}))

(x (0x12345678 z))
(z [val 0x87654321])

(main 
    {opsA
    {<- x give -> eval}
    each

    opsB
    {<- z -> eval}
    each

    dump_stack !})

(dump_stack { 
    {depth}
        {collect ! dup
            <- give ->
            rev 
            {bbl2str nl <<} each}
        {"nil\n" <<}
    ifte})

(collect    { -1 take }))

--Test vectors:
--[val 0xf8765432 ]
--[val 0x18765432 ]
--[val 0x76543218 ]
--[val 0x8765432 ]
--[val 0x76543210 ]
--[val 0x789abcde ]
--[val 0xfddbbddf ]
--[val 0x7abefefe ]
--[val 0x6aaeeaa6 ]
--[val 0x95511559 ]
--[val 0x97755779 ]
--[val 0x2244220 ]

