--main: ( `17 `(1 (2 (3 4 (5 6) 7 (8 9) 10 ) (11 x&) ) 13) del dump << )

--main: ( `0x1234 `0x4321 + `0x5678 `0x7 cushl `0xff + - $ << `"\n" << )

main: ( `0x17
        `nil 
        `"abc" `0xaaaa inskha 
        `"def" `0xbbbb inskha 
--        `"ghi" `0xcccc inskha
--        `"jkl" `0xdddd inskha 
--        `"mno" `0xeeee inskha 
--        `"pqr" `0xffff inskha
        del
        dump << )

--main: ( `0x17
--        `nil 
--        `"abc" `0xaaaa inskha 
--        `"def" `0xbbbb inskha 
--        `"ghi" `0xcccc inskha
--        `"jkl" `0xdddd inskha 
--        `"mno" `0xeeee inskha 
--        `"pqr" `0xffff inskha
--        del
--        dump << )
--
--main: ( `nil `"xyz" `0xbabeface inskha 
--             `"abc" `0xdeadbeef inskha
--        dump << `"\n" << )
--

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

