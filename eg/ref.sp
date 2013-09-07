-- ref.sp

((main (code b eval))

(a (code 
    (ref  
        nil -- sym_table-relative
        (tag '/babel/tag/ref_string' (val "argv"))
        (tag '/babel/tag/ref_array'  (val 0)))
    cr stdout8 ))

(b (code 
    (ref  
        x
        (tag '/babel/tag/ref_list' (val 1))
        (tag '/babel/tag/ref_list' (val 1)))
    cr stdout8 ))

(x (list 
    (list "Hello" "world") 
    (list "Goodbye" "cruel" "world")))

(symbol (list nil)))

