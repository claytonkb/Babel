<head>
    <title>Babel Programming Language</title>
</head>

![](sacred_scarab.jpg)

[**Contact Us**](babel_contact.html)  
<a href="https://github.com/claytonkb/Babel" target="_blank">Babel on github</a>  
<a href="http://rosettacode.org/wiki/Category:Babel" target="_blank">Babel at RosettaCode</a>  
<a href="https://thestrangeloop.com/sessions/babel-an-untyped-stack-based-hll" target="_blank">Babel at Strange Loop 2013</a>  

[Getting Started Guide](babel_gsg.html)

Babel Guts
==========

This document describes the internals of the Babel interpreter.

Note that this document does not detail the syntax of Bipedal (Babel Program
Description Language).

- <a href="#data_struct">   Data Structures</a>
- <a href="#bvm">           Babel Virtual Machine</a>
- <a href="#operators">     Babel Operators</a>
- <a href="#mem_management">Memory Management</a>
- <a href="#misc">          Miscellaneous Topics</a>

<a name="data_struct"></a>
Data Structures
===============

mword
----- 

MWORD is short for machine-word and stands for an unsigned int or 
pointer whose size is the native word size of the machine. In other words,
an mword is either a 32-bit or 64-bit unsigned int or unsigned int pointer
that is treated like a void pointer (to put it in C terminology).

MWORD\_SIZE is the native word size (in bits) divided by 8 bits per byte.
On 32-bit machines, MWORD\_SIZE is 4. On 64-bit machines, it is 8.

bstruct
-------

The data structure in which all data and code (and anything else) reside
is called a bstruct (which stands for Babel-struct). It is important to 
note that the Babel interpreter does not maintain any state which is not 
stored in a bstruct. This means that a bstruct at all times contains a 
complete image of the running program. This makes it trivial to load, save 
and restore Babel programs, even while they are mid-stream.

(NB: "at all times" means whenever an operator has finished executing)

A bstruct consists of one or more of the following data structures, 
connected together:

- leaf-array    : stores values
- interior-array: stores pointers
- tagged-pointer

A bstruct may consist of:

- a single leaf-array OR
- an interior-array or tagged-pointer that may (recursively) point at 
  one or more interior-arrays, tagged-pointers and/or leaf-arrays 

A bstruct is (intentionally) defined in such a way that it may contain
any sort of data. There is nothing specific to Babel about a bstruct. For
example, a large bitmap can be stored as a leaf-array by simply prepending
it with the appropriate s-field in memory.

s-field
-------

To tell apart the three types of array, each array has an S-FIELD.
The s-field is a single mword at position 0 in the allocated array. That
is, the array can be freed by passing a pointer to the s-field to the 
free() function. This permits clean destruction of a bstruct at any time.

- X.s > 0             X is a leaf-array
- X.s = 0             X is a tagged-pointer
- X.s < 0             X is an interior-array

Aside from telling the array type, the s-field also tells the array size.
The size is encoded in bytes so you have to divide by MWORD\_SIZE to get
the size in mwords.

- X.s > 0             X.size = X.s / MWORD\_SIZE
- X.s = 0             X.size = 1 + HASH\_SIZE
- X.s < 0             X.size = -1 * (X.s / MWORD\_SIZE)

The size of every array in a bstruct is an even multiple of MWORD\_SIZE.

The least-significant bit of the s-field is used during traversal of a
bstruct, see below.

Entries
-------

All other mwords in an array other than the s-field are called entries.
The zeroth entry is located at array index 1, immediately following the
s-field.

Leaf-array
----------

A leaf-array contains one or more values stored in an array of mwords. The
contents of a leaf-array do not have to be accessed in mword-aligned 
fashion. For example, it is possible to access a particular byte in a 
leaf-array.

The defining feature of a leaf-array is that it cannot contain pointers to
any other arrays.

_Array-8_

A leaf-array may also be stored as an "array-8". Array-8 is just a 
convention for padding the last mword of a leaf array with a special 
mword that indicates the byte length of the array. Babel strings are 
stored as array-8 in native form, see "Strings" section below.

Interior-array
--------------

An interior-array contains one or more pointers, each mword-sized. 

The defining feature of an interior-array is that every mword in an 
interior-array must contain a pointer to the zeroth entry of an array.

A pointer in an interior-array may point at any other kind of array.

Every pointer in an interior-array must contain a valid pointer to an 
array.

Tagged-Pointer
--------------

A tagged-pointer is a pointer with an associated tag. A tag is a unique
hash. The Babel interpreter recognizes certain tags. User-tags are not
recognized by the interpreter but there are automatic mechanisms for
specifying how user-tags are to be handled.

References
----------

A reference is a specially tagged structure. References permit generalized
indexing of a bstruct. A reference occurring in the code-stream will cause 
the referenced code to to be evaluated. As an operand, the reference will 
be automatically indexed through the symbol table.

Traversing a bstruct
--------------------

A bstruct is a graph, not a tree. Hence, it may contain cycles. Traversal
requires that each array be marked as it is visited. The least significant 
bit of the s-field is used for this. The bstruct is actually traversed
twice, once to set the LSB of each s-field in the bstruct and once to 
clear it again.

Lists
-----

As a convention, the zeroth entry of an interior-array is also termed the 
'car' field and the first entry is also termed the 'cdr' field. This 
permits the construction of bstructs formally identical to lists in Lisp.

Babel borrows the 'cons' 'car' 'cdr' etc. terminology from Lisp for 
this purpose. "A list" always means the portion of a bstruct that conforms
to this convention.

nil behaves slightly differently in Babel than it does in Lisp. Since 
Babel does not have a notion of an atomic value, nil cannot be an atom and
never acts like one. Its primary use in Babel is to mark the end of a 
list. It also serves some special functions in control-flow behavior.

Hashes
------

Hashing is performed with the pearson16() function which is a modification of 
[Pearson's 8-bit permutation hash](http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf) 
to generate 16-byte (128-bit) hash values.

Babel implements extendible hashing (_Fagin, Nievergelt, Pippenger, 
Strong_ 1979) which means the hash table never needs to be re-hashed when
items are inserted or deleted.

Namespaces
----------

Namespaces in Babel are hierarchical. A namespace is "just a label" by
which to refer to something, it is not a "container", "object" or 
"package". Everything lives in one namespace, so you cannot have a
variable with the same name as a function, etc.

The sym\_table is just a namespace but the operators for using a
namespace can be used by the user to maintain user namespaces. The key
is to separate the idea of namespace-as-data-structure and *the* Babel
namespace. The former will be referred to as simply "namespace" and the
latter will be referred to as the sym\_table.

The /babel/path namespace maintains a list of paths similar to Perl's
@EXPORT variable. You can manipulate this variable using any of the 
applicable Babel operators.

Strings
-------

Babel has native support for UTF-8 encoded strings. Babel strings are not
null-terminated. However, a Babel-string stored in array-8 form is always
C-string safe because the alignment-word at the end of an array-8 always
contains one or more null bytes. For example, in 32-bit Babel, the 
alignment word is one of:

    0x00000000      byte-length % 4 = 0
    0xffffff00      byte-length % 4 = 1
    0xffff0000      byte-length % 4 = 2
    0xff000000      byte-length % 4 = 3

Babel handles strings in several different forms:

- Native form. The string is UTF-8 encoded WITHOUT a null terminator in
an array8 leaf-array

- C-style. This is just a native string with a null terminator appended.

- String-array. This is a leaf-array such that each entry in the array 
contains the Unicode code-point of the encoded character. It is created
from a native-form string via the str2ar operator.

- String-list. This is a string-array on which the the ar2ls operator
has been called.    


<a name="bvm"></a>
Babel Virtual Machine (bvm)
===========================

A bvm is a bstruct with a particular structure to it.

BVM Code
--------

Code is a list. Each element of the list is accessed in order. 

When the item pointed at by code\_ptr is: 

- A leaf-array, it is treated as an opcode and a lookup is performed
  in jmp\_table

- A reference, it is looked up in the sym\_table and control is
  transferred there, in a manner equivalent to eval.

- An interior-array and its car is:

    - Not a reference, it is pushed on the stack

    - A reference, it is looked up in the sym\_table and the result
      is pushed on the stack

BVM Stack
---------

The stack is where all operations are performed in Babel. The Babel 
interpreter is a stack machine. Each operator operates on the stack and
returns its results on the stack. There are no registers in Babel. No 
internal state is maintained by the interpreter in C variables.

The stack is itself a list, however, it includes some additional 
information for memory-management. In order to convert the stack to a list, 
use the take operator and use the give operator for vice-versa.

BVM Rstack
----------

The rstack is the "managed stack" (h/t Forth) - it is what is used to 
implement the iteration, control-flow and stack nesting operators.

BVM Interpreter
---------------

The interpreter is a stack machine (prefix order). This means that compilers
that generate Babel code do not need to use parentheses to implement 
operator precedence. The precedence is encoded in the order of operations.

This simplifies the parsing requirements of any Babel front-end language.

Invoking a BVM
--------------

BVMs can be invoked in a nested manner, that is, a BVM may construct 
another BVM and then invoke that BVM, transferring control to it until it
exits. In fact, every Babel program executes inside of an invisible "root"
BVM that is compiled into babel.exe (see src/rt.pb).

This BVM contains code for the debugger and other basic commandline and 
house-keeping functions.

<a name="operators"></a>
Operators
=========

The "active" component of Babel code consists of any of a number of 
operators. There are two types of operators: built-in and extended. Each 
operator is invoked through the jmp\_table.

- Built-in operators

    Built-in operators have a fixed numerical value below 0x1000. 

- Extended operators

    The encoding for an extended operator has a value greater than 0x1000 
    but it is not fixed - an extended operator will be installed in the 
    "next available" jmp\_table entry when it is installed. Babel code 
    should never attempt to directly rely on the encoded value of an 
    operator.

    An extended operator can be given an encoding with the newop operator.

    Extended operators can also be invoked by reference. If the next
    entry in the code list is a reference, a lookup will be performed
    in the sym\_table and the linked code will be invoked. Naturally, this
    is a lower-performance alternative.

<a name="mem_management"></a>
Memory management
=================

Babel uses a combination of automatic and manual memory management.
Memory created by built-in operators is automatically managed, according to 
specific rules. All other memory must be created and destroyed by the user.

<a name="misc"></a>
Miscellaneous Topics
====================

File I/O
--------

Babel provides some "quick-and-dirty" I/O operators inspired by Perl's 
slurp functionality.

For more robust file functionality, Babel provides memory-mapped files.

Jump table
----------

Each built-in opcode is an offset into a jump table. New opcodes that are
added in with the newop operator are dynamically assigned jump table offsets.

When constructing a bvm for launch, the parent bvm can restrict the built-in
operators that are available through the hidden section of the header.

pearson.pl
----------

To hash something using the built-in hash function, use the %% operator:

>       `"nil" %% --> { 0x3023f4e7 0x8c2f644d 0x71cf647b 0xe974b23a }

But a hash is still just a bunch of bytes. In order to create a 
tagged-pointer, you can use the newtag operator:

>       `"nil" %% newtag

Built-in functions, naming conventions and mnemonics
----------------------------------------------------

Babel v0.10 supports a subset of the final built-in functions that are
planned for Babel 1.0. See doc/babel\_ref.txt for a list of Babel operators.

    abbr    meaning
    ------------------
    ar      array
    ls      list

    in      interior
    lf      leaf

    pt      pointer
    va      value

    str     string

    stk     stack

A 'c' as the first letter of the operand name usually means "C-language".
'cu' means "C unsigned" and 'ci' means "C integer (signed)" and 'ca' means
"C arithmetic"

So, for example:

**cu**add       "C unsigned addition"  
**ci**add       "C integer addition"  
**ca**shl       "C arithmetic shift-left"  

As for ordering of operands, I applied the following rule as consistently
as possible:

>       A B op   <===>   "A op B"

For example, consider subtraction:

>       A B -    <===>   "A - B"

Or division,

>       A B /    <===>   "A / B"

The flow of data is as consistently left-to-right as possible.

