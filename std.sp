-- Creates a map (hash-table) hash-table from a list: ((key1 val1) (key2 val2) ...)
-----------------------------------------------------------------------------
--                                                                         --
--                                rc.sp                                    --
--                                                                         --
-----------------------------------------------------------------------------

-- Loads a .sp file
-- "foo.sp" include !
-- Creates a map (hash-table) from a list: ((key1 val1) (key2 val2) ...)
{ls2map 
    { cp 
    <- newha ->
    { <- dup   ->
        dup  1 ith
        down 0 ith ->
        inskha }
    each}
    <

-- Takes a list 2 elements at a time and returns a list-of-lists
-- (1 2 3 4 5 6) lsby2 !  -->  ((1 2) (3 4) (5 6))
lsby2 
        { { { iter 1 + 2 rem
            { 2 take }
            { fnord  }
        selr eval }
        each
        collect ! }
    nest }
    <

-- Creates a map (hash-table) from a flat list: (key1 val1 key2 val2 ...)
mkmap
    { lsby2 ! ls2map ! }
    <

-- Get all string keys in a map in list form
keymap
    { entsha 1 matcol ! }
    <

-- Look-up a key in a map: <map> "foo" lumap !
lumap
    { hash luha 2 ith }
    <

-- Returns the ith column in a matrix
-- ((1 2) (3 4) (5 6)) 0 matcol !  -->  (1 3 5)
matcol
        { 2 take
        { give <-
            { -> dup <- ith }
        each
        collect ! }
    nest }
    <

-- Collects entire dstack into a list
collect
    {-1 take}
    <

-- Imports a hash-table into the symbol table
import
    { dup keymap ! { dup <- <- dup -> lumap ! -> insym } ... zap }
    <

mkbvm2_soft_root [tag "/babel/tag/trie" ptr () ()] <

-----------------------------------------------------------------------------
--                                                                         --
--                            rc import list                               --
--                                                                         --
-----------------------------------------------------------------------------
-- Includes a bpdl file and loads it as a bstruct
('include' 
    { >>> sexpr bpdli }

-- Loads a .bbl file
-- > 'foo.bbl' loadfs !
'loadfs' 
    { slurpm load }

-- Saves a bstruct to .bbl file
-- > (1 2 3) 'foo.bbl' savefs !
'savefs' 
    { <- unload -> spitm }

-- Compiles a .sp file to a .bbl BVM (run with -b switch)
-- 'foo.sp' 'foo.bbl' compile !
'compile'
    { <- include ! mkbvm ! unload -> spitm }

-- Applies code to a list with folding (left-associative)
-- (1 2 3) {+} fold !  -->  1 2 + 3 +
-- (3 2 1) {-} fold !  -->  3 2 - 1 -
'fold'
    { <- pop car swap -> ... }

-- Applies code to a list, accumulating a sequence-of-terms list as a result
-- (1 1 1 1) {+} sigma !   --> (1 2 3 4)
'sigma'
    { 2 take { give <- pop car swap {<- dup ->} cp -> compose ! each collect ! } nest }

-- Applies code to a list, differentiating a sequence-of-terms list as a result
-- (1 2 3 4) {-} delta !   --> (1 1 1)
'delta'
    { 2 take { give 
        <- pop car swap {dup <- swap} cp -> 
        {->} cp
        compose ! 
        compose ! 
        each zap 
        collect !} 
    nest }

-- Collects entire dstack into a list
'collect' 
    {-1 take}

-- Take n items as a ptr-array
'takeptr' 
    {take bons}

-- Take n items as a val-array
'takeval' 
    {take ls2lf}

-- Wrap TOS in parentheses
-- X wrap ! --> (X)
'wrap' 
    {1 take}

-- Wrap TOS in ptr-array
-- X wrapptr ! --> [ptr X]
'wrapptr' 
    {1 take bons}

-- Note: internal utility, not intended for end-use
'wrapcode' 
    { wrap ! wrap ! }

-- Evaluate code over a list and collect the results back into a list
-- (1 2 3) {1 +} over !  --> (2 3 4)
'over'   
    { 2 take { give each collect ! } nest }

-- Similar to each but copies a single object for repeated use
-- 'a' ('b' 'c' 'd') {.+} reduce !  --> "abcd"
'reduce'
    { <- { <- dup -> } cp -> compose ! each }

-- Filter a list based on truth-value returned by code evaluated on each
-- element of the list
'filter'   
    { <- {dup} cp -> {true? {zap} {fnord} selr !} cp compose ! compose ! over ! }

-- Perform an operation over an array and collect the results as a ptr-array
-- [ptr 1 2 3] {1 +} overptr !  --> [ptr 2 3 4]
'overptr' 
    { 2 take { give eachar -1 takeptr ! } nest }

-- Perform an operation over an array and collect the results as a val-array
-- [1 2 3] {1 +} overval !  --> [2 3 4]
'overval' 
    { 2 take { give eachar -1 takeval ! } nest }

-- Returns true if any code-list returns true; otherwise, returns false
-- X ({Y} {Z}) any?
-- NOTE: does not short-circuit
'any?'   
    { 2 take { give {<- dup -> ! <-} each flip collect ! {or} fold ! } nest }

-- Returns true if all code-lists return true; otherwise, returns false
-- X ({Y} {Z}) all?
-- NOTE: does not short-circuit
'all?'   
    { 2 take { give {<- dup -> ! <-} each flip collect ! {and} fold ! } nest }

-- Generates a bstruct-signature
'bsig' 
    { unload hashm }

-- Duplicate the two top items on the stack
--  A B --> A A B B
'dup2'
    { <- dup -> dup }

-- Nest with no arguments
--      { { 1 randlf 10 rem } 100 times collect ! } nest0 !
'nest0'
    { <- nil {zap} cp -> compose ! nest }

'carray'
    { unload unload {"0x" << hex08 ! << ", " <<} eachar "\n" << }

-----------------------------------------------------------------------------
--                                                                         --
--                           dev utilities                                 --
--                                                                         --
-----------------------------------------------------------------------------

-- Evals a code-list and prints the elapsed time (in milliseconds)
'timed' 
    { timems <- ! -> timems swap - itod say ! }

-- Total mem usage
'tmu' 
    { 2 dev mu }

-- Measures in-use memory-usage
'delta_tmu' 
    { tmu ! <- ! -> tmu ! swap - itod say ! }

-- Total mem-alloc'd (in-use or not)
'tma'
    { 23 dev dup <-
    51 th
    -> dup <- 53 th + 
    -> 52 th - }

'spaced'
    { tma ! <- ! -> tma ! swap - itod say ! }

-- Debug utility
'dump'
    { bs2gv "test.dot" <<< }

'cache_stats_labels'
    ("self" "code_ptr" "rstack_ptr" "dstack_ptr" "ustack_ptr" "sym_table"
    "soft_root" "local_root" "local_path" "local_pwd" "thread_id" "thread_id value"
    "steps" "steps value" "advance_type" "advance_type value"  "bvm_initd"
    "dstack_depth" "dstack_diameter" "mask_table" "flags" "MC_ALLOC_BLOCKING"
    "MC_GC_BLOCKING" "MC_GC_PENDING" "MC_GC_CONVERT_PENDING_TO_OP_RESTART"
    "MC_GC_PNR" "MC_GC_OP_RESTART" "INTERP_BOOT_IN_PROGRESS"
    "BVM_INSTR_IN_PROGRESS" "BVM_INCOHERENT" "BVM_SYMBOLS_DEFINED" "BVM_CLEAN"
    "BVM_INTERP_OP_TRACE" "BVM_CODE_LIST_EMPTY" "BVM_RETURN_TOS_ON_EXIT"
    "BVM_CACHE_INVALID" "BVM_CACHE_DIRTY" "BVM_CACHE_ONLY" "BVM_CACHE_BLOCKING"
    "NO_ASYNC" "NO_EXCEPT" "MC_USE_MALLOC" "interp" "cat_ex" "argc" "argv" "envp"
    "interp_argv" "mem" "nil" "jump_table" "empty_string" "utc_epoch" "srand"
    "null_hash" "thread_counter" "global_tick_count" "primary->base_ptr"
    "primary->alloc_ptr" "primary->size" "secondary->base_ptr"
    "secondary->alloc_ptr" "secondary->size" "GC_count" "op_restart_alloc_size"
    "last_GC_tick_count")

'cache_dump'
    { 23 dev ar2ls
    cache_stats_labels
    lspair ! 
    { give << " : " << show << "\n" << } each }

'cache_table'
    { cache_stats_labels
    23 dev ar2ls
    lspair ! 
    ls2map ! }

-----------------------------------------------------------------------------
--                                                                         --
--                           list utilities                                --
--                                                                         --
-----------------------------------------------------------------------------

-- Joins/appends two lists; works on code-lists as well as ordinary lists
-- {2 3} {+} compose !  --> {2 3 +} 
'compose'
    { 2 take append }

-- Joins two code-lists, where the first code-list is to be eval'd and the
--   second code-list is to perform the eval
-- {+} {over !} compose_eval !  --> { {+} over ! } 
'compose_eval'
    { <- cp wrapcode ! -> cp compose ! }

-- Returns the last element of a list
-- (1 2 3) lsend !  --> (3)
'lsend'
    { dup cdr nil? { cdr lsend ! } { fnord } selr ! }

-- Shows each element of a list
-- (1 2 3) lsshow !  --> ( [val 0x1 ] [val 0x2 ] [val 0x3 ]  )
'lsshow'
    { '( ' << {show <<} each ' )/n' << }

-- Prints each element of a list of strings (all elements must be strings)
-- ("foo" "bar" "baz") lsstr ! 
--    ( "foo" "bar" "baz" ) 
'lsstr'
    { '( ' << prq ! ')/n' << }

-- Prints each element of a list of numbers (all elements must be values)
-- (1 2 3) lsnum !  
--    ( 1 2 3 )
'lsnum'
    { '( ' << prd ! ')/n' << }

-- Compares two lists element-wise and returns a list containing the results
-- (1 2 3) (1 2 3) lscmp !  --> ( 1 1 0 )
'lscmp'
    { { cmp } lswise ! }

-- Takes a list n elements at a time and returns a list-of-lists
-- (1 2 3 4 5 6) 3 group !  -->  ((1 2 3) (4 5 6))
'group'
    { 2 take
        { give <-
        { iter 1 + -> dup <- rem
            {-> dup <- take}
            {fnord}
        selr eval }
        each
        collect ! }
    nest }

-- Pairs off two lists, element-by-element; returns list-of-lists
-- Equivalent to 'map' operator in some other languages
-- (1 2 3) (4 5 6) lspair !  --> ((1 4) (2 5) (3 6))
'lspair'
        { <- cp -> cp 
        2 take 
        { give { <- dup iter ith -> 2 take <- } each zap flip collect ! }
    nest } 

-- Unpairs a list-of-lists into two lists
-- Equivalent to 'unmap' operator in some other languages
-- ((1 4) (2 5) (3 6)) lsunpair !  --> (1 2 3) (4 5 6) 
'lsunpair'
    { dup 
        <- {0 ith} over ! ->
        {1 ith} over !}

-- Rotates a list left by one element
-- (1 2 3 4 5) lsrol !  --> (2 3 4 5 1)
'lsrol'
    { pop unshift }

-- Rotates a list right by one element
-- (1 2 3 4 5) lsror !  --> (5 1 2 3 4)
'lsror'
    { shift push }

-- Rotates a list right by n elements
-- (1 2 3 4 5) 2 lsrotn !  --> (4 5 1 2 3)
'lsrotn'
    { <- dup len -> - wrap ! split rev append }

-- Evaluates code on two lists, element-wise and returns a list result
-- (1 2 3) (4 5 6) {+} lswise ! --> (5 7 9)
'lswise'
    { <- lspair ! {give} cp -> compose ! over ! }

-- Evaluates code on two lists, element-wise - does NOT return a list result
-- (foo bar baz) (1 2 3) {<} eachpair !
'eachpair'
    { <- lspair ! {give} cp -> compose ! each }

-- Mixes or selects elements from a list according to an index-list
-- (1 2 3 4) (3 0 1 0 2) lsmix ! --> (4 1 2 1 3)
-- ^src      ^index
'lsmix'
    { 2 take { give {<- dup -> ith <-} each flip collect ! } nest }

-- Performs a cdr i times
-- (1 2 3 4) 2 cdri ! --> (3 4)
'cdri'
    { <- {cdr} -> times }

-- Sets the ith element of a list
-- (1 2 3 4) dup 2  7    lsset ! --> (1 2 7 4)
-- ^dest         ^i ^src
'lsset'
    { 1 takeptr ! <- cdri ! [0] -> set }

-- Swaps two items in a list
-- (6 7 8 9 10) 1 3 lsswap ! --> (6 9 8 7 10)
'lsswap'
    { 3 take dup dup dup
    
    give
    swap zap
    ith 

    <-
    give
    zap
    ith

    <- give swap zap ->
    lsset !

    give zap ->
    lsset ! }

-- Returns true if all elements of list are true - short-circuits
-- (1 1 1) lstrue ! --> 1
-- (1 0 1) lstrue ! --> 0
'lstrue'
    { <- 1 -> {true? {zap 0 last} {fnord} selr !} each }

-- Returns true if all elements of list are false - short-circuits 
-- (0 0 0) lsfalse ! --> 1
-- (0 1 0) lsfalse ! --> 0
'lsfalse'
    { <- 1 -> {false? {zap 0 last} {fnord} selr !} each }

-- Returns a list of integers over a range start from 0 up to the argument
-- 10 lsrange ! --> (0 1 2 3 4 5 6 7 8 9)
'lsrange'
    { { 1 - <- 0 { dup 1 + } -> times collect ! } nest }

-- Sorts a list with user-supplied comparison-code
-- <list> {comparison-code} lssort ! --> <sorted-list>
'lssort'
    { <- bons dup -> merge_sort ar2ls }

-- Duplicates every element of a list
'lsdup'
    { {dup} over ! }

-- Keeps every nth element of a list, where n is even
'lseven'
    { {iter 2 rem {fnord} {zap} selr !} over ! }

-- Keeps every nth element of a list, where n is odd
'lsodd'
    { {iter 2 rem {zap} {fnord} selr !} over ! }

-- Sorts a list of strings lexicographically
-- <list-of-strings> strsort ! --> <sorted-string-list>
'lexsort'
    { { arcmp8 0 lt? } lssort ! }

-- Sorts a list of strings alphabetically 
-- <list-of-strings> strsort ! --> <sorted-string-list>
'strsort'
    { { strcmp8 0 lt? } lssort ! }

-- Slices a string into a list of strings
-- <string> <index-list> strslice ! --> <list-of-strings>
'strslice'
    { dup cp <- shift zap -> pop zap lspair !
    2 take
    { give { <- dup -> give slice8 <- } each flip collect ! } nest }

'strslice_BAD'
    { lsdup ! 
    pop zap shift zap
    2 group !
    2 take
    { give { <- dup -> give slice8 <- } each flip collect ! } nest }

-- Equivalent to Perl's split() with quoted match (no regex support yet)
-- <string> <split-string> strsplit ! --> <list-of-strings>
'strsplit'
    { 2 take dup

        { dup

        <- give strmatch8 dup ->

        {arlen8} over ! give swap

        <- wrapcode ! {+} cp compose ! over ! lspair ! append ->

        <- dup dup len 1 - ith ->
        dup <-
        cmp
            {-> wrap ! unshift}
            {-> zap} 
        selr ! }
    nest

    <- give swap ->

    dup 0 ith
    0 cmp
    {(0) cp push} {fnord}
    selr !

    strslice ! 

    dup <- 0 ith
    cmp    
        {-> lseven !}
        {-> lsodd  !}
    selr ! }

--"/foo/bar/baz" "/" strmatch8 --> (0 4 8)
--( 0 1 4 5 8 9 strlen-1) strslice !

--'lsmod'
--    [\ (x y) { {iter x rem y ne? {fnord} {zap} selr !} over ! } ]

-- Joins a list of strings into a single string
-- <list-of-strings> strjoin ! --> <string>
'strjoin'
    { dup
    {arlen8} over !
    dup cp <- sum ! ' ' newstr swap ->
    {+} sigma !
    shift zap (0) cp push
    swap lspair !
    { <- dup -> give dup arlen8 0 swap move8 } each }

-- Joins a list of strings into a single string
-- <list-of-strings> strjoin ! --> <string>
'lsstrlen'
    { <- 0 -> {arlen8 +} each }

'lsstrlen2'
    { {arlen8} over ! sum ! }

-- General-purpose list-sort; "do what I mean" for most numeric object types 
--   (val-arrays, lists of arrays, lists of lists of arrays, etc.)
-- <list> sort ! --> <sorted-list>
'gpsort'
    { bons dup { <- unload -> unload arcmp 0 lt? } merge_sort ar2ls }

-- Poor man's macro; a generic each macro
'eachify' 
    { wrapcode ! {...} cp compose ! }

-- Poor man's macro; a generic fold macro
'foldify'
    { wrapcode ! <- { pop car swap } cp -> compose ! { ... } cp compose ! }


-----------------------------------------------------------------------------
--                                                                         --
--                          matrix utilities                               --
--                                                                         --
-----------------------------------------------------------------------------

-- Returns the ith column in a matrix
-- ((1 2) (3 4) (5 6)) 0 matcol !  -->  (1 3 5)
'matcol'
        { 2 take
        { give <-
            { -> dup <- ith }
        each
        collect ! }
    nest }

-- Returns the ith row in a matrix
-- ((1 2) (3 4) (5 6)) 1 matrow !  -->  (3 4)
'matrow' 
    {ith}

-- Returns the diagonal of a matrix
-- ((1 2 3) (4 5 6) (7 8 9)) matdiag !  -->  (1 5 9)
'matdiag'
    { { { iter ith } ...
        collect ! }
    nest }

-- Returns the matrix-transpose
-- ((1 2 3) (4 5 6)) mattrans !  -->  ((1 4) (2 5) (3 6))
'mattrans'
    { { dup car len
        <- { dup iter matcol ! swap } ->
    times
    zap
    collect ! 
    rev }
    nest }
-- flip collect ! is faster than collect ! rev

-- Applies an operation to each element of a matrix
-- ((1 2 3) (4 5 6)) {itod << ' ' <<} mateach !
--   1 2 3 4 5 6
'mateach'
    { <- { {-> dup <- !} each } each -> zap }

-- Applies an operation to each element of a matrix and collects the results back into a matrix
-- ((1 2 3) (4 5 6)) {1 +} matover !  --> (( 2 3 4 ) ( 5 6 7 ))
'matover' 
    { cp
      <- dup car len -> swap 
      <- 2 take { give mateach ! collect ! } nest -> 
      group ! }

-- Selects a particular element from a matrix
-- ((6 7) (8 9)) (1 0) matij !  --> 8
--                i j   --> i selects row, j selects column
'matij'
    { give <- ith -> ith }

-- Pairs two matrices (of same dimension) together; see lspair
-- ((1 2) (3 4)) ((5 6) (7 8)) matpair !  -->
--    (((1 5) (2 6))
--     ((3 7) (4 8)))
'matpair'
    { 2 take 
        { give
        { <- dup iter ith -> lspair ! <- } 
        each 
        zap 
        flip collect ! } 
    nest }

-- Evaluates code on two matrices, element-wise
-- ((1 2) (3 4)) ((5 6) (7 8)) {+} matwise !  --> (( 6 8 ) ( 10 12 ))
'matwise'
    { cp <- matpair ! {give} cp -> compose ! matover ! }

-- Matrix addition
'matadd'
    { {+} matwise ! }

-- Matrix subtraction 
'matsub'
    { {-} matwise ! }

-- Compares two matrices element-wise and returns a matrix containing the results
-- ((1 2) (3 4)) ((1 2) (3 5)) matcmp !  --> ((1 1) (1 0))
'matcmp' -- different from standard cmp because element-wise
    { {cmp} matwise ! }

-- Mixes/selects the rows of a matrix according to a mix list
-- ((6 7) (8 9)) (1 0) matmix !  --> ((8 9) (6 7))
'matmix'
    { lsmix ! }

-- Mixes/selects the columns of a matrix according to a mix list
-- ((6 7) (8 9)) (1 0) matcolmix !  --> ((7 6) (9 8))
'matcolmix'
    { <- mattrans ! -> lsmix ! mattrans ! }

-- Joins two matrices row-wise (horizontally)
-- ((1 2) (3 4)) ((5 6) (7 8)) matrowcat !  --> (( 1 2 5 6 ) ( 3 4 7 8 ))
'matrowcat'
    { 2 take { give { <- dup iter ith -> compose ! <- } each flip collect ! } nest }

-- Joins two matrices column-wise (vertically)
-- ((1 2) (3 4)) ((5 6) (7 8)) matcolcat !  --> ((1 2)(3 4)(5 6)(7 8))
'matcolcat'
    { 2 take append }

-- Arranges the terms of two matrices as a matrix-of-lists in the order
-- required for matrix-multiplication
'matapp'
    { 2 take cp 
        { give 
        dup 0 ith len 
        <- 
            <- mattrans ! -> cart ! {give lspair !} each collect ! 
        -> 
        group ! } 
    nest 
    mattrans ! }

-- Multiplies two square matrices of same dimension, order 2 or greater
-- ((1 2) (3 4)) ((5 6) (7 8)) matsqmul ! --> (( 23 34 )( 31 46 ))
'matsqmul'
    { matapp ! { { { {*} fold ! } over ! {+} fold ! } over ! } over ! }

-- Returns the dimensions of a matrix in the form: [val rows cols]
-- ((1 2) (3 4) (5 6)) matdim !  --> [val 3 2]
'matdim'
    { dup <- len -> car len 2 takeval ! }

'matrowvec?'
    { matdim ! 0 th 1 eq? }

'matcolvec?'
    { matdim ! 1 th 1 eq? }

-- non-square matrices:
--    vectors: 1xn * nx1  --> rowvecmul
--    vectors: nx1 * 1xn  --> colvecmul
--    matrix*vector: nxn * nx1 --> matvecmul
'rowvecmul' 
    { mattrans ! <- car -> car lspair ! { {*} fold ! } over ! {+} fold ! }

'colvecmul' 
    { car swap 
        2 take { give {<- dup cp -> cart ! <-} each flip collect ! } nest 
    { {*} fold ! } matover ! }

-- matwin --> Selects a "window" out of a matrix

-- Prints the elements of a matrix to stdout using show operator
'matshow'
    { { { show << " " << } each "\n" << } each }

-- Sums the matrix diagonal
'trace' 
    { matdiag ! sum ! }

-- Takes the cartesian product of a matrix
'cart' 
        { 2 take 
            { give 
            wrap ! wrap ! { {<- dup -> 2 take swap} each zap } compose !
            each
        collect ! } 
        nest } 

-- same as cart but deep-copies
-- { cartd
--        { 2 take 
--            { give 
--            wrap ! wrap ! { {<- dup -> 2 take swap} each zap } lsjoin !
--            each
--        collect ! } 
--        nest } 
--    < }


-----------------------------------------------------------------------------
--                                                                         --
--                            map utilities                                --
--                                                                         --
-----------------------------------------------------------------------------

-- lumap (see above)

-- Look-up a tag in a map
-- <map> <tag> tlumap !
'tlumap'
    { luha 2 ith }

-- Look-up a list of keys in a map
-- <map> (key-list) tlumapls !
'lumapls'
    { 2 take { give { <- dup -> lumap ! <- } ... zap flip collect ! } nest }

-- Look-up a list of tags in a map
-- <map> (tag-list) tlumapls !
'tlumapls'
    { 2 take { give { <- dup -> tlumap ! <- } ... zap flip collect ! } nest }

-- Remove a list of tags from a map
'trmmapls'
    { { <- dup -> trmmap ! zap } each zap }

-- Insert into a map
-- <map> "foo" (1 2 3) insmap !
'insmap'
    { inskha }

-- Insert a list of key-value pairs into a map
-- <map> (key-value-list) insmap !
'insmapls'
    { { <- dup -> give inskha } ... }

-- Insert a list of tag-value pairs into a map
-- <map> (key-value-list) insmap !
'insmaptls'
    { { <- dup -> give insha } ... }

-- Test existence in a map
-- <map> "foo" exmap !  --> 0 or 1
'exmap'
    { hash exha }

-- Remove entry from a map
-- <map> "foo" rmmap !  --> 0 or 1, 0=not removed
'rmmap'
    { hash rmha }

-- Remove entry from a map
-- <map> <hash> trmmap !  --> 0 or 1, 0=not removed
'trmmap'
    { rmha }

-- Get all values in a map in list form
'valmap'
    { entsha 2 matcol ! }

-- lumap (see above)

-- Get all tag keys in a map in list form
'tagmap'
    { entsha 0 matcol ! }

-- Performs an operation over every entry in a map; compare to over
'overmap' 
    { <- dup map2tls ! dup <- 0 matcol ! -> 1 matcol ! -> over ! lspair ! -> insmaptls ! }

-- Performs an operation on each value in a map; compare to each, eachar, etc.
'eachmap' 
    { <- valmap ! -> each }

-- Prints the list returned by keymap
'dirmap' 
    { keymap ! prs ! }

-- Prints the list returned by keymap
'showmap' 
    { entsha 
        { dup 1 ith 
        dup nil? 
            { << }
            { zap dup 0 ith show << }
        selr !
        " : " <<
        2 ith show <<
        "\n" << } 
    each }

-- Undoes ls2map
'map2ls'
    { entsha { { dup <- 1 ith -> 2 ith 2 take} ... collect ! } nest }

-- Undoes ls2map, but uses tags instead of keys
'map2tls'
    { entsha { { dup <- 0 ith -> 2 ith 2 take} ... collect ! } nest }

-- <destmap> <srcmap> mergemap !
-- For every key in srcmap, inserts/overwrites in destmap
--    Effect is like destmap += srcmap
--    Make cp of destmap if you do not want to alter/overwrite it
'mergemap'
    { map2ls ! insmapls ! }

-- <destmap> <srcmap> tmergemap !
-- For every tag in srcmap, inserts/overwrites in destmap; see mergemap
'tmergemap'
    { map2tls ! insmaptls ! }

-- Returns the number of elements in a map
'sizemap'
    { entsha len }

-- Tests if map is empty
'emptymap'
    { sizemap ! 0 eq? }

-- Tag-based intersection of two maps
'tintermap'
    { fnord }

-- Intersection of two maps
'intermap'
    { fnord }


-----------------------------------------------------------------------------
--                                                                         --
--                           enum utilities                                --
--                                                                         --
-----------------------------------------------------------------------------

-- An enum is an enumeration of Babel objects (bstructs). An enum is implemented
-- as a map constructed in such a way that any given object can only map to a
-- single key. An enumeration is a finite set where each set-element has a
-- "contents". Among other things, you can take the union, intersection,
-- subtraction, and Venn difference of two enums. Since enums are maps, you can
-- use any of the map utilities to operate on them but an enum is not the
-- right abstraction if you find yourself needing to use a map operator. 
-- 
-- Because an enum has a "contents", you can use an enum as a container for Babel
-- objects but the concept of "lookup" does not make sense for an enum (just use a
-- map if you want to perform by-name lookups). Instead, you should use the enum
-- utilities to select the subset of interest and then retrieve the contents of
-- the resulting enum (which may have only one element) using enum2ls.

-- Construct an enum from a list of (arbitrary) objects
'ls2enum'
    { dup nil?
        { dup 
            { bsig ! } over ! 
            swap 
            lspair !
            <- newha ->
            insmaptls ! }
        { zap newha }
    selr ! }

-- Construct a value-list from an enum; undoes ls2enum
'enum2ls'
    { valmap ! }

-- Construct a tag-list from an enum; suitable for use with map operators
'enumtags'
    { tagmap ! }

-- Check set membership
'enumelem?'
    { bsig ! luha nil? not } 

-- Join two enums
'enumunion'
    { <- cp -> tmergemap ! }

-- Enum cardinality
'enumcard'
    { sizemap ! } 

-- Test if enum empty
'enumempty?'
    { sizemap ! 0 eq? } 

-- Test if two enums are equal
'enumeq?'
    { cmp } 

-- Test the subset relation between two enums
'enumsubset'
    { 2 take { give tagmap ! {<- dup -> exha <-} each flip collect ! } nest lstrue ! }

-- Take the intersection of two enums
'enumintersect'
    { <- dup -> 
        2 take { give 
        tagmap ! 
            { <- dup -> 
            dup 
            <- exha -> 
            swap 
            {zap} {<-} selr ! } 
        each 
        flip collect ! } 
    nest
    dup nil?
        { tlumapls ! ls2enum ! }
        {fnord}
    selr ! }

-- Set left-difference
-- <enum1> <enum2> enumleft ! --> <enum1-only>
'enumleft'
    { <- dup -> 
    enumintersect !
    tagmap ! 
    { <- dup -> rmha zap } each }

-- Enum Venn difference
-- <enum1> <enum2> enumdiff ! --> (<enum1-only> <enum1-enum2-intersection> <enum2-only>)
'enumdiff'
    { <- > -> > 
    2 take dup cp dup cp
    <- <- give enumleft !      ->
          give enumintersect ! ->
          give swap enumleft !
    3 take }

-- Applies a predicate to filter elements of an enum
--      <enum> {predicate} enumpred ! --> <filtered-enum>
--      Read: "All elements of enum such that predicate."
'enumpred'
    { <- valmap ! -> filter ! ls2enum ! }

-- Reduces a list to its unique elements, in no particular order
'uniq'
    { ls2enum ! enum2ls ! }

-- Run-length encode a list of arbitrary objects
--      <list> rlencode ! --> <list-of-pairs>
'rlencode'
        {{ <- 1 -> pop car swap
        { dup2 ! <- cmp
            { swap 2 take -> swap <- 1 swap }
            { <- 1 + -> -> zap }
        selr ! }
        each 
        swap 2 take <-
        flip collect ! } 
    nest } 

-- Run-length decode a list-of-pairs of form ((object1 count1) (object2 count2) ...)
'rldecode'
    { {give <- {dup cp} -> 1 - times} over ! }


-----------------------------------------------------------------------------
--                                                                         --
--                           hist utilities                                --
--                                                                         --
-----------------------------------------------------------------------------

-- A histogram is a map where the key can be an arbitrary object but where the
-- value is always a numeric value.

-- Construct a histogram from a list of (arbitrary) objects
'ls2hist'
    { dup nil?
        { <- newha -> { <- dup -> inchist ! } each }
        { zap newha }
    selr ! }

'hist2ls'
    { { entsha { dup <- 0 ith -> 1 ith 2 take }  collect ! } nest }

-- Insert or increment an element in a histogram
--      <hist> ex? <bsig>
'inchist'
    { bsig ! dup2 ! <- exlut ! 
        { -> 1 cp inslut ! }
        { dup -> dup <- lulut ! -> swap 1 + inslut ! }
    selr ! }

-- Test existence in a histogram
'exhist'
    { bsig ! luha nil? not }

-- Decrement an element in a histogram
'dechist'
    { bsig ! dup2 ! <- exlut ! 
        { -> 0 cp inslut ! }
        { dup -> dup <- lulut ! -> swap 1 - inslut ! }
    selr ! }

-- Look-up an element in a histogram
'luhist'
    { bsig ! luha 2 ith }


-----------------------------------------------------------------------------
--                                                                         --
--                            LUT utilities                                --
--                                                                         --
-----------------------------------------------------------------------------

-- A look-up table (LUT) is a map constructed in such a way that any given object 
-- can be used to index into the map (not just strings). Unlike an enum, a LUT
-- can map one object to another. For example, a LUT might be used to map tags
-- to code-lists.

-- Construct a lookup-table from a list
'ls2lut'
    { cp 
    <- newha ->
    { <- dup   ->
        dup  1 ith
        down 0 ith ->
        inslut ! }
    each}

--    { {give <- bsig ! -> 2 take} over ! ls2map ! }

-- New LUT
'newlut'
    { newha }

-- Look-up in a LUT
'lulut'
    { bsig ! luha 2 ith }

-- Insert into a LUT
'inslut'
    { <- bsig ! -> insha }

-- Test existence in a LUT
'exlut'
    { bsig ! exha }

-- Remove from a LUT
'rmlut'
    { bsig ! rmha }

-----------------------------------------------------------------------------
--                                                                         --
--                            bvm utilities                                --
--                                                                         --
-----------------------------------------------------------------------------

-- Makes a BVM from a code-list
-- {"Hello, world\n" <<} mkbvm ! babel
'mkbvm'
    { <-

   (("ustack_ptr"  (()))
    ("soft_root"   [ptr () ()])
    ("rstack_ptr"  (()))
    ("thread_id"    (1))
    ("advance_type" (0))
    ("bvm_initd"    (0))
    ("dstack_ptr"  (()))
    ("local_root"  (()))
    ("steps"       (-1)))
    ls2map !
    dup

    "code_ptr"
    -> () cons
    inskha
    "/babel/tag/sparse_bvm" newtag }

'mkbvm2'
    { <-

   (("ustack_ptr"  (()))
    ("soft_root"   mkbvm2_soft_root)
    ("rstack_ptr"  (()))
    ("thread_id"    (1))
    ("advance_type" (0))
    ("bvm_initd"    (0))
    ("dstack_ptr"  (()))
--    ("local_root"  (()))
    ("local_root"  mkbvm2_soft_root)
    ("steps"       (-1)))
    ls2map !
    dup

    "code_ptr"
    -> () cons
    inskha
    "/babel/tag/sparse_bvm" newtag }


-- Makes a BVM and imports a symbol-table (not implemented)
'mkbvms'
    { <-

   (("ustack_ptr"  (()))
    ("soft_root"   [tag "/babel/tag/trie" ptr () ()])
    ("rstack_ptr"  (()))
    ("thread_id"    (1))
    ("advance_type" (0))
    ("bvm_initd"    (0))
    ("dstack_ptr"  (()))
    ("local_root"  (()))
    ("steps"       (-1)))
    ls2map !
    dup

    "code_ptr"
    -> () cons
    inskha
    "/babel/tag/sparse_bvm" newtag }


-- Executes a lookup in BVM root hash-table
'lubvm'
    { <- detag -> lumap ! }

-- Executes a BVM one step at a time
-- <bvm> stepbvm !
'stepbvm'
    { dup detag "steps" (1) cp insmap ! <- () -> babel }

-- Executes a BVM for n steps
-- <bvm> 10 stepnbvm !
-- FIXME: Change to insert argument value into steps
'stepnbvm'
    { <- dup detag "steps" -> wrap ! cp insmap ! <- () -> babel }

-- Injects code into a BVM
-- { "bar" .+ } mkbvm ! { "foo" } cibvm ! babel   --> "foobar"
'cibvm'
    { <- dup detag dup "code_ptr" lumap ! car ->
          swap compose ! wrapptr !
          <- "code_ptr" -> insmap ! }

-- Injects a symbol into a BVM
-- { "bar" .+ } mkbvm ! { "foo" } cibvm ! babel   --> "foobar"
'sibvm'
    { <- <- detag dup "soft_root" lumap ! -> -> insmap ! }

'symbvm'
    { "soft_root" lubvm ! }

-- Re-execute a BVM
'rebabel'
    { dup detag "/babel/sym/code_restart_point" lumap !
    cibvm ! 
    babel }

-- Read-Eval-Print-Loop
--    Insert this into your code as a "debug breakpoint"
--    Enter "last" to exit the REPL and resume execution from the breakpoint
'repl'
    {{ "{ "
        "babel> " <<
        >>
        dup
        0 eq?
            { " }" .+ .+
                sexpr bpdli
                eval }
            { zap zap }
        selr eval }
    loop }

-- Looks up the dstack_ptr in a BVM and returns it
-- <bvm> dstackbvm ! --> <dstack>
'dstackbvm'
    { detag "dstack_ptr" lumap ! [0 0 0] get car }

-- Returns the dstack of a BVM in list form
-- <bvm> stackbvm ! --> ( TOS TOS-1 ... ) 
'stackbvm'
    { dstackbvm !
        { dup
        len dup <-
        <- { dup -> dup <- iter - ith swap } -> times
        -> zap
        zap
        collect !
        { dup car nil? {zap} {cdr car} selr ! } ...
        collect !}
    nest }

-- Prints with a newline
'say'
    { << "\n" << }

-- Applies say on each element of a list
'speak' 
    { {say !} ... } -- same as prn ... deprecate

-- Stack Display
'sd'
    { 5 dev }

-- Converts integer to 0-prefixed hexadecimal of width 8
'hex08'
    { <- "00000000" cp dup ->
      itoh dup
      <-
          arlen8 8 swap -
      ->
      dup arlen8
      <- 0 ->
      move8 }

-- create a str.sp for this kind of stuff...
-- { newlines_to_spaces {str2ar ar2ls {dup "\n" 0 th arcmp not {fnord} {zap ' '} selr !} over ! ls2lf ar2str} < }

'true'    [tag "/babel/tag/true"    ptr ()]
'false'   [tag "/babel/tag/false"   ptr ()]
'accept'  [tag "/babel/tag/accept"  ptr ()]
'reject'  [tag "/babel/tag/reject"  ptr ()]
'exist'   [tag "/babel/tag/exist"   ptr ()]
'unexist' [tag "/babel/tag/unexist" ptr ()]

---------------------------------------------------------------------
-- MISC (dev only)
---------------------------------------------------------------------

'lsopcodes' -- Dump the keys in the opcode_table file
    { 'opcode_table.bbl' loadfs ! keymap ! {<< ' ' <<} ... })

mkmap ! 
import ! 

-- Prints a list of strings, without spaces
pr  {<<}                eachify ! <

-- Prints a list of strings, with spaces
prs {<< " " <<}         eachify ! <

-- Prints a list of strings, surrounded with spaces and quote-marks
prq {'"' << << '" ' <<} eachify ! <

-- Prints a list of integers with spaces
prd {itod << " " <<}    eachify ! <

-- Prints a list of integers in hex, with spaces
prh {itoh << " " <<}    eachify ! <

-- Prints a list of strings, with newlines
prn {<< "\n" <<}        eachify ! <

-- Sums a list of integers 
sum   {+}   foldify ! <

-- Takes the product of a list of integers
prod  {*}   foldify ! <

-- Takes the min/max of TOS and TOS-1
min { dup2 ! <- gt? { -> zap } { -> swap zap } selr ! } <
max { dup2 ! <- lt? { -> zap } { -> swap zap } selr ! } <

-- Applies min/max to a list
lsmin { {min !} fold ! } <
lsmax { {max !} fold ! } <

-- Concatenates a list of strings
lscat {.+}  foldify ! <

-- Concatenates a list of strings with interposing spaces
wordcat {" " swap .+ .+}  foldify ! <

linecat {"\n" swap .+ .+}  foldify ! <

-- Takes the logical-AND of a list of values
-- Does not short-circuit; see lstrue
lsand {and} foldify ! <

-- Takes the logical-OR of a list of values
-- Does not short-circuit; see lsfalse
lsor  {or}  foldify ! <

-- Takes the logical-XOR of a list of values
lsxor {xor} foldify ! < }

-- Clayton Bauman 2016

