main:
    [ hidden
    [ code
    [ *data
    [ *stack
    [ *rstack
    [ *jmp_table
    [ *sym_table
    [ *nada
    ]]]]]]]]]

hidden:
    [nil nil]

code:

    [   [ [{0x2} nil]
        [ [{0x3} nil]
        [ cuadd
        [ [{"%x"} nil]
        [ cprintf
        nil ]]]]]
    [ eval
    nil ]]

--    [   [   [ [{2} nil]
--            [ [{3} nil]
--            [ cuadd
--            [ [{"%x"} nil]
--            [ cprintf
--            nil ]]]]]]
--    [ eval
--    nil]]

data:
    [nil]

stack:
    [nil]

rstack:
    [nil]

jmp_table:
    [nil]

sym_table:
    [nil]

nada:
    [{0} nil]

nil:
    [nil nil]

