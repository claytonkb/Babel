-- test.sp
-- perl sparse.pl test.sp src/opcodes.sp src/construct.sp

-- (code "Hello world\n" stdout8) loop)

(   (main   (code (code "Hello" cr stdout8) eval))

    (symbol (list nil))

    (d (code bs2gv stdout8))

    (t2 (code (list 1 2 3) (list 4 5 6) ins d eval ))

    (t1 (code 
            (ptr (ptr 0 1) (ptr 2 3)) 
            (list 1) 
            --trav
            d eval))

    (t0 
        (code 
            "Babel" str2ar ar2ls 
            rev 
            ls2lf ar2str 
            cr stdout8)))

