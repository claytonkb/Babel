-- cmp.pb
-- This program demonstrates Babel's C-style comparison operators

ops: 
    (("cne" (cne)  ["equal"                "not equal"])   
    ("ceq"  (ceq)  ["not equal"            "equal"])
    ("cult" (cult) ["greater or equal"     "less than"])
    ("cule" (cule) ["greater"              "less than or equal"])
    ("cugt" (cugt) ["less than or equal"   "greater"])
    ("cuge" (cuge) ["less than"            "greater or equal"]) 
    ("cilt" (cilt) ["greater or equal"     "less than"]) 
    ("cile" (cile) ["greater"              "less than or equal"]) 
    ("cigt" (cigt) ["less than or equal"   "greater"]) 
    ("cige" (cige) ["less than"            "greater or equal"]))

x: [0]

main: (     `( `"- " << 
            `ops x ith `0 ith cr << 
            prompts !
            `ops x ith `0 ith `": " . << 
            `ops x ith `1 ith !
            <- `ops x ith `2 ith -> th cr <<
            x `1 + x set )
        `ops ## times )

prompts: (( prompt ! prompt ! ))

prompt: (( `"Enter a number:\n> " <<
        >> $d ))

