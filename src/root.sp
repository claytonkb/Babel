-- Babel Root Virtual Machine
--
-- This is the code and symbol-table for The Construct, the
-- root virtual machine...

(   (main   ( code  ( code 0xaa cu2hex cr ) 
                    ( code 0xaa cu2dec cr ) 
                    0x0 sel eval 
                    stdout8 ) )

    (symbol (list nil))

    (x (val 42)))

