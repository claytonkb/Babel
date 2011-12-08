-- carith.pb
-- This program demonstrates Babel's C-style arithmetic operators

op_name:
   ["cuadd" "cusub" "cumul" "cudiv" 
    "ciadd" "cisub" "cimul" "cidiv"]

op:
   [[cuadd nil] [cusub nil] [cumul nil] [cudiv nil]
    [ciadd nil] [cisub nil] [cimul nil] [cidiv nil]]

code:
    ---
     ((([inform]  call ["\n"] stdout
        [prompts] call

        [op] x cxr eval

        [inform] call
        [": "] stdout

        ci2dec stdout
        ["\n"] stdout

        x  [1] cuadd
        [x] [0] save))
    [[8]] times)
    ---

inform:
   ([op_name]
    x
    cxr
    stdout
    ret)

prompts:
   ([prompt] call
    [prompt] call
    ret)

prompt:
   (["Enter a number:\n> "] stdout
    stdinln dec2ci
    ret)

x: [0]

