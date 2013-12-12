-- cond.sp

((main (code 

    x

    (list 
    (code 0 =) (code 'a')
    (code 1 =) (code 'b')
    (code 2 =) (code 'c')
    (code 3 =) (code 'd'))
    condc !

    mkcell !

    -- appears that in order for this 'set' to work, we need the list
    -- to be nested one more level
    set

    -- die

    (code

    1 rand 4 %

--    (list 
--    (code dup 0 =) (code zap 'a')
--    (code dup 1 =) (code zap 'b')
--    (code dup 2 =) (code zap 'c')
--    (code 3 =) (code 'd'))

    x

    cond

    <<)

    3000 times

    "\n" <<))

(x (list nil))

(mkcell (code (ptr nil) car cons))

-- Too expensive:
(condd (code 
    (code (code
            (code iter odd !)
                (code (list zap) cp <-> zap)
                (code (list dup) cp <-> zap)
            ifte
            push)
        each
        -1 take)
    nest
    cond))

(condc (code 
    (code (code
            (code iter odd !)
                (code (list zap) cp <-> zap)
                (code (list dup) cp <-> zap)
            ifte
            push)
        each
        -1 take)
    nest))

(odd  (code 2 %))
(even (code odd ! not)))


