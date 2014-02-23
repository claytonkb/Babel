-- list.sp
-- This program illustrates Babel's list operators
-- Use the debugger to explore this demo program
--    % perl babel demo/list.sp
--    % perl babel demo/debug.sp demo/list.sp.bbl
--    type 'h' in the debugger for help

((main (code
    (list 1 2 3)
    dup
    car zap
    cdr zap

    1 (list 2 3)
    cons uncons
    zap zap

    (ptr 1 2 3)
    ar2ls bons
    zap

    (list 1 2 3)
    ls2lf ar2ls
    zap

    (list 2 3)
    (list 1)
    push pop
    zap zap

    (list 1 2)
    (list 3)
    unshift shift
    zap zap)))

