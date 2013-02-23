-- The Construct
--
-- This is the "loading program" created by the Resistance to run 
-- simulations or upload virtual objects when they hack into the 
-- Matrix...

(   (root       (tlist '/babel/tag/bvm'
                    bvm_code
                    bvm_stack
                    jump_table
                    sym_table))

    (bvm_code   (tlist '/babel/tag/bvm_code'
                    (list code)
                    nil))

        -- (rstack (list nil))

    (bvm_stack  (tlist '/babel/tag/bvm_stack' 
                    dstack
                    nil))

        (dstack (list nil))
        -- (ustack (list nil))

    (jump_table (tlist '/babel/tag/jump_table' nil ))

    (sym_table (tlist '/babel/tag/sym_table' sym_table_hash_table ))

    (sym_table_hash_table (tlist '/babel/tag/hash_table' symbol )))


