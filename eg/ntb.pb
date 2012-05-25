main: ( readfile !
        dup

        `is_newline
        matchar !

        carve !

        nonil ! 
    
        get_dents ! 

        lsprint ! `"\n" << )

----        `( `"==>\n" << ar2str cr << ) ... )
--
--    
--        `( dup `is_space matchar ! carve ! ) ...
--
--        collect !
--
--        `( `( ar2str cr << ) ... ) ... )

get_dents: (( nest `( dent ! ) ... collect ! reverse unnest ))

collect: (( `-1 take reverse ))

stack_reverse: (( collect ! give ))

nonil:
    (( nest 
    give
                `( dup nil?
                `( down )
                `( zap  )
            if )
        `( empty? ! not )
    while

        `( up )
        `( full? ! not )
    while 

    collect !
    reverse

    unnest ))

empty?: (( stack nil? ))

full?:  (( ustack nil? ))

-- TOS-0: list of offsets in monotonic order
-- TOS-1: array to be carved
carve: 
    (( lsinc !  -- perf: this is not needed, see below
    reverse
    `2 take nest 
    give
        `( dup `1 - 
        <- cut <-> -> 
        cut 
        zap ) 
    ...
    collect !
    unnest ))

lsinc: (( dup `( `1 <-> += ) ... ))

lsprint: (( `( %d `" " . << ) ... ))

readfile: ((argv `1 th >>> str2ar))

is_space:   ( `0x20 == not )

is_newline: ( dup `0xa == not <-> `0xd == not or )

matchar: 
    (( `0 `matchar_x set
    `matchar_fn set

    dup nil?

        -- if nil:
        `( nest
            `( `matchar_fn !
                `( zap )
                `(  `matchar_y `matchar_x `0 + 
                    `nil cons push zap )
                if
            `1 `matchar_x += )
        eachar

        `matchar_y shift zap 
        unnest )

        -- else if not nil:
        `( (nil) )

    if ))

matchar_fn: ( nil )

matchar_x: {0}

matchar_y: ( nil )

dent: (( `0 <-> 
        `( `0x20 == not
                `( last )
                `( `1 + )
            if )
        eachar ))

--Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec 
--ornare, mauris id tincidunt posuere, massa sapien pellentesque
-- urna, ac sodales nisl metus non ipsum. Aenean ligula augue, 
--accumsan non aliquam in, tempor id velit. Nullam eu odio ac 
--augue facilisis posuere. Cras ullamcorper nibh at tellus auctor 
--suscipit. Proin orci quam, egestas nec mattis tincidunt, sagittis 
--non mi. Vestibulum ante ipsum primis in faucibus orci luctus et
-- ultrices posuere cubilia Curae; Fusce ut turpis a ipsum placerat 
--convallis scelerisque vitae odio. Donec dui nisi, rutrum nec facilisis 
--ut, vulputate non diam. Donec iaculis, dolor id pellentesque
-- congue, ante nibh tempus ipsum, vitae pretium odio quam vel urna.
--

sdd: (( sdp ! die ))

sdp: (( stack dump << ))

