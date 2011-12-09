-- shift.pb
-- This program illustrates Babel's C-style logical 
-- shift operators

val: 0xbabeface

op_name: ["cushl" "cushr" "cashr" "curol" "curor"]

operator: [[cushl nil] [cushr nil] [cashr nil] [curol nil] [curor nil]]

x: [0]

code:
   (["Shifting/rotating hex value 0x"] stdout [val] cu2hex stdout [" by 4 bits:\n"] stdout
    ---
      (([op_name] x cxr stdout [": "] stdout
        [val] [4] [operator] x cxr eval
        [disp] call

        x  [1] cuadd
        [x] [0] save))
    [[5]]
    times)
    ---

disp: (bbl2str stdout [{"\n"}] stdout ret)

