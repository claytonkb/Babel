-----------------------------------------------------------------------------
--                                                                         --
--                            macro_table.sp                               --
--                                                                         --
-----------------------------------------------------------------------------

-- [if {this} this]
-- [ifte {if-this} {then-this} {else-this}]

(('map'
    { collect !
    cp 
    lsby2 !
    <- newha ->
    { <- dup   ->
        dup  1 ith
        down 0 ith ->
        inskha }
    each
    return})

('fold'
    { wrapcode ! 
    <- { pop car swap } cp -> 
    compose ! 
    { ... } cp 
    compose ! 
    return })

--('infix'
--    { { dup val? {unshift} {flip} selr ! } each collect ! return})

('sum'
    { collect !
    pop car swap
    {+}
    each
    return })

('pair'
    { lspair ! return })

('transpose'
    { mattrans ! return })

('cart'
    { cart ! return })

('bvm'
    { mkbvm2 ! return })

('strcat'
    { collect !
    pop car swap
    {.+} each
    return })

('arcat'
    { collect !
    pop car swap
    {cat} each
    return })

('wordcat'
    { collect !
    pop car swap
    {" " swap .+ .+} each
    return })

('linecat'
    { collect !
    pop car swap
    {"\n" swap .+ .+} each
    return })

('append'
    { collect ! append return })

('over'
    { twist <- twist collect ! -> each collect ! return }))

