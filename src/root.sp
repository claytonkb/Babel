-- Babel Root Virtual Machine
--
-- This is the code and symbmol-table for The Construct, the
-- root virtual machine...

(   (code   (list 
                (ptr "33") cr stdout8
                (ptr 13) cu2dec cr stdout8
                (ptr x) cu2dec cr stdout8
                die (ref 0xbabe 0xdead 0xface ) ))

    (symbol (list nil))

    (x (val 42)))


