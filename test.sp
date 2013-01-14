-- sparse.pl test code

-- ( ( root ( j "Hello, world" ) ) )

(   (root ( k branch ( j 1 2 3 ) ) )
    (branch (k root nil (j 4 value 6) ) ) 
    (value (j 17 23 31) ) 
    (nil (q 'nil' (k nil nil))) )

-- ((root 
--     (k (k (j 1 2 3) (j 4 5 6)) 
--        (k (j 7 8 9) (j 0xa 0xb 0xc)))
--     ))

-- ((root 
--     (x (j 1) (j 2) (j 3) (x (j 1) (j 2) (j 3)))
--     ))

-- ((root 
--     (q '/babel/bvm' (j 1) (j 2) (j 3))
--     ))

