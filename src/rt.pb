-- rt.pb
-- Babel run-time BVM
-- This is the "root" virtual machine, it is here to act as a handler for
-- unhandled exceptions and as a debug console. This is just a stub for
-- now

code:
    [ argv
    [ [{0}]
    [ cxr
    [ slurp8
    [ load
    [ dup
    [ dup
    [ dup
    [ dup
    [ dup
    [ dup
    [ dup
    [ dup
    [ dup
    [ dup
    [ dup
    [   [[ [{"> "}] 
        [ stdout
        [ stdinln
        [ dec2ci
        [ bvmstep
        [ bbl2gv
        [ [{"snapshot.dot"}]
        [ spit8
        nil]]]]]]]]]
    [ [[{-1}]]
    [ times
    nil]]]]]]]]]]]]]]]]]]]

x:
    [nil]

