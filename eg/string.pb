--string.pb

            -- print
code:       (["Hello "] dup
                stdout8
            str2ar dup
                stdout 
            ar2ls dup
                [(stdout)] swap
                each
            ls2lf dup
                stdout
            ar2str
                stdout8

            ["\n"] pr  -- pr is a synonym for stdout8

            -- strlen and conversions
            ["supercalifragilisticexpialidocious"] dup dup 
                ["\n"] cat8 pr
                size8 cu2dec
                ["\n"] cat8 pr
            str2ar dup
                size cu2dec
                ["\n"] cat8 pr
            ar2ls dup
                len cu2dec
                ["\n"] cat8 pr
            ls2lf dup
                size cu2dec
                ["\n"] cat8 pr
            ar2str
                size8 cu2dec
                ["\n"] cat8 pr

            -- strcpy
                ["flibbertigibbit"] cp
                [(["\n"] cat8 pr)]
            [2] times

            -- strcat
            ["jabber"] ["wocky"]
                cat8 ["\n"] cat8 pr

            ["jabber"] str2ar ["wocky"] str2ar
                cat ["\n"] str2ar cat stdout
        
            -- implement append

            )

