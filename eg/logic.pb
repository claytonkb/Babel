-- logic.pb
-- This program demonstrates Babel's C-style logic operators

op_name:  [ "cnot" "cand" "cor" "cnand"
            "cnor" "cxor" "cxnor"]
operator: [ [cnot nil] [cand nil] [cor nil] [cnand nil]
            [cnor nil] [cxor nil] [cxnor nil] ]

code:
    ---
     ((([op_name] x cxr stdout [":\n"] stdout

        [0xb33d1d84] dup [disp] call
        [0xffbc5a53] dup [disp] call

        [operator] x cxr eval 
        bbl2str stdout [" <- result\n\n"] stdout

        x [1] cuadd
        [x] [0] save))
    [[7]]
    times)
    ---

x: [0]

disp:
   (bbl2str
    stdout
    ["\n"]
    stdout
    ret)

