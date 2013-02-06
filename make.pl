# make.pl

if(!-e 'bin'){
    mkdir 'bin';
}

system("perl sparse.pl src/construct.sp");
system("perl tags.pl");

`rm -f bin/babel.exe`;

@files = `ls src/*.c`;

chomp for @files;

`tcc/tcc.exe -DDEBUG -Wall -o bin/babel.exe @files`;


