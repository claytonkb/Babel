{ page 1 <

tut_head
    { " ----------------------- Babel Command-Line Tutorial --------[ Page "
      <<
      "  " dup 0 cp "%d" page sprint
      <<
      " ]---- \n\n" 
      << } <

tut_pages
    (("Welcome to the Babel command-line tutorial. At each step, the tutorial will"
    "explain one or more facts about Babel, then turn the command-line over to you"
    "to explore as you like.  When you are ready to move on to the next step of the"
    "tutorial, enter 'ok !' without the quotes.")

    ("Babel is still in development, so if you mistype something during the"
    "tutorial, it may cause the interpreter to crash. Simply restart the"
    "interpreter and restart the tutorial. You can enter the command:"
    ""
    "    page n < ok !"
    ""
    "where n is the page you want to go to. You can exit at any time by entering"
    "'exit'. Enter 'ok !' when you're ready.")

    ("To begin with, let's explore the basic operation of the command-line. The"
    "command-line consists of a prompt (see the > below). You can type whatever you"
    "like at the prompt and then press Enter and Babel will perform the code as"
    "entered. Babel's command-line does not automatically display any results unless"
    "you explicitly tell it to. There are many ways to display results in Babel. The"
    "simplest and most common is the semicolon operator, which can be inserted"
    "anywhere you like to display the results at that point. Enter 'ok !' when you're"
    "ready.")

    ("To illustrate, let's enter some strings. There are two ways to enter"
    "strings in Babel - single-quote or double-quote. Single-quote and double-quote"
    "strings are identical, except that the escape-character is a backslash for"
    "double-quote strings and a slash for single-quote strings. Try entering the"
    "following lines at the prompt. Enter 'ok !' when you are ready for the next"
    "step of the tutorial."
    ""
    '    "hello" ;'
    "    'hello' ;"
    "    '\"' ;"
    '    "\n" ;'
    "    '/n' ;")

    ("You will notice that the 'results' displayed by the semicolon operator don't look"
    "anything like what you typed in. This is because the semicolon operator does not"
    "know the type of the thing it is trying to display. It displays the raw hexadecimal"
    "bytes (ASCII, in this case) of each string that you type in. Babel does not impose"
    "types, though it can support types."
    ""
    "Now, let's look at numerics. You can enter numbers in either decimal or"
    "hexadecimal form:"
    ""
    "    10 ; 348598 ; 0x239a38f ;")

    ("Babel is a stack-based language. As you have been typing things in, they have been"
    "building up on the stack. Because this is a development build of Babel, you can use"
    "the dev operator to quickly and easily view the stack at any time. Try the following"
    "commands:"
    ""
    "    5 dev"
    "    clear"
    "    5 dev"
    ""
    "Because we will be using this dev operation frequently throughout the tutorial, we"
    "have defined the operation sd (for 'stack display') that does the same thing as"
    "5 dev. Invoke it as follows:"
    ""
    "    1 2 3 sd !")

    ("As you can see, the clear operator removes everything from the stack. TOS"
    "stands for Top-Of-Stack and BOS stands for Bottom-Of-Stack. The Babel stack"
    "actually consists of two stacks joined together to form a circle. The main"
    "stack is called the 'down-stack' or dstack. Whenever you enter something at"
    "the command prompt, it goes onto the dstack. The top of the dstack is the"
    "TOS. The other stack is called the 'up-stack' or ustack. The top of the"
    "ustack is BOS. We will return to this topic later in the tutorial and explain"
    "it in more detail.")

    ("Babel supports basic I/O operations, such as text input and output. To print"
    "a string, use the << operator. Try printing some strings:"
    ""
    '    "Hello, world\n" <<'
    "    'foo' << 'bar' <<"
    ""
    "A line-comment in Babel is specified with a double dash: --"
    "You can place comments on the command-line and they will work as expected"
    ""
    '   -- "This statement has no effect" <<')

    ("If you check the stack after printing a string, you will see that the stack is"
    "empty. This is because the print operator *consumes* the string. Not all"
    "operators consume their inputs, but most do. If you want to preserve an operand"
    "(so you can use it later), use the dup operator:"
    ""
    '    "Goodbye, cruel world.\n" dup << <<')

    ("Sometimes, after performing several operations, the items on the stack are not"
    "in the correct order for the next operation you want to perform. If you need to"
    "reverse the top two items on the stack, use the swap operator:"
    ""
    '    "Who/'s " << "on " "first\n" swap << <<')

    ("The swap operator is very limited - it can only reverse the order of the top two"
    "items on the stack. This is one reason that Babel maintains two stacks. You can"
    "move items from the dstack to the ustack by using the down (or <-) operator."
    "Conversely, you can move items back from the ustack to the dstack by using the"
    "up (or ->) operator. It is easier to see for yourself how this works than it is"
    "to explain it. Try the following:"
    ""
    "    clear 0x123 0x456 sd !"
    "    down sd !"
    "    clear 0x123 0x456 0x789 down down sd !"
    "    up up sd !")

    ("Like C, Babel requires you to explicitly convert a number to a string before"
    "displaying it. To convert a number to decimal, use the itod operator:"
    ""
    "    123 itod <<")

    ("Because Babel is stack-based, the order of operations is called postfix-order."
    "In particular, arithmetic expressions are entered in postfix or Reverse Polish"
    "Notation order. Here are some example arithmetic expressions:"
    ""
    "    2 2 + itod <<"
    "    7 10 + 3 * itod <<"
    "    120 4 / 10 - itod <<")

    ("On the Babel command-line, you can create a variable simply by giving a name"
    "and assigning something to that name using the easy-set operator <."
    ""
    "    Yo 'Adrian' <"
    "    Yo <<"
    "    foo 3 <"
    "    foo 10 * itod <<")

    ("The bang operator ! means 'do this'. It can also be written as 'eval' and"
    "its operation is similar to the eval operator in Lisp or other languages"
    "that support eval. Use the curly-braces to define code, just as you would"
    "have typed it at the command-prompt. In Babel, code is just another kind of"
    "data, so we can create a bit of code on the command-line and use eval (bang)"
    "to run it:"
    ""
    '    {"Howdy do\n" <<}'
    "    eval"
    '    {"I/'m fine, thank ya\n" <<} !')

    ("It is not convenient to always have to type the same thing repeatedly, as we"
    "have with the itod and print operation. So, we can create some code and store"
    "it in a variable for later use. This is not the same as a function definition,"
    "because the code we define in this way does not have its own local context. It"
    "is equivalent to defining a word in Forth. Let's define some code in a variable"
    "'np' (for 'numeric print') to save keystrokes when displaying numbers:"
    ""
    '    np {itod << "\n" <<} <'
    "    9 dup * np !"
    "    2 3 5 7 + + + np !")

    ("Babel has a variety of looping constructs. The simplest is the times operator."
    "The times operator allows you to repeat a bit of code as many times as you specify."
    "For example:"
    ""
    '    "Larry" "Mo" "Curly" {<< "\n" <<} 3 times')

    ("This code didn't do what we might have naively expected it to, because the stack"
    "reverses the visual order of things. The Babel way to print a series of strings"
    "is to create a *list* of strings, and then use the each operator, or ellipsis '...'"
    "Use parentheses to create a list:"
    ""
    '    ("Larry" "Mo" "Curly") {<< "\n" <<} each'
    "    (1 1 2 3 5 8) { itod << } ..."
    ""
    "Use the eachar operator to iterate over a pointer-array (not leaf-array):"
    ""
    "   ['fido' 'spot' 'clifford'] {<< ' ' <<} eachar"
    ""
    "Use the fold utility from std.sp to interpolate code between elements of a list:"
    ""
    "    (1 2 3 4 5) {+} fold ! itod <<")

    ("To illustrate the general power of the operators we have explored up to this point,"
    "let's see how to perform several different operations on the same data. Let"
    "us say we want to calculate the integer sum, difference, product and ratio of"
    "two numbers. In Babel, we can create a list of code, and then use the each operator"
    "to iterate over this list and apply each element of the list to our numbers:"
    ""
    '    ({+} {-} {*} {//}) {down 173 42 up eval itod << "\n" <<} each')

    ('To get input from the user, use the >> operator:'
    ''
    '  "What is your name? " << >> "Hello, " << << ", it/'s nice to meet you.\n" <<')

    ("As of this writing, Babel's control-flow constructs are still in the process"
    "of being coded. You can perform a conditional statement by using the selr"
    "operator."
    ""
    '    sex { "M" arcmp "male" "female" selr <<} <'
    '    "Enter M if you are male, F otherwise " << >> "You are " << sex !'
    ""
    "{'Enter a number: ' << >>} 2 times 'The first number ' << ge? 'was' 'was not' selr << ' greater/n' <<")

    ("In Babel, all data, code, symbols, stack, etc. are stored in a uniform,"
    "recursive data-structure called a bstruct. A bstruct consists of one or more"
    "of the following array (vector) data-structures:"
    ""
    "    - value-array (or, leaf-array, val, va, leaf or lf)"
    "    - pointer-array (or, interior-array, ptr, inte or in)"
    "    - tagged-pointer (or tptr or tag)")

    ("A value-array contains numeric or string data or other kinds of data that do"
    "not contain pointers. In Babel Programming Description Language (BPDL), the "
    "square-brackets are the general-purpose constructor. To create a value-array:"
    ""
    "    [val 1 2 3 4]"
    '    [val "Hello, world\n"]'
    ""
    "A value-array that contains a string should contain only one string. The"
    "syntax will permit you to define a value-array with multiple strings but the"
    "behavior of such 'strings' is not well-defined in Babel. As a shorthand, you"
    "can omit 'val', because square-brackets containing only constants are defined"
    "to be a value-array:"
    ""
    "    clear [val 1 2 3 4] [1 2 3 4] sd !")

    ("A pointer-array contains only pointers to other bstructs (value-array, pointer-"
    "array or tagged-pointer). Pointer-arrays are how you create 'nested' data-"
    "structures in Babel. The regular parentheses construct a Lisp-style cons-list,"
    "but this is just a short-hand for nested pointer-arrays:"
    ""
    "    clear"
    "    (1 2)"
    "    [ptr [val 1] [ptr [val 2] nil ]]"
    "    [list 1 2]"
    "    sd !"
    ""
    "As a shorthand, if a value-array contains only one value, you can eliminate the"
    "square brackets for that array:"
    ""
    "    clear [ptr 1 2 3] ; [ptr [val 1] [val 2] [val 3]] ;"
    ""
    "This works for strings, as well:"
    ""
    '    clear ["foo" "bar"] ; [ptr "foo" "bar"] ; [ptr [val "foo"] [val "bar"]] ;')

    ("A tagged-pointer is a single pointer that has an associated tag. A tag is a 128-"
    "bit numeric value that is generated by using a hashing function. The pointer in"
    "a tagged pointer can point to any bstruct - value-array, pointer-array or tagged-"
    "pointer. To see the hashing function in operation:"
    ""
    "    clear"
    "    'foo' hash ;"
    "    [tag 'foo' nil] ;"
    ""
    "You will notice that the header of the tptr that is generated matches the value-"
    "array generated by the hash operator. You can nest anything in a tagged-pointer:"
    ""
    "    clear"
    "    [tag 'foo' ptr 1 2 3] ;"
    "    [tag 'bar' val 1 2 3] ;"
    "    [tag 'baz' code 'Hi!' <<] ;"
    "    [tag 'bop' tag 'wop' val 0x123456] ;"
    "    [tag 'hop' list 1 2 3] ;"
    "    polo (4 5 6) <"
    "    [tag 'marco' polo] ;")

    ("Tags are how Babel assigns type-labels to data-objects within a bstruct. A"
    "hash-table, for example, contains hash_table_entry tags in order to demarcate"
    "entries in the hash-table from the branches of the table itself. You can use"
    "the newtag operator to create a tagged-pointer from an existing bstruct:"
    ""
    '    [1 2 3] "foo" newtag ;'
    ""
    "Given this tptr, you can get the pointer itself by using the detag operator:"
    ""
    "    dup detag ;"
    ""
    "Or, if you just want the tag from this tptr, you can use the get_tag operator"
    "to get a leaf-array containing a copy of the tag alone:"
    ""
    "    zap get_tag ;"
    ""
    "You can check whether two tptr's have the same tag, or compare a leaf-array tag"
    "against a tptr using the tageq?  operator:"
    ""
    '    [tag "foo" nil] "foo" hash tageq? ;')

    ("It can be difficult to visualize a bstruct data-structure when it contains more"
    "than a few elements. Babel provides a robust suite of introspection operators"
    "to assist in visualization of bstruct data-structures. The bs2gv operator converts"
    "a bstruct to a string containing a Graphviz dot language file which can be "
    "converted to an image using the free software utility, dot. For example:"
    ""
    "    [tag 'foo' list (1 2 3) ('a' 'b' 'c') (4 5 6)] bs2gv"
    ""
    "You could print the string to STDOUT but this would not be very helpful. Instead,"
    "write it to file using the <<< operator:"
    ""
    '    "my_bstruct.dot" <<<')

    ("If you have the Graphviz dot utility available on your platform, you can use a"
    "command like the following to convert this dot-file to a suitable image format:"
    ""
    "    dot -Tsvg my_bstruct.dot > my_bstruct.svg"
    ""
    "Open the image file in a suitable viewer, and you will see a visual depiction"
    "of the bstruct data-structure. Bold-outlined boxes are value-arrays, normal-"
    "outlined boxes are pointer-arrays, and tagged-pointers are displayed as an array"
    "of four mwords linked to a single pointer without an arrow. Note that nil is"
    "never displayed and is implied by any pointer field without an outgoing arrow."
    "The dash-outlined box is the base-pointer (unless it is a tptr).")

    ("The semicolon operator is actually a pseudo-operator which utilizes the show"
    "operator behind the scenes. You can explicitly call show to generate a string on"
    "the stack which you can later use for something other than printing to STDOUT."
    "Here, we will simply print it out:"
    ""
    "    (1 2 3) show <<")

    ("The show operator can only fully display certain kinds of bstructs (tree-like"
    "bstructs, that is, bstructs which are not graphs). A robust operator that displays"
    "every field of a bstruct in BPDL syntax is the bs2str operator. Use this operator"
    "to display complex bstructs."
    ""
    "    (1 2 3) {dup cp 2 take} 2 times bs2str <<")

    ("If you want to access a specific field of a bstruct array, use the th operator"
    "- the mnemonic for this operator is the most common ordinal suffix - fourTH,"
    "fifTH, sixTH, and so on."
    ""
    "    [2 4 6 8 10] 3 th itod <<"
    ""
    "The th operator works for pointer-arrays, as well. Arrays, lists, etc. are "
    "0-indexed.")

    ("The Babel stack facilitates advanced operations beyond the <- and -> (down and "
    "up) operators. There are two stacks, each of which is double-ended and the ends"
    "are joined together to form a circular data structure. It may help to visualize"
    "the stack graphically:"
    ""
    "   'a' 'b' 'c' 'd' down down"
    ""
    "     +----------------+"
    "     |                |"
    "    'a'               |"
    "    'b'               |"
    "    ---               ^ BOS"
    "    'c' <==TOS        |"
    "    'd'               |"
    "     |                |"
    "     +----------------+")

    ("Let us suppose you want to use the babel stack as a FIFO (queue data-"
    "structure)."
    ""
    "    clear"
    '    "Larry" "Mo" "Curly" flip rot {<< " " <<} 3 times'
    ""
    "Notice that, this time, the stooges printed in the visual order in which they"
    "are written. You can treat the stack as a FIFO by using flip rot to toggle "
    "between the input and ouput ends of the FIFO. You can examine the function of "
    "flip alone to understand what it does:"
    ""
    "    clear"
    "    1 2 3 sd !"
    "    flip sd !"
    "    flip sd !"
    ""
    "It puts everything in the dstack on the ustack and vice-versa.")

    ("The rot operator 'vertically rotates' the stack around BOS:"
    ""
    "    clear"
    "    1 2 3 sd !"
    "    rot sd !"
    "    rot sd !"
    ""
    "Note that these operators operate on both stacks simultaneously:"
    ""
    "    clear"
    "    1 2 3 4 5 <- <- sd !"
    "    flip sd !"
    "    rot sd !"
    "    flip sd !"
    "    rot sd !")

    ("The final advanced stack operator operates only on the dstack - it reverses the"
    "order of the dstack without altering the ustack."
    ""
    "   clear 1 2 3 4 5 <- <- sd !"
    "   twist sd !"
    ""
    "These three operations - flip, rot and twist - allow all the possible ways of"
    "constructing a FIFO, stack, cicular-FIFO, dual-stack, and so on. In addition,"
    "the up and down operators leverage the doubly-linked list structure of the"
    "stack.")

    ("If you want to take the first n elements on the stack as a list, use the take"
    "operator:"
    ""
    "    'a' 'b' 'c' 2 take --> ('b' 'c')"
    ""
    "If you want to place all the individual elements of a list onto the stack, use"
    "the give operator:"
    ""
    "    clear (1 2 3) give sd !"
    ""
    "To get the number of items on the dstack, use the depth operator. To get the"
    "total number of items on both the dstack and ustack (called the 'diameter'),"
    "use the dia operator:"
    ""
    "    clear 1 2 3 4 5 <- <- sd !"
    "    depth ; zap dia ;")

    ("Babel's swap, zap, dup and other basic stack operators are inspired by the Joy"
    "programming language by the late Manfred von Thun which, in turn, drew much of"
    "its inspiration from the Forth language by Charles Moore. In these languages,"
    "there are a number of operators dedicated to re-ordering the stack. One"
    "criticism of such operators is that they are concessions to 'stack-noise' and"
    "that they force the programmer to think about details of the order of"
    "operations that can be calculated automatically by the compiler. Babel's only"
    "operator that is dedicated to re-ordering the stack is the swap operator. The"
    "advanced stack operators - up, down, rot, flip and twist - provide a different"
    "approach to managing the stack as a doubly-linked circular list ADT."
    ""
    "There are two primary uses of the stack within a standard language, such as C."
    "The first use is the passing of arguments and return-values between functions."
    "The other, hidden use, is the re-ordering of operations within statements into"
    "a sequential stream of operations. Since Babel is a stack-based language, both"
    "uses are always visible to the programmer. The stack operations we have explored"
    "up to this point have primarily to do with re-ordering operations into a "
    "sequential stream."
    ""
    "The following link has a great introduction to Forth:"
    "    http://skilldrick.github.io/easyforth/")

    -- array operators
    ("To find the size of an array (leaf or pointer), use the arlen operator:"
    ""
    "    [1 2 3] arlen ;"
    "    ['a' 'b' 'c'] arlen ;"
    ""
    "To join two arrays together (leaf or pointer), use the cat operator:"
    ""
    "    [1 2 3] [4 5 6] cat ;"
    ""
    "To compare whether two arrays (leaf or pointer) are the same, use the arcmp "
    "operator:"
    ""
    "    [1 2 3] [1 2 3] arcmp ;"
    ""
    "The arcmp operator returns 0 when two arrays are the same. Otherwise, it returns"
    "either 1 or -1. It is based on C's memcmp() function.")

    ("To get a slice of an array (leaf or pointer), use the slice operator:"
    ""
    "    [1 2 3 4 5 6] 1 4 slice ;"
    ""
    "In Babel, slicing and cutting operations count the spaces between values in an"
    "array. To get the first element:"
    ""
    "    [7 8 9] 0 1 slice ;"
    ""
    "If you give the same start and end index, slice will always return nil"
    "(nothing).")

    ("Babel can perform a fast move between arrays (leaf or pointer), based on C's"
    "memmove() function. Use the move operator to perform this operation:"
    ""
    "    <dest_arr> <dest_index> <src_array> <src_index> <size> move"
    ""
    "The move operator does not leave the destination array on the stack, so you may"
    "need to dup it if you want to operate on it later:"
    ""
    "    [0 1 2 3 4 5] dup 0 [6 7 8 9 10] 0 3 move ;"
    ""
    "Note that you may provide the same array as both destination and source, that"
    "is, you may perform an in-array move. The source and destination within the"
    "array may also be overlapping."
    ""
    "You can use the move8 operator to move bytes."
    ""
    '    "abcdef" dup 0 "ghijkl" 0 3 move8 <<')

    ("We met the easy-set < operator earlier in this tutorial. Babel provides a couple"
    "different ways to set individual pointers and values in arrays. The easy-set"
    "and easy-get operators work as follows:"
    ""
    "    foo 3 <     --> Puts 3 in the symbol foo"
    "    foo > 5 + ; --> Gets whatever is in foo and adds 5 to it")

    ("Most of the time, you do not need to use the easy-get operator, because the Babel"
    "interpreter automatically looks up any unrecognized tptr in the symbol-table."
    "The standard get and set operators use a pure-Babel-pointer (pbp) to perform a"
    "nested look-up in a bstruct. A pbp is a leaf array containing a series of"
    "indices by which the get and set operators can traverse the bstruct."
    ""
    "    [1 2 3 4] [3] get ; --> 4 is the 3th element of the array"
    "    [ptr [1 2] [3 4] [5 6]] [2 0] get ; --> 5 is the 0th element of the 2th element"
    "    [1 2 3] dup [2] 7 set ;")

    -- string/array8 operators
    ("In Babel, a string is stored in a leaf-array. You can see this by comparing a"
    "string with its hexadecimal encoding:"
    ""
    "    'a' ; [0x61 0xffffff00] ;"
    ""
    "You can concatenate two strings using the cat8 (also .+) operator:"
    ""
    "    'door' 'way' .+ <<")

    ("You can convert a number to a string format or vice-versa using Babel's dedicated"
    "conversion operators:"
    ""
    "    123 itod <<"
    "    123 itoh <<"
    "    123 itoo <<"
    "    123 itob <<"
    ""
    "You can convert a numeric string to its integer numeric value using the opposite"
    "conversion operators:"
    ""
    "    '123' dtoi 3 + ;"
    "    '7b' htoi 3 + ;"
    "    '7623' otoi 3 + ;"
    "    '01111011' btoi 3 + ;")

    ("You can convert to/from an arbitrary base using the itoa and atoi operators:"
    ""
    "    '12' 3 atoi ; --> results in 5 (since 12base3 = 5)"
    "    5 3 itoa << --> '12' since 12base3 = 5")

    ("Babel provides a restricted version of sprintf with the sprint operator:"
    ""
    "    <array8> <offset> <format_string> <format_argument> sprint"
    ""
    "For example,"
    ""
    '    "   " dup 0 "%03x" 157 sprint <<'
    ""
    "Like the move operator above, sprint does not leave the destination array8 on"
    "the stack, so you may need to dup it, as we have in the example above. Because"
    "the sprint operator only takes one format-argument, you should not use multiple"
    "format codes in the format-string - sprint's behavior is undefined in this case.")

-- cut8/slice8/paste8/move8

    -- list operators
    ("To access a specific element of a list, use the ith operator - as a"
    "mnemonic, think of the I in Ith as 'lIst TH'."
    ""
    "    (1 1 2 3 5 8 13) 4 ith itod <<"
    ""
    "Use the len operator to get the length of a list:"
    ""
    "    ('a' 'b' 'c' 'd') len ;"
    ""
    "Use the car/cdr operators as you would in Lisp:"
    ""
    "    (1 2 3) dup"
    "    car ; zap"
    "    cdr ;")

    ("Use the cons operator as you would in Lisp:"
    ""
    "    'a' ('b' 'c') cons {<<} ..."
    ""
    "Use the uncons operator to undo a cons:"
    ""
    "    clear ('a' 'b' 'c') ; uncons cons ;"
    ""
    "Babel provides four list operators that act like Perl's operators for"
    "manipulating the ends of a list - push, pop, shift and unshift:"
    ""
    "    (1 2 3) ; pop       --> (2 3) (1)"
    "              push ;    --> (1 2 3)"
    "    (1 2 3) ; shift     --> (1 2) (3)"
    "              unshift ; --> (1 2 3)"
    ""
    "As you can see, push and pop operate on the 'left-end' of a list. Shift and"
    "unshift operate on the 'right-end'.")

    ("Use the ls2lf operator to convert a list containing only leaf values to a leaf-"
    "array:"
    ""
    "    ([1 2 3] 4 5 [6]) ls2lf ;"
    ""
    "Use the bons operator to convert a list to a pointer-array:"
    ""
    "    ('foo' 'bar' 'baz') bons ;"
    ""
    "The name bons stands for Babel-cons, a play on Lisp's cons.  Use the ar2ls"
    "operator to convert an array (leaf or pointer) to a list."
    ""
    "    [1 2 3] ar2ls --> (1 2 3)"
    "    [ptr 1 2 3] ar2ls --> (1 2 3)"
    '    ["foo" "bar"] ar2ls --> ("foo" "bar")')

    ("Use the split operator to split a list in one or more places. Use the append"
    "operator to undo a split."
    ""
    "    (1 2 3 4 5 6) (1 3) split --> ((1) (2 3) (4 5 6))"
    "    append ; --> (1 2 3 4 5 6)"
    ""
    "Use the lsjoin utility from std.sp to easily join two lists together:"
    ""
    "    (1 2 3) (4 5 6) lsjoin !")

    ("Use the ins operator to insert one list into another, already existing list"
    "after the first element. This operator can be used to avoid unnecessary use of"
    "split and append when all you want to do is insert one list into another:"
    ""
    "    (1 2 3) (4 5) ins ; --> (1 4 5 2 3)"
    ""
    "Use the rev operator to reverse a list:"
    ""
    "    (1 2 3 4) rev ; --> (4 3 2 1)")

    -- bstruct operators
    ("Because the bstruct is the uniform data-structure in which every aspect of a"
    "Babel program is stored, Babel provides a suite of special-purpose operators"
    "for managing all aspects of a bstruct. We will cover most of them in the next"
    "few pages of the tutorial."
    ""
    "To create a new, empty value-array, use the newval operator:"
    ""
    "    [0 0 0] ;"
    "    3 newval ;"
    ""
    "To create a new, empty pointer-array, use the newptr operator:"
    ""
    "    [ptr nil nil nil] ;"
    "    3 newptr ;"
    ""
    "Use the newtag operator, which we discussed earlier, to create a new tag.")

    ("You can directly inspect the s-field of a bstruct by using the sfield operator."
    "This field is how Babel decides whether a particular bstruct is a value-array,"
    "pointer-array or tag."
    ""
    "    [7 8 9] sfield ; "
    '    ["foo" "bar"] sfield ;'
    '    [tag "foo" nil] sfield ;')

    ("For large and complex data-structures, you may need to know how many arrays of"
    "which kind are present in the data-structure. You can use the following operators"
    "to measure every aspect of a bstruct:"
    ""
    "    my_list ([1 2 3] [4 5 6]) <"
    "    my_list nva ;  --> number-of-values = 6"
    "    my_list nlf ;  --> number-of-leaf-arrays = 2"
    "    my_list npt ;  --> number-of-pointers = 4"
    "    my_list nin ;  --> number-of-interior-arrays = 2"
    "    my_list ntag ; --> number-of-tags = 1 (that is, nil)"
    "    my_list mu itod << --> memory-usage is 84 bytes"
    ""
    "You can use the bs2gv operator discussed earlier in the tutorial to visualize"
    "and verify these metrics.")

    ("You can check what kind of bstruct is on the stack at any time by using the"
    "following operators:"
    ""
    "    [1 2 3] val? ;"
    "    [1 2 3] ptr? ;"
    "    ['foo'] ptr? ;"
    "    [tag 'foo' nil] tag? ;"
    "    [1 2 3] tag? ;"
    ""
    "You can check if the top-of-stack is nil:"
    ""
    "    nil nil? ;")

    ("The arcmp operator can only check the equality of two leaf arrays or that two"
    "pointer-arrays contain exactly the same references (pointers). To check the"
    "equality of two general bstructs use the cmp operator."
    ""
    "    (1 (2 3) 'foo') (1 (2 3) 'foo') cmp ;"
    "    (1 (2 3) 'foo') (1 (2 3) 'zoo') cmp ;"
    ""
    "Equality of bstructs is defined as value-equality and topological homomorphism"
    "of the entire bstruct. Basically, if bs2gv generates the same image output for"
    "two bstructs, cmp will return true, otherwise, it will return false.")

    ("Babel supports a variety of mechanisms for C-style function organization. The"
    "nest operator provides a 'nested' stack mechanism. Note that the nest operator"
    "does *not* create a local context (a local namespace). The operator does the "
    "following:"
    ""
    "    - Remove the top item from the current stack (TOS)"
    "    - Save the current stack"
    "    - Create a new stack and place TOS on it; this is now the current stack"
    "    - When the nested statement has finished, remove the top item from the"
    "        current stack (TOS)"
    "    - Restore the old stack and place TOS on it")

    ("Let's look at a specific example to illustrate the nest operator:"
    ""
    "    clear 0x123 0x456 {0x789 0xabc} nest sd !"
    ""
    "If you stare at the result for a while, you should eventually see what the "
    "operator did. This operator is useful for naming chunks of code that are not"
    "large enough to require their own local namespace, but too large or complicated"
    "to operate on the current stack. To illustrate a practical use of nest, let's"
    "use the matdiag utility from std.sp. This utility expects a matrix"
    "(list-of-lists) and returns a list containing the diagonal of the matrix."
    "You can open std.sp in a text-editor and examine the code for this utility."
    ""
    "    ((1 2 3) (4 5 6) (7 8 9)) matdiag ! ;")

    ("This tutorial is focused on the Babel command-line. However, we will quickly"
    "explore some tools for executing Babel .sp files (sp is short for 'S-expression"
    "Parse'). You can run any .sp file by simply providing it as an argument to the"
    "Babel executable. The program will run without the Babel CLI, so it will not be"
    "interactive like this tutorial."
    ""
    "It is also possible to execute a Babel .sp file from the Babel CLI. To make"
    "this easier, there is a suite of tools provided in the special file std.sp."
    ""
    "The include command allows you to pull in a .sp file and load it. If it is"
    "code, you can then eval it. If it is data, you can operate on it. This is the"
    "command you used to pull in the 'tut.sp' file and begin executing the tutorial.")

    ("Let's try writing our own .sp file:"
    ""
    '    /'{ ("three" "billy" "goats" "gruff") {<< " " <<} ... }/' "goat.sp" <<<'
    ""
    "The program we typed at the CLI has now been written to the file goat.sp. Now,"
    "let's pull the file back in, include it and eval it:"
    ""
    '    "goat.sp" include ! !')

    ("Babel has built-in support for maps (aka hash-tables, directories, or"
    "associative-arrays). To create a map, first make a list containing the key-value"
    "pairs, then invoke the ls2map command from the std.sp file:"
    ""
    "    birth (('Washington' 1732) ('Lincoln' 1809) ('Roosevelt' 1882)) ls2map ! <"
    ""
    "Now, we can look up a value in the map using its key:"
    ""
    "    birth 'Lincoln' lumap ! itod <<")

    ("We can perform other kinds of operations on maps. Given the map in the previous "
    "page, we can insert, test existence or remove an individual key."
    ""
    "    clear"
    "    birth 'Kennedy' 1917 insmap !"
    "    birth 'Kennedy' lumap ! itod <<"
    "    birth 'Jefferson' exmap ! ;"
    "    birth 'Washington' exmap ! ;"
    "    birth 'Washington' rmmap ! ;"
    "    birth 'Washington' exmap ! ;")

    ("Using the map from the previous pages, we can also get a list of all key"
    "strings, all key tags or all values from the entire map, or get back the"
    "original list we used to construct the map."
    ""
    "    clear"
    '    birth keymap ! {<< " " <<} ...'
    '    birth tagmap ! {;} ...'
    '    birth valmap ! {itod << " " <<} ...'
    '    birth map2ls !'
    '    { dup <- 0 ith << -> " " << 1 ith itod << "\n" << } ...')

    ("Once we've loaded a Babel object, it is 'live' in memory, it cannot be directly"
    "written out to file. In order to write a live Babel object out to file, we must"
    "first unload it. The unload operator can be used for this purpose but we will"
    "defer that topic until later in the tutorial. For now, we will use the savefs"
    "command from the std.sp file, which does all the dirty work for us. Using the "
    "map from the previous pages, let's try saving an object out to file and then "
    "bringing it back."
    ""
    "    birth 'birth.bbl' savefs !"
    "    'birth.bbl' loadfs ! 'Roosevelt' lumap ! itod <<")

    ("In Babel, many operations only act on references (pointers), that is, they do"
    "not automatically create a separate copy of the data being referred to, even if"
    "it is in a code-stream. Use the cp operator to avoid program errors caused by"
    "this aspect of Babel's operation."
    ""
    "Any bstruct can be copied with the cp operator. To illustrate, we will use a"
    "dev operation to make a copy of the symbol-table, which is large because this"
    "entire tutorial is stored in it."
    ""
    "    7 dev --> This loads the symbol-table on the stack"
    "    dup mu itod << --> This calculates the memory-usage of the symbol-table"
    "    cp"
    ""
    "We now have a complete copy of the symbol-table on the stack. If we make changes"
    "to this bstruct, it will not affect the live symbol-table because it is a deep-"
    "copy.")

    ("As a more mundane example of reference-copy versus deep-copy, let's try the set"
    "and get operators on a leaf-array that has been dup'd versus one that has been"
    "copied:"
    ""
    "    clear [1 2 3] dup dup [1] 7 set sd !"
    ""
    "Notice that both copies of the array have 7 in the 1st position - the stack just"
    "contains two references to the same array."
    ""
    "    clear [1 2 3] dup cp dup [1] 7 set sd !"
    ""
    "In this instance, the top array contains the 7, but the original array remains"
    "unchanged.")

    -- bvm
    ("Babel's operators - even looping and control-flow constructs - are designed in"
    "a way that avoids nested C-function calls within the interpreter. The reason for"
    "this is that the Babel Virtual Machine will support rigorous resource limits"
    "to restrict which resources a Babel program can use and how much of those"
    "resources it can use. In order to regulate memory usage of the interpreter's"
    "call-stack, the interpreter would have to track how deeply a Babel program has"
    "recursed at any point during execution. Instead, Babel is designed without "
    "C-level recursion, with the exception of the babel operator itself. The babel"
    "operator starts running a fresh instance of the interpreter on a Babel Virtual"
    "Machine.")

    ("Building a BVM from scratch involves some low-level details that we will skip"
    "in this tutorial. Instead, std.sp provides some convenience functions for managing"
    "and debugging BVMs so you can run your own instance of the interpreter with the"
    "babel operator:"
    ""
    '    {"Hello, world\n" <<} mkbvm ! babel'
    ""
    "The mkbvm utility takes a code-list and embeds it in a fresh copy of a BVM. We"
    "then invoke the babel operator on this BVM to run it. When the babel operator"
    "is finished running its BVM, it will leave a return value on the stack (nil for"
    "now).")

    ("If you'd like to step through your program one step at a time, try using the"
    "step utility:"
    ""
    "    my_bvm {'a' << 'b' << 'c' <<} mkbvm ! <"
    "    my_bvm stepbvm !"
    "    my_bvm stepbvm !"
    ""
    "And so on. If you want to step through a program more than one step at a time"
    "use the stepnbvm utility:"
    ""
    "   my_bvm 2 stepnbvm !"
    ""
    "Once you have run to the end of the code-list, further invocations of the"
    "babel operator will have no effect. If you want to run through a BVM more"
    "than once, make a copy of it before using the step* utilities.")

-- PAGE REMOVED, BROKEN; PROBABLY JUST NEED TO REBUILD cli.bbl XXX
--    ("As another example of launching a new BVM within the current BVM, try loading"
--    "and running the cli.bbl file, which contains another copy of the CLI. Note that"
--    "std.sp is not active in the new CLI until you enter 'rc !' and none of the symbols"
--    "that you defined in this BVM will be present in the new BVM. Enter 'exit' when"
--    "you are finished in the nested CLI to get back to this CLI:"
--    ""
--    '    "cli.bbl" loadfs ! babel'
--    ""
--    "Alternatively, you can use the cli utility in std.sp to do the same thing:"
--    ""
--    "    cli !")

    ("Babel is an untyped language, but it will have the capability to infer and"
    "enforce typing. Types are not built into Babel because the traditional conception"
    "of what a type is and what types are useful for is overly restrictive. If we"
    "were to think of arguments passed between functions as if they were physical"
    "widgets, typing would be like making the parameter lists to functions with "
    "shaped slots that only allow a certain type of widget to pass through. The "
    "compiler's type-inferencing boils down to making sure that the wrong kind of"
    "widget can never attempt to pass through the wrong kind of slot. While this use"
    "of typing is a powerful tool for preventing a large class of program bugs, it"
    "is not a general solution to the problem of bugs, nor does it encapsulate the"
    "general concept of types and typing.")

    ("Babel's view of typing is more like the way we think of file format "
    "specifications - a bitmap is not just any file with '.bmp' as a filename suffix,"
    "it is all and only those files that conform to the bitmap file format "
    "specification. Babel's support for this kind of type checking and inference is"
    "not yet coded, but it will be built on the tag architecture we have explored"
    "above. Let's look at an example to illustrate the basic idea:"
    ""
    "    (1 2 3) unload dup hashm newtag ;"
    ""
    "This code flattens the list to a value-array using the unload operator. Then it"
    "takes the hash of the unloaded array to generate a signature. Then, it creates"
    "a tptr that points at the flattened array. This can be thought of as a type-"
    "definition for the type '[list 1 2 3]' - a list containing the numbers 1, 2 and"
    "3 as elements. In this way, you can see how we can define types even for "
    "constants, or for objects as large and complex as we like. In particular, we"
    "want to be able to define a type for type-checking code - this is how general"
    "type-inference and enforcement will be integrated into Babel.")

    ("Babel is still in development. There are many implemented features that we have"
    "not covered in this tutorial for the sake of brevity. There are many other"
    "features that are on their way but have not yet been developed into the Babel"
    "interpreter. Some of the most important features that are on their way:"
    ""
    "    - user-defined lists (macros)"
    "    - standard control-flow operators"
    "    - local contexts (for C-style function organization)"
    "    - alternative control-flow for constraint-solving"
    "    - constant-folding, code-list pre-processing"
    "    - block-mode syntax"
    "    - built-in crypto/compression operators"
    "    - built-in arbitrary-precision arithmetic"
    "    - memory/speed performance enhancements")

    ("If you'd like to dig around Babel's guts and play around with dev operations,"
    "use the 0 dev command to get a listing of the dev operations. Many of these"
    "operations can make the interpreter unstable, so use them with care and be "
    "prepared for unexpected failures."
    ""
    "    0 dev")

    ("End of Babel Command-Line Tutorial"))
    <

ok
    { "\n" <<
      tut_head !
      tut_pages page 1 - ith speak !
      "\n" <<
      page dup 1 + < }
    < 

ok !}

