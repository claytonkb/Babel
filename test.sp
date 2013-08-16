-- test.sp
-- perl sparse.pl test.sp src/opcodes.sp src/construct.sp

(   (main   (code (list 1 2 3) mu cu2dec cr stdout8 ))

    (symbol (list nil))

    (d (code bs2gv stdout8))

    (t2 (code (list 1 2 3) (list 4 5 6) ins d eval ))

    (t1 (code 
            (ptr (ptr 0 1) (ptr 2 3)) 
            (list 1) 
            trav
            d eval))

    (t0 
        (code 
            "Babel" str2ar ar2ls 
            reverse 
            ls2lf ar2str 
            cr stdout8)))

