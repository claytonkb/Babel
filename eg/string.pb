--string.pb

            -- print
main:       (`"Hello " dup <<
            str2ar dup stdout 
            ar2ls  dup
                <- [(stdout)] ->
                ...
            ls2lf  dup stdout
            ar2str <<

            `"\n" pr  -- pr is a synonym for stdout8

            -- strlen and conversions
            `"supercalifragilisticexpialidocious" dup dup `"\n" . <<
                #8  %d `"\n" . <<
            str2ar dup
                #   %d `"\n" . <<
            ar2ls dup
                len %d `"\n" . <<
            ls2lf dup
                #   %d `"\n" . <<
            ar2str
                #8  %d `"\n" . <<

            -- strcpy
                `"flibbertigibbit" cp
                `(cr <<) `2 times

            -- strcat
            `"jabber" `"wocky"
                . `"\n" . <<

            `"jabber" str2ar `"wocky" str2ar
                cat `"\n" str2ar cat stdout)
        
            -- for more append operators (push, unshift), see list.pb


