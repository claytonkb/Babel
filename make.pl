# make.pl

system("perl proto_babel.pl src/rt.pb pb/header.pb pb/opcodes.pb");

`rm -f bin/babel.exe`;

@files = `ls src/*.c`;

chomp for @files;

`tcc/tcc.exe -DDEBUG -Wall -o bin/babel.exe @files`;

