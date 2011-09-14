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

    [ [{0x0}]
    [ [{0x1}]
    [ [{0x2}]
    [ [{0x3}]
    [ [{0x4}]
    [ [{0x5}]
    [ [{0x6}]
    [ [{0x7}]
    [ zap
    [ zap
    [ zap
    [ zap
    [ zap
    [ zap
    [ zap
    nil ]]]]]]]]]]]]]]]
    
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

