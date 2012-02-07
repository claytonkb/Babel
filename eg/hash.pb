code:
   (["Progressive hashing:\n"] stdout
    [{0 0 0 0}] ["G"] hash8 ["G  -> "] stdout
    disp call
    [{0 0 0 0}] ["GG"] hash8 ["GG -> "] stdout
    disp call
    ["\n"] stdout
    [{0 0 0 0}] ["G"] hash8 ["G"] hash8 ["G -> G -> "] stdout
    disp call

    [nil]
    [(dup [{0 0 0 0}] swap hash8 swap insha)]
    [("foo" "bar" "bap" "baz")]
    each

    ["\nHash lookup:\n"] stdout
    [([{0 0 0 0}] swap hash8 luha stdout ["\n"] stdout)]
    [("foo" "bar" "bap" "baz")]
    each

    ["\nHash existence:\n"] stdout
    [([{0 0 0 0}] swap hash8 exha bbl2str stdout ["\n"] stdout)]
    [("foo" "bar" "bap" "baz" "bop")]
    each

    ["\nHash removal and keys:\n"] stdout
    [([{0 0 0 0}] swap hash8 rmha)]
    [("foo" "bar")]
    each

    keysha bbl2str stdout)

--    bbl2gv ["hash.dot"] spit8)

x: [x x]

disp: [( bbl2str stdout ["\n"] stdout )]

