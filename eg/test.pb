main: ( `nil `"xyz" %% `0xbabeface inskha 
             --`"abc" %% `0xdeadbeef insha )
        dump << `"\n" << )


-- code: 
--     (("")        cr << 
--     ("nil") %% $ cr << )

--code: ( setup ! -- ((show ['\n'] . <<)) <-> ... die
--        ["Ctl+C to quit\n"] pr
--        [(  ["Type something\n> "] <<
--            stdinln dup
--                ["quit"] ==
--                [( die )]
----                [(  ["You typed: "] <<
----                    ["\n"] . << )]
--                [( [x] <-> %% $ << )]
--            ? !
--        )] loop 
--        ["Done."] pr)
--
--setup: ((    (nil)
--                ((dup
--                <- car ->
--                cdr car << die
--                inskha)) --> FIXME: inskha and hash8 generating diff hashes
--            opcodes ...
--            [x] [(0)] paste
--            x keysha))
--
--x: [nil nil]
--
--opcodes: (( (0x001 'cushl') (0x003 'cushr') (0x005 'curol') 
--            (0x007 'curor') (0x00b 'cashr')))
--

