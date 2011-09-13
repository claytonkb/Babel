code:
    [ loop
    [ [{"Hello, world. "}]
    [ stdout
    [ [[ loop
       [ [{"Hiya! "}]
       [ stdout
       [ last
       nil]]]]]
    [ eval
    [ next
    nil]]]]]]

