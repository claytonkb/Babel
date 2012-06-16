main: ( die )

--foo: (1 2 3)
--bar: (4 5 6)

--main: ( [(1 2 3)] [(4 5 6)] cons dump << )

--main: 
--        ( `foo 
--        `[(4 5 6)] dup 
--        <- <-> -> 
--    set 
--    self dump << 
--    (foo) )
--
--foo: [(1 2 3)]

--main: ( 
--    foo show cr << 
--    bar show cr << 
--    `(foo bar) 
--        `( 
--        `foo `(7 8 9)       set 
--        `bar `(0xa 0xb 0xc) set 
--        foo show cr << 
--        bar show cr << ) 
--    let 
--    foo show cr <<
--    bar show cr << )
--
--foo: [(1 2 3)]
--bar: [(4 5 6)]

--main: 
--    ( `(1 2 3 4 5) dup cp
--    sum_stack ! <->
--    prod_stack !
--    %d cr << 
--    %d cr << )
--
--sum_stack: 
--    (( `( give  
--        `( + )
--        `( depth `1 > )
--    while ) nest ))
--
--prod_stack: 
--    (( `( give  
--        `( cumul )
--        `( depth `1 > )
--    while ) nest ))


--main: 
--    ( `1
--    `(nil)
--    `(`"true\n" <<)
--    `(`"false\n" <<)
--    ifte 
--    fnord )

--main: 
--    (`nil 
--    `"foo" `0x13 inskha 
--    `"bar" `0x42 inskha 
--    dump <<)

-- main: ( `((0)) `( `"true\n" << ) `( sdd ! `"false\n" << ) ifte fnord ) 

--main: ( `"[]][[]" balanced? ! )
--
--balanced?: (( str2ar 
--    ar2ls 
--        `( `( `"]" ord = )
--            `( zap
--                `( zap empty? ! ) 
----                    `( `"unbalanced\n" << die )
--                    `( `1 `"unbalanced\n" << die )
--                    `(  zap )
--                ifte )
--            `(nil)
--        ifte ) 
----    `( show cr << )
--    each 
--
--    `( empty? ! ) 
--        `( `"balanced\n" << )
--        `( `"unbalanced\n" << )
--    ifte ))

--main: ( `0x13 dump << )

--main:
--    ( `foo& `42 set
--    `bar& `13 set 
--    `foo& `bar& -=
--    `foo& deref
--    %d    cr    << )

--main: 
--    ( `0xbabeface `test0& set 
--    `0xdeadbeef `test1& set 
--    `0xfadedcab `test2& set 
--    self dump << )


--main: ( `0 `(6 14 6 7 1 4) `( + ) ... )

--main: ( `[0x13] unload `( hex08 ! cr << ) eachar )

listobj: (( `( hex08 ! cr << ) eachar ))

hex08: 
        (( %x dup #8 
        `8 <-> -
        <-
            `"00000000" cp
        ->
        paste8 ))

-- main: ( endian show << )

--main: ( `(1 2 (3 4) 5 6) unload size %d cr << )

--main: ( `(1 2 (3 4) 5 6) mu_check ! )

mu_check: 
    (( dup mu <->
    mu_components !
    sum ! 
    = show cr << ))

sum: (( <- `0 -> `( + ) ... ))

mu_components:
        (( `( dup nva
        <-> dup npt 
        <-> dup nlf
        <-> dup nin
        <-> dup nhref
        <-> nhword 
    collect ! ) 
    nest ))

-- main: ( `10 dup cp stack dump << )

-- main: ( `( `( `"*" << ) iter times `"\n" << ) `10 times )

-- star_triangle: { <- { { "*" << } iter times "\n" << } -> times }

--main: ( `"ha" `5 print_repeat ! )
--
--print_repeat: 
--    (( <- `( dup << ) -> times ))

--main: ( `0xbabe 
--        `( `"C\n" << (0) ) 
--        `( `"A\n" << ) 
--        `( `"B\n" << ) 
--        ifte fnord )

-- main: ( `[1 2 3 4] dup dup # rand perm sdd ! )

--main: 
--    ( randlf !
--    `( rand_bstruct ! ) 
--    `200 times 
--    collect !
--    bons
--    sdd ! )
--
--rand_bstruct:
--    (( `1 rand `7 %
--    
--    dup `5 > 
--    `( dup `4 > 
--        `( dup `3 > 
--            `( dup `2 > 
--                `( dup `1 > 
--                    `( `0 > 
--                        `( <- swap -> )
--                        `( dup # rand perm ) if ) 
--                    `( zap dup # <- `1 rand -> % cut ) if ) 
--                `( zap cp ) if ) 
--            `( zap dup ) if ) 
--        `( zap randinte ! ) if )
--    `( zap randlf ! ) if ))
--
--randlf: (( `1 rand `10 % `1 + rand ))
--
--randinte: 
--    (( depth `1 - dup
--    `1 >
--        `( zap )
--        `( <- `1 rand -> % `1 + take bons )
--    if ))

--main: 
--        ( `((0 0) 
--        (0 1)
--        (0 2)
--        (0 3)
--        (0 4)
--        (1 0)
--        (1 1)
--        (1 2)
--        (1 3)
--        (1 4)
--        (2 0)
--        (2 1)
--        (2 2)
--        (2 3)
--        (3 0)
--        (3 1)
--        (3 2)
--        (4 0) ) 
--    `( dup
--        `"A(" << `( %d `" " . << ) ... `") = " <<
--        reverse
--        give ack ! %d cr << ) ... )

ack: 
    (( dup zero? !
        `( <-> dup zero? !
            `( <-> 
                cp
                `1 -
                <- <- `1 - ->
                ack ! -> 
                ack ! )
            `( <->
                `1 - 
                <- `1 -> 
                ack ! )
        if )
        `( zap `1 + )
    if ))

-- main: ( self dump << die test& )

sumar: (( <- `0 -> `( + ) eachar ))

product: (( <- `1 -> `( cumul ) eachar ))

sp: (( dup 
    sum ! %d cr <<
    product ! %d cr << ))

-- main: ( `[1 [2 [3 nil&]]] ## %d cr << )

-- main: ( `"nil" %% $ cr << )

-- main: ( `(1 2 3) sum ! %d << )
--
--sum: (( `0 <-> `( + ) ... ))

grab: (( argv `1 th ))

--main: 
--    ( argv `1 th $d
--    fib ! 
--    %d cr << )

fib: 
    (( dup zero? !
        `( dup one? ! 
            `( cp <- `2 - fib ! -> `1 - fib ! + )
            `( zap (1) )
        if )
        `( zap (1) )
    if ))


zero?: (( `0 = ))

one?: (( `1 = ))


--main: 
--    ( 
--
----    `(( dup ) 
----    ( ("a") ("b") ("c") ("d") ("e") ))
----    cartesian ! 
----
----    `( == )
----    `2 take
----
----    cartesian !
----
----    `( ( `1 %d << ) 
----        ( `2 %d << )
----        ( `3 %d << )
----        ( `4 %d << )
----        ( `5 %d << ))
----
----    `2 take
----
----    transpose !
--
--    `( ( x `"a" == )
--    ( x `"b" == ) 
--    ( x `"c" == ) 
--    ( x `"d" == ) 
--    ( x `"e" == ) )
--
--    `( ( `1 %d << ) 
--        ( `2 %d << )
--        ( `3 %d << )
--        ( `4 %d << )
--        ( `5 %d << ) )
--
--    `2 take
--
--    transpose !
--
--    cond !
--
--    sdd ! )

x: ("d")

-- main: ( `(1 2 3) `(1 2 3) == $ cr << )

--main: 
--    ( `( ( ("a" "b") ("c" "d") )
--         ( ("e" "f") ("g" "h") ) )
--    cartesian !
----    $ cr << )
--    sdd ! )

cartesian: 
    (( `( dup 
        <- cdr car -> 
        car 
        `( <- cp -> 
        <-> 
            `( <- cp ->
            `2 take
            <-
            ) ...
            zap
        ) ...
        zap 
            `( -> )
            `( full? ! not )
        while
        collect ! )
    nest ))

enlist: (( `( `( `nil cons ) ... collect ! ) nest ))

-- main: ( `( ( ``0 ( `"one\n" << ) ) ( ``1 ( `"two" << ) )  ) cond ! )

cond: 
    (( `( dup
    <- cdr ->
    car ! sdd !
        `( nil )
        `( car ! last )
    if
    ) ... ))

-- main: ( `( fnord ) eval fnord fnord die )

--main: ( `1 rand dup disp ! 
--        `1 rand dup disp ! 
--        cuadd       disp ! )
--
--disp: (( %d cr << ))

-- main: ( `5 `"eg/foo.pb.bbl" slurp load babel fnord )

-- main: ( `(1 2 3) $ cr << )

-- main: ( grab ! $d fac ! %d cr << )

fac: 
    (( dup zero? !
        `( dup one? !
            `(  cp `1 - 
                fac ! 
                cumul )
            `( zap (1) )
        if )
        `( zap (1) )
    if ))
--
--one?: (( `1 = ))

-- main: ( `(nil) nil? $ cr << )

--main: ( `((1 2) (3 4 5) (6))
--        cdrall ! 
--        cdrall ! 
--        cdrall ! 
--        cdrall ! 
--        allnil? !  
--        $ cr << )

--main: 
--    ( `((1 2 3 4) (5 6 7 8) (9 10 11 12)) 
--    cp 
--    transpose ! transpose ! 
--    unload <- unload ->
--    == not
--    $ cr << )

allnil?:
    (( `1 <->
    `( car nil? 
        `( zap `0 last )
        `( nil )
    if) ... ))

cdrall: 
    (( `( `( cdr ) ...
        collect ! )
    nest ))

transpose:
        (( `( `( dup
            all_cars   !     
            <-> cdrall ! )
            `( dup allnil? !  not )
        while 
        zap
        collect ! ) 
    nest ))

all_cars: 
    (( `( `( car cp 
        <-> zap  ) ... 
        collect ! )
    nest ))

collect: (( `-1 take ))

--allnil?:
--    { 1 <->
--    { car nil? 
--        { zap 0 last }
--        { nil }
--    if} ... }

lsmax: 
    (( `(
        `( ## ) ... 
        collect ! )
    nest
    greatest ! ))

--main: 
--    ( `( 1 3 5 7 9 8 13 6 4 2 0 ) 
--    `( %d cr 
--        iter %d << `": " << 
--    << ) ... )

--main: ( `( `"iter: " << iter $ << ) loop )

-- main: ( `( 1 3 5 7 9 8 13 6 4 2 0 ) greatest ! $ cr << )
--
greatest:    
    (( dup car <-> 
    `( <- dup -> 
    dup <-
        < 
        `( -> zap ) 
        `( zap -> ) 
    if ) ... ))

-- main: ( `1 `2 `3 `( down ) `2 times ustack dump << )

--main: 
--    ( `9
--    `( `1 + `"yo " << )
--    `( dup `10 < )
--    while )

-- main: ( `( 1 2 3 ) nest unnest sdd ! )

-- main: ( `(1 2 3 4) give `-1 take sdd ! )

-- main: ( `( 1 2 3 4 ) `( nil ) ... sdd ! ) 

-- main: ( `1 `2 `3 stkrev ! sdd ! )

-- stkrev: (( collect ! give ))

-- main: ( `1 `nil `3 `( down ) `2 times sdd ! )

-- main: ( `(1 nil 3 nil nil nil nil 4 5 nil 6) nonil ! sdd ! )

nonil:
        (( `( 
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
        reverse )

        nest ))

--main: 
--        (`(1 2 3 4 5) 
--            `(dup `3 == not
--                `( zap )
--                `( last )
--            if) 
--        ... $ cr << )

--main: ( `"hello\n" empty ! $ << zap empty ! $ << )

empty?: (( stack nil? ))

full?:  (( ustack nil? ))

-- main: (  `"    Hello" str2ar dent ! $ << )
--
--dent: (( `0 <-> 
--        `( `0x20 == not
--                `( last )
--                `( `1 + )
--            if )
--        eachar ))

-- main: ( `nil dup nil? $ cr << )

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

--main: ( `x `( `( $ cr << ) ... ) ... )
--
--x: ( (1 2) (3 4) (5 6) )

-- main: ( `(1 2 3 4 nil) pop sdd ! )

--main: ( `( `1 `x + `x set ) loop )
--
--x: {0}

-- main: ( `0 `( `1 + ) `-1 times $ cr << )

--main: ( `(15 15 15 15) sum ! %d cr << )
--
--sum: (( `0 <-> `( + ) ... ))

--main: ( `(123 456 789 987 654 321) unload listobj ! )

-- main: ( `"fnord" `"frobnicate" `9 paste8 cr << )

--main: ( `[6 7] `[1 2 3 4 5] `2 paste $ cr << )

-- main: ( `(1 2 3) reverse sdd ! )

-- main: ( `0 `"hello" `"world" sel << )

--main: 
--    ( `1 `2 `3 `4 `5 `-1 take unload 
--    `(1 2 3 4 5) unload 
--    == 
--    not
--    <- `"take - test #0" -> 
--    test_finish ! )
--
--test_finish: 
--    (( <- << ->
--        `" FAILED"
--        `" PASSED"
--    sel
--    cr << ))

-- main: ( `1 `2 `3 `( 4 5 6 ) nest up sdd ! up $ << die stack dump << )

--main: ( `x cr << `x cr <<  )
--
--x: {"Hello"}

--main: ( `x `(4 5 6) push `x set `x dump << )
--
--x: (nil)

--main: ( `10 `x += `x $ cr << )
--
--x: {0}

--main: ( `(1 2 3) `(4 5 6) push dump << )

--main: ( `x `0 + `nil cons sdd ! )
--
--x: {0xf}
--
--d: (($ cr <<))



--main: ( readfile !
--        dup
--
--        `is_newline
--        matchar !
-- 
--        carve !
--
--        clean_nil !
--    
--        `( dup `is_space matchar ! carve ! ) ...
--
--        collect !
--
--        `( `( ar2str cr << ) ... ) ... )
--
--collect: (( `-1 take reverse ))
--
--clean_nil: (( nest
--            collect !
--            `( dup nil? 
--                `( nil )
--                `( zap ) 
--            if ) ... up ))
--
---- TOS-0: list of offsets in monotonic order
---- TOS-1: array to be carved
--carve: 
--        (( dup nil?
--            `( lsinc !
--            reverse
--            `2 take nest 
--                `( dup `1 - 
--                <- cut <-> -> 
--                cut 
--                zap ) 
--            ... 
--            collect !
--            give 
--            up )
--            `( (nil) )
--        if ))
--
--lsinc: (( dup `( `1 <-> += ) ... ))
--
--lsprint: (( `( %d `" " . << ) ... ))
--
--readfile: ((argv `1 th >>> str2ar))
--
--is_space:   ( `0x20 == not )
--
--is_newline: ( dup `0xa == not <-> `0xd == not or )
--
--matchar: 
--    ((  `0 `matchar_x set
--        `matchar_fn set
--        dup nil?
--            -- if nil:
--            `( `nil cons nest
--                `( `matchar_fn !
--                    `( zap )
--                    `(  `matchar_y `matchar_x `0 + 
--                        `nil cons push zap )
--                    if
--                `1 `matchar_x += )
--            eachar
--            `matchar_y shift zap up car )
--            -- else:
--            `( (nil) )
--    if ))
--
--matchar_fn: ( nil )
--
--matchar_x: {0}
--
--matchar_y: ( nil )



-- main: ( `( cr << ) `( "hello" "world" ) each fnord )

-- main: ( `( cr << ) `[ "hello" "world" ] eachar fnord )

-- main: ( `( ("hello ") << ) `3 times fnord )

--main: ( `{1 2 3} cp 
--        `[1 2 3] cp 
--        `(1 2 3) cp
--        `x
--        `"one" `0x11111111 inskha 
--        `"two" `0x22222222 inskha 
--        `"three" `0x33333333 inskha
--        cp
--        stack dump << )
--
--x: [nil nil]

--main: ( `x 
--        `"xyz" `0xaaaa inskha 
--        `"abd" `0xbbbb inskha 
--        `"ghi" `0xcccc inskha
----        `"jkl" `0xdddd inskha 
----        `"mno" `0xeeee inskha 
----        `"pqr" `0xffff inskha
----        `x set
--        `x `"abd" luha $ cr << )
--
--x: [nil nil]

-- main: ( `{7 8} `x set `x $ cr << )
-- x: 1

-- main: ( `1 `"hello\n" `"world\n" sel <<  )

-- main: ( `( ("hello\n") ) `( ("world\n") ) `1 if <<  )

-- main: ( fnord nil fnord )

-- main: ( `( fnord ) loop )

-- main: ( fnord `x goto )
-- x: ( fnord )

-- main: ( `( fnord  `( `( fnord ) ! fnord ) ! ) ! fnord )

-- main: ( `{7 8} `{1 2 3 4 5} `4 paste $ cr << )

-- main: ( `{7 8} `{1 2 3 4 5} `3 paste $ cr << )

-- main: ( `nil nil? $ << )

-- main: ( `[7 8] `[1 2 3 4 5] `3 paste $ cr << )

-- main: ( `( (1 2) (3 4) (5 6) ) `(1 0) walk $ cr << )

-- main: ( `( 1 2 3 4 5 6 ) `4 ith $ cr << )

-- main: ( `{ 1 2 3 4 5 6 } `4 th $ cr << )

-- main: ( `[ 1 2 3 4 5 6 ] `4 th $ cr << )

-- main: ( `[ {1 2} {3 4} {5 6} ] `(1 0) trav $ cr << )

-- main: ( `[ [1 2] [3 4] [5 6] ] `(1 0) trav $ cr << )

-- main: ( `17 `(1 2 x&) del dump << )

--main: ( `17 `[x&]
--        dup mu    `"mu   : " << $ << `"\n" << 
--        dup nin   `"nin  : " << $ << `"\n" << 
--        dup nlf   `"nlf  : " << $ << `"\n" << 
--        dup nhref `"nhref: " << $ << `"\n" << 
--        dup nva   `"nva  : " << $ << `"\n" << 
--        dup npt   `"npt  : " << $ << `"\n" << )

--main: ( `0x1234 `0x4321 + `0x5678 `0x7 cushl `0xff + - $ << `"\n" << )


-- x: [nil nil]

--main: ( `0x17
--        `nil 
--        `"abc" `0xaaaa inskha 
--        `"def" `0xbbbb inskha 
--        `"ghi" `0xcccc inskha
--        `"jkl" `0xdddd inskha 
--        `"mno" `0xeeee inskha 
--        `"pqr" `0xffff inskha
--        del
--        dump << )
--
--main: ( `nil `"xyz" `0xbabeface inskha 
--             `"abc" `0xdeadbeef inskha
--        dump << `"\n" << )
--

sdd: (( sdp ! die ))

sdp: (( stack dump << ))


