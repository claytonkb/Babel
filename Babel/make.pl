# make.pl

`rm -f bin/babel.exe`;

@files = `ls src/*.c`;

chomp for @files;

`tcc/tcc.exe -DDEBUG -Wall -o bin/babel.exe @files`;

