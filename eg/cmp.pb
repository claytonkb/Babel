code:

    [ prompts
    [ call
    [ cne
    [ [{"cne: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ ceq
    [ [{"ceq: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cult
    [ [{"cult: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cule
    [ [{"cule: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cugt
    [ [{"cugt: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cuge
    [ [{"cuge: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cilt
    [ [{"cilt: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cile
    [ [{"cile: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cile
    [ [{"cigt: "}]
    [ stdout
    [ disp
    [ call

    [ prompts
    [ call
    [ cile
    [ [{"cige: "}]
    [ stdout
    [ disp
    [ call

    nil ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]]

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

