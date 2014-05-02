-- list.sp

((main {
    ('a' 'b' 'c' 'd' 'e' 'f')
    3 ith
    
    (1 2 3)
    dup
    <- car ->
    cdr

    1 (2 3)
    cons uncons

    [ptr 1 2 3]
    ar2ls bons

    (1 2 3)
    ls2lf ar2ls

    (2 3)
    (1)
    push pop

    (1 2)
    (3)
    unshift shift

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

