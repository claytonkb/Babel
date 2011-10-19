code:
    [ [{"Shifting/rotating hex value 0xbabeface by 4 bits:\n"}]
    [ stdout
    
    [ [{"cushl: "}]
    [ stdout
    [ [{0xbabeface}]
    [ [{4}]
    [ cushl
    [ disp
    [ call

    [ [{"cushr: "}]
    [ stdout
    [ [{0xbabeface}]
    [ [{4}]
    [ cushr
    [ disp
    [ call

    [ [{"cashr: "}]
    [ stdout
    [ [{0xbabeface}]
    [ [{4}]
    [ cashr
    [ disp
    [ call

    [ [{"curol: "}]
    [ stdout
    [ [{0xbabeface}]
    [ [{4}]
    [ curol
    [ disp
    [ call

    [ [{"curor: "}]
    [ stdout
    [ [{0xbabeface}]
    [ [{4}]
    [ curor
    [ disp
    [ call

    nil ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]]]]]] ]]

disp:
    [[ bbl2str
    [ stdout
    [ [{"\n"}]
    [ stdout
    [ ret
    nil]]]]]]

