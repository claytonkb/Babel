-- cp.sp

((main 
    {(('a' 'b' 'c')
      ('d' 'e' 'f')
      ('g' 'h' 'i'))
    dup cp

    dump_stack !})

(dump_stack { 
        { depth }
        { depth take rev { bbl2str nl << } each }
        { "nil\n" << }
    ifte}))
