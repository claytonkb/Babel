code: (["Ctl+C to quit\n"] pr
        [(  ["Type something\n> "] pr
            stdinln
            ["You typed: "] pr
            ["\n"] cat8 pr
        )] loop 
        ["Done."] pr)

