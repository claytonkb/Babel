-- rt.pb
-- Babel run-time BVM
-- This is the "root" virtual machine, it is here to act as a handler for
-- unhandled exceptions and as a debug console. This is just a stub for
-- now

main:
    ( argv `0 th
    slurp load

    dup dump `"boot.dot" <<<

    babel )

----    dup `x set
--
--        `( --`"step\n" <<
--            dup dup
--
--            `"next op: " <<
--            dup `0 th car dup lf? -- $ cr <<
--                `( zap `"push\n" << )
--                `( `opcode_map <-> th cr << )
--            if
--
----            `8 th
--                `"> " <<
--                >> $d
----            `1 <-> set
--            set_steps !
--            babel )
--        `( (1) ) -- dup `0 th )
--    while )

x: [ nil ]

set_steps: (( <-> `8 th set ))

--    empty? !
--        `( unload listobj ! )
--        `( nil )
--    if )
--
--empty?: (( stack nil? ))
--
--listobj: (( `( hex08 ! cr << ) eachar ))
--
--hex08: 
--        (( %x dup #8 
--        `8 <-> -
--        <-
--            `"00000000" dup <- zap -> 
--        ->
--        paste8 ))

--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--
--
-- BABEL OPCODE MAP
--
--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--

opcode_map:
[

------------------------------------------------------------------------------
--
-- PAGE 0
--
------------------------------------------------------------------------------

-- 0x00x
-- 0             1             2             3             4             5             6             7
   "rsvd"        "cushl"       "rsvd"        "cushr"       "rsvd"        "curol"       "rsvd"        "curor" 
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "cashr"       "cnot"        "rsvd"        "rsvd"        "rsvd"

-- 0x01x
-- 0             1             2             3             4             5             6             7
   "cne"         "ceq"         "cult"        "cule"        "cugt"        "cuge"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "cilt"        "cile"        "cigt"        "cige"        "rsvd"        "rsvd"        

-- 0x02x
-- 0             1             2             3             4             5             6             7
   "F0"          "F1"          "cand"        "F3"          "F4"          "F5"          "cxor"        "cor"         
-- 8             9             a             b             c             d             e             f
   "cnor"        "cxnor"       "Fa"          "Fb"          "Fc"          "Fd"          "cnand"       "Ff"

-- 0x03x
-- 0             1             2             3             4             5             6             7
   "cuadd"       "cusub"       "cumul"       "cudiv"       "rsvd"        "curem"       "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "ciadd"       "cisub"       "cimul"       "cidiv"       "ciabs"       "cirem"       "rsvd"        "rsvd"        

-- 0x04x
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x05x
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x06x
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x07x
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x08x
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x09x
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x0ax
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x0bx
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x0cx
   "cprintf"     "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "cprints"     "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x0dx
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x0ex
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x0fx
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

------------------------------------------------------------------------------
--
-- PAGE 1
--
------------------------------------------------------------------------------

------------------------------------------------------------------------------
--
-- BVM
--
------------------------------------------------------------------------------

-- 0x10x
-- 0             1             2             3             4             5             6             7
   "fnord"       "rsvd"        "babel"       "rsvd"        "rsvd"        "rsvd"        "rsvd"        "msize"        
-- 8             9             a             b             c             d             e             f
   "len8"        "free"        "rmha"        "size"        "cat8"        "rsvd"        "rsvd"       "cat"

-- 0x11x
-- 0             1             2             3             4             5             6             7
   "mu"          "nva"         "npt"         "nlf"         "nin"         "load"        "unload"      "dump"    
-- 8             9             a             b             c             d             e             f
   "slice8"      "rsvd"        "rsvd"        "slice"       "cmp8"        "rsvd"        "rsvd"        "cmp"       

-- 0x12x
-- 0             1             2             3             4             5             6             7
   "sleep"       "die"         "goto"        "rsvd"        "rsvd"        "loop"        "last"        "next"    
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "cut"         "trunc"       "th"          "rsvd"        "rsvd"    

-- 0x13x
-- 0             1             2             3             4             5             6             7
   "eval"        "rsvd"        "rsvd"        "rsvd"       "rsvd"         "while"       "times"       "rsvd"        
-- 8             9             a             b             c             d             e             f
   "in?"         "rsvd"        "newin"       "ar2ls"       "lf?"         "del"         "newlf"       "s"    

-- 0x14x
-- 0             1             2             3             4             5             6             7
   "%%"          "rand"        "srand"       "argv"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "cp"          "paste"       "rsvd"        "archr"       "rsvd"        "rsvd"        "rsvd"        "rsvd"    

-- 0x15x
-- 0             1             2             3             4             5             6             7
   "hash"        "luha"        "rsvd"        "rsvd"        "exha"        "rsvd"        "rsvd"        "rsvd"    
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
 
-- 0x16x
-- 0             1             2             3             4             5             6             7
   "rsvd"        "rsvd"        "inskha"      "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x17x
-- 0             1             2             3             4             5             6             7
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        


-- 0x18x
-- 0             1             2             3             4             5             6             7
   "ar2str"      "str2ar"      "rsvd"        "rsvd"        "%u"          "%x"          "%d"          "rsvd"        
-- 8             9             a             b             c             d             e             f
   "##"          "take"        "give"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x19x
-- 0             1             2             3             4             5             6             7
   "$"           "rsvd"        "rsvd"        "rsvd"        "rsvd"        "$x"          "$d"          "rsvd"        
-- 8             9             a             b             c             d             e             f
   "uncons"      "cdr"         "pop"         "zap"         "rsvd"        "rsvd"        "<->"         "?"

-- 0x1ax
-- 0             1             2             3             4             5             6             7
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "cons"        "car"         "push"        "dup"         "rsvd"        "rsvd"        "shift"       "unshift"

-- 0x1bx
-- 0             1             2             3             4             5             6             7
   "slurp"       "spit"        "journal"     "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "bons"        "rsvd"        "nil?"        "rsvd"        "rsvd"        

-- 0x1cx
-- 0             1             2             3             4             5             6             7
   ">>>"         "<<<"         "journal8"    "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x1dx
-- 0             1             2             3             4             5             6             7
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x1ex
-- 0             1             2             3             4             5             6             7
   "<<"          "stderr"      "stdin"       ">>"          "stdinf"      "nhref"       "and"         "||"        
-- 8             9             a             b             c             d             e             f
   "~~"          "href?"       "..."         "newref"      "span"        "eachar"      "rsvd"        "rsvd"        

-- 0x1fx
-- 0             1             2             3             4             5             6             7
   "<-"          "->"          "take"        "depth"       "give"        "nest"        "clear"       "rsvd"        
-- 8             9             a             b             c             d             e             f
   "ls2ar"       "trav"        "ith"         "cr"          "break"       "continue"    "walk"        "set"        

-- 0x20x
-- 0             1             2             3             4             5             6             7
   "perm"        "+="          "stack"       "code"        "if"          "ustack"      "reverse"     "boilerplate"        
-- 8             9             a             b             c             d             e             f
   "paste8"      "iter"        "unnest"      "conjure"     "self"        "rsvd"        "rsvd"        "rsvd"        

]


