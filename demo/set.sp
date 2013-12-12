-- set.sp
-- Strange Loop Conference 2013 ELC

((main (code 

    x 0x17 set
    y (ptr 0x31) set
    z (list 0x42 "Hello") 0 paste

    a 3 a 4 3 move
    b 3 b 4 3 move8

    x
    y
    z
    a
    b

    collect !
    display !))

(x (val 0))
(y (ptr nil))
(z (list nil))

(a (val 0 1 2 3 4 5 6 7))
(b (val "abcdefg")))

