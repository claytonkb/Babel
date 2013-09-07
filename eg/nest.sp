-- nest.sp
-- Strange Loop Conference 2013 ELC

((main (code 
    (list 1 2 3 4 5) 
    sum eval 
    cu2dec cr stdout8))

    (sum (code down 0 (code cuadd) up each))

    (symbol (list nil)))

