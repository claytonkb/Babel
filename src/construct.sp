-- The Construct
--
-- This is the "loading program" created by the Resistance to run 
-- simulations or upload virtual objects when they hack into the 
-- Matrix...

(   (root       (tag '/babel/tag/bvm'
                    (list bvm_code
                    bvm_stack
                    jump_table
                    sym_table)))

    (bvm_code   (tag '/babel/tag/bvm_code'
                    (list (list main)
                    nil)))

        -- (rstack (list nil))

    (bvm_stack  (tag '/babel/tag/bvm_stack' 
                    (list dstack ustack)))

        (dstack (list nil))
        (ustack (list nil))

    (jump_table (tag '/babel/tag/jump_table' nil ))

    (sym_table (tag '/babel/tag/sym_table' sym_table_hash_table ))

    (sym_table_hash_table (tag '/babel/tag/hash_table' symbol )))


