# make.pl

if(!-e 'bin'){
    mkdir 'bin';
}

system("perl sparse.pl src/construct.sp src/root.sp src/opcodes.sp");
system("perl tags.pl");

`rm -f bin/babel.exe`;

#@files = `ls src/*.c`;
#
#chomp for @files;

if($#ARGV > -1){

    `op_doc_gen.pl > doc/babel_op.md`;

    $rev_id = `git rev-parse HEAD`;
    $date = `date`;

    @files = qw{
        doc/babel_op 
        doc/babel_gsg
        doc/babel_guts
        README };

    for(@files){

        open FILE, "$_.md";
        @file_lines = <FILE>;
        close FILE;

        push @file_lines, ("\n", "Git ID: $rev_id\n", "Generated: $date\n");

        open FILE, ">$_.stamped.md";
        print FILE @file_lines; #"\nGit ID: $rev_id\nGenerated: $date\n";

    }

    `perl markdown.pl doc/babel_gsg.stamped.md > html/babel_gsg.html`;
    `perl markdown.pl doc/babel_contact.md > html/babel_contact.html`;
    `perl markdown.pl doc/babel_guts.stamped.md > html/babel_guts.html`;
    `perl markdown.pl doc/babel_op.stamped.md > html/babel_op.html`;
    `perl markdown.pl README.stamped.md > html/README.html`;

}

@files = qw {   babel pearson16 stack array load list bstruct 
                string utf8 hash tptr alloc ref arith bvm eval
                cmp except io logic mt19937ar shift 
                tags util interp mem };

$_ = "src/$_.c" for @files;

`tcc/tcc.exe -DDEBUG -Wall -o bin/babel.exe @files`;


