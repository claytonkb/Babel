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

    [ [{0xface} nil]
    [ [{0xbabe0000} nil]
    [ cuadd
    [ [{0x4} nil]
    [ cuadd
    [ [{"%x"}]
    [ cprintf
    [ zap
    [ fnord
    [ zap
    nil ]]]]]]]]]]
    
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

