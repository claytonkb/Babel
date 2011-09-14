code:
    [ [{"Enter a number: "}]
    [ stdout
    [ stdinln
    [ dup
    [ stdout
    [ [{" "}]
    [ stdout
    [ dec2ci
    [ [countdown]
    [ call
    nil ]]]]]]]]]]

countdown:
    [ [{1}]
    [ cusub
    [ dup
    [ [{0}]
    [ ceq
    [ [ [ dup
        [ ci2dec
        [ stdout
        [ [{" "}]
        [ stdout
        [ [countdown]
        [ call
        nil] ]]]]]]]
    [ [ [ [{"0"}]
        [ stdout
        [ ret
        nil] ]]]
    [ sel
    [ eval
    nil ]]]]]]]]]

