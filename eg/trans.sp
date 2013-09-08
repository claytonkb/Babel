-- transpose.sp

((main (code 
    (list 
        (list 1 2 3) 
        (list 4 5 6) 
        (list 7 8 9) )
    transpose !
    d ! ))

(collect (code -1 take))

(cdrall (code (code 
        (code cdr ) ...
        collect ! )
    nest ))

(all_cars (code (code 
        (code 
            car cp 
            <-> zap )
        ... 
        collect ! )
    nest ))

-- Only works for symmetric arrays...
(transpose (code (code 
    dup car len
    <-
        (code 
        dup
        all_cars ! 
        <-> cdrall !)
    ->
    times 
    zap
    collect ! ) 
    nest ))

(allnil
    (code 
    1 <->
    (code
        (code car isnil)
            (code fnord)
            (code zap 0 last)
        ifte)
    ... )))

