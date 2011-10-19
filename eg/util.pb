code:
    [ [{"A random number: "}]
    [ stdout
    [ [{1}]
    [ rand
    [ bbl2str
    [ stdout
    [ [{"\nSleeping for 3 seconds... "}]
    [ stdout
    [ [{3}]
    [ sleep
    [ [{"Awake!\nYou passed "}]
    [ stdout
    [ argv
    [ arlen
    [ [{1}]
    [ cusub
    [ ci2dec
    [ stdout
    [ [{" arguments on the command-line\n"}]
    [ stdout
    [ [{"This version of Babel is "}]
    [ stdout
    [ msize
    [ [{8}]
    [ cumul
    [ ci2dec
    [ stdout
    [ [{" bits\n"}]
    [ stdout
    nil]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

