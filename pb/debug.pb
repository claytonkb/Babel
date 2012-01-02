-- rt.pb
-- Babel run-time BVM
-- This is the "root" virtual machine, it is here to act as a handler for
-- unhandled exceptions and as a debug console. This is just a stub for
-- now

code:
   (argv [0] cxr slurp8
    load dup
    [x] [0] paste

      [([x] dup dup

        ["next op: "] stdout
        cdr car car
        dup

            islf
            [(["push "] stdout bbl2str stdout)]
            [([opcode_map] swap cxr stdout)]
        sel eval

        ["\n"] stdout
        ["> "] stdout
        
        stdinln dec2ci 
        bvmstep

        bbl2gv
        ["snapshot.dot"] spit8

        ["TOS: "] stdout
        cdr cdr cdr car car car
        bbl2str stdout
        ["\n"] stdout)]

    [[-1]]
    times)

x:
    [nil nil]

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
   "fnord"       "bvmroot"     "bvmexec"     "bvmkill"     "bvmdb"       "bvmsave"     "bvmfork"     "msize"        
-- 8             9             a             b             c             d             e             f
   "arlen8"      "arlen16"     "arlen32"     "arlen"       "arcat8"      "arcat16"     "arcat32"     "arcat"

-- 0x11x
-- 0             1             2             3             4             5             6             7
   "mu"          "nva"         "npt"         "nlf"         "nin"         "load"        "unload"      "bbl2gv"    
-- 8             9             a             b             c             d             e             f
   "slice8"      "slice16"     "slice32"     "slice"       "arcmp8"      "rsvd"        "rsvd"        "arcmp"       

-- 0x12x
-- 0             1             2             3             4             5             6             7
   "sleep"       "die"         "goto"        "call"        "ret"         "loop"        "last"        "next"    
-- 8             9             a             b             c             d             e             f
   "w8"          "w16"         "w32"         "w"           "trunc"       "cxr"         "rsvd"        "ardeq"    

-- 0x13x
-- 0             1             2             3             4             5             6             7
   "eval"        "areval"      "lseval"      "meval"       "exec"        "while"       "times"       "rsvd"        
-- 8             9             a             b             c             d             e             f
   "isin"        "rsvd"        "newin"       "ar2ls"       "islf"        "del"         "newlf"       "s"    

-- 0x14x
-- 0             1             2             3             4             5             6             7
   "hash8"       "rand"        "srand"       "argv"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "cp"          "save"        "rsvd"        "archr"       "armv8"       "rsvd"        "rsvd"        "armv"    

-- 0x15x
-- 0             1             2             3             4             5             6             7
   "hash"        "luha"        "insha"       "delha"       "exha"        "valsha"      "keysha"      "cpha"    
-- 8             9             a             b             c             d             e             f
   "mkptr"       "deref"       "mksptr"      "sderef"      "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x16x
-- 0             1             2             3             4             5             6             7
   "rsvd"        "rsvd"        "inskha"      "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "add"         "mul"         "exp"         "mod"         "abs"         "max"         "ceil"        "rsvd"        

-- 0x17x
-- 0             1             2             3             4             5             6             7
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "sub"         "div"         "log"         "rem"         "eq"          "min"         "floor"       "rsvd"        


-- 0x18x
-- 0             1             2             3             4             5             6             7
   "ar2str"      "str2ar"      "rsvd"        "rsvd"        "cu2dec"      "cu2hex"    "ci2dec"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "lslen"       "take"        "give"        "rsvd"        "lscat"       "rsvd"        "rsvd"        "rsvd"        

-- 0x19x
-- 0             1             2             3             4             5             6             7
   "bbl2str"     "rsvd"        "rsvd"        "rsvd"        "rsvd"        "hex2cu"      "dec2ci"      "rsvd"        
-- 8             9             a             b             c             d             e             f
   "uncons"      "cdr"         "pop"         "zap"         "ls2stk"      "permstk"     "swap"        "sel"

-- 0x1ax
-- 0             1             2             3             4             5             6             7
   "strint"      "macint"      "interpol"    "rsvd"        "mkpeg"       "pegpar"      "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "cons"        "car"         "push"        "dup"         "stk2ls"      "dupstk"      "shift"       "unshift"

-- 0x1bx
-- 0             1             2             3             4             5             6             7
   "slurp"       "spit"        "journal"     "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "isls"        "rsvd"        "rsvd"        "ls2ar"       "isat"        "isnil"       "rsvd"        "rsvd"        

-- 0x1cx
-- 0             1             2             3             4             5             6             7
   "slurp8"      "spit8"       "journal8"    "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "mkns"        "luns"        "insns"       "delns"       "exns"        "lsns"        "cpns"        "rsvd"        

-- 0x1dx
-- 0             1             2             3             4             5             6             7
   "rdfile"      "wrfile"      "clfile"      "cntfile"     "rsvd"        "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x1ex
-- 0             1             2             3             4             5             6             7
   "stdout"      "stderr"      "stdin"       "stdinln"     "stdinf"      "rsvd"        "rsvd"        "rsvd"        
-- 8             9             a             b             c             d             e             f
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

-- 0x1fx
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        
   "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        "rsvd"        

]

