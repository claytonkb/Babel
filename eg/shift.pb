-- shift.pb
-- This program illustrates Babel's C-style logical 
-- shift operators

val: 0xbabeface

op_name: ["cushl" "cushr" "cashr" "curol" "curor"]

operator: [[cushl nil] [cushr nil] [cashr nil] [curol nil] [curor nil]]

x: [0]

main:
   (`"Shifting/rotating hex value 0x" << [val] cu2hex << `" by 4 bits:\n" <<
    ---
      ((`op_name `x car th << `": " <<
        `val `4 `operator `x car th !
        disp !
        x `1 + x set))
    `5
    times)
    ---

disp: (( $ cr << ))

