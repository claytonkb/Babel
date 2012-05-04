-- rt.pb
-- Babel run-time BVM
-- This is the "root" virtual machine, it is here to act as a handler for
-- unhandled exceptions and as a debug console. This is just a stub for
-- now

main:
    ( argv [0] th
    slurp load
    babel )

