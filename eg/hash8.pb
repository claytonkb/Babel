code:
    [ argv_check
    [ call
    [ argv
    [ [{1}]
    [ cxr
    [ hash8
    [ bbl2str
    [ stdout
    nil]]]]]]]]

argv_check:
    [[ argv
    [ arlen
    [ [{1}]
    [ cugt
    [ [ [ [{"Please provide an argument.\n"}]
        [ stdout
        [ die
        nil]]]]
    [ [[nil]]
    [ sel
    [ eval
    nil]]]]]]]]]

