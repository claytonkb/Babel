-- logic.pb
-- This program demonstrates Babel's C-style logic operators

op_listA:  (("cand" (cand))       ("cor" (cor))         ("cxor" (cxor)) 
            ("cxnor" (cxnor))     ("cnor" (cnor))       ("cnand" (cnand)))

op_listB:  (("cnot" (cnot))       ("cushl" ([4] cushl)) ("cushr" ([4] cushr)) 
            ("curol" ([4] curol)) ("curor" ([4] curor)) ("cashr" ([4] cashr)))

code: ([(dup car stdout [":\n"] stdout
            cdr car [0x12345678] swap [0x87654321] swap eval 
            bbl2str stdout ["\n"] stdout)]
        [op_listA] 
        each
      [(dup car stdout [":\n"] stdout
            cdr car [0x87654321] swap eval 
            bbl2str stdout ["\n"] stdout)]
        [op_listB] 
        each)

