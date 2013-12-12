-- guess.sp
-- Strange Loop Conference 2013 ELC

((main (code (code 
    1 rand 
    10 %
    dup
    <- digits -> ith
    <->
    "Guess a number between 0 and 9> " <<
    >>
    $d
    (code =)
        (code "That was epic! ")
        (code "Complete fail! ")
    ifte
    "The number was: " . << 
    cr <<)
    loop))

(digits (list '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' '0')))

