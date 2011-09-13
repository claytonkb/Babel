main:
    [ hidden
    [ code
    [ *data
    [ *stack
    [ *rstack
    [ *nstack
    [ *jmp_table
    [ *sym_table
    [ *nada
    ]]]]]]]]]

hidden:
    [nil nil]

code:

    [ [ [{0xbabe} nil] [ [{"%x"} nil] [ cprintf nil ]] nil ]
    [ eval
    nil ]]
    
data:
    [nil]

stack:
    [nil]

rstack:
    [nil]

nstack:
    [nil]

jmp_table:
    [nil]

sym_table:
    [nil]

nada:
    [{0} nil]

nil:
    [nil nil]

