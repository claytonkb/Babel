-- pwgen.pb
-- Must call this with a numerical argument:
-- 
-- perl babel eg/pwgen.pb 5

main:
        ( argv # `1 <=
        `( argv `1 th $d )
        `( (5) )
    if
    x set
          `( consonant
            `1 rand `20 %
            th
        <<
            vowel
            `1 rand `6 %
            th
        <<)
    x times
    `"\n" << die )

x: [0]

consonant: [["b" "c" "d" "f" "g" "h" "j" "k" "l" "m" "n" "p" "q" "r" "s" "t" "v" "w" "x" "z"]]

vowel: [["a" "e" "i" "o" "u" "y"]]

