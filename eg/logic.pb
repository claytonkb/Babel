-- logic.pb
-- This program demonstrates Babel's C-style logic operators

op_listA:  ((("cand "  (cand))     ("cor  "  (cor))      ("cxor "  (cxor)) 
             ("cxnor" (cxnor))     ("cnor " (cnor))      ("cnand" (cnand))))

op_listB:  ((("cnot "  (cnot))     ("cushl" ((4) cushl)) ("cushr" ((4) cushr)) 
             ("curol" ((4) curol)) ("curor" ((4) curor)) ("cashr" ((4) cashr))))

main: ( op_listA disp_loop eval
        op_listB disp_loop eval )

disp_loop: (((( disp_op eval
                down (0x12345678) (0x87654321) up
                cdr car 
            eval
            show_cr eval ))
        swap
        each))

show_cr: ((show ("\n") cat8 pr))

disp_op: ((dup car (": ") cat8 pr))

