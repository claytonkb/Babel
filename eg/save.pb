code:
    [ [x]    -- get x
    [ [{0}]
    [ cxr    -- extract the first element (leaf-array) of x
    [ [{1}]
    [ cusub  -- subtract 1 from the value of x
    [ [x]
    [ [{0}]
    [ save   -- write the result back to x
    [ [x]    -- get x again
    [ [{0}]
    [ cxr
    [ ci2dec -- convert the value of x to decimal
    [ stdout -- print it
    nil]]]]]]]]]]]]]

x:
    [{10}]

