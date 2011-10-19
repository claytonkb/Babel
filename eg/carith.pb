code:

    [ prompts
    [ call
    [ cuadd
    [ [{"cuadd: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cusub
    [ [{"cusub: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cumul
    [ [{"cumul: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cudiv
    [ [{"cudiv: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ ciadd
    [ [{"ciadd: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cisub
    [ [{"cisub: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cimul
    [ [{"cimul: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cidiv
    [ [{"cidiv: "}]
    [ stdout
    [ disp
    [ call

    nil ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]]

prompts:
    [[ prompt
    [ call
    [ prompt
    [ call
    [ ret
    nil]]]]]]

prompt:
    [[ [{"Enter a number:\n> "}]
    [ stdout
    [ stdinln
    [ dec2ci
    [ ret
    nil]]]]]]

disp:
    [[ ci2dec
    [ stdout
    [ [{"\n"}]
    [ stdout
    [ ret
    nil]]]]]]

