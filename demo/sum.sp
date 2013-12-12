-- sum.sp
-- Strange Loop Conference 2013 ELC

-- BROKEN

((main (code 
    (list 1 2 3 4 5) 
    sum ! 
    %d nl <<))

(sum (code <- 0 (code +) -> each)))

