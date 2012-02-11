code: (["Ctl+C to quit\n"] pr
        [(  ["Type something\n> "] pr
            stdinln dup
            ["quit"] cmp
            [( die )]
            [(  ["You typed: "] pr
                ["\n"] cat8 pr )]
            sel eval
        )] loop 
        ["Done."] pr)

