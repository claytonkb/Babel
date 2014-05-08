# docgen.pl
# Generates babel reference documentation

use strict;
use Data::Dumper;

open FILE, "html/common_prologue";
my @common_prologue = <FILE>;
close FILE;

open OP_FILE, ">doc/babel_op.md";

print OP_FILE <<'PROLOGUE';
Babel Operator Reference
========================

- <a href="#arithmetic">  Arithmetic Operators</a>
- <a href="#shift">       Shift Operators</a>
- <a href="#logic">       Logic Operators</a>
- <a href="#comparison">  Comparison Operators</a>
- <a href="#i/o">         I/O Operators</a>
- <a href="#bstruct">     Bstruct Operators</a>
- <a href="#array">       Array Operators</a>
- <a href="#alloc">       Alloc Operators</a>
- <a href="#string">      String Operators</a>
- <a href="#list">        List Operators</a>
- <a href="#hash">        Hash Operators</a>
- <a href="#pearson">     Pearson Operators</a>
- <a href="#stack">       Stack Operators</a>
- <a href="#eval">        Eval Operators</a>
- <a href="#bvm">         BVM Operators</a>
 
_Notation_

Stack diagrams are shown under the "Stack effect" section. The item
next to the vertical bar '|' is TOS (Top-Of-Stack), the arrow separates
before (LHS) and after (RHS) execution of the operator. Unless otherwise
indicated, the rest of the stack is left as-is.

Upper-case denotes an array
Lower-case means a single value

Symbolic operators
------------------

To provide a short-hand for certain common operators and to make Babel 
operators more recognizable in terms of the operators used in other 
languages, I have decided to create a variety of non-alphanumeric
operator aliases. Because these aliases may be cryptic when first 
encountered, I have collected them in one place here:

       Symbol       Operator        Mnemonic
      ------------------------------------------------------------------
        @           trav            what is AT this location?
        ?           sel             a question
        ??          cond            a big question
        %%          hash8           Perl hash sigil: %
        #           size            number-of
        #8          size8           number-of (8-bit)
        ##          len             number-of (lists take up more space)
        !           eval            do it!
        <<          stdout8         like cout <<
        >>          stdinln8        like cin >>
        <<<         spit8           like stdout8 but going all the way to file
        >>>         slurp8          like stdinlin8 but coming all the way from file
        <->         swap            reverse TOS and TOS-1
        <-          down            stack on left, moving left is going "down" 
        ->          up              stack on left, moving right is going "up"    
        +           ciadd           same as C
        -           cisub           same as C
        *           cimul           same as C
        /           cidiv           same as C
        %           curem           same as C
        =           ceq             same as C ==
        <           cilt            same as C
        >           cigt            same as C
        >=          cige            same as C
        <=          cile            same as C
        u+          cuadd           "unsigned +"
        u-          cusub             "    "
        u*          cumul             "    "
        u/          cudiv             "    "
        u<          cult              "    "
        u>          cugt              "    "
        u>=         cuge              "    "
        u<=         cule              "    "
        &           cand            same as C
        |           cor             same as C
        ^           cxor            same as C
        ~           cnot            instead of !
        ~&          cnand           see ~
        ~|          cnor            see ~
        ~^          cxnor           see ~
        ~=          cne             see ~
        &&          and             same as C
        ||          or              same as C
        ~~          not             see ~
        ==          cmp             equality (for bigger things than just mwords)
        ...         each            traversing a list
        $           show            Perl scalar sigil (deprecate?)
        %d          ci2dec          like printf
        %u          cu2dec          like printf
        %x          cu2hex          like printf
        $d          dec2ci          reverse of printf
        $x          hex2cu          reverse of printf
        .           cat8            like the Perl operator
PROLOGUE

my @files = qw{ arith shift logic cmp io 
                bstruct array alloc string 
                list hash pearson16 stack
                eval bvm };

for my $filename (@files){

    open FILE, "src/$filename.c" or die "Couldn't open src/$filename.c: $!";
    my @file = <FILE>;
    close FILE;

    my $docs_line=0;
    my $line_num=0;
    my $type;
    my $block;
    my $refs = {};
    my $new_ref;

    for my $line (@file){
        $line_num++;
        if($line =~ /^\/\*\s+(\S+)\soperator/){
            ($type) = $line =~ /^\/\*\s+(\S+)\soperator/;
            $new_ref = {};
            $new_ref->{lines} = [];
            $new_ref->{file} = "src/$filename.c";
            $new_ref->{line_num} = $line_num;
            push @{$refs->{$type}}, $new_ref;
            $docs_line=1;
            next;
        }
        next unless($docs_line);
        if($line =~ /^\*\//){
            $docs_line=0;
            next;
        }
        push @{$refs->{$type}[$#{$refs->{$type}}]{lines}},$line;
    }

    #print Dumper($refs);

    for my $type (keys $refs){
        print OP_FILE "<a name=\"$type\"></a>\n";
        print OP_FILE uc($type) . " OPERATORS  \n";
        print OP_FILE "-----------------\n\n";
        for my $oper (@{$refs->{$type}}){
            print OP_FILE $_ for(@{$oper->{lines}});
            print OP_FILE "> _($oper->{file}, line $oper->{line_num})_  \n\n\n";
        }
    }
}

close OP_FILE;

my $rev_id = `git rev-parse HEAD`;
my $date = `date`;

my @files = qw{
    babel_op 
    babel_gsg
    babel_guts
    babel_contact
    babel_example
    babel_home
    README };

for(@files){

    open FILE, "doc/$_.md";
    my @file_lines = <FILE>;
    close FILE;

    push @file_lines, "\n<span class=\"gen_stamp\">Git ID: $rev_id</span>  \n<span class=\"gen_stamp\">Generated: $date  </span>\n";
#    push @file_lines, "\nGit ID: $rev_id  \nGenerated: $date  \n";
#    push @file_lines, "GAHHHHHHHHHHHHHHH\n";

    open FILE, ">doc/$_.stamped.md";
    print FILE @file_lines;
    close FILE;

    `perl markdown.pl doc/$_.stamped.md > doc/$_.html`;

    @file_lines = ();

push @file_lines, <<BLOCK1;
<p><head>
<title>Babel Programming Language</title>
<link href="style.css" rel="stylesheet" type="text/css">
</head></p>

<div id="header2"></div>
<div id="header">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Babel</div>
<div id="leftnav">
<p>
BLOCK1

if($_ eq "babel_home"){
    push @file_lines, "<span class=\"current_page\">Home</span> <br />";
}
else{
    push @file_lines, "<a href=\"babel_home.html\">Home</a><br />";
}

if($_ eq "babel_gsg"){
    push @file_lines, "<span class=\"current_page\">Getting Started</span> <br />";
}
else{
    push @file_lines, "<a href=\"babel_gsg.html\">Getting Started</a><br />";
}

if($_ eq "babel_op"){
    push @file_lines, "<span class=\"current_page\">Babel Operators</span> <br />";
}
else{
    push @file_lines, "<a href=\"babel_op.html\">Babel Operators</a><br />";
}

if($_ eq "babel_guts"){
    push @file_lines, "<span class=\"current_page\">Babel Guts</span> <br />";
}
else{
    push @file_lines, "<a href=\"babel_guts.html\">Babel Guts</a><br />";
}

if($_ eq "babel_example"){
    push @file_lines, "<span class=\"current_page\">Babel Examples</span> <br />";
}
else{
    push @file_lines, "<a href=\"babel_example.html\">Babel Examples</a><br />";
}

push @file_lines, <<BLOCK2;
</p>
<p><a href="babel_contact.html"><strong>Contact Us</strong></a> <br /></p>
<p><a href="https://github.com/claytonkb/Babel" target="_blank">Babel on github</a> <br />
<a href="http://rosettacode.org/wiki/Category:Babel" target="_blank">Babel at RosettaCode</a> <br />
<a href="http://www.infoq.com/presentations/babel" target="_blank">Babel at Strange Loop 2013</a>  </p>
</div>

<div id="rightnav"></div>
<div id="body">
BLOCK2

    open FILE, "doc/$_.html";
    push @file_lines, <FILE>;
    close FILE;

    push @file_lines, "</div>";

    open FILE, ">html/$_.html";
    print FILE join('', @file_lines);

}



