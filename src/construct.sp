-- The Construct
--
-- This is the "loading program" created by the Resistance to run 
-- simulations or upload virtual objects when they hack into the 
-- Matrix...

((root (tag '/babel/tag/sparse_bvm' root_hash_table))

(root_hash_table (tag '/babel/tag/hash_table' hash_table_base))

(hash_table_base (ptr thread_id_etc soft_root_rstack_advance))

(soft_root_rstack_advance (ptr soft_root_etc rstack_ptr_and_advance_type))

(rstack_ptr_and_advance_type (ptr rstack_ptr advance_type))

(soft_root_etc (ptr soft_root (ptr nil (ptr (ptr nil (ptr (ptr (ptr (ptr (ptr (ptr nil (ptr nil code_ptr_and_ustack_ptr)) nil) nil) nil) nil) nil)) nil))  ))

(code_ptr_and_ustack_ptr (ptr code_ptr ustack_ptr))

(dstack_ptr_and_local_root (ptr dstack_ptr local_root))

(steps_etc (ptr (ptr (ptr dstack_ptr_and_local_root nil) nil) steps) )

(thread_id_etc (ptr (ptr nil (ptr nil steps_etc))  (ptr (ptr bvm_initd thread_id) nil) ))

(steps 
(tag '/babel/tag/hash_table_entry'
    (list (hash "steps")
    "steps"
    (list -1))))

(code_ptr 
(tag '/babel/tag/hash_table_entry'
    (list (hash "code_ptr")
    "code_ptr"
    (list main))))

(ustack_ptr 
(tag '/babel/tag/hash_table_entry'
    (list (hash "ustack_ptr")
    "ustack_ptr"
    (list nil))))

(soft_root 
(tag '/babel/tag/hash_table_entry'
    (list (hash "soft_root")
    "soft_root"
    (tag '/babel/tag/hash_table' soft_root_base))))
--    (list (tag '/babel/tag/hash_table' (list (list nil)))))))

(soft_root_base (ptr nil nil))

(rstack_ptr
(tag '/babel/tag/hash_table_entry'
    (list (hash "rstack_ptr")
    "rstack_ptr"
    (list nil))))

(thread_id
(tag '/babel/tag/hash_table_entry'
    (list (hash "thread_id")
    "thread_id"
    (list 1))))

(advance_type
(tag '/babel/tag/hash_table_entry'
    (list (hash "advance_type")
    "advance_type"
    (list 0))))

(bvm_initd
(tag '/babel/tag/hash_table_entry'
    (list (hash "bvm_initd")
    "bvm_initd"
    (list 0))))

(dstack_ptr 
(tag '/babel/tag/hash_table_entry'
    (list (hash "dstack_ptr")
    "dstack_ptr"
    (list nil))))

(local_root 
(tag '/babel/tag/hash_table_entry'
    (list (hash "local_root")
    "local_root"
    (list nil)))))

-- (main (code "Hello, world\n" stdout_8d)))

--    (list (list (ptr "Hello, world\n") 0x1e0)))))

