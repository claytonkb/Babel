<a name="getting_started"></a>
Getting Started
===============

Babel is written in C and will compile on any C99-compliant compiler. During development, I am using the following setup:

- <a href="http://www.cygwin.com/install.html" target="_blank">Cygwin</a>
- <a href="http://www.activestate.com/activeperl/downloads" target="_blank">ActivePerl</a>
- Babel repo cloned from <a href="https://github.com/claytonkb/Babel" target="_blank">github</a>

To make Babel using the scripts in the repo, I run the following command from the prompt:

> `% perl make.pl`

To run Babel code in a file named foo.sp (from the make directory), I run the 
following command from the prompt:

> `% bin/babel.exe foo.sp`

To begin writing your own Babel program, see the section below on <a href="#bipedal">Bipedal</a>. The legacy Sparse syntax is going to be phased out as soon as Bipedal support is fully implemented.

<a name="overview"></a>
Overview
========

Babel is an interpreted language.  It is an untyped, stack-based, postfix 
language with support for arrays, lists and hashes (dictionaries). Babel 
1.0 will support built-in crypto-based verification of code in order to 
enable safer remote code execution.

There are two kinds of Babel syntax. The first kind is being used to 
bootstrap the Babel construct, it is called Sparse (S-expression parser).
The second kind is the built-in Babel parser, which is called Bipedal
(Babel Program Description Language), and support for this is currently being 
added.

<a name="bipedal"></a>
Babel Program Description Language (Bipedal)
--------------------------------------------

There is partial support for Bipedal in Babel, as described below.

*Lexicals*

Bipedal is UTF-8 encoded - this means you can give your sections any name 
that can be encoded in UTF-8. You can easily alias the built-in operator 
names, as well.

A **number** can be a decimal integer, decimal floating-point, hexadecimal, 
or binary.

> `-42`  
> `32348`  
> `0x13`  

The limits are dependent on MWORD\_SIZE (use the msize operator to get 
this) except for pnums which can be of arbitrary size.

A **string** is a set of characters wrapped in quotes:

> `"Lorem ipsum dolor sit amet"  `  
> `'Lorem ipsum dolor sit amet'  `  

... or in a quote-block (see below). The quote character can be escaped 
with a backslash in the usual way. 

Line **comments** in Bipedal are specified by double-dash:

> `-- This is a comment  `  

An **identifier** consists of any space-delimited raw string which is not a
number, string or comment, as defined above. For example, in Bipedal, you 
could have an identifier such as: foo348&$@9&

*Sections*

Bipedal is organized at the top-level as a list of labeled sections. The top-
level section is the section named main. You may define as many sections as
you like, but only those sections that are referred to directly or indirectly
by main will be compiled into the resulting babel code.

> `((main  (foo))`  
> `(foo   (bar))`  
> `(bar   (foo)))`  

*Leaf-arrays*

A leaf-array is defined by enclosing a string or one or more numbers in 
square-brackets with the 'val' tag:

> `[val 1 1 2 3 5 8 13 0x15]`  
> `[val 101]`  
> `[val "Let me not to the marriage of true minds admit impediments"]`  

*Interior-array*

An interior-array is defined by enclosing one or more leaf-arrays, labels 
or interior arrays in square-brackets with the 'ptr' tag:

> `[ptr [val "achilles"] [val "tortoise"] ] `  

*Lists*

Lists are created with parentheses. A list in Babel means a linked-list 
in the Lisp-sense. The following are equivalent:

> `(1 2 3)  `  
> `[ptr 1 [ptr 2 [ptr 3 nil]]]  `  

You can verify this with the following code:

> `{ (1 2 3) [ptr 1 [ptr 2 [ptr 3 nil]]] == }  `  

... which will evaluate to 0 (which means they are the same, see `arcmp`).

The nil element is distinct from that of Lisp in that Babel has no notion of 
an atomic and nil is just implemented as an entry in the symbol table - 
but you can take its car or cdr and you will always get nil again.

One final difference between Bipedal syntax and Lisp syntax is that 
Bipedal does not use the dot-notation. In Lisp, the following syntax

> `(1 2 . 3)`  

... expresses the same list as the following written in Bipedal:

> `(1 [ptr 2 3])`  

A code-list is created with curly-braces.

> `{ 2 3 + }  `  
> `{ "Hello, "  "world\n" . << }  `  

*Tagged-pointers*

The tagged-pointer is the third basic sub-type alongside interior-arrays 
and leaf-arrays that together comprise the bstruct type in which all 
Babel data is stored. A tagged-pointer is created with the following
syntax:

> `[tag "foo" bar]`  

This creates a tagged pointer named "foo" which points at bar.

See the .sp files in the demo/ directory and in the sp/smoke directory for 
examples of working Bipedal files.

<a name="sparse"></a>
S-expression parser (sparse.pl)
-------------------------------

The s-expression parser (sparse.pl) is designed as an ultra-lightweight 
front-end for generating native Babel code. It uses S-expressions to encode
the various Babel data structures. To compile a set of .sp files into a
single .bbl file, pass the filenames as arguments to sparse.pl. Evaluation
of the compiled code begins at root:

> `(root *your Babel code here*)  `  

A .sp file is a list-of-lists (each list defining a "section"), so you must
place the root list inside another in order for it to be parseable:

> `((root *code*))  `  

Perhaps the simplest program you might compile:

> `((root (val 10)))  `  

To see a listing of the binary file that was created, open filename.sp.lst.
For example, the above code would result in the following lines in the
.lst file:

> `0000 00000004`  
> `0004 0000000a`   

iThe first line is at offset 0 and has a value of 4, meaning a leaf-array of
4 bytes in length. The second line is at offset 4 and has the value of 0xa,
which is the hexadecimal equivalent of the value 10 in the example .sp 
program.

The kinds of lists which sparse recognizes are:

> `(val  ...)  Creates a leaf-array (values)  `  
> `(ptr  ...)  Creates an interior-array (pointers)  `  
> `(tag  ...)  Creates a tagged-pointer  `  
> `(list ...)  Creates a Lisp-style list  `  
> `(hash ...)  Returns the Pearson16 hash of a string  `  
> `(oper ...)  Inserts a constant value into a code-list as an operator  `  
> `(ref  ...)  Creates a reference  `  
> `(code ...)  Creates a code-list  `  

Examples. Paste these into some filename.sp file and use sparse.pl to compile
and see the results in the .lst file. Note that none of these examples are
runnable as a Babel Virtual Machine, that is, you cannot pass the generated
.bbl file to Babel.

> `((root (val  1 2 3)))`  
> `((root (ptr  1 2 3)))`  
> `((root (ptr root)))`  
> `((root (list 1 2 3)))`  
> `((root (tag 'foo' nil)))`  
> `((root (hash 'foo')))`  
> `((root (oper 0x134)))`  
> `((root (ref 'foo')))`  

In order to generate meaningful code-lists, you will need to pass 
src/opcodes.sp to sparse.pl in addition to your .sp filename.

> `((root (code fnord fnord)))`  

In order to create a loadable BVM, you will need to pass both src/opcodes.sp
and src/construct.sp to sparse.pl in addition to your .sp filename. You must
*not* define 'root' in your .sp file (since it is already defined in 
construct.sp) and you must define a 'main' section and a 'symbol' section.

For example, if foo.sp is the following:

> `(   (main (code "Hello, world\n" stdout8))`  
> `(symbol (list nil)))`  
  
... then you can compile a loadable BVM out from it with the following command:

> `perl sparse.pl foo.sp src/construct.sp src/opcodes.sp src/libdev.sp src/libparse.sp`  

Now, foo.sp.bbl can be executed by the Babel interpreter directly:

> `bin/babel.exe -c foo.sp.bbl`  

