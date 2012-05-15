main: ( `( `1 `x + `x set ) loop )

x: {0}

-- main: ( `0 `( `1 + ) `-1 times $ cr << )

--main: ( `(15 15 15 15) sum ! %d cr << )
--
--sum: (( `0 <-> `( + ) ... ))

--main: ( `(123 456 789 987 654 321) unload listobj ! )
--
--listobj: (( `( hex08 ! cr << ) eachar ))
--
--hex08: 
--        (( %x dup #8 
--        `8 <-> -
--        <-
--            `"00000000" dup <- zap -> 
--        ->
--        paste8 ))

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
--        newlines !
--        sdd !
--        lsinc !
--
--        reverse
--
--        <- `( cut <-> ) -> ... 
--
--        `-1 take
--        reverse
--
--        <- `( `"==>" << ar2str << `"\n" << ) -> ... )
--
--lsinc: (( dup <- `( `1 <-> += ) -> ... ))
--
--retrieve: (( <- `( %d `" " . << ) -> ... ))
--
--part_x: ( nil )
--
--lsprint: (( <- `( %d `" " . << ) -> ... ))
--
--readfile: ((argv `1 th >>> str2ar))
--
--newlines: 
--    (( `nil cons nest
--        <- `( `0xd == not
--                `( zap )
--                `( `newlines_y `newlines_x `0 + `nil cons push zap )
--            if
--            `1 `newlines_x += ) -> 
--    eachar
--    `newlines_y shift zap up car ))
--
--newlines_x: {0}
--
--newlines_y: ( nil )

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


