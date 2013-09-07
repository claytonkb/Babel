-- times.sp
-- Strange Loop Conference 2013 ELC

((main (code 
    'argv' lusym 1 th dec2ci
    down
    (code (code 
            '*' stdout8) 
        iter 
        times
        "\n" stdout8)
    up
    times))

    (symbol (list nil)))

