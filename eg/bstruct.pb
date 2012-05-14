-- bstruct.pb
-- This program demonstrates Babel's bstruct data-structure

main: ( `"To view .dot files: dot -Tsvg foo.dot > foo.svg\n" <<
        `{1 2 3}           
        `[1 2 3]           
        `(1 2 3)           
        `[1 [2 [3 nil]]]   `"arrays_lists.dot"          out !

        `"x: " x show . `"\n" . <<
        `"Memory usage of x (words): "         x mu  pr_val !
        `"Number of interior arrays in x: "    x nin pr_val !
        `"Number of leaf arrays in x: "        x nlf pr_val !
        `"Number of values in x: "             x nva pr_val !
        `"Number of pointers in x: "           x npt pr_val !

        `"Unloaded size of x: " x unload size pr_val !

        x cp cons `"cp.dot" out !

        `"Traversing x: " x `(1 0 0) trav pr_val !

--      Broken:        
--        `"Writing x: " `4 x `(1 0 0) paste die
--        x `(1 0 0) trav pr_val !
--        `"x: " x show . `"\n" . << 
        )

out: [( <- stack dump ->
        dup `"Writing " <-> . `"...\n" . <<
        <<< )]

pr_val: (( %u . `"\n" . << ))

x: [[[[1 2][3 4]][[5 6][7 8]]]]

