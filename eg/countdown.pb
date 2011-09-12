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
    [ goto
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
        [ goto
        nil] ]]]]]]]
    [ [ [ [{"0"}]
        [ stdout
        nil] ]]
    [ sel
    [ eval
    nil ]]]]]]]]]

