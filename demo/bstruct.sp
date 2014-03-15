-- bstruct.sp
-- Strange Loop Conference 2013 ELC

((main (code 
        file_in !
        load
        bs2gv <<))

(file_in (code 1 get_argv ! slurp ))
(get_argv  (code <- self 'argv' hash8 luha 2 ith -> ith)))

-- val.sp ptr.sp list.sp tag.sp

