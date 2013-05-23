Philosophy of Babel

- Let it be

    There's no point dictating to people, they're going to use the language
    they like and they'll like a language based on how well it is suited to
    help them achieve their goals. Hence, my primary design object with Babel 
    has been to make it adept at helping people achieve their goals. This is
    complicated by the fact that there is not exactly a list of the most 
    common goals which programmers have.

- Optimize for best performance on the programmer's brain

    Larry Wall has pointed out that we tend to over-optimize for the machine 
    and often fail to optimize for performance on the programmer's mind. This
    results in poor overall performance when code is filled with bugs and 
    sub-optimal algorithms or falls into disrepair due to the difficulty of
    maintenance.

    Babel's initial inspiration came from my first encounter with Perl. I 
    learned Perl only after having earned my degree and I realized that what
    I could do in minutes with Perl would have taken me weeks of intense C++ 
    programming to achieve, even with the STL or utilizing specialized, 
    third-party libraries. This is not because Perl has some special powers
    over other languages, it's because of Perl's optimization philosophy.

- Make the common case fast (Amdahl's Law)

    The most important law of computer design is to make the common case fast.
    There's very little performance benefit to optimizing something that only
    happens a tiny fraction of the time, however fun the optimization of that 
    particular problem may be.

    Babel tries to avoid optimization for its own sake. Clearly, a more 
    optimal solution should always be preferred over a less optimal solution,
    all things equal. But all things are rarely ever equal so smart 
    optimization is directed towards the common case.

    No language can magically solve all your problems but it can sure make 
    them worse. Babel attempts to avoid making things worse than they have
    to be.

- Respect tradition

    There is a large body of common practice across a wide spectrum of 
    computer cultures. These traditions usually represent the "best in class"
    and deserve respect for that reason.

    Babel's design is inspired by and indebted to many computing traditions:
    Unix, GNU, linux, C, C++, Perl, Lisp, Forth (and Joy), YAML and many 
    others.

    Babel looks to tradition to identify what the common case is in order to
    optimize to that. For example, it might seem strange on first brush that
    I have included bzip2 and bunzip2 as core operators in Babel but the need 
    to quickly and easily compress/decompress data without a lot of headache
    is a common programming need.

    Babel says: Why hide the compression operators off in some library where 
    the programmer has to know they exist, go look them up and then import 
    them? Why not just make the common case fast? If you hate bzip2, you can
    always roll your own operator and add it into Babel or import an existing
    library.

- Be humble

    A lot of programming concepts have very lofty names but when you start
    tearing off the Christmas-packaging and look underneath, you often find 
    that the reality is decidedly less grandiose. Babel attempts to take a 
    more humble approach. While there are 'reflection' 'polymorphism' 
    'pure functional evaluation' 'call with current continuation' and even 
    'lazy evaluation' in Babel, they are not called these things.

- The machine IS the object

    Babel notably lacks classes and objects. After Perl was forced to adopt
    objects (with the bless operator), isn't it crazy to once again try to
    have a language without objects?

    My answer is that the machine IS the object - the virtual-machine, that 
    is. The Babel Virtual Machine (BVM) is an object on steroids - it has
    a symbol-table, it has code, stack and data, it has I/O, it has resource-
    limits, etc. It is containerized. Most notably, it can implement and 
    enforce access rights and code-execution rights.

    To implement objects in Babel would be redundant and kludged - why go for
    half-hearted objects when you can encapsulate your code into a fully-
    featured virtual machine?

- The way of no way

    Perl's philosophy is the famous TIMTOWTDI - There Is More Than One Way To
    Do It. This is one of the many reasons I fell in love with Perl 5. Tim
    Peters famously expresses a different sentiment in The Zen of Python: 
    "There should be one - and preferably only one - obvious way to do it."

    Babel is basically attempting to out-Perl5 Perl5 - in Babel, there is NO 
    way to do it... in the sense that Bruce Lee said, "No way is the way." 
    I have attempted to avoid creating "ways" to do things at all. Of course,
    this is not completely possible but, in time, I am confident we can train 
    Babel to Be Water, My Friend.

- Preserve symmetry over orthogonality

    Many Babel operators are redundant. I have yet to understand all the 
    hubbub about orthogonality - the basic operations of a pure Turing machine
    are orthogonal and they are everything you need to build a fully 
    functioning programming language but I'd hate to program a Turing machine.

    Instead of orthogonality, Babel has chosen symmetry as the guiding light.
    Whenever possible, I have tried to make operators such that they exactly
    undo each other.

    [1 2 3] 2 cut cat --------> [1 2 3]

    I believe this design philosophy will make it easier for the programmer to
    memorize the operation of the core operators. If you know what cut does,
    you also know what its opposite (cat) does.

- Be cooperative

    Babel's core operators are designed to consume input from as many other 
    operators as possible and generate output that can be consumed by as many
    other operators as possible. This makes the suite of operators available
    for operation on a particular piece of data as large as possible.

    Babel attempts to apply this same philosophy to the external environment,
    as well. Babel will read in any file type, and can save its internal state
    in a variety of the most widely supported data formats (UTF-8, Graphviz 
    dot, CSV, YAML-subset, S-expressions etc.)

    Babel is also easily built into your C or other language project and can
    be easily compiled with C (and possibly other) language extensions. Babel
    can dynamically link with any library that understands how to register new
    opcodes in Babel which is an almost trivial operation.

- Be literate

    Babel's layout is partly inspired by Knuth's philosophy of literate 
    programming. The primacy of the namespace hierarchy in the layout of 
    Bipedal, indent-delimitation of comments, quotes and namespaces, the 
    "normally open" nature of a Bipedal file, the avoidance of arbitrary 
    punctuation characters as operators, the permission of Unicode characters
    as identifier names and the use of "visible whitespace" are all inspired 
    by literate-programming ideas.

    Babel is conscientiously international. The non-English-speaking world is 
    the future of technological and economic growth and they need to be able
    to program in the language they speak. This is why Babel has seamless 
    support for UTF-8 strings (parsing, string operations, etc.) and why 
    Bipedal files are UTF-8 encoded, permitting Unicode identifier and 
    operator names.

- Avoid intellectual property tarpits

    Babel does not originate in the university system, it has been developed
    entirely in my own spare time. I have decided to keep Babel out of the GNU
    tarpit - Babel does not use any GNU software and is BSD-licensed. This 
    means that Babel can be used in closed-source, commercial products without
    royalties or other hinderances.

    As anyone who has spent five minutes in honest, unhampered reflection on 
    the issue knows, intellectual property law is in severe disrepair in the 
    developed world. Babel is open-source but respects the right (the 
    commercial need, in fact) of software developers that release their 
    software onto open platforms - such as the PC - to keep their source code
    closed.

- Sell it

    OK, here's my sales pitch. Why should you use Babel? Here are some of the
    things that Babel can do that no other major language can do (easily).

    - It is easy to containerize and launch nested Babel programs, making it 
      easy to manage the risks of executing arbitrary code.

    - Built-in crypto makes it easy to enforce a "white-list" code execution 
      policy, reducing the risks associated with executing remote code.

    - As a by-product of containerization, it is easy to save and restore your
      Babel program to and from disk at any point, making it easy for the user
      to suspend and resume at will. This is a prerequisite to any fully-
      virtualized compute environment.

    - Your data structures are all stored in an underlying, uniform bstruct
      structure, making it trivial to save and restore them to and from disk,
      to make deep copies of them, to delete all memory associated with them, 
      and so on.

    - You can easily compress and encrypt your program data objects, (or whole 
      Babel programs).

    - Visualizing your data is uniquely easy with Babel's built-in support for
      generating Graphviz dot files. These can be post-processed with the dot
      tool to generate graphical snapshots of your data. This speeds up debug
      and helps the programmer fully absorb the semantic significance of the 
      syntax.

    - Babel provides both Lisp-style lists and array structures, permitting you
      to organize your data in a manner to maximize both flexibility and 
      performance. Modern computer architecture is array-based and can perform
      array lookups many times faster than list-traversal. But for data that is
      constantly changing size or continually undergoing complex permutations,
      lists are a clear performance winner by permitting you to perform more of
      your operations in-place.

    - Babel provides a comprehensive suite of abstract data-structures, 
      including array, list, hash (map/dictionary) and namespace (or directory).
      This gives you the tools you need for rapid prototyping of very complex
      code even if you eventually choose to implement your solution in a lower-
      level language and link it in.

    - The Babel namespace is modeled on a Unix-style directory structure, making
      it possible to nest data and code in arbitrarily deep and complex 
      structures. Manipulating namespace paths permits implementation of 
      OO-style templates, polymorphism and more.

    - Arbitrary-precision arithmetic is built right into the Babel core, making
      it easy to implement crypto algorithms in an academic (non-optimized)
      fashion. Other applications such as desktop calculation are also 
      facilitated.

