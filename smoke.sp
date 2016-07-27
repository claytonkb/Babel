-- BUG LIST:
-- split_d causing seg-fault when executed from a .bbl

{ 'std.sp' >>> sexpr bpdli eval

global_fail 0 < 

pass_fail { { "FAIL " << global_fail dup 1 + < } { "PASS " << } selr ! } <

optest
        { give
        << ":" <<
        <- <- give ->
              eval ->
        1 <-
            {cmp -> and <-} each
        ->
        pass_fail ! }
    <

-- operator list (test vectors)
   (( (0x3f4c0fab 0x6abd08a5) {cadd_iod} (0xaa091850) "cadd_iod"  )
    ( (0xfb50be0c 0x90b5c67a) {csub_iod} (0x6a9af792) "csub_iod"  )
    ( (0x60801950 0xf416b1a4) {cmul_iod} (0xd0708740) "cmul_iod"  )
    ( (0x1c89     0x70db    ) {cmul_iod} (0xc945933 ) "cmul_iod"  )
    ( (0x1cb11ddf 0x723e65  ) {cdiv_iod} (0x40      ) "cdiv_iod"  )
    ( (0xe1bebfe6 0x8557c9  ) {cdiv_iod} (0xffffffc6) "cdiv_iod"  )
    ( (           0x8ef77656) {cabs_iod} (0x710889aa) "cabs_iod"  )
    ( (           0x4a1f5709) {cabs_iod} (0x4a1f5709) "cabs_iod"  )
    ( (0x8e038a45 0x77fa    ) {crem_uod} (0x4c63    ) "crem_uod"  )
    ( (0xa9b4882f 0x3864f3bc) {crem_iod} (0xe2197beb) "crem_iod"  )
    ( (0xfdb02b8d 0x99004ae5) {cshl_uod} (0xb60571a0) "cshl_uod"  )
    ( (0x9ea11bc2 0x35136ba7) {crol_uod} (0x508de14f) "crol_uod"  )
    ( (0xd19c0023 0xd7da3f2a) {cshr_uod} (0x346700  ) "cshr_uod"  )
    ( (0x612dcf59 0xbcbc328f) {cshr_iod} (0xc25b    ) "cshr_iod"  )

    ( (0x9b243ba9 0xc85683a7) {logicF0_od} (0x0       ) "logicF0_od"  )
    ( (0x1b2b8814 0x67a30a79) {logicF1_od} (0x3230810 ) "logicF1_od"  )
    ( (0xa32fa0b5 0x927b4ec0) {logicF2_od} (0x10504e40) "logicF2_od"  )
    ( (0x5004371a 0x6ec74bac) {logicF3_od} (0x6ec74bac) "logicF3_od"  )
    ( (0x49e02ba6 0x10d2579 ) {logicF4_od} (0x48e00a86) "logicF4_od"  )
    ( (0x7dfea891 0xe7045a97) {logicF5_od} (0x7dfea891) "logicF5_od"  )
    ( (0x2f8b028e 0x85733075) {logicF6_od} (0xaaf832fb) "logicF6_od"  )
    ( (0x51e3e0c7 0x86f67258) {logicF7_od} (0xd7f7f2df) "logicF7_od"  )

    ( (0xc760c38e 0x22e51ca2) {logicF8_od} (0xdf7ae3df) "logicF8_od"  )
    ( (0xfe77943f 0xe2dba6c2) {logicF9_od} (0xe353cd02) "logicF9_od"  )
    ( (0xe0006951 0x3ad64a28) {logicFa_od} (0x1fff96ae) "logicFa_od"  )
    ( (0xa5a71d3e 0x6a605a04) {logicFb_od} (0x7a78fac5) "logicFb_od"  )
    ( (0x1afb3dd5 0xa7533f6c) {logicFc_od} (0x58acc093) "logicFc_od"  )
    ( (0xd72d8db9 0x1ff667fe) {logicFd_od} (0xf72d9db9) "logicFd_od"  )
    ( (0x3d013bf0 0x206a6881) {logicFe_od} (0xdfffd77f) "logicFe_od"  )
    ( (0x82f893a5 0x6d17f9b ) {logicFf_od} (0xffffffff) "logicFf_od"  )

    ( (           0x316cc247) {cnot_od}    (0xce933db8) "cnot_od"     )
    ( (           0x97c8b334) {not_d}      (0x0       ) "not_d"       )
    ( (           0x0       ) {not_d}      (0x1       ) "not_d"       )
    ( (0xb07ebc80 0x1c360373) {and_d}      (0x1       ) "and_d"       )
    ( (0x0        0xaa390c2a) {and_d}      (0x0       ) "and_d"       )
    ( (0x0        0x0       ) {or_d}       (0x0       ) "or_d"        )
    ( (0x0        0x49819cc5) {or_d}       (0x1       ) "or_d"        )
    ( (0xe2a19cfd 0x914b3e16) {xor_d}      (0x0       ) "xor_d"       )
    ( (0x0        0x256de330) {xor_d}      (0x1       ) "xor_d"       )

    ( (0x23b7fb30 0xad0d8a82) {clt_iod}    (0x0       ) "clt_iod"     )
    ( (0x9e3edb93 0x29e2c7ba) {clt_iod}    (0x1       ) "clt_iod"     )
    ( (0x66b04b68 0x17301069) {cle_iod}    (0x0       ) "cle_iod"     )
    ( (0xdc83b61c 0xebb4e184) {cle_iod}    (0x1       ) "cle_iod"     )
    ( (0xf7799abe 0x6894e771) {cgt_iod}    (0x0       ) "cgt_iod"     )
    ( (0x4d42dd25 0xcabe9096) {cgt_iod}    (0x1       ) "cgt_iod"     )
    ( (0xb690b6e6 0xd03e88c4) {cge_iod}    (0x0       ) "cge_iod"     )
    ( (0x17cbfc1b 0xdef53aa6) {cge_iod}    (0x1       ) "cge_iod"     )
    ( (0x106b966f 0x106b966f) {cne_iod}    (0x0       ) "cne_iod"     )
    ( (0xe76f0865 0x980d52e3) {cne_iod}    (0x1       ) "cne_iod"     )
    ( (0xd165906a 0x412d9716) {ceq_od}     (0x0       ) "ceq_od"      )
    ( (0x8d9c01a9 0x8d9c01a9) {ceq_od}     (0x1       ) "ceq_od"      )

    ( (              [1 2 3]) {mu_d}       (16        ) "mu_d"        )
    ( (              [1 2 3]) {sfield_d}   (0xc       ) "sfield_d"    )
    ( (        ['a' 'b' 'c']) {sfield_d}   (0xfffffff4) "sfield_d"    )
    ( (      [tag 'foo' nil]) {sfield_d}   (0x0       ) "sfield_d"    )

    ( (              [1 2 3]) {isval_d}    (0x1       ) "isval_d"     )
    ( (        ['a' 'b' 'c']) {isval_d}    (0x0       ) "isval_d"     )
    ( (      [tag 'foo' nil]) {isval_d}    (0x0       ) "isval_d"     )

    ( (              [1 2 3]) {isptr_d}    (0x0       ) "isptr_d"     )
    ( (        ['a' 'b' 'c']) {isptr_d}    (0x1       ) "isptr_d"     )
    ( (      [tag 'foo' nil]) {isptr_d}    (0x0       ) "isptr_d"     )

    ( (              [1 2 3]) {istag_d}    (0x0       ) "istag_d"     )
    ( (        ['a' 'b' 'c']) {istag_d}    (0x0       ) "istag_d"     )
    ( (      [tag 'foo' nil]) {istag_d}    (0x1       ) "istag_d"     )

    ( (                  nil) {is_nil}   (0x1       ) "istag_d"     )
    ( (      [tag 'foo' nil]) {is_nil}   (0x0       ) "istag_d"     )

    ( (          (1 [2 3] 4)) {nva_d}      (0x4       ) "nva_d"       )
    ( (          (1 [2 3] 4)) {npt_d}      (0x6       ) "npt_d"       )
    ( (          (1 [2 3] 4)) {nlf_d}      (0x3       ) "nlf_d"       )
    ( (          (1 [2 3] 4)) {nin_d}      (0x3       ) "nin_d"       )

    ( (               3) {newptr_d} ([ptr nil nil nil]) "newptr_d"    )
    ( (               3) {newval_d} ([val   0   0   0]) "newval_d"    )

    ( (    [ptr 1 [2 3] 4] 1) {th_rmd}   ([2 3]       ) "th_rmd"      )
--    ( ([1 2 3] 1 [4 5 6] 0 2) {move_md}  ([1 4 5]     ) "move_md"     )
    ( (  [ptr 1 2 [3 4] 5 6]) {arlen_md} (5           ) "arlen_md"    )
    ( (      "abcdefghijklm") {arlen_8d} (13          ) "arlen_8d"    )

    ( (            [1 2 3] [4 5 6]) {cat_md} ([1 2 3 4 5 6]) "cat_md" )
    ( (['a' 'b' 'c'] ['d' 'e' 'f']) {cat_md} (['a' 'b' 'c' 'd' 'e' 'f']) "cat_md" )
    ( (      "abcdefgh" "ijklmnop") {cat_8d} ("abcdefghijklmnop") "cat_8d" )

    ( (      [1 2 3 4 5 6] 1 4)     {slice_md} ([2 3 4])   "slice_md" )
    ( (        [1 2 3] [1 2 3])     {arcmp_md} (0)         "arcmp_md" )
    ( (        [1 2 3] [4 5 6])     {arcmp_md} (-1)        "arcmp_md" )
    ( (          [1 2 3 4 5 6])     {ar2ls_d}  ((1 2 3 4 5 6)) "ar2ls_d" )
    ( (       [0x61 0x62 0x63])     {ar2str_d} ("abc")     "ar2str_d" )

--    ( ( [0xfa926faa 0xb0b79440 0xc4c91e4a 0xc68d1b3d 0x3f58b06b 0x465685a 0x80789da5 0x39d7cb81 0x998d1970 0x54981d8f ] )
--        {dup sortlf_d} 
--        ([0x465685a 0x39d7cb81 0x3f58b06b 0x54981d8f 0x80789da5 0x998d1970 0xb0b79440 0xc4c91e4a 0xc68d1b3d 0xfa926faa ] )
--        "sortlf_d" )

    ( (    [ptr [1 2] [3 4] [5 6]] [2 0]  )  {get}                 (5)                       "get" )
    ( (    [ptr [1 2] [3 4] [5 6]] [2 0] 7)  {<- <- dup -> -> set} ([ptr [1 2] [3 4] [7 6]]) "set" )

    ( (               (1 2 3) ) {car_rd}    (1)              "car_rd" )
    ( (               (1 2 3) ) {cdr_rd}    ((2 3))          "cdr_rd" )
    ( (               1 (2 3) ) {cons_d}    ((1 2 3))        "cons_d" )
--    ( (               (1 2 3) ) {uncons_d}  (1 (2 3))      "uncons_d" )
    ( (               (1 2 3) ) {pop_d}     ((1)(2 3))        "pop_d" )
    ( (              (2 3)(1) ) {push_d}    ((1 2 3))        "push_d" )
    ( (               (1 2 3) ) {shift_d}   ((3)(1 2))      "shift_d" )
    ( (              (1 2)(3) ) {unshift_d} ((1 2 3))     "unshift_d" )

    ( (          (1 [2 3] 4) 1) {ith_rd}    ([2 3])          "ith_rd" )
    ( (          (1 2 3 4 5 6)) {len_d}     (6)               "len_d" )
    ( (          (1 2 3 4 5 6)) {ls2lf_d}   ([1 2 3 4 5 6]) "ls2lf_d" )
    ( (('a' 'b' 'c' 'd' 'e' 'f' 'g')) {bons_d} (['a' 'b' 'c' 'd' 'e' 'f' 'g']) "bons_d" )
--    ( (    (1 2 3 4 5 6) (1 4)) {split_d}   (((1)(2 3 4)(5 6)))  "split_d" )
    ( (      ((1)(2 3 4)(5 6))) {append_d}  ((1 2 3 4 5 6))     "append_d" )
    ( (        (1 2 3) (4 5 6)) {ins_d}     ((1 4 5 6 2 3))        "ins_d" )
    ( (          (1 2 3 4 5 6)) {rev_d}     ((6 5 4 3 2 1))        "rev_d" )

    ( (          [1 2 3 4 5 6]) {hash_md}   ([0x7fd3852  0x4a3caa73 0x92a91f6  0xd7ec36b1]) "hash_md" )
    ( (        "abcdefghijklm") {hash_8d}   ([0xed0f5530 0x2f51e415 0x28fcae96 0xb3572d23]) "hash_8d" )

    ( (                  "abc") {str2ar_d}     ([0x61 0x62 0x63])      "str2ar_d" )
    ( (            "-92375234") {str2num_id}   ([0xfa7e773e])        "str2num_id" )
    ( (             "92375234") {str2num_id}   ([0x58188c2])         "str2num_id" )
    ( (           "0xfbd8f1be") {str2num_xd}   ([0xfbd8f1be])        "str2num_xd" )
    ( (           "5241661037") {str2num_od}   ([0x2a87621f])        "str2num_od" )
    ( ("11101000001110100100110111010") {str2num_bd} ([0x1d0749ba])  "str2num_bd" )

    ( (             0xfa7e773e) {num2str_id}   ("-92375234")         "num2str_id" )
    ( (              0x58188c2) {num2str_id}   ("92375234")          "num2str_id" )
    ( (             0xfbd8f1be) {num2str_xd}   ("fbd8f1be")          "num2str_xd" )
    ( (             0x2a87621f) {num2str_od}   ("5241661037")        "num2str_od" )
    ( (             0x1d0749ba) {num2str_bd}   ("11101000001110100100110111010")  "num2str_bd" ))

    { optest ! } 
    ... 

    -- hash-table operators
--    smoke_map (('foo' 0x123) ('bar' 0x456) ('baz' 0x789)) ls2map ! <
--
--    "ls2map:" <<
--
--    [val 0x00000000 0xa897520c 0x49a38104 0x583f7095 0x44c9db7f 0xfffffffc 0x00000020 0xfffffff8 
--         0x0000002c 0x00000124 0xfffffff8 0x00000038 0x000000bc 0x00000000 0x591589e0 0x1a9ec87a 
--         0x078adc82 0xdbae9012 0xfffffffc 0x00000054 0xfffffff8 0x00000060 0x00000074 0x00000010 
--         0xa479b720 0x6d8d8f0e 0x15cb2f71 0xb87e13a2 0xfffffff8 0x00000080 0x0000008c 0x00000008 
--         0x006f6f66 0xff000000 0xfffffff8 0x00000098 0x000000a0 0x00000004 0x00000123 0x00000000 
--         0x3023f4e7 0x8c2f644d 0x71cf647b 0xe974b23a 0xfffffffc 0x000000a0 0x00000000 0x591589e0 
--         0x1a9ec87a 0x078adc82 0xdbae9012 0xfffffffc 0x000000d8 0xfffffff8 0x000000e4 0x000000f8 
--         0x00000010 0x7b230d72 0x307f4576 0x98305e48 0xc01208d2 0xfffffff8 0x00000104 0x00000110 
--         0x00000008 0x007a6162 0xff000000 0xfffffff8 0x0000011c 0x000000a0 0x00000004 0x00000789 
--         0x00000000 0x591589e0 0x1a9ec87a 0x078adc82 0xdbae9012 0xfffffffc 0x00000140 0xfffffff8 
--         0x0000014c 0x00000160 0x00000010 0xa5b49fe9 0x2826ffed 0x78e1c638 0xb4e21669 0xfffffff8 
--         0x0000016c 0x00000178 0x00000008 0x00726162 0xff000000 0xfffffff8 0x00000184 0x000000a0 
--         0x00000004 0x00000456 ]
--
--    smoke_map unload cmp
--    pass_fail !
--
--    "lumap:" <<
--    smoke_map 'foo' lumap ! 0x123 cmp
--    pass_fail !
--
--    "insmap:" <<
--    smoke_map 'bop' 0xabc insmap !
--    smoke_map 'bop' exmap ! 1 cmp
--    pass_fail !
--
--    "exmap:" <<
--        smoke_map 'foo' exmap ! 1 cmp
--        smoke_map 'fop' exmap ! 0 cmp
--    and
--    pass_fail !
--
--    "rmmap:" <<
--        smoke_map 'foo' rmmap ! 1 cmp
--        smoke_map 'foo' exmap ! 0 cmp
--        smoke_map 'fop' rmmap ! 0 cmp
--    and and
--    pass_fail !

   -- stack operators
    "swap:" <<
    0x123 0x456 swap 2 take (0x456 0x123) cmp
    pass_fail !

    "zap:" <<
    clear
    0x123 0x456 zap -1 take (0x123) cmp
    pass_fail !

    "dup:" <<
    clear
    0x123 dup 0x123 cmp <- 0x123 cmp -> and
    pass_fail !

    "up/down:" <<
    clear
    0x123 0x456 0x789 down down up -1 take (0x123 0x456) cmp
    pass_fail !

    "flip:" <<
    clear
    0x123 0x456 0x789 0xabc down down flip -1 take (0xabc 0x789) cmp
    pass_fail !

    "rot:" <<
    clear
    0x123 0x456 0x789 0xabc down down rot -1 take (0x789 0xabc) cmp
    pass_fail !

    "twist:" <<
    clear
    0x123 0x456 0x789 0xabc down down twist -1 take (0x456 0x123) cmp
    pass_fail !

    "depth:" <<
    clear
    0x123 0x456 0x789 0xabc down down depth 2 cmp
    pass_fail !

    "dia:" <<
    clear
    0x123 0x456 0x789 0xabc down down dia 4 cmp
    pass_fail !

    "give:" <<
    clear
    (0x123 0x456 0x789 0xabc) give
    0xabc cmp <-
    0x789 cmp <-
    0x456 cmp <-
    0x123 cmp <-
    flip -1 take {and} fold !
    pass_fail !

--    -- misc tests
--    "blns.txt:" <<
--    'blns.txt' >>> dup str2ar ar2str cmp
--    pass_fail !

    "return:" <<
    (1 2 3) {return} mkbvm2 ! babel 3 cmp
    pass_fail !

    "last:" <<
    { 0x91c81eed last 1 + } 2 times 0x91c81eed cmp
    pass_fail !

    -- loop operators
    "each:" <<
    0 (1 2 3) {+} each 6 cmp
    pass_fail !

    "eachar:" <<
    0 [1 2 3] {+} eachar 6 cmp
    pass_fail !

    "times:" <<
    0 1 2 3 {+} 3 times 6 cmp
    pass_fail !

    -- NOT COVERED --
    -- I/O operators

    "\nTOTAL FAILURES: "     <<
    global_fail itod << "\n" <<

--22 dev
--
-- dup 57 th show say !
-- dup 58 th show say !
-- dup 59 th show say !
--
-- dup 60 th show say !
-- dup 61 th show say !
--     62 th show say !
--
--16 dev

    clear }


