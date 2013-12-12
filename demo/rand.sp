-- rand.sp

((main (code 
    0 (code randbs !) 
    30 times
    collect !
    display ! ))

(randbs (code 
    1 rand 
    2 %
    (code 0 =)
        (code 8 randlf !)
        (code randinte !)
    ifte))

(randlf (code 
    <- 1 rand -> 
    % 
    1 + 
    rand))

(randinte (code 
    depth 1 - dup
    (code 1 <)
        (code zap)
        (code 
            <- 1 rand -> 
            % 
            1 + 
            take 
            bons)
    ifte ))

(collect (code -1 take)))

