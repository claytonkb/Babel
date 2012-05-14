-- logic.pb
-- This program demonstrates Babel's C-style logic operators

op_listA:  ((("cand "  (cand))     ("cor  "  (cor))      ("cxor "  (cxor)) 
             ("cxnor" (cxnor))     ("cnor " (cnor))      ("cnand" (cnand))))

op_listB:  ((("cnot "  (cnot))     ("cushl" ((4) cushl)) ("cushr" ((4) cushr)) 
             ("curol" ((4) curol)) ("curor" ((4) curor)) ("cashr" ((4) cashr))))

main: ( op_listA disp_loop !
        op_listB disp_loop ! 

        `"zero: " <<
        `0 `"is false\n" `"is true\n" ? <<

        `"non-zero: " <<
        `0 not `"is false\n" `"is true\n" ? <<

        `"nil: " <<
        `nil `"is false\n" `"is true\n" ? <<

        `"false and true: " <<
        `0 `0 ~ and `"is false\n" `"is true\n" ? <<

        `"false or true: " <<
        `0 `0 ~ || `"is false\n" `"is true\n" ? << )

disp_loop: (((( disp_op !
                <- (0x12345678) (0x87654321) ->
                cdr car 
            !
            disp ! ))
        <->
        ...))

disp: (($ cr <<))

disp_op: (( dup car (": ") . << ))

