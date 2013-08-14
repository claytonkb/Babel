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

    open FILE, ">>doc/babel_op.md";
    print FILE "\nGit ID: $rev_id\nGenerated: $date\n";
    close FILE;

    open FILE, ">>doc/babel_gsg.md";
    print FILE "\nGit ID: $rev_id\nGenerated: $date\n";
    close FILE;

    open FILE, ">>doc/babel_guts.md";
    print FILE "\nGit ID: $rev_id\nGenerated: $date\n";
    close FILE;

    open FILE, ">>README.md";
    print FILE "\nGit ID: $rev_id\nGenerated: $date\n";
    close FILE;

    `perl markdown.pl doc/babel_gsg.md > html/babel_gsg.html`;
    `perl markdown.pl doc/babel_guts.md > html/babel_guts.html`;
    `perl markdown.pl doc/babel_op.md > html/babel_op.html`;
    `perl markdown.pl README.md > html/README.html`;

}

@files = qw {   babel pearson16 stack array load list bstruct 
                string utf8 hash tptr alloc ref arith bvm eval
                cmp debug except io logic mt19937ar shift 
                tags util interp };

$_ = "src/$_.c" for @files;

`tcc/tcc.exe -DDEBUG -Wall -o bin/babel.exe @files`;


