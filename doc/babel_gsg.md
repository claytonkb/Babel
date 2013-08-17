![](sacred_scarab.jpg)

You can browse or download Babel on <a href="https://github.com/claytonkb/Babel" target="_blank">github</a>.

Be sure to check out the many examples of <a href="http://rosettacode.org/wiki/Category:Babel" target="_blank">Babel code on rosettacode.org.</a>

[Babel Internals](babel_guts.html)

[Babel Operator Reference](babel_op.html)

Getting Started
===============

Babel is an interpreted language.  It is an untyped, stack-based, postfix 
language with support for arrays, lists and hashes (dictionaries). Babel 
1.0 will support built-in crypto-based verification of code in order to 
enable safer remote code execution.

Babel was designed with the TIMTOWTDI  philosophy in mind – there are many 
right ways to solve any particular problem in Babel. However, some 
solutions are a bad fit for Babel. In particular, trying to write Babel 
code to resemble standard function-call code – while possible – will 
result in sub-optimal, hard-to-read Babel code.

Babel is a stack-based language - the stack is the primary means of 
communicating data from one place to another. The ideal situation is to 
reduce your code to a set of sections that can be thought of as 
combinators where each combinator takes a small number or zero operands 
from the stack and returns one or zero results on the stack. In this 
case, you will not have “stack-noise” induced by the need to re-arrange 
arguments on the stack.

The next preferred situation is if your sections take multiple values on 
the stack and each value is used only once or twice in the section, in 
order or nearly so. In this case, you can simply use the up and down 
operators to move cleanly from operand to operand.

But if one or more operands are going to be used in more than a couple 
places in a section, this will create significant stack-noise which is 
not only a performance problem but drastically reduces the readability 
of your Babel code.

In this case, consider using symbolic-variables or lexical variables, 
depending on your needs. Symbolic variables are entries in the symbol 
table and are primarily intended for structural or spatial organization. 
Lexical variables (symbolic variables can also be treated lexically) 
permit you to make a section re-entrant and are primarily intended for
functional or temporal organization.

Many array- and list-processing tasks are most easily done by monopolizing 
the entire stack with an each loop. To facilitate this, Babel provides the 
`nest` operator. Within a nested context, you have a fresh stack – when you 
return from the nested context, that temporary stack will be destroyed, 
except for TOS which is returned. You can read doc/babel_ref.txt for more 
information on the nest operator.

There are two kinds of Babel syntax. The first kind is being used to 
bootstrap the Babel construct, it is called Sparse (S-expression parser).
The second kind is the built-in Babel parser, which is called Bipedal
(Babel Program Description Language).

S-expression parser (sparse.pl)
-------------------------------



Babel Program Description Language (Bipedal)
--------------------------------------------

The primary design goal in Bipedal has been to make the syntax reflect the 
underlying data structure that is being described as closely as possible. 
Bipedal is UTF-8 encoded - this means you can give your sections any name 
that can be encoded in UTF-8. You can easily alias the built-in operator 
names, as well.

Like any other postfix language, operands come first, then operators:

>       2 3 + 4 *

The above expression, when evaluated, will result in 20.

>       "Hello, " "world" . <<

When this expression is evaluated, it will place the two strings on the 
stack, concatenate them and then print them to STDOUT.

Babel differentiates between values and pointers. **Values** are stored in 
leaf-arrays and **pointers** are stored in interior-arrays. In Bipedal, there 
are two syntactic elements that describe values: a number or a string.

A **number** can be a decimal integer, decimal floating-point, hexadecimal, 
binary or a p-number (or pnum - support will be added in Babel 2.0). 
Bipedal differentiates between integer and floating-point based on the 
presence of a decimal-point and the default is integer.

> `-42e15`  
> `32.7152e-12`  
> `0x13`  
> `0b1101111`  
> `0p(101)1.11e-5`  

The limits are dependent on MWORD\_SIZE (use the msize operator to get 
this) except for pnums which can be of arbitrary size.

A string is a set of characters wrapped in quotes:

> `"Lorem ipsum dolor sit amet"`  
> `'Lorem ipsum dolor sit amet'`  

... or in a quote-block (see below). The quote character can be escaped 
with a backslash in the usual way. Line comments in Bipedal are specified 
by double-dash:

> `-- This is a comment`  

Bipedal is organized at the top-level as a set of labeled text blocks 
which can be nested. A label is a string of characters that begins at 
the left-edge and is followed by a colon or equal-sign:

> `this_is\ a\ label\!\@          :`  
> `this_is\ a\ label_too:`  
> `this_is\ a\ label           =`  
> `and\=so\=is\=this=`  

Any character can be used in a label, including the colon and equal-sign. 
However, any ASCII character that is not an identifier-character must be 
escaped with the backslash. Non-ASCII Unicode characters can be used 
freely. The first text in a Bipedal file is ignored until the first, 
left-justified label is encountered.

A label opens an indented text block which is terminated either by the end 
of file or by a dedent. The indent must be at least one space. Tabs or any 
other whitespace cannot be used as an indent character. A line that is 
less than one space further indented than the left-edge of the current 
block is a dedent and ends the current text block. A labeled text block 
allows the definition of a section.

> `my_section : ( 10 9 8 7 6 5 4 3 2 1 "Blast off!" )`  

Every section is entered into the symbol table at compile-time. Hence, 
every section label is a symbol. However, symbols can also be created 
dynamically so not every symbol is a section.

A leaf-array is defined by enclosing a string or one or more numbers in 
square-brackets:

> `[1 1 2 3 5 8 13 0x15]`
> `["Smart men are named Leonardo"]`

If the section is a leaf-array that only contains a single number or a 
single string value, the square-brackets can be removed because they are 
implied:

> `foo : 1234`    
> `bar : "baz"`    

A leaf-array cannot nest and an interior-array cannot contain values, so 
the following are illegal:

> `["speeding" ["jaywalking"]]`  
> `[["littering"] "loitering"]`  

An interior-array is defined by enclosing one or more leaf-arrays, labels 
or interior arrays in square-brackets:

> `[["the"]["cuckoo’s"]["nest"]]`  

Note that values are represented in bold boxes, pointers are represented in standard boxes. 
Because the square brackets are overloaded between leaf-arrays and interior-arrays, it is possible to have ambiguous sections:

> `A : [B]`  
> `B : [A]`  

Whenever Babel cannot unambiguously resolve the meaning of square-brackets
in two or more sections, it will treat them as interior-arrays. So, the 
above example would resolve to section A being an interior-array with a 
single direct-reference to section B and vice-versa.

An empty namespace defaults to nil:

> `foo :`  
> `bar : 123`  

... is equivalent to:

> `foo : nil`  
> `bar : 123`  

Lists are created with parentheses. A list in Babel means a linked-list 
in the Lisp-sense. The following are equivalent:

> `(1 2 3)`  

> `[[1] [[2] [[3] nil]]]`  

You can verify this with the following code:

> `{ (1 2 3) [[1] [[2] [[3] nil]]] eq }`  

... which will evaluate to 1 (true).

The nil element is distinct from Lisp’s in that Babel has no notion of an 
atomic and nil is just implemented as an entry in the symbol table - 
but you can take its car or cdr and you will always get nil again. In 
keeping with the spirit of Lisp, you can also notate nil as 
empty-parentheses or empty-brackets:

> `[] == () == nil`

One final difference between Bipedal syntax and Lisp syntax is that 
Bipedal does not use the dot-notation. In Lisp, the following syntax

> `(1 2 . 3)`  

... expresses the same list as the following written in Bipedal:

> `(1 [2 3])`  

In Babel, operands in the code-stream are differentiated from operators 
by being nested in an interior-array. For example, to add two numbers, 
the following are all equivalent:

> `A: ( [42] [23] + )`  
> `B: ( (42) (23) + )`  
> `C: ( [42 nil] [23 nil] + )`  
> `D: ( [42 nil] [23 nil] 0x38 )`  


A-D all produce the same result when executed. A and B do not generate 
exactly the same byte-code but B, C and D do.

A code-list is created with curly-braces. A code-list is just a list but 
it performs the nesting of non-operators automatically, thus reducing 
visual clutter. It also alters the meaning of labeled references. The 
following are identically equivalent:

> `( [2] [3] + )`  

> `{ 2 3 + }`  

And:

> `( ["Hello, "] ["world\n"] . << )`  

> `{ "Hello, "  "world\n" . << }`  

You can check that this is the case:

> `{ { 1 2 3 "Hello, world" } ( [1] [2] [3] ["Hello, world"] ) eq }`  

Note that the default meaning of labeled references is different 
depending on whether you are in code-list or data-list context.

The tagged-pointer is the third basic sub-type alongside interior-arrays 
and leaf-arrays that together comprise the bstruct type in which all 
Babel data is stored. A tagged-pointer is created using the 
angle-brackets:

< tag stuff >

The stuff that follows the tag (if any) is dependent on the tag. Certain 
built-ins, such as nil, are implemented internally using tagged-pointers.

Sigils
------

Babel permits the use of a sigil (magic symbol) to specify the meaning of 
a label or symbol. There are six sigils:

<table>
  <tr>
    <th>Sigil</th><th>Magical property</th>
        <th>Bare symbol equivalent in</th><th>Mnemonic</th>
  </tr>
  <tr>
    <td>*</td><td>Lexical substitution</td><td>Leaf-array</td><td>"Contents-of" (C)</td>
  </tr>
  <tr>
    <td>&</td><td>Direct reference</td><td>Interior-array and list</td><td>Reference (C++)</td>
  </tr>
  <tr>
    <td>$</td><td>Indirect reference</td><td>N/A</td><td>Reference (Perl)</td>
  </tr>
  <tr>
    <td>%</td><td>Soft reference</td><td>Code-list</td><td>Hash-sigil (Perl)</td>
  </tr>
  <tr>
    <td>!</td><td>Auto-eval reference</td><td>N/A</td><td>Imperative - do it!</td>
  </tr>
  <tr>
    <td>@</td><td>List location</td><td>N/A</td><td>Where it's at</td>
  </tr>
</table>

Appending a sigil to a section label or to a symbol imparts that magical 
property to it and may alter its default behavior. A sigil appended to a 
label overrides the default sense of that label in any context. However, 
you can double-override the symbol by appending a different sigil in a 
particular context:

> `foo% : 127`  
> `bar : ( foo& ) --> Overrides foo back to a direct-reference`  

The default behavior of a bare symbol varies depending on the context in 
which it occurs. 

The asterisk (*) specifies that a symbol or section is to be used for 
lexical substitution. Lexical substitution allows a section or string to 
be expanded in a manner similar to a #define in C or string-interpolation 
in Perl. Note that lexical substitutions do not take arguments and are 
not macros.

This:

> `foo : (1 2 3)`  
> `bar : (foo* 4 5)`  

... is equivalent to this:

> `bar : (1 2 3 4 5)`  

The order in which you define sections is maintained by Bipedal. In the 
above example, foo has precedence over bar.

Note that you can only use like with like - interior arrays can be 
substituted only into interior arrays, leaf arrays only into leaf arrays, 
lists only into lists, code-lists only into code-lists and strings only 
into strings (a compile-time version of string interpolation).

> `hi : "hello"`  
> `hw : "hi* world"`  

hw evaluates to "hello world"

Only double-quotes (and interpolating quote blocks) will perform lexical 
substitution of strings. Use single-quotes if you want to use asterisks in 
a string without having to escape each one.

If you plan to use a section primarily for lexical substitution, you can 
append an asterisk to the section-definition label.

> `foo* : [bar baz]`  
> `bop : [foo doo wop]`  

Equivalent to:

> `bop : [bar baz doo wop]`  

And:

> `add2 : { 2 + }`  
> `bar : { 1 add2* }`  

Equivalent to:

> `bar : { 1 2 + }`  

In a leaf-array, the default (and only) meaning of a symbol is lexical 
substitution.

> `foo : [ 1 2 3 ]`  
> `bar : [ foo 4 5 ]`  

Equivalent to:

> `bar : [ 1 2 3 4 5 ]`  

Gratuitous use of sigils is permitted:

> `foo* : [1 2 3]`  
> `bar : [foo* 4 5]`  

You cannot use any other sigil than the asterisk on a symbol that occurs 
in a leaf-array. If the symbol cannot be lexically substituted into the 
leaf-array, an error will occur. A reference to ‘foo’ refers directly to 
the thing called ‘foo’ at compile-time rather than performing a lookup in 
the symbol table at run-time. There are two types of references - direct 
references (pointer) and indirect references (pointer-pointer). 

In list context or interior-array context, the default meaning of a bare 
symbol is a direct reference.

> `foo : [1 2 3]`  
> `bar : [foo [4] [5]]`  

Equivalent to:

> `bar : [[1 2 3] [4] [5]]`  

The same is true of lists:

> `foo : (1 2 3)`  
> `bar : (foo 4 5)`  

Equivalent to:

> `bar : ((1 2 3) 4 5)`  

Note that, unlike lexical substitution, a direct-reference to foo is 
actually a pointer to foo and, therefore, preserves the nesting but does 
not make a copy. If you want to nest and make a copy:

> `foo : [1 2 3]`  
> `bar : [[foo*] [4] [5]]`  

If you want an indirect reference in list, interior-array or code-list 
context, use the dollar-sign sigil ($):

> `foo : [1 2 3]`  
> `bar : [foo$ [4] [5]]`  

Equivalent to:

> `bar : [[[1 2 3]] [4] [5]]`  

Note the extra pair of brackets. As a general rule, you should never need 
to use indirect references in interior-array or list context.

> `foo : (1 2 3)`  
> `bar : { foo$ len }`  

Equivalent to:

> `bar : ( [foo] len )`  

In code-lists, if you want a direct-reference, use the ampersand sigil 
(&):

> `foo& : 0x38`  
> `bar : { foo }`  

Equivalent to:

> `bar : ( 0x38 )`  

If you want to use a symbol as a hash-reference in an interior array or a 
list, use the percent sigil (%):

> `foo : [1 2 3 bar%]`  
> `bar : (1 2 3 bar%)`  

In a code-list, the default meaning of a bare symbol is as a hash-reference. 

> `foo : (1 2 3)`  
> `bar : { foo len }`  

Equivalent to:

> `bar : ([foo%] len)`  

In Babel code, a hash-reference can have one of two effects - it can either 
auto-eval a named section or it can auto-lookup data from the symbol table 
and place it on the stack. The default sense is to auto-lookup data. If 
you want to auto-eval it, you can use the auto-eval sigil, bang (!):

> `add2 : { 2 + }`  
> `foo : { 3 add2! }`  

Note that this has exactly the same effect as:

> `foo : { 3 add2 eval }`  

Most of the time, you will likely want to simply define the section with 
the auto-eval sigil:

> `add2! : { 2 + }`  
> `foo : { 3 add2 }`  

The auto-eval sigil can only be used in code-list context.

The location sigil (@) can never be used as a section definition label or 
in array context. It allows you to create direct references within a list 
or code-list. For example:

> `foo: ( 1 2 three@ 3)`  
> `bar: { foo/three car %d << }`  

Equivalent to:

> `{ foo cdr cdr car %d << }`  

You can use this facility to create a _compile-time hash_ that permits 
your code to reach the elements of a list but without the overhead of 
run-time hashing. For example:

> `( first_name@        "John"`  
> `last_name@           "Smith"`  
> `ID@                  "19010378" )`  

An unenclosed bare symbol is treated as a simple alias.

> `foo : bar`  

Anywhere foo is used, it will be as if bar had been used instead. You 
cannot use sigils on either side of an alias statement and the alias 
inherits any sigil attached to the aliased section. You cannot alias a 
location label. Aliases have global visibility, so you can alias any 
section by using its full name. You can use this feature to alias the 
built-in operators to a name of your preference:

> `suma : +`  
> `foo : { 2 3 suma }`  

If a symbol is not defined by a labeled section in your Bipedal file, it 
is treated as if it defines a symbolic section. That is, use of an 
otherwise undefined symbol implicitly creates an entry in the symbol-table 
for that symbol. This is the case regardless of whether the symbol refers 
to a symbolic variable or a reference variable.



Git ID: 17a2c14a367047db9a27da458859b4cc78675e56

Generated: Wed, Aug 14, 2013 12:30:39 PM


Git ID: 17a2c14a367047db9a27da458859b4cc78675e56

Generated: Wed, Aug 14, 2013 12:33:14 PM


Git ID: 17a2c14a367047db9a27da458859b4cc78675e56

Generated: Wed, Aug 14, 2013  4:19:54 PM


Git ID: 791a40ed0cbfbe1d48e69d9c216f4af843f77088

Generated: Wed, Aug 14, 2013  5:43:09 PM


Git ID: e5f7d0e78d86ce29d5e034b3442f465d5c9a9445

Generated: Fri, Aug 16, 2013  7:30:59 PM

