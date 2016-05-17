{ man_head
    { " ----------------------- Babel Command-Line Tutorial --------[ Page "
      <<
      "  " dup 0 cp "%d" page sprint
      <<
      " ]---- \n\n" 
      << } <

speak_indent
    { {"    " << say !} ... } <

man
    { dup <- man_pages -> exmap !
        { "There is no man topic for '" << << "'\n" <<}
        { "\n" << "Topic: '" << dup << "'\n\n" <<
            <- man_pages -> lumap ! speak_indent ! "\n" << }
    selr ! }
    <

man_pages 
(("toc"
    ("-- Logic & Arithmetic ---------------- | -- I/O -------------------------------"
    "- & * / | ^ ~ ~& ~^ ~| + abs add and   | << <<< >> >>> journal slurp slurpm "
    "ashr cand cnand cnor cnot cor cxnor    | spit spitm stdinln8 stdout stdoutm "
    "cxor div eq? false ge? gt? le?         | include loadfs savefs say pr prs prn "
    "logicF0_od logicF1_od logicF2_od       | "
    "logicF3_od logicF4_od logicF5_od       | -- List ------------------------------"
    "logicF6_od logicF7_od logicF8_od       | append bons car cdr cons ins ith len "
    "logicF9_od logicFa_od logicFb_od       | ls2lf rev shift split uncons unshift "
    "logicFc_od logicFd_od logicFe_od       | fold compose lsbyn lspair lsrol lsror "
    "logicFf_od lt? mul ne? not or rem rol  | lsunpair lsrotn cart "
    "rot shl shr sub true xor any? all?     | "
    "sum prod lsand lsor lsxor              | -- Array -----------------------------"
    "                                       | ar2ls arcmp arlen arlen8 cat cat8 cut "
    "-- Stack ----------------------------- | cut8 move move8 randlf shuf slice "
    "-> <- bottom clear depth dia dup flip  | slice8 sortlf th th8"
    "give pop push swap take top twist up   | "
    "zap collect takeptr takeval            | -- Bstruct ---------------------------"
    "                                       | bpdli cmp cp detag get_tag load mu "
    "-- Eval ------------------------------ | nil? nin nlf npt ntag nva ptr? sfield "
    "! ? alt babel cond accept? die exec    | span tag? tageq? unload val?"
    "exit fail fnord fork goto hibernate    | "
    "if ifte last let nest next pass? pass  | -- Hash-Table ------------------------"
    "reject? reset sel selr seq sleep       | entsha exha hash hashm insha inskha "
    "                                       | luha newha newptr newtag newval phash "
    "-- Loop ------------------------------ | rmha lumap insmap exmap rmmap valmap "
    "... eachar iter loop while times       | keymap tagmap map2ls dirmap ls2map "
    "                                       | "
    "-- String ---------------------------- | -- Matrix ----------------------------"
    ".+ ar2str atoi btoi char dtoi htoi     | matcol matrow matdiag mattrans mateach "
    "itoa itob itod itoh itoo otoi sexpr    | matover matij matpair matwise matrotn "
    "str2ar lscat                           | matmix matcolmix matcolcat matrowcat "
    "                                       | matshow "
    "-- Introspection --------------------- | "
    "; bs2gv bs2str show sd                 | -- BVM -------------------------------"
    "                                       | mkbvm stepbvm stepnbvm cibvm stackbvm "
    "-- Symbols --------------------------- |"
    "< > get get2 insym lusym root self set |"
    "set2 import                            |"))

-----------------------------------------------------------------------------
--                                                                         --
--                        Operators (Alphabetical)                         --
--                                                                         --
-----------------------------------------------------------------------------
    ("-"
        ("A B - ==> (A-B)"
        ""
        "Subtracts two numeric values. Numeric result."
        ""
        "See also: 'sub'"))
    ("->"
        ("Also 'up'. The up operator moves a value from the ustack to the"
        "dstack. The down or <- operator does the opposite."
        ""
        "See also: '<-', 'stack'"))
    ("!"
        ("{A} ! ==> result of evaluating A"
        ""
        "Also 'eval'. The eval or bang (!) operator takes a code-list A and"
        "evaluates it. Its stack effect depends on A."))
    ("&"
        ("n n & ==> n"
        ""
        "Performs bitwise-AND on two numeric values. Numeric result."))
    ("*"
        ("n n * ==> n"
        ""
        "Multiplies two numeric values. Numeric result."))
    ("..."
        ("(A) {B} ..."
        ""
        "Also 'each'. The each operator evaluates code-list B for each element"
        "of list A. Its stack effect depends on B."))
    (".+"
        ("'A' 'B' .+ ==> 'AB'"
        ""
        "Concatenates two strings. String result."))
    ("/"
        ("A B / ==> (A/B)"
        ""
        "Divides two signed numeric values. Signed numeric result."))
    (";"
        ("Displays the item on top of the stack. Does not remove the item"
        "displayed."))
    ("?"
        ("..."))
    ("^"
        ("n n ^ ==> n"
        ""
        "Performs bitwise-XOR on two numeric values. Numeric result."))
    ("|"
        ("n n | ==> n"
        ""
        "Performs bitwise-OR on two numeric values. Numeric result."))
    ("~"
        ("n ~ ==> n"
        ""
        "Performs bitwise-complement on a numeric value. Numeric result."))
    ("~&"
        ("n n ~& ==> n"
        ""
        "Performs bitwise-NAND on two numeric values. Numeric result."))
    ("~^"
        ("n n ~^ ==> n"
        ""
        "Performs bitwise-XNOR on two numeric values. Numeric result."))
    ("~|"
        ("n n ~| ==> n"
        ""
        "Performs bitwise-NOR on two numeric values. Numeric result."))
    ("+"
        ("n n + ==> n"
        ""
        "Adds two signed numeric values. Signed numeric result."))
    ("<"
        ("foo X <"
        ""
        "Stores X in symbol foo."))
    ("<-"
        ("Also 'down'. The down operator moves a value from the dstack to the"
        "ustack. The up or -> operator does the opposite."))
    ("<<"
        ("'S' <<"
        ""
        "Prints string S to STDOUT. S is removed from the stack."))
    ("<<<"
        ("'S' 'foo' <<<"
        ""
        "Writes string S to file named 'foo'. Creates file if it does not exist"
        "and overwrites file if it exists. S and filename are removed from the stack."))
    (">"
        ("foo >"
        ""
        "Retrieves whatever is stored in symbol foo."))
    (">>"
        (">> ==> 'S'"
        ""
        "Gets a string from STDIN. String result."))
    (">>>"
        ("'foo' >>>" 
        ""
        "Reads in file named 'foo'. Result is array-8 (byte-wise array)."
        "and overwrites file if it exists. Filename is removed from the stack."))
    ("abs"
        ("n abs ==> n"
        ""
        "Takes absolute-value of a numeric value. Numeric result."))
    ("accept?"
        ("..."))
    ("add"
        ("n n + ==> n"
        ""
        "Adds two unsigned numeric values. Unsigned numeric result."))
    ("alt"
        ("..."))
    ("and"
        ("b b and ==> b"
        ""
        "Performs logical-and of two boolean values. Boolean result."))
    ("append"
        ("((A)(B)...) append ==> (A B ...)"
        ""
        "Appends all lists in a list-of-lists. List result."))
    ("append_d"
        ("See append"))
    ("ar2ls"
        ("[val A B ...] ar2ls ==> (A B ...)"
         "[ptr A B ...] ar2ls ==> (A B ...)"
        ""
        "Converts an array (pointers or values) to a list."))
    ("ar2ls_d"
        ("See arl2s"))
    ("ar2str"
        ("[val c c ...] ar2str ==> 'S'"
        ""
        "Converts an array of UTF-32 characters to string S."))
    ("ar2str_d"
        ("See ar2str"))
    ("arcmp"
        ("[val A B ...] [val C D ...] arcmp ==> 0|1|-1"
         "[ptr A B ...] [ptr A B ...] arcmp ==> 0|1|-1"
        ""
        "Directly compares two arrays. Based on C's memcmp()."))
    ("arcmp_md"
        ("See arcmp"))
    ("arlen"
        ("[val A B ...] arlen ==> n"
         "[ptr A B ...] arlen ==> n"
        ""
        "Calculates the length of an array. Numeric result."))
    ("arlen_md"
        ("See arlen"))
    ("arlen8"
        ("      'S' arlen8 ==> n"
         "[val ...] arlen8 ==> n"
        ""
        "Calculates the length of an array-8 or string. Numeric result."))
    ("arlen_8d"
        ("See arlen8"))
    ("arlen_1d"
        ("..."))
    ("ashr"
        ("A B ashr ==> n"
        ""
        "Shifts A right by B, preserves sign. Numeric result."))
    ("atoi"
        ("'S' n atoi ==> n"
        ""
        "Converts numeric string S from an arbitrary base n to an integer."
        "value. Numeric result."))
    ("babel"
        ("(arg-list) bvm babel ==> x"
        ""
        "Executes a Babel Virtual Machine, bvm, with arguments arg-list, returning result x;"
        "x depends on bvm. In the example, we pass two arguments to the BVM created by "
        "mkbvm. The BVM adds the numbers we passed to it, converts the result to a string"
        "then returns the result back. We then print the result with the say utility."
        ""
        "    (2 2) {+ itod return} mkbvm ! babel say !"
        ""
        "See also: babels, mkbvm, return, say"))
    ("babels"
        ("sym-table (arg-list) bvm babel ==> x"
        ""
        "Executes a Babel Virtual Machine, bvm, with arguments arg-list and symbol-table,"
        "returning result x; x depends on bvm. In the example, we pass two arguments to"
        "the BVM created by mkbvm. The BVM adds the numbers we passed to it, converts"
        "the result to a string then prints the result with the say utility."
        ""
        "    root ('Hello') {say !} mkbvm ! babels"
        ""
        "Note that the say utility is accessible inside the BVM because we passed root"
        "as the symbol-table. Otherwise, we would have to load std.sp inside the BVM we"
        "launched."
        ""
        "See also: babel, mkbvm, root, return, say"))
    ("bons"
         ("(A B ...) bons ==> [ptr A B ...]"
        ""
        "Converts a list to a pointer-array"))
    ("bons_d"
        ("See bons"))
    ("bottom"
        ("..."))
    ("bpdli"
         ("s-expr bpdli ==> bstruct"
        ""
        "Converts an S-expression structure (output of sexpr operator) to"
        "a coresponding bstruct structure."))
    ("bpdli_d"
        ("..."))
    ("bs2gv"
        ("bstruct bs2gv ==> 'S'"
        ""
        "Converts a bstruct to a string S containing a Graphviz dot-language"
        "description of an graph representing the bstruct. Save to S file and"
        "use the dot utility to convert the string to an image. You can inspect"
        "the SVG-format images in the svg/ directory under the Babel working"
        "directory to see examples of bs2gv output when converted by dot."))
    ("bs2gv_d"
        ("See bs2gv"))
    ("bs2str"
        ("bstruct bs2str ==> 'S'"
        ""
        "Converts a bstruct to a string S containing a BPDL description of the"
        "bstruct. Use bpdl operator to convert S back to a bstruct."))
    ("bs2str_d"
        ("See bs2str"))
    ("bscmp_d"
        ("See cmp"))
    ("btoi"
        ("'S' btoi ==> n"
        ""
        "Converts string S containing a binary numeric representation to"
        "value n. See also: 'itob'"))
    ("cabs_iod"
        ("See abs"))
    ("cadd_iod"
        ("See '+'"))
    ("cadd_uod"
        ("See add"))
    ("cand"
        ("See '&'"))
    ("car"
        ("(A B C) car ==> A"
        ""
        "Behaves like the car operator in Lisp. Returns the first element of"
        "a list."))
    ("car_rd"
        ("See car"))
    ("cat"
        ("[val A B ...] [val C D ...] cat ==> [val A B ... C D ...]"
        "[ptr A B ...] [ptr C D ...] cat ==> [ptr A B ... C D ...]"
        ""
        "Concatenates two arrays (pointers or values). Either both arrays"
        "must be pointer-arrays or both must be value-arrays. You cannot"
        "concatenate a value-array with a pointer-array."))
    ("cat_md"
        ("See cat"))
    ("cat8"
        ("See .+"))
    ("cat_8d"
        ("See .+"))
    ("cat_1d"
        ("..."))
    ("cdiv_iad"
        ("The cdiv_iad operator ..."))
    ("cdiv_iod"
        ("See '/'"))
    ("cdiv_uod"
        ("See div"))
    ("cdr"
        ("(A B C) cdr ==> (B C)"
        ""
        "Behaves like the cdr operator in Lisp. Returns the rest of a list"
        "after the first element."))
    ("cdr_rd"
        ("See cdr"))
    ("ceq_ad"
        ("The ceq_ad operator ..."))
    ("ceq_od"
        ("The ceq_od operator ..."))
    ("cge_iod"
        ("See 'ge?'"))
    ("cge_uod"
        ("..."))
    ("cgt_iod"
        ("See 'gt?'"))
    ("cgt_uod"
        ("..."))
    ("char"
        ("..."))
    ("char_d"
        ("See char"))
    ("cle_iod"
        ("See 'le?'"))
    ("cle_uod"
        ("..."))
    ("clear"
        ("Clears the stack"))
    ("clt_iod"
        ("See 'lt?'"))
    ("clt_uod"
        ("..."))
    ("cmp"
        ("bstruct bstruct cmp ==> 0|1"
        ""
        "Compares any two bstructs. Equality of bstructs is defined as value-"
        "equality and topological homomorphism of the entire bstruct. Basically," 
        "if bs2gv generates the same image output for two bstructs, cmp will"
        "return true, otherwise, it will return false."))
    ("cmul_iod"
        ("See '*'"))
    ("cmul_uod"
        ("See mul"))
    ("cnand"
        ("See '~&'"))
    ("cne_od"
        ("See 'ne?'"))
    ("cnor"
        ("See '~|'"))
    ("cnot"
        ("See '~'"))
    ("cnot_od"
        ("See '~'"))
    ("cond"
        ("..."))
    ("conjure"
        ("..."))
    ("cons"
        ("A (B C) cons ==> (A B C)"
        ""
        "Behaves like the cons operator in Lisp. Returns a list with A as its"
        "first element and (B C) as the rest of the list."))
    ("cons_d"
        ("See cons"))
    ("cor"
        ("See '|'"))
    ("cp"
        ("bstruct cp ==> bstruct"
        ""
        "Performs a 'deep-copy' of a bstruct. Contrast with the dup operator"
        "which only performs a shallow copy (copies the reference on the stack)."))
    ("cp_d"
        ("See cp"))
    ("crem_iod"
        ("..."))
    ("crem_uod"
        ("See rem"))
    ("crol_uod"
        ("See rol"))
    ("cshl_uod"
        ("See shl"))
    ("cshr_iod"
        ("See ashr"))
    ("cshr_uod"
        ("See shr"))
    ("csub_iod"
        ("See '-'"))
    ("csub_uod"
        ("See sub"))
    ("cut"
        ("..."))
    ("cut_1d"
        ("..."))
    ("cut_8d"
        ("..."))
    ("cut_md"
        ("..."))
    ("cut8"
        ("..."))
    ("cxnor"
        ("See '~^'"))
    ("cxor"
        ("See '^'"))
    ("depth"
        ("depth ==> n"
        ""
        "Finds the depth of the dstack. Returns a numeric value."))
    ("detag"
        ("tptr detag ==> ptr"
        ""
        "Returns the pointer in a tagged-pointer. See also: 'newtag', 'get_tag'"))
    ("detag_d"
        ("See detag"))
    ("dev"
        ("This operator only exists in development-builds of Babel. Give the"
        "command '0 dev' to get a listing of available dev codes."))
    ("dia"
        ("dia ==> n"
        ""
        "Finds the 'diameter' of the stack. This is equivalent to the depth of"
        "the dstack added to the depth of the ustack. Returns a numeric value."))
    ("die"
        ("Forcibly exits the interpreter. This opcode should be disabled in a"
        "BVM that is untrusted, before launching it."))
    ("div"
        ("A B div ==> (A/B)"
        ""
        "Divides two unsigned numeric values. Unsigned numeric result."))
    ("down"
        ("See '<-'"))
    ("dtoi"
        ("'S' dtoi ==> n"
        ""
        "Converts a signed decimal string S to its numeric value n"))
    ("dup"
        ("A dup ==> A A"
        ""
        "Duplicates the reference on top-of-stack. This is a 'shallow-copy',"
        "so altering A through one reference alters it through both. See also:"
        "'cp'"))
    ("each"
        ("See '...'"))
    ("eachar"
        ("[ptr A B ...] {C} eachar"
        ""
        "The eachar operator evaluates code-list C for each pointer in the"
        "pointer-array. Its stack effect depends on C."))
    ("endian"
        ("..."))
    ("entsha"
        ("hash-table entsha ==> ((entry) (entry))"
        ""
        "Returns a list-of-lists containing the entries in the hash-table."
        "Each entry consists of three elements: tag, key and value. Use the"
        "rc.sp utilities 'tagmap' 'keymap' and 'valmap' to access these"
        "components more easily."))
    ("entsha_d"
        ("See entsha"))
    ("eq?"
        ("n n eq? ==> 0|1"
        ""
        "Returns true if two numeric values are equal. Otherwise, returns"
        "false"))
    ("eval"
        ("See '!'"))
    ("exec"
        ("..."))
    ("exec_d"
        ("See exec"))
    ("exha"
        ("hash-table hash-key exha ==> 0|1"
        ""
        "Returns a boolean indicating if an entry corresponding to the hash-key"
        "exists in the hash-table. Use the rc.sp utility exmap to perform this"
        "operation more easily."))
    ("exha_d"
        ("See exha"))
    ("exit"
        ("Exits the current BVM. This operator does not exit the interpreter"
        "unless it is executed in the root BVM."))
    ("fail"
        ("..."))
    ("false"
        ("..."))
    ("flip"
        ("Flips the stack. Specifically, it swaps out the dstack and ustack."))
    ("fnord"
        ("Does nothing. Equivalent to a NO-OP."
        "Useful for emphasizing unused code-lists, for example: {fnord}"))
    ("fork"
        ("..."))
    ("fork_d"
        ("See fork"))
    ("ge?"
        ("A B ge? ==> 0|1"
        ""
        "Returns true if A is greater-than-or-equal to B. Otherwise, returns"
        "false. A and B numeric."))
    ("get"
        ("array pbp get ==> bstruct"
        ""
        "The get operator uses a pure-Babel-pointer (pbp) to traverse the"
        "array (or bstruct) that is passed to it. It returns the bstruct that"
        "it finds." 
        ""
        "Example:"
        ""
        "    [ptr [1 2] [3 4] [5 6]] [2 0] get"
        ""
        "Returns 5 because 5 is the 0th element of the 2th element of the array"))
    ("get_tag"
        ("tptr get_tag ==> tag"
        ""
        "Returns the tag in a tagged-pointer. See also: 'newtag', 'detag'"))
    ("get_tag_d"
        ("See get_tag"))
    ("get2"
        ("See '>'"))
    ("give"
        ("(A B ...) give ==> A B ..."
        ""
        "Gives the elements of a list onto the stack. See also: 'take'"))
    ("goto"
        ("..."))
    ("gt?"
        ("A B gt? ==> 0|1"
        ""
        "Returns true if A is greater-than B. Otherwise, returns false. A and"
        "B numeric."))
    ("hash"
        ("'S' hash ==> hashed-string"
        ""
        "Hashes string S and returns a 128-bit hash of the string."))
    ("hash_8d"
        ("See hash"))
    ("hashm"
        ("val-array hashm ==> hashed-array"
        ""
        "Hashes the value-array and returns a 128-bit hash of the array."))
    ("hash_md"
        ("See hashm"))
    ("hibernate"
        ("..."))
    ("htoi"
        ("'S' htoi ==> n"
        ""
        "Converts string S containing a hexadecimal numeric representation to"
        "value n."))
    ("if"
        ("..."))
    ("ifte"
        ("..."))
    ("ins"
        ("Inserts one list into another, already existing list after the first"
        "element."
        ""
        "Example:"
        ""
        "    (1 2 3) (4 5) ins ==> (1 4 5 2 3)"))
    ("ins_d"
        ("See ins"))
    ("insha"
        ("hash-table hash-key bstruct insha"
        ""
        "Inserts bstruct into hash-table with hash-key. Returns nothing. Use the"
        "rc.sp utility insmap to perform this same operation more easily."))
    ("insha_d"
        ("See insha"))
    ("inskha"
        ("hash-table key bstruct inskha"
        ""
        "Inserts bstruct into hash-table with key. Returns nothing. Use the rc.sp"
        "utility insmap to perform this same operation more easily."))
    ("inskha_d"
        ("See inskha"))
    ("insym"
        ("..."))
    ("insym_d"
        ("See insym"))
    ("is_accept"
        ("See accept?"))
    ("is_false"
        ("See false?"))
    ("is_nil"
        ("See nil?"))
    ("is_reject"
        ("See reject?"))
    ("is_true"
        ("See true?"))
    ("isptr_d"
        ("See ptr?"))
    ("istag_d"
        ("See tag?"))
    ("isval_d"
        ("See val?"))
    ("iter"
        ("iter ==> n"
        ""
        "Returns the current loop iteration (inner-most loop). Returns numeric"
        "value."))
    ("ith"
        ("(A B ...) n ith ==> nth element of list"
        ""
        "Returns element at index n of list."))
    ("ith_rd"
        ("See ith"))
    ("itoa"
        ("A B itoa ==> 'S'"
        ""
        "Converts numeric value A in base B to numeric string S. String"
        "result."))
    ("itob"
        ("n itob ==> 'S'"
        ""
        "Converts numeric value to numeric string S containing a binary numeric"
        "representation of n. See also: 'btoi'"))
    ("itod"
        ("n itod ==> 'S'"
        ""
        "Converts numeric value to numeric string S containing a decimal numeric"
        "representation of n. See also: 'dtoi'"))
    ("itoh"
        ("n itoh ==> 'S'"
        ""
        "Converts numeric value to numeric string S containing a hexadecimal"
        "numeric representation of n. See also: 'htoi'"))
    ("itoo"
        ("n itoo ==> 'S'"
        ""
        "Converts numeric value to numeric string S containing a octal numeric"
        "representation of n. See also: 'otoi'"))
    ("journal"
        ("..."))
    ("journal_md"
        ("See journal"))
    ("journal_8d"
        ("..."))
    ("last"
        ("Exits the current code-context. Equivalent to C's 'break' keyword."))
    ("le?"
        ("A B le? ==> 0|1"
        ""
        "Returns true if A is less-than-or-equal to B. Otherwise, returns false."
        "A and B numeric."))
    ("len"
        ("(A B ...) len ==> n"
        ""
        "Returns the length of a list. Numeric result."))
    ("len_d"
        ("See len"))
    ("let"
        ("..."))
    ("load"
        ("val-array load ==> bstruct"
        ""
        "Loads an unloaded bstruct (as a value-array). Result is bstruct. See"
        "also: 'unload'"))
    ("load_d"
        ("See load"))
    ("logicF0_od"
        ("Result is always 0"))
    ("logicF1_od"
        ("See '&'"))
    ("logicF2_od"
        ("..."))
    ("logicF3_od"
        ("..."))
    ("logicF4_od"
        ("..."))
    ("logicF5_od"
        ("..."))
    ("logicF6_od"
        ("See '^'"))
    ("logicF7_od"
        ("See '|'"))
    ("logicF8_od"
        ("See '~|'"))
    ("logicF9_od"
        ("See '~^'"))
    ("logicFa_od"
        ("..."))
    ("logicFb_od"
        ("..."))
    ("logicFc_od"
        ("..."))
    ("logicFd_od"
        ("..."))
    ("logicFe_od"
        ("See '~&'"))
    ("logicFf_od"
        ("Result is always -1"))
    ("loop"
        ("{A} loop"
        ""
        "Repeats code-list A indefinitely or until last operator is executed."))
    ("ls2lf"
        ("(A B ...) ls2lf ==> [val A B ...]"
        ""
        "Converts a list of values to a value-array."))
    ("ls2lf_d"
        ("The ls2lf_d operator ..."))
    ("lt?"
        ("A B lt? ==> 0|1"
        ""
        "Returns true if A is less-than B. Otherwise, returns false. A and"
        "B numeric."))
    ("luha"
        ("hash-table hash-key luha ==> nil|entry"
        ""
        "Looks up hash-key in hash-table. Returns nil if there is no entry in"
        "hash-table corresponding to hash-key. Otherwise, returns the"
        "corresponding entry."
        ""
        "See also: 'lumap'"))
    ("luha_d"
        ("See luha"))
    ("lusym"
        ("..."))
    ("move"
        ("dest_arr dest_index src_array src_index size move"
        ""
        "Performs a fast move between arrays (leaf or pointer). Based on C's"
        "memmove() function. Destination and source can be the same, and the"
        "ranges of source and destination can overlap."))
    ("move_md"
        ("See move"))
    ("move8"
        ("The move8 operator ..."))
    ("move_8d"
        ("The move_8d operator ..."))
    ("move_1d"
        ("The move_1d operator ..."))
    ("mu"
        ("bstruct mu ==> n"
        ""
        "Calculates the memory-usage (in bytes) of a bstruct. Numeric result."))
    ("mu_d"
        ("See mu"))
    ("mul"
        ("n n mul ==> n"
        ""
        "Multiplies two unsigned numeric values. Unsigned numeric result."))
    ("ne?"
        ("n n ne? ==> 0|1"
        ""
        "Returns true if two numeric values are not-equal. Otherwise, returns"
        "false."))
    ("nest"
        ("A {C} nest ==> B"
        "The nest operator constructs a 'nested' stack mechanism. Note that"
        "the nest operator does not create a local context (a local namespace)."
        "The nest operator does the following:"
        ""
        "    - Removes the top item, A, from the current stack"
        "    - Saves the current stack"
        "    - Creates a new stack and places A on it; this is now the current"
        "        stack"
        "    - When the nested statement C has finished, removes the top item,"
        "        B, from the current stack"
        "    - Restores the old stack and puts B on it"
        ""
        "Example:"
        ""
        "    clear 0x123 0x456 {0x789 0xabc} nest"))
    ("newha"
        ("newha ==> empty-hash-table"
        ""
        "Creates an empty hash-table and places it on the stack."))
    ("newptr"
        ("n newptr ==> [ptr nil nil ...]"
        ""
        "Creates an empty pointer-array (all elements initialized to nil) of"
        "size n."))
    ("newptr_d"
        ("See newptr_d"))
    ("newtag"
        ("x 'S' newtag ==> [tag 'S' x]"
        ""
        "Builds a new tagged-pointer pointing at x, with hash of 'S' as tag."))
    ("newtag_d"
        ("See newtag"))
    ("newval"
        ("n newval ==> [ptr nil nil ...]"
        ""
        "Creates an empty value-array (all elements initialized to 0) of size"
        "n."))
    ("newval_d"
        ("See newval"))
    ("next"
        ("..."))
    ("nil?"
        ("x nil? ==> 0|1"
        ""
        "Returns true if x is nil. Otherwise returns false. Boolean result."))
    ("nin"
        ("x nin ==> n"
        ""
        "Calculates the number of interior-arrays (pointer-arrays) in x. Numeric"
        "result."))
    ("nin_d"
        ("See nin"))
    ("nl"
        ("..."))
    ("nl_d"
        ("..."))
    ("nlf"
        ("x nlf ==> n"
        ""
        "Calculates the number of leaf-arrays (value-arrays) in x. Numeric"
        "result."))
    ("nlf_d"
        ("See nlf"))
    ("not"
        ("b not ==> b"
        ""
        "Performs logical-not of a boolean value. Boolean result."))
    ("npt"
        ("x npt ==> n"
        ""
        "Calculates the number of pointers in x. Numeric result."))
    ("npt_d"
        ("See npt"))
    ("ntag"
        ("x ntag ==> n"
        ""
        "Calculates the number of tags in x. Numeric result."))
    ("ntag_d"
        ("See ntag"))
    ("num2str_bd"
        ("See itob"))
    ("num2str_id"
        ("See itod"))
    ("num2str_xd"
        ("See itoh"))
    ("nva"
        ("x nva ==> n"
        ""
        "Calculates the number of values in x. Numeric result."))
    ("nva_d"
        ("See nva"))
    ("or"
        ("b b and ==> b"
        ""
        "Performs logical-or of two boolean values. Boolean result."))
    ("ord"
        ("..."))
    ("ord_d"
        ("..."))
    ("otoi"
        ("'S' otoi ==> n"
        ""
        "Converts string S containing an octal numeric representation to"
        "value n."))
    ("pass?"
        ("..."))
    ("pass"
        ("..."))
    ("phash"
        ("..."))
    ("phash_8d"
        ("..."))
    ("phash_md"
        ("..."))
    ("pop"
        ("(A B C) pop ==> (B C) (A)"
        ""
        "Removes the left-most element from a list and places it on the stack."))
    ("pop_d"
        ("See pop"))
    ("ptr?"
        ("x ptr? ==> 0|1"
        ""
        "Returns true if x is a pointer-array. Otherwise, returns false. Boolean"
        "result."))
    ("push"
        ("(B C) (A) push ==> (A B C)"
        ""
        "Pushes an element onto the left-most end of a list. Leaves the resulting"
        "list on the stack."))
    ("push_d"
        ("See push"))
    ("put"
        ("..."))
    ("randlf"
        ("n randlf ==> [val ...]"
        ""
        "Creates a value-array of size n, populated with pseudorandom values"
        "generated with Mersenne Twister algorithm. Not suitable for any"
        "cryptographic application."))
    ("randlf_d"
        ("See randlf"))
    ("reject?"
        ("..."))
    ("rem"
        ("A B rem ==> (A modulo B)"
        ""
        "Modulo division of two unsigned numeric values. Unsigned numeric result."))
    ("reset"
        ("..."))
    ("rev"
        ("(A B ...) rev ==> (... B A)"
        ""
        "Reverses a list. List result."))
    ("rev_d"
        ("See rev"))
    ("rmha"
        ("hash-table hash-key rmha ==> 0|1"
        ""
        "Inserts bstruct into hash-table with hash-key. Returns nothing. Use the"
        "rc.sp utility insmap to perform this same operation more easily."))
    ("rmha_d"
        ("See rmha"))
    ("rol"
        ("A B rol ==> n"
        ""
        "Rotates A left by B. Numeric result."))
    ("root"
        ("root ==> <symbol-table-pointer>"
        ""
        "Puts a pointer to the root symbol-table onto the stack. The root symbol-table"
        "is a map."
        ""
        "See also: lumap"))
    ("rot"
        ("Rotates the stack. Specifically, it rotates dstack and ustack about"
        "the Bottom-of-Stack."))
    ("sel"
        ("A B b sel ==> A|B"
        ""
        "Selects A if b is false. Otherwise, selects B."))
    ("self"
        ("..."))
    ("selr"
        ("b A B sel ==> A|B"
        ""
        "Selects A if b is false. Otherwise, selects B."))
    ("seq"
        ("..."))
    ("set"
        ("array pbp x set"
        ""
        "The set operator uses a pure-Babel-pointer (pbp) to traverse the"
        "array (or bstruct) that is passed to it and then sets the value selected"
        "by the pbp to x."
        ""
        "Example:"
        ""
        "    [ptr [1 2] [3 4] [5 6]] dup [2 0] 7 set"
        ""
        "Returns 5 because 5 is the 0th element of the 2th element of the array"))
    ("set2"
        ("See '<'"))
    ("sexpr"
         ("'S' sexpr ==> s-expr"
        ""
        "Converts a string S to an S-expression structure suitable for input"
        "to bpdli or for direct processing."))
    ("sfield"
        ("x sfield ==> n"
        ""
        "Retrieves the sfield of array x. Numeric result. See also: 's-field'"))
    ("sfield_d"
        ("See sfield"))
    ("shift"
        ("(A B C) pop ==> (A B) (C)"
        ""
        "Removes the right-most element from a list and places it on the stack."))
    ("shift_d"
        ("See shift"))
    ("shl"
        ("A B shl ==> n"
        ""
        "Shifts A left by B. Numeric result."))
    ("show"
        ("bstruct show ==> 'S'"
        ""
        "Converts a bstruct to a string S containing a complete or partial"
        "description of the bstruct in BPDL format."))
    ("show_d"
        ("See show"))
    ("shr"
        ("A B shr ==> n"
        ""
        "Shifts A right by B. Does not preserve sign, see ashr. Numeric result."))
    ("shuf"
        ("..."))
    ("shuf_d"
        ("..."))
    ("sleep"
        ("..."))
    ("sleep_d"
        ("..."))
    ("slice"
        ("    [val A B ...] n n slice ==> nil|[val ]"
        "    [ptr A B ...] n n slice ==> nil|[ptr ]"
        ""
        "Slices a value- or pointer-array beginning and ending at the numeric points"
        "specified. In Babel, slicing operations count the spaces between values in"
        "an array. To get the first element of an array.:"
        ""
        "    [1 2 3] 0 1 slice"
        ""
        "If you give the same start and end index, slice will always return nil"
        "(nothing)."))
    ("slice_md"
        ("See slice"))
    ("slice8"
        ("..."))
    ("slice_8d"
        ("..."))
    ("slice_1d"
        ("..."))
    ("slurp"
        ("See '>>>'"))
    ("slurp_8d"
        ("See '>>>'"))
    ("slurpm"
        ("'foo' slurpm" 
        ""
        "Reads in file named 'foo'. Result is mword value-array. Overwrites file"
        "if it exists. Filename is removed from the stack."))
    ("slurp_md"
        ("See slurpm"))
    ("sortlf"
        ("[val A B ...] sortlf ==> sorted-array"
        ""
        "Sorts a value-array (leaf-array). Algorithm is in-place mergesort."))
    ("sortlf_d"
        ("See sortlf"))
    ("span"
        ("x span ==> [ptr ...]"
        ""
        "Constructs a pointer-array with each entry a pointer to an array"
        "in the bstruct x. Every array in x will have a corresponding entry"
        "in the resulting pointer-array."))
    ("span_d"
        ("See span"))
    ("spit"
        ("See '<<<'"))
    ("spit_8d"
        ("See '<<<'"))
    ("spitm"
        ("[val A B ...] 'foo' spitm" 
        ""
        "Writes value-array to file named 'foo'. Creates file if it does not exist"
        "and overwrites file if it exists. The value-array and filename are removed"
        "from the stack."))
    ("spit_md"
        ("See spitm"))
    ("split"
        ("(A B ...) (C D ...) split ==> ((A)(B)...)"
        ""
        "Splits a list A,B,... at points C,D,... List-of-lists result. See"
        "append for opposite operation."))
    ("split_d"
        ("See split"))
    ("sprint"
        ("array8 offset format-string format-argument sprint"
        ""
        "Restricted version of C's sprintf(). The sprint operator only takes one"
        "format-argument, you should not use multiple format codes in the format-"
        "string - sprint's behavior is undefined in this case."
        ""
        "Example:"
        ""
        "'   ' dup 0 '%03x' 157 sprint <<"))
    ("stdinln8"
        ("See '>>'"))
    ("stdout"
        ("See '<<'"))
    ("stdout_8d"
        ("See '<<'"))
    ("stdout_md"
        ("..."))
    ("stdoutm"
        ("..."))
    ("str2ar"
        ("'S' str2ar ==> [val c c ...]"
        ""
        "Converts string S to a value-array of characters, c, corresponding"
        "to the UTF-32 encodings of each character in S."))
    ("str2ar_d"
        ("See str2ar"))
    ("str2num_bd"
        ("See btoi"))
    ("str2num_id"
        ("See dtoi"))
    ("str2num_xd"
        ("See htoi"))
    ("sub"
        ("A B - ==> n"
        ""
        "Subtracts A from B; A and B unsigned numeric values. Unsigned numeric"
        "result."))
    ("swap"
        ("A B swap ==> B A"
        ""
        "Swaps the top two items on the stack."))
    ("tag?"
        ("x tag? ==> 0|1"
        ""
        "Returns true if x is a tag. Otherwise, returns false. Boolean result."))
    ("tageq?"
        ("x y tageq? ==> 0|1"
        ""
        "Returns true if the tags of x and y are equal. Otherwise, returns false."
        "x and y may each either be a tagged-pointer or a value-array containing"
        "a hash."
        ""
        "Examples:"
        ""
        "[tag 'foo' nil] [tag 'foo' nil] tageq?"
        "[tag 'foo' nil] 'foo' hash tageq?"
        "'foo' hash 'foo' hash tageq?"))
    ("tageq_d"
        ("See tageq?"))
    ("take"
        ("A B ... n take ==> (A B ...)"
        ""
        "Takes n elements off the stack and returns them as a lis. See also: 'give'"))
    ("th"
        ("[val A B ...] n th ==> nth element of array"
        "[ptr A B ...] n th ==> nth element of array"
        ""
        "Returns element at index n of value-array or pointer-array."))
    ("th_rmd"
        ("See th"))
    ("th_r1d"
        ("..."))
    ("th_r8d"
        ("..."))
    ("th8"
        ("..."))
    ("times"
        ("{C} n times" 
        ""
        "Repeats code-list C n times."))
    ("timems"
        ("timems ==> value"
        ""
        "Returns a value containing the current system time in milliseconds. This is"
        "not a high-resolution timer, so there is no realism guarantee. The system"
        "timer continues counting even when the current application is not in focus, so"
        "code-profiling measurements do not necessarily reflect performance of the code"
        "in isolation. See also 'timed'."))
    ("top"
        ("..."))
    ("true"
        ("..."))
    ("trunc"
        ("..."))
    ("trunc_d"
        ("..."))
    ("twist"
        ("Reverses the dstack without affecting the ustack."))
    ("uncons"
        ("(A B C) uncons ==> A (B C)"
        ""
        "Undoes the cons operator."))
    ("uncons_d"
        ("See uncons"))
    ("unload"
        ("bstruct unload ==> val-array"
        ""
        "Unloads a bstruct. Result is val-array. See also: 'load'"))
    ("unload_d"
        ("See unload"))
    ("unshift"
        ("(A B) (C) unshift (A B C)"
        ""
        "Unshifts an element onto the right-most end of a list. Leaves the resulting"
        "list on the stack."))
    ("unshift_d"
        ("See unshift"))
    ("up"
        ("See '->'"))
    ("val?"
        ("x val? ==> 0|1"
        ""
        "Returns true if x is a value-array. Otherwise, returns false. Boolean"
        "result."))
    ("while"
        ("..."))
    ("wrcar"
        ("..."))
    ("wrcdr"
        ("..."))
    ("xor"
        ("b b xor ==> b"
        ""
        "Performs logical-xor of two boolean values. Boolean result."))
    ("zap"
        ("A B zap ==> A"
        ""
        "Removes the top item from the stack."))

-----------------------------------------------------------------------------
--                                                                         --
--                           babel guts topics                             --
--                                                                         --
-----------------------------------------------------------------------------
    ("mword"
        ("MWORD is short for machine-word and stands for an unsigned int or"
        "pointer whose size is the native word size of the machine. In other words,"
        "an mword is either a 32-bit or 64-bit unsigned int or unsigned int pointer"
        "that is treated like a void pointer (to put it in C terminology)."
        ""
        "MWORD_SIZE is the native word size (in bits) divided by 8 bits per byte."
        "On 32-bit machines, MWORD_SIZE is 4. On 64-bit machines, it is 8."))
    ("bstruct"
        ("A bstruct consists of one or more of the following data structures,"
        "connected together:"
        ""
        "- leaf-array     : stores values"
        "- interior-array : stores pointers"
        "- tagged-pointer"
        ""
        "A bstruct is (intentionally) defined in such a way that it may contain"
        "any sort of data. There is nothing specific to Babel about a bstruct. For"
        "example, a large bitmap can be stored as a leaf-array by simply prepending"
        "it with the appropriate s-field in memory."
        ""
        "See also: 'val', 'ptr', 'tptr', 's-field'"))
    ("s-field"
        ("To tell apart the three types of array, each array has an s-field."
        "The s-field is a single mword at position 0 in the allocated array."
        ""
        "    s(X) > 0             X is a leaf-array"
        "    s(X) = 0             X is a tagged-pointer"
        "    s(X) < 0             X is an interior-array"
        ""
        "Aside from telling the array type, the s-field also tells the array size."
        "The size is encoded in bytes so you have to divide by MWORD_SIZE to get"
        "the size in mwords."
        ""
        "    s(X) > 0             size(X) = s(X) / MWORD_SIZE"
        "    s(X) = 0             size(X) =   1  + HASH_SIZE"
        "    s(X) < 0             size(X) =  -1  * (s(X) / MWORD_SIZE)"
        ""
        "The size of every array in a bstruct is an even multiple of MWORD_SIZE."
        "The array8 and array1 structures use an alignment word in the last position"
        "of the array but the size of the array itself is still an even multiple"
        "of MWORD_SIZE."))
    ("val"
        ("A value-array contains one or more values stored in an array of mwords." 
        "The defining feature of a value-array is that it cannot contain pointers to"
        "any other arrays. A value-array can contain one or more numeric values"
        "or it may contain a string. You can create a multi-line string within a"
        "value-array simply by listing multiple quotations. Valid value-arrays:"
        ""
        "    [val 1 2 3]"
        "    [1 2 3]"
        "    [0x123 0x456]"
        "    ['foobar']"
        "    ['foo' 'bar']"
        '    ["hello, world"]'))
    ("array8"
        ("An array8 is a value-array with an added alignment-word at the end of the value-"
        "array. See also: 'string'"))
    ("ptr"
        ("A pointer-array (ptr) contains one or more pointers, each mword-sized. "
        ""
        "The defining feature of a pointer-array is that every mword in a pointer-"
        "array must contain a pointer to a bstruct."))
    ("tptr"
        ("A tagged-pointer is a pointer bundled with an associated tag. A tag is a "
        "unique hash. The Babel interpreter recognizes certain tags. The special value"
        "nil is a tptr that is always recognized by the interpreter. User-tags are not"
        "recognized by the interpreter but there are automatic mechanisms for"
        "specifying how user-tags are to be handled."
        ""
        "See also: 'bstruct', 'val', 'ptr', 's-field'"))
    ("stack"
        "Babel is a stack-based language. The stack is the single most important structure"
        "in Babel. Every operand is placed on the stack before it is operated on. To"
        "manipulate the stack, use one of Babel's stack operators. For more information"
        "about the stack operators, enter: 'toc' man !"
    ("map"
        ("Babel supports associative-arrays - in Babel, they are referred to as maps. The"
        "root symbol-table is implemented as a map, so you can use any of the built-in"
        "map operators to perform lookups in the symbol-table. The std.sp utility defines"
        "a wide range of map utilities. To learn more, read the man entries for any of the"
        "following utilities."
        ""
        "    lumap, lumapls, insmap, insmapls, exmap, rmmap, valmap, tagmap, overmap,"
        "    eachmap, dirmap, showmap, map2ls, mergemap, tmergemap, sizemap, emptymap,"
        "    mkmap, keymap, ls2map, group"))
    ("list"
        ("Babel has support for lists. To create a list lexically, use the parentheses."
        "For example, to create a list of numbers:"
        ""
        "    (1 2 3)"
        ""
        "Babel's list data-type is modeled on Lisp. To see topics covering Babel's list"
        "operators, enter: 'toc' man !"
        ""
        "The std.sp library also contains user-friendly support for common list"
        "operations. To learn more, read the man entries for any of the following utilities."
        ""
        "    collect, compose, compose_eval, lsend, lsshow, lsstr, lsnum, lscmp, group,"
        "    lspair, lsunpair, lsrol, lsror, lsrotn, lswise, eachpair, lsmix, cdri, lsset,"
        "    lsswap, lstrue, lsfalse, lsrange, lssort, lsdup, lseven, lsodd"))
    ("string"
        ("Babel has native support for UTF-8 encoded strings. Babel strings are not"
        "null-terminated. However, a Babel-string stored in array-8 form is always"
        "C-string safe because the alignment-word at the end of an array-8 always"
        "contains one or more null bytes. For example, in 32-bit Babel, the "
        "alignment word is one of:"
        ""
        "    0x00000000      byte-length % 4 = 0"
        "    0xffffff00      byte-length % 4 = 1"
        "    0xffff0000      byte-length % 4 = 2"
        "    0xff000000      byte-length % 4 = 3"
        ""
        "Babel handles strings in several different forms:"
        ""
        "- Native form. The string is UTF-8 encoded WITHOUT a null terminator in"
        "an array8 leaf-array"
        ""
        "- C-style. This is just a native string with a null terminator appended."
        ""
        "- String-array. This is a leaf-array such that each entry in the array "
        "contains the Unicode code-point of the encoded character. It is created"
        "from a native-form string via the str2ar operator."
        ""
        "- String-list. This is a string-array on which the the ar2ls operator"
        "has been called."))

-----------------------------------------------------------------------------
--                                                                         --
--                            std.sp utilities                             --
--                                                                         --
-----------------------------------------------------------------------------
    ("include" 
        ("filename include !"
        ""
        "Loads BPDL file (typically .sp) from disk and then converts it to bstruct form."
        ""
        "Defined in std.sp."))
    ("loadfs" 
        ("filename loadfs !"
        ""
        "Loads Babel file (typically .bbl) from disk and then converts it to bstruct form."
        ""
        "Defined in std.sp."))
    ("savefs" 
        ("filename savefs !"
        ""
        "Saves bstruct to Babel file on disk."
        ""
        "Defined in std.sp."))
    ("fold" 
        ("(A B ...) {code} fold !"
        ""
        "Evaluates {code} over list as if it were an infix operation. Example:"
        ""
        "    (1 2 3 4) {+} fold ! itod <<     --> Result is 10"
        ""
        "Defined in std.sp."))
    ("compose" 
        ("(A B ...) (C D ...) compose ! ==> (A B ... C D ...)"
        ""
        "Directly joins two lists. See also: 'append'"
        ""
        "Defined in std.sp."))
    ("ls2map" 
        ("(('foo' A) ('bar' B) ...) ls2map ! ==> map with keys 'foo', 'bar', etc."
        ""
        "Builds a map (hash-table) from a list of key-value pairs. Each key-value pair"
        "must be in its own two-element list."
        ""
        "Defined in std.sp."))
    ("lsbyn" 
        ("(A B ...) N ls2byn ! ==> ( (A B ...) ... )"
        ""
        "Takes input list by N. For example, if N=2, takes the input two by"
        "two. Result is a list-of-lists (matrix)."
        ""
        "Defined in std.sp."))
    ("lspair" 
        ("(A B ...) (C D ...) lspair ! ==> ( (A C) (B D) ... )"
        ""
        "Pairs elements of two lists. Same as 'map' in some other programming"
        "languages. See also: 'lsunpair'"
        ""
        "Defined in std.sp."))
    ("lsrol" 
        ("(A B ...) N lsrol ! ==> (B ... A)"
        ""
        "Rotates input list to the left. See also: 'lsror', 'lsrotn'"
        ""
        "Defined in std.sp."))
    ("lsror" 
        ("(A ... Z) N lsrol ! ==> (Z A ...)"
        ""
        "Rotates input list to the left. See also: 'lsror', 'lsrotn'"
        ""
        "Defined in std.sp."))
    ("collect" 
        ("... B A collect ! ==> (... B A)"
        ""
        "Collect puts entire dstack into a list."
        ""
        "Defined in std.sp."))
    ("takeptr" 
        ("... B A N takeptr ! ==> [ptr ... B A]"
        ""
        "Takes N elements from dstack and puts them into a ptr-array."
        ""
        "Defined in std.sp."))
    ("takeval" 
        ("... B A N takeval ! ==> [val ... B A]"
        ""
        "Takes N elements from dstack and puts them into a val-array."
        ""
        "Defined in std.sp."))
    ("over" 
        ("(A B ...) {code} over ! ==> list"
        ""
        "Performs {code} over each element of input list. Resulting list depends"
        "on {code}. Example:"
        ""
        "    (1 2 3) {1 +} over ! ==> (2 3 4)"
        ""
        "Defined in std.sp."))
    ("overptr" 
        ("[ptr A B ...] {code} overptr ! ==> ptr-array"
        ""
        "Performs {code} over each element of input array. Resulting array depends"
        "on {code}. Example:"
        ""
        "    [ptr 1 2 3] {1 +} overptr ! ==> [ptr 2 3 4]"
        ""
        "Defined in std.sp."))
    ("overval" 
        ("[val A B ...] {code} overval ! ==> val-array"
        ""
        "Performs {code} over each element of input list. Resulting list depends"
        "on {code}. Example:"
        ""
        "    [val 1 2 3] {1 +} overval ! ==> [val 2 3 4]"
        ""
        "Defined in std.sp."))
    ("any?" 
        ("X ({A} {B} ...) any? ! ==> 0|1"
        ""
        "Evaluates each code-list over X. Returns 1 if any code-list returns"
        "true, otherwise, returns 0."
        ""
        "Defined in std.sp."))
    ("all?" 
        ("X ({A} {B} ...) all? ! ==> 0|1"
        ""
        "Evaluates each code-list over X. Returns 1 if all code-lists return"
        "true, otherwise, returns 0."
        ""
        "Defined in std.sp."))
    ("lsunpair" 
        ("( (A C) (B D) ... ) lsunpair ! ==> (A B ...) (C D ...) "
        ""
        "Un-pairs elements of a list of paired lists. See also: 'lspair'"
        ""
        "Defined in std.sp."))
    ("matcol" 
        ("matrix N matcol ! ==> list"
        ""
        "Returns the Nth column of a matrix as a list. Example:"
        ""
        "    ((1 2 3) (4 5 6)) 1 matcol ! ==> (2 5)"
        ""
        "Defined in std.sp."))
    ("matrow" 
        ("matrix N matrow ! ==> list"
        ""
        "Returns the Nth row of a matrix as a list. Example:"
        ""
        "    ((1 2 3) (4 5 6)) 1 matcol ! ==> (4 5 6)"
        ""
        "Defined in std.sp."))
    ("matdiag" 
        ("matrix matdiag ! ==> list"
        ""
        "Returns the diagonal of a matrix as a list. Example:"
        ""
        "    ((1 2 3) (4 5 6) (7 8 9)) matdiag ! ==> (1 5 9)"
        ""
        "Defined in std.sp."))
    ("mattrans" 
        ("matrix mattrans ! ==> matrix"
        ""
        "Returns the transpose of a matrix."
        ""
        "    ((1 2 3) (4 5 6) (7 8 9)) mattrans ! matshow ! ==>"
        ""
        "    [val 0x1 ]  [val 0x4 ]  [val 0x7 ]"
        "    [val 0x2 ]  [val 0x5 ]  [val 0x8 ]"
        "    [val 0x3 ]  [val 0x6 ]  [val 0x9 ]"
        ""
        "Defined in std.sp."))
    ("mateach" 
        ("matrix {A} mateach !"
        ""
        "Evaluates A for each element of input matrix."
        ""
        "Defined in std.sp."))
    ("matover" 
        ("matrix {A} matover ! ==> matrix"
        ""
        "Evaluates A over each element of input matrix and collects results into"
        "a matrix."
        ""
        "Defined in std.sp."))
    ("matij" 
        ("matrix i j matij ! ==> element"
        ""
        "Retrieves the i,jth element from input matrix."
        ""
        "Defined in std.sp."))
    ("lsrotn" 
        ("list N lsrotn ! ==> rotated-list"
        ""
        "Rotates input list to the right by N elements."
        ""
        "Defined in std.sp."))
    ("matpair" 
        ("matrix matrix matpair ! ==> paired-matrix"
        ""
        "Pairs elements of two matrices."
        ""
        "Defined in std.sp."))
    ("matwise" 
        ("A B {C} matwise !"
        ""
        "Evaluates C on matrices A and B element-wise, that is, element-by-element."
        "Example:"
        ""
        "    ((1 2) (3 4)) ((5 6) (7 8)) {+} matwise ! matshow ! ==>"
        ""
        "    [val 0x6 ]  [val 0x8 ]"
        "    [val 0xa ]  [val 0xc ]"
        ""
        "Defined in std.sp."))
    ("matrotn" 
        ("matrix N matrotn ! ==> rotated-matrix"
        ""
        "Rotates input matrix to the right by N elements."
        ""
        "Defined in std.sp."))
    ("matmix" 
        ("matrix (row-indices) matmix ! ==> mixed-matrix"
        ""
        "Mixes input matrix rows according to list of row-indices, I. Can increase"
        "or decrease matrix row dimension. Example:"
        ""
        "    ((1 2) (3 4) (5 6)) (1 0 2 0) matmix ! matshow ! ==>"
        ""
        "    [val 0x3 ]  [val 0x4 ]"
        "    [val 0x1 ]  [val 0x2 ]"
        "    [val 0x5 ]  [val 0x6 ]"
        "    [val 0x1 ]  [val 0x2 ]"
        ""
        "Defined in std.sp."))
    ("matcolmix" 
        ("matrix (col-indices) matcolmix ! ==> mixed-matrix"
        ""
        "Mixes input matrix columns according to list of col-indices, I. Can increase"
        "or decrease matrix column dimension. Example:"
        ""
        "    ((1 2 3) (4 5 6)) (1 0 2 0) matcolmix ! matshow ! ==>"
        ""
        "    [val 0x2 ]  [val 0x1 ]  [val 0x3 ]  [val 0x1 ]"
        "    [val 0x5 ]  [val 0x4 ]  [val 0x6 ]  [val 0x4 ]"
        ""
        "Defined in std.sp."))
    ("matcolcat" 
        ("matrix matrix matcolcat ! ==> cat'd-matrix"
        ""
        "Concatenates two matrices column-wise. Example:"
        ""
        "    ((1 2) (3 4)) ((5 6) (7 8)) matcolcat ! matshow ! ==>"
        ""
        "    [val 0x1 ]  [val 0x2 ]"
        "    [val 0x3 ]  [val 0x4 ]"
        "    [val 0x5 ]  [val 0x6 ]"
        "    [val 0x7 ]  [val 0x8 ]"
        ""
        "Defined in std.sp."))
    ("matrowcat" 
        ("matrix matrix matrowcat ! ==> cat'd-matrix"
        ""
        "Concatenates two matrices row-wise. Example:"
        ""
        "    ((1 2) (3 4)) ((5 6) (7 8)) matrowcat ! matshow ! ==>"
        ""
        "    [val 0x1 ]  [val 0x2 ]  [val 0x5 ]  [val 0x6 ]"
        "    [val 0x3 ]  [val 0x4 ]  [val 0x7 ]  [val 0x8 ]"
        ""
        "Defined in std.sp."))
    ("matshow" 
        ("matrix matshow !"
        ""
        "Displays the elements of a matrix."
        ""
        "    ((1 2) (3 4)) matshow !"
        ""
        "    [val 0x1 ]  [val 0x2 ]"
        "    [val 0x3 ]  [val 0x4 ]"
        ""
        "Defined in std.sp."))
    ("cart" 
        ("list list cart ! ==> list-of-lists"
        ""
        "Takes the Cartesian-product of two lists. Returns a list-of-lists (matrix)."
        ""
        "    (1 2) (3 4) cart ! matshow !"
        ""
        "    [val 0x1 ]  [val 0x3 ]"
        "    [val 0x1 ]  [val 0x4 ]"
        "    [val 0x2 ]  [val 0x3 ]"
        "    [val 0x2 ]  [val 0x4 ]"
        ""
        "Defined in std.sp."))
    ("lumap" 
        ("map 'foo' lumap ! ==> map-entry"
        ""
        "Looks up string-key 'foo' in map. Returns the value stored in the respective"
        "entry, if it exists. Otherwise, returns nil."
        ""
        "Defined in std.sp."))
    ("lumapls" 
        ("<map> (key-list) lumapls ! ==> (val-list)"
        ""
        "For each string-key in key-list, looks up string-key in map. Returns a list;"
        "one value for each key in key-list, respectively, if it exists. If the entry"
        "corresponding to a key does not exist in the map, nil is added to the val-list"
        "for that entry. The resulting val-list is in the same order as key-list."
        ""
        "Defined in std.sp. See also: 'lumap', 'luha'"))
    ("insmap" 
        ("map 'foo' A insmap !"
        ""
        "Inserts A into map with string-key 'foo'. No return value."
        ""
        "Defined in std.sp."))
    ("insmapls"
        ("<map> (key-value-pair-list) insmapls ! ==>"
        ""
        "Inserts a list of key-value pairs into map."
        ""
        "Defined in std.sp. See also: 'insmap', 'insha'"))
    ("exmap" 
        ("map 'foo' exmap ! ==> 0|1"
        ""
        "Tests existence of entry with string-key 'foo' in map. Returns 1 if"
        "the entry exists. Otherwise, returns 0."
        ""
        "Defined in std.sp."))
    ("rmmap" 
        ("map 'foo' rmmap ! ==> 0|1"
        ""
        "Removes entry with string-key 'foo' in map, if it exists. Returns 1 if"
        "the entry was removed. Otherwise, returns 0."
        ""
        "Defined in std.sp."))
    ("valmap" 
        ("map valmap ! ==> (val0 val1 ...)"
        ""
        "Returns a list of values for every entry in map."
        ""
        "Defined in std.sp."))
    ("keymap" 
        ("map keymap ! ==> (key0 key1 ...)"
        ""
        "Returns a list of string-keys for every entry in map."
        ""
        "Defined in std.sp."))
    ("tagmap" 
        ("map tagmap ! ==> (tag0 tag1 ...)"
        ""
        "Returns a list of tag-keys for every entry in map."
        ""
        "Defined in std.sp."))
    ("map2ls" 
        ("map map2ls ! ==> ((key0 val0) (key1 val1) ...)"
        ""
        "Returns a list of key/value pairs, one for each entry in map."
        ""
        "Defined in std.sp."))
    ("import" 
        ("map import !"
        ""
        "Inserts each entry in map into the symbol-table. No return value."
        ""
        "Defined in std.sp."))
    ("mkbvm" 
        ("{A} mkbvm !"
        ""
        "Converts code-list A into a Babel Virtual Machine (BVM) suitable for"
        "execution by the babel operator."
        ""
        "Defined in std.sp."))
    ("stepbvm"
        ("bvm stepbvm !"
        ""
        "Steps bvm by one step. Equivalent to 1 stepnbvm !"
        ""
        "Defined in std.sp."))
    ("stepnbvm" 
        ("bvm N stepnbvm !"
        ""
        "Steps bvm by N steps."
        ""
        "Defined in std.sp."))
    ("cibvm" 
        ("bvm {A} cibvm !"
        ""
        "Injects code-list A into bvm. Stepping the bvm will now result in evaluation"
        "of code-list A. When the end of A is reached, continued stepping will"
        "continue with the code-list that was already in bvm when A was injected."
        ""
        "Defined in std.sp."))
    ("stackbvm" 
        ("bvm stackbvm !"
        ""
        "Takes the dstack of bvm as a list and places on TOS. The dstack can"
        "now be inspected by viewing the list, for example, by using the lsshow"
        "operator."
        ""
        "Defined in std.sp."))
    ("say" 
        ("'A' say !"
        ""
        "Prints string A and then prints a newline."
        ""
        "Defined in std.sp."))
    ("sd" 
        ("sd !"
        ""
        "Displays the stack in an easy-to-read format. 'TOS' stands for 'Top Of Stack'"
        "and 'BOS' stands for 'Bottom Of Stack.' Because Babel has two stacks (dstack"
        "and ustack), the stack display will arrange items between the TOS and BOS"
        "labels for items on the dstack, and it will arrange items below BOS that"
        "are on the ustack."
        ""
        "Defined in std.sp."))
    ("pr" 
        ("('A' 'B' ...) pr !"
        ""
        "Prints a list of strings. Does not print a newline between list items."
        "See also: 'prs', 'prn', 'say'"
        ""
        "Defined in std.sp."))
    ("prs" 
        ("('A' 'B' ...) prs !"
        ""
        "Prints a list of strings. Prints a space between list items. See also:"
        "'pr', 'prn', 'say'"
        ""
        "Defined in std.sp."))
    ("prn" 
        ("('A' 'B' ...) prs !"
        ""
        "Prints a list of strings. Prints a newline between list items. See also:"
        "'pr', 'prs', 'say'"
        ""
        "Defined in std.sp."))
    ("sum" 
        ("(A B ...) sum !"
        ""
        "Sums the elements of the input list. Returns numeric value."
        ""
        "Defined in std.sp."))
    ("prod" 
        ("(A B ...) prod !"
        ""
        "Multiplies the elements of the input list. Returns numeric value."
        ""
        "Defined in std.sp."))
    ("lscat" 
        ("('A' 'B' ...) lscat !"
        ""
        "Concatenates a list of strings. Returns string."
        ""
        "Defined in std.sp."))
    ("lsand" 
        ("(A B ...) lsand !"
        ""
        "Takes the bitwise-AND of a list of numeric values. Returns numeric value."
        ""
        "Defined in std.sp."))
    ("lsor" 
        ("(A B ...) lsor !"
        ""
        "Takes the bitwise-OR of a list of numeric values. Returns numeric value."
        ""
        "Defined in std.sp."))
    ("lsxor" 
        ("(A B ...) lsxor !"
        ""
        "Takes the bitwise-XOR of a list of numeric values. Returns numeric value."
        ""
        "Defined in std.sp."))
    ("timed"
        ("{code} timed ! ==>"
        ""
        "Prints the number of milliseconds transpired during execution of"
        "code to STDOUT. Places nothing on stack."
        ""
        "Defined in std.sp."))
    ("dirmap" 
        ("map dirmap ! ==> (key0 key1 ...)"
        ""
        "Prints a list of keys for every entry in map."
        ""
        "Defined in std.sp.")))
    ls2map ! < 

    "To see the man Table of Contents, enter: 'toc' man !\n" << }

