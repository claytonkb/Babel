# make.pl

if(!-e 'bin'){
    mkdir 'bin';
}

system("perl proto_babel.pl src/construct.pb pb/opcodes.pb");

`rm -f bin/babel.exe`;

@files = `ls src/*.c`;

chomp for @files;

`tcc/tcc.exe -DDEBUG -Wall -o bin/babel.exe @files`;

