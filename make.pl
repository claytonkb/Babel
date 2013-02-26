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

@files = qw {   babel pearson16 stack array load list bstruct 
                string utf8 hash tlist alloc ref arith bvm eval
                cmp debug except io logic mt19937ar shift 
                tags util };

$_ = "src/$_.c" for @files;

`tcc/tcc.exe -DDEBUG -Wall -o bin/babel.exe @files`;


