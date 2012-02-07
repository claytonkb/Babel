code: ([(["Type something\n> "] stdout 
        stdinln

        [(["quit"] cmp)]
        [(last)]
        [(["You typed: "] stdout stdout ["\n"] stdout)]
        sel eval

        )] 
        loop 
        ["Done."] stdout)

