code:
   (
    [{0 0 0 0}] ["nil"] hash8
    disp call
    ["Progressive hashing:\n"] stdout
    [{0 0 0 0}] ["G"] hash8
    disp call
    [{0 0 0 0}] ["GG"] hash8
    disp call
    ["\n"] stdout
    [{0 0 0 0}] ["G"] hash8
    dup disp call
    ["G"] hash8
    dup disp call)

disp: [( bbl2str stdout ["\n"] stdout)]

