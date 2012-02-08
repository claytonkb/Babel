

code:
   (argv [1] cxr dec2ci
    [x] [0] save
          (([consonant]
            [1] rand [20] curem
            cxr
        stdout
            [vowel]
            [1] rand [6] curem
            cxr
        stdout))
    x times)

x: [0]

consonant: ["b" "c" "d" "f" "g" "h" "j" "k" "l" "m" "n" "p" "q" "r" "s" "t" "v" "w" "x" "z"]

vowel: ["a" "e" "i" "o" "u" "y"]

