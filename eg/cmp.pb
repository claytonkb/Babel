-- cmp.pb
-- This program demonstrates Babel's C-style comparison operators

op_name:  ["cne"  "ceq"  "cult" "cule" "cugt" 
           "cuge" "cilt" "cile" "cigt" "cige"]

operator: [[cne nil]  [ceq nil]  [cult nil] [cule nil] [cugt nil] 
           [cuge nil] [cilt nil] [cile nil] [cigt nil] [cige nil]]

result:
   [["equal"                "not equal"]
    ["not equal"            "equal"]
    ["greater or equal"     "less than"]
    ["greater"              "less than or equal"]
    ["less than or equal"   "greater"]
    ["less than"            "greater or equal"]
    ["greater or equal"     "less than"]
    ["greater"              "less than or equal"]
    ["less than or equal"   "greater"]
    ["less than"            "greater or equal"]]

x: [0]

code:
     ((([op_name] x cxr stdout ["\n"] stdout
        [prompts] call
        [op_name] x cxr stdout [": "] stdout
        [operator] x cxr eval
        [disp] call
        x [1] cuadd
        [x] [0] save))
    [[10]]
    times)

prompts:
   ([prompt] call
    [prompt] call
    ret)

prompt:
   ([{"Enter a number:\n> "}] stdout
    stdinln dec2ci
    ret)

disp: ([result] x cxr swap cxr stdout ["\n\n"] stdout ret)

