-- bbl2dot.sp
--

((main (code 
    load_program !
    bs2gv
    << ))

(load_program (code 
    1 argvn ! 
    slurp 
    load))

