code:

    [ [{0xf0c3a569}]
    [ ddisp
    [ call

    [ [{"cnot: "}]
    [ stdout
    [ cnot
    [ disp
    [ call

    [ [{"cnot: "}]
    [ stdout
    [ cnot
    [ disp
    [ call

    [ [{0xffffffff}]
    [ ddisp
    [ call
    [ [{0xf0c3a569}]
    [ ddisp
    [ call
    [ [{"cand: "}]
    [ stdout
    [ cand
    [ disp
    [ call

    [ [{0xffffffff}]
    [ ddisp
    [ call
    [ [{0xf0c3a569}]
    [ ddisp
    [ call
    [ [{"cor: "}]
    [ stdout
    [ cor
    [ disp
    [ call

    [ [{0xffffffff}]
    [ ddisp
    [ call
    [ [{0xf0c3a569}]
    [ ddisp
    [ call
    [ [{"cnand: "}]
    [ stdout
    [ cnand
    [ disp
    [ call

    [ [{0xffffffff}]
    [ ddisp
    [ call
    [ [{0xf0c3a569}]
    [ ddisp
    [ call
    [ [{"cnor: "}]
    [ stdout
    [ cnor
    [ disp
    [ call

    [ [{0xffffffff}]
    [ ddisp
    [ call
    [ [{0xf0c3a569}]
    [ ddisp
    [ call
    [ [{"cxor: "}]
    [ stdout
    [ cxor
    [ disp
    [ call

    [ [{0xffffffff}]
    [ ddisp
    [ call
    [ [{0xf0c3a569}]
    [ ddisp
    [ call
    [ [{"cxnor: "}]
    [ stdout
    [ cxnor
    [ disp
    [ call

    nil ]]]]]]]]]]] ]]]]]]]]]]] ]]]]]]]]]]] ]]]]]]]]]]] ]]]]]]]]]]] ]]]]]]]]]]] ]]]]] ]]]]] ]]]


ddisp:
    [[ dup
    [ disp
    [ call
    [ ret
    nil]]]]]

disp:
    [[ bbl2str
    [ stdout
    [ [{"\n"}]
    [ stdout
    [ ret
    nil]]]]]]

