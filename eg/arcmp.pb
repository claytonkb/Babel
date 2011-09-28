code:
    [ [{0xbabeface}]
    [ [print_tos]
    [ call
    [ [{0x0 0x0}]
    [ [print_tos]
    [ call
    [ arcmp
    [ [print_tos]
    [ call
    [ [{"\n"}]
    [ stdout
    [ [{0xaced}]
    [ [print_tos]
    [ call
    [ [{0xbabeface}]
    [ [print_tos]
    [ call
    [ arcmp
    [ [print_tos]
    [ call
    nil]]]]]]]]]]]]]]]]]]]]

print_tos:
    [ dup
    [ bbl2str
    [ stdout
    [ ret
    nil ]]]]

