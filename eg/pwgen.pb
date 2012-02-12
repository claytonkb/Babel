

code:
   (argv [1] cxr $d
    [x] [(0)] paste
          ((consonant
            [1] rand [20] curem
            cxr
        <<
            vowel
            [1] rand [6] curem
            cxr
        <<))
    x times
    ["\n"] <<)

x: [0]

consonant: [["b" "c" "d" "f" "g" "h" "j" "k" "l" "m" "n" "p" "q" "r" "s" "t" "v" "w" "x" "z"]]

vowel: [["a" "e" "i" "o" "u" "y"]]

