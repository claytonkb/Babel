main:
    [ *hidden
    [ code
    [ *data
    [ *stack
    [ rstack
    [ jmp_table
    [ sym_table
    [ nada
    ]]]]]]]]

hidden:
    [nil]

code:
    [ [ [{0xbabe} nil] [ [{"%x"} nil] [ cprintf nil ]] nil ]
    [ eval
    [ [{0xface} nil]
    [ [{"%x"} nil]
    [ cprintf
    nil ]]]]]

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

