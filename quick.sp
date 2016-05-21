{ page 1 <

tut_head
    { " ---------------------- Babel Quick Start -------------------[ Page "
      <<
      "  " dup 0 cp "%d" page sprint
      <<
      " ]---- \n\n" 
      << } <

tut_pages

    ((("Introduction")
    ("Welcome to the Babel quick-start guide. This guide is intended to help you get"
    "a feel for the Babel language in 30 minutes or less. It assumes you have a"
    "basic understanding of at least one standard programming language, such as"
    "Python or Java. This guide does not cover advanced Babel topics, so if you want"
    "to explore those topics and discover the power of programming in Babel, enter"
    "'help !' (no quotes) and follow the directions to start the advanced tutorial."
    ""
    "At each step, this guide will explain one or more facts about Babel, then turn"
    "the command-line over to you to explore as you like.  When you are ready to"
    "move on to the next step of the tutorial, enter 'ok !' without the quotes.")
    nil)


    (("Introduction cont'd")
    ("Babel is still in development, so if you mistype something during the"
    "tutorial, it may cause the interpreter to crash. Simply restart the"
    "interpreter and restart the tutorial. You can return to the page you were"
    "at by entering the command:"
    ""
    "    page n < ok !"
    ""
    "where n is the page you want to go to. You can exit at any time by entering"
    "'exit'. Enter 'ok !' when you're ready.")
    nil)


    (("Introduction cont'd")
    ("On some pages of the guide, there will be one or more examples listed. You can"
    "try these examples without typing them (or copy/pasting) by giving the example"
    "number and using the tryit command. Enter '1 tryit !' (without quotes) to try"
    "the code example listed below:")
    ('    "Hello, world\n" <<'))


    (("Introduction cont'd")
    ("You may have questions about specific operators or utilities while working"
    "through the quick-start guide. In order to answer those questions for yourself,"
    "we will load the Babel manual so you can use the man utility whenever you like."
    "Run the example code below by entering '1 tryit !' (no quotes) to load the"
    "Babel manual. Test the manual using '2 tryit !' (no quotes).")
    ('    "man.sp" include ! !'
    '    "<<" man !'))


    (("Babel")
    ("Babel is a stack-based or postfix-order language. This changes the order of"
    "operations in arithmetic expressions. For example, to add two numbers in Babel,"
    "you write 2 2 +, not 2 + 2. It is not difficult to use postfix order but if you "
    "are not familiar with stack-based languages and how to write in postfix-order,"
    "you should take a few minutes to review a tutorial on the Forth language (Forth"
    "is one of the inspirations for Babel) at the following link before continuing "
    "this quick-guide:"
    ""
    "    http://skilldrick.github.io/easyforth")
    nil)


    (("Strings")
    ("There are two ways to enter strings in Babel - single-quote or double-quote. "
    "Single-quote and double-quote strings are identical, except that the escape-"
    "character is a backslash for double-quote strings and a slash for single-quote "
    "strings. Try the following examples:")
    ('    "hello\n" <<'
    "    'hello/n' <<"))


    (("Strings cont'd")
    ("Here are some more string examples to try:")
    ("    '\"' <<"
    '    "/'" <<'
    '    "\"" <<'
    "    '/'' <<"))


    (("Numbers")
    ("You can enter numbers in decimal or hexadecimal format. Like C, Babel requires"
    "you to explicitly convert an internal number to a string before printing it."
    "You can use the ito* operators to do this:")
    ('    255 itod << "\n" <<'
    '    0xff itod << "\n" <<'
    '    255 itoh << "\n" <<'
    '    255 itob << "\n" <<'))


    (("Comments")
    ("In interactive mode, you will not need to use comments but comments are available"
    "for your use, whether in interactive mode or in file mode. A line comment begins"
    "with a double-dash and makes whatever follows of no effect to the end of the"
    "line.")
    ('    "Hello\n" << -- "World\n" <<'))


    (("Operators")
    ("Up to this point, we have been showing the rules for *operands*. Strings and"
    "numbers are two kinds of operands in Babel. The other tokens we have been using"
    "are *operators*. Everything in Babel is either an operand or an operator. The"
    "print operator is the double left-chevron, <<. The itod operator is the 'integer-"
    "to-decimal' operator. The itoh operator converts to hex, itob to binary. Let"
    "us meet a few arithmetic operators:")
    ('    2 2 + itod << "\n" <<'
    '    9 9 * itod << "\n" <<'
    '    12 6 - itod << "\n" <<'
    '    120 10 // itod << "\n" <<'))


    (("Semicolon")
    ("One special operator is the semicolon operator. This operator displays a view"
    "of the current operand that is close to Babel's internal view. Because the"
    "semicolon is an operator (not a syntactical token), you should always put"
    "a space before and after it.")
    ('    9 ;'
    '    0x12345678 ;'
    '    "Hello" ;'))

    (("Value Arrays")
    ("You can create an array of numbers - or value-array - by using the square-brackets."
    "A value-array can only contain numbers. You cannot nest value-arrays.")
    ('    [1 2 3 4] ;'))


    (("Pointer Arrays")
    ("You can create an array of pointers - or pointer-array - by using the"
    "square-brackets with the ptr special-form. A pointer-array *cannot* contain"
    "values. Thus, an array like [ptr 1 2 3] will actually create nested value"
    "arrays (see below). You can nest any array inside of a pointer-array, including"
    "other pointer-arrays.")
    ('    [ptr 1 2 3] ;'
    '    [ptr [1 2] [3 4]] ;'
    '    [ptr [1 2] [3 4] [ptr [5]]] ;'))


    (("Symbols")
    ("You can store any operand in a symbol by giving a symbol name and using the < "
    "operator. The following examples illustrate how this works.")
    ('    foo "Hello\n" <'
    '    foo <<'
    '    bar 8 <'
    '    bar 1 + itod << "\n" <<'
    '    baz [ptr bar 2 3] <'
    '    baz ;'))


    (("Symbols cont'd")
    ("It is not convenient to always have to type the same thing repeatedly, as we"
    "have with the itod and print operation. Instead, we can create some code and"
    "give it a symbolic name for later use. This should not be confused with"
    "function definition, because the code we define in this way does not have its"
    "own local context. It is equivalent to defining a word in Forth. Let's define"
    "some code and give it the symbolic name 'np' (for 'numeric print') to save"
    "keystrokes when displaying numbers. You must execute example (1) before the"
    "others.")
    ('    np {itod << "\n" <<} <'
    '    2 2 + np eval'
    '    9 9 * np eval'
    '    12 6 - np eval'
    '    120 10 // np eval'))


    (("Standard Utilities")
    ("The file std.sp is loaded whenever you invoke Babel in interactive mode. This"
    "file contains standard utilities that are useful in many contexts. Because these"
    "utilities are not operators, they must be evaluated, that is, you must use the"
    "bang operator, !, to invoke them. Let's meet a useful std utility, say:")
    ('    42 itod say !'
    '    "Hello, world" say !'))


    (("Eval")
    ("Earlier, we used the eval operator to evaluate the np word we had defined"
    "symbolically. The bang symbol, !, is a synonym for the eval operator.  Eval"
    "does not operate only on symbols, you can directly quote code and eval it:")
    ('    {"Hello, again" say !} eval'
    '    {2 2 +} ! itod say !'))


    (("Code-lists")
    ("In the last two pages, we encountered the curly-braces. The curly-braces allow"
    "you to define a *code-list*. You can nest code-lists, that is, you can put"
    "anything you like into a code-list. A code-list is an *operand*. By itself, it"
    "does nothing but you can use the eval operator to evaluate it. Evaluation is"
    "equivalent to having directly entered the code at the prompt. Unlike arrays or"
    "lists (next page), only code-lists can contain operators.")
    ('    "I/'m sorry, Dave, I/'m afraid I can/'t do that." say !'
    '    { "I/'m sorry, Dave, I/'m afraid I can/'t do that." say ! } eval'))


    (("Lists")
    ("You should rarely need to explicitly construct pointer-arrays in your own code."
    "Pointer-arrays and value-arrays are how data is represented internally in Babel."
    "If you need to manipulate a collection of things, you will usually want to use"
    "a list. Lists in Babel work just like they do in Lisp - they are very flexible."
    "Create a list by using ordinary parentheses:")
    ('    (1 2 3) ;'
    '    ("foo" "bar") ;'))


    (("Lists cont'd")
    ("The internal view of lists is confusing. The following words are defined in"
    "std.sp and are always available in interactive-mode. Use them to display lists"
    "in a more easily readable format:")
    ('    (1 2 3) lsnum !'
    '    ("foo" "bar") lsstr !'
    '    ([ptr 1 2] [ptr 3 4]) lsshow !'))


    (("Lists cont'd")
    ("To access a specific element of a list, use the ith operator. Use the len operator"
    "to get the length of a list.")
    ("    (1 1 2 3 5 8 13) 4 ith itod say !"
    "    ('a' 'b' 'c' 'd') len ;"))


    (("Lists cont'd")
    ("In Babel, as in Lisp, you can use car and cdr to access the first element of"
    "a list or the rest of the list, respectively. To join two lists as a cons-cell,"
    "use the cons operator. You can use the uncons operator to separate lists back.")
    ("    (1 2 3) car ;"
    "    (1 2 3) cdr lsnum !"
    "    'a' ('b' 'c') cons lsstr !"
    "    ('a' 'b' 'c') uncons lsstr ! say !"))


    (("Lists cont'd")
    ("Frequently, you want to operate on the end of a list. To put things onto a list"
    "or take them off at one end or the other, Babel provides four list operators"
    "that act like Perl's operators for manipulating the ends of a list - push, pop,"
    "shift and unshift.")
    ("    (1 2 3) pop lsnum ! lsnum !"
    "    (2 3) (1) push lsnum !"
    "    (1 2 3) shift lsnum ! lsnum !"
    "    (1 2) (3) unshift lsnum !"))


    (("Lists cont'd")
    ("In order to join two lists end-to-end, use the compose utility."
    ""
    "There are many other advanced topics related to lists. To learn more about"
    "lists in Babel, go through the advanced tutorial (tut.sp) or load the manual"
    "(man.sp) and read about the list operators and utilities. Enter 'help !' (no"
    "quotes) for more information on loading the tutorial or the manual.")
    ("    (1 2 3) (4 5 6) compose ! lsnum !"))


    (("Maps")
    ("In Babel, the associative-array ADT is referred to as a map. Other languages"
    "call them hashes, dictionaries, multi-maps, and so on. To construct a map from"
    "a list, use the ls2map utility. We will assign the map to the symbol foo. The"
    "examples on the following pages will only work if you evaluate the following"
    "example.")
    ('    foo (("foo" 17) ("bar" 42)) ls2map ! < "done" say !'))


    (("Maps cont'd")
    ("To look-up an entry in a map by its key, use the lumap utility. To insert into"
    "a map, use the insmap utility.")
    ('    foo "bar" lumap ! itod say !'
    '    foo "bop" 73 insmap !'
    '    foo "bop" lumap ! itod say !'))


    (("Maps cont'd")
    ("To get a list of keys from a map, use the keymap utility. To get a list of"
    "values, use the valmap utility. To convert the entire map back to list form,"
    "use the map2ls utility."
    ""
    "There are many other advanced topics related to maps. To learn more about"
    "maps in Babel, go through the advanced tutorial (tut.sp) or load the manual"
    "(man.sp) and read about the map operators and utilities. Enter 'help !' (no"
    "quotes) for more information on loading the tutorial or the manual.")
    ('    foo keymap ! lsstr !'
    '    foo valmap ! lsnum !'
    '    foo map2ls ! {give swap << " " << itod say !} each'))


    (("The Stack")
    ("Because Babel is a stack-based language, the central structure that unifies"
    "Babel operations is the stack. The semicolon operator is actually viewing the"
    "top-of-stack or TOS. You can view the whole stack at any time by using the"
    "sd utility.")
    ('    1 2 3 sd !'
    '    clear'
    '    sd !'))


    (("The Stack cont'd")
    ("The Babel stack actually consists of two stacks joined together in a circular"
    "data-structure. The main stack is called the down-stack or dstack. The rest of"
    "the stack is the up-stack or ustack. The sd utility displays the dstack as the"
    "list of items below TOS and the ustack as the list of items below BOS. To put"
    "things on the ustack, use the down, or <-, operator. To place items back onto"
    "the dstack from the ustack, use the up, or ->, operator.")
    ('    1 2 3 sd !'
    '    clear 1 2 3 down sd !'
    '    up sd !'))


    (("The Stack cont'd")
    ("Use the dup operator to duplicate the top item on the stack. The duplication is"
    "by reference only, so changes to one affect both. Use the swap operator to"
    "reorder the top two items on the stack. Use the zap operator to remove the top"
    "item from the stack. Note that down and up can create the same effect as swap.")
    ('    clear 1 2 3 dup sd !'
    '    clear 1 2 3 swap sd !'
    '    clear 1 2 3 zap sd !'
    '    clear 1 2 <- 3 -> sd !'))


    (("The Stack cont'd")
    ("To put the items in a list onto the stack, in order, use the give operator. To"
    "take a number of items off the stack and put them into a list, use the take"
    "operator. To gather the entire stack back into a list, use the collect utility.")
    ('    clear (1 2 3 4 5) give sd !'
    '    2 take lsnum !'
    '    collect ! lsnum !'))


    (("The Stack cont'd")
    ("There are many other advanced topics related to the stack. To learn more about"
    "the stack in Babel, go through the advanced tutorial (tut.sp) or load the manual"
    "(man.sp) and read about the stack operators and utilities. Enter 'help !' (no"
    "quotes) for more information on loading the tutorial or the manual.")
    nil)


    (("Input")
    ("To get user input, use the >> (read from STDIN) operator.")
    ('  "What is your name? " << >> "Hello, " << << ", it/'s nice to meet you." say !'))


    (("Each")
    ("To iterate over the elements of a list, use the each operator. The each operator"
    "places each element of the list on the stack, one by one, and invokes the provided"
    "code-list on each element.")
    ('    ("Washington" "Lincoln" "Kennedy") {say !} each'
    '    (1 2 3) { itod "a" .+ say !} each'
    '    ({"Hello, " <<} {"world" say !}) {eval} each'))


    (("Times")
    ("The times operator allows you to repeat a code-list multiple times. The iter"
    "operator, when used inside of a loop, returns the iteration-number of the loop."
    "In the case of the times operator, it is a count-down value. In the case of"
    "each, it is a count-up value.")
    ('    {"Ho " <<} 3 times "\n" <<'
    '    {iter itod << " " <<} 10 times "\n" <<'))


    (("Select and If/While/For")
    ("Babel does not currently have support for ordinary if statements, while loops"
    "or for loops. This support will be added after macros have been implemented"
    "in the language. The sel and selr operators are the 'select' and 'select (reverse)'"
    "operators - these operators are how Babel performs conditional operations under"
    "the hood. Here are some examples showing how to use these operators.")
    ('    "a" "b" 0 sel say !'
    '    "a" "b" 1 sel say !'
    '    0 "a" "b" selr say !'
    '    1 "a" "b" selr say !'
    '    0 0 cmp {"The value is not zero." <<} {"The value is zero." <<} selr ! "\n" <<'))


    (("Useful Utilities")
    ("The std.sp file contains many useful utilities. You can open the file and read"
    "about the utilities and try them out, or you can read the manual. Here is a"
    "    '    'gselection of a few of some of the available utilities to illustrate the power"
    "of Babel's built-in operators and standard utilities.")
    ('    ("foo" "bar" "baz") prs ! "\n" <<'
    '    ("foo" "bar" "baz") prn !'
    '    (1 2 3) {itod} over ! prs ! "\n" <<'
    '    ("Fourscore" "and" "seven" "years" "ago\n") wordcat ! <<'
    '    (1 2 3) (4 5 6) {+} lswise ! lsnum !'))

    (("Useful Utilities cont'd")
    ("More examples of useful utilities from std.sp:")
    ('    10 lsrange ! lsnum !'
    '    (1 2 3 4 5) {+} fold ! itod say !'
    '    (1 1 1 1) {+} sigma ! lsnum !'
    '    (1 2 3 4) {-} delta ! lsnum !'
    '    (1 0 2 0 3) {0 eq? not} filter ! lsnum !'
    '    "a" ("b" "c" "d" "\n") {.+} reduce ! <<'
    '    (1 2 1 3 1 4 5 1 4 2 2 3 1) uniq ! lsnum !'))

    (("Useful Utilities cont'd")
    ("More examples of useful utilities from std.sp:")
    ('    ( 8 5 1 6 7 4 10 9 3 2 ) {lt?} lssort ! lsnum !'
    '    ( 8 5 1 6 7 4 10 9 3 2 ) {gt?} lssort ! lsnum !'
    '    ( "576" "7230" "5495" "6609" "2052" "778" "5224" "6309" "658" "3361" ) strsort ! lsstr !'
    '    ( "576" "7230" "5495" "6609" "2052" "778" "5224" "6309" "658" "3361" ) lexsort ! lsstr !'
    '    "Twas brillig, and the slithy toves did gyre and gimble in the wabe" " " strsplit ! lsstr !'
    '    ("To " "da " "choppa!") strjoin ! say !'))

    (("Further Directions")
    ("The Babel Quick Start Guide is intended to help you get a feel for Babel within"
    "30 minutes or less. There are a lot of topics that this guide does not address."
    "If you are interested in diving deeper into Babel, enter 'help !' (no quotes) at"
    "the prompt and follow the directions to load the advanced tutorial or the Babel"
    "manual in order to explore further. If you have specific questions or a bug report,"
    "you can email me at claytonkb at gmail dot com.")
    nil))
    <


tut_show_title
    { ">>> " << tut_pages page 1 - ith 0 ith speak ! "\n" <<} 
    <

tut_show_body 
    { tut_pages page 1 - ith 1 ith speak ! "\n" <<} 
    <

tut_show_examples
    { tut_pages page 1 - ith 2 ith nspeak ! "\n" <<} 
    <

--debug_examples
--    { tut_pages page 1 - ith 2 ith show << "\n" << } 
--    <

nspeak
    { { "(" << iter 1 + itod << ")" << say ! } ... }
    <

tryit
    { 1 - <- tut_pages page 2 - ith 2 ith -> ith <- "{" -> "}" .+ .+ sexpr bpdli eval } 
    <

ok
    { "\n" <<
    tut_head !
    tut_show_title !
    tut_show_body !
    tut_show_examples !
    page dup 1 + < }
    < 

ok !}

