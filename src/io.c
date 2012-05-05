// io.c
//

#include "babel.h"
#include "io.h"
#include "stack.h"
#include "bstruct.h"
#include "except.h"
#include "list.h"
#include "bvm_opcodes.h"
#include "io.h"
#include "array.h"
#include "utf8.h"
#include "string.h"

//_slurp
//
mword *_slurp(mword *filename){ // FIXME: Lots of bad things in here...

    long   size;
    FILE*  file;
    mword* buffer;
    size_t result;
    long   file_mword_size;

    file = fopen ( (char*)_b2c(filename) , "rb" );
//    file = fopen(filename , "rb");

    if (file==NULL) {
        error(filename)
        error("_slurp: error opening file")
    }

    //file size
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind (file);
    mword last_mword;

    file_mword_size = size/MWORD_SIZE; 

    if(size % MWORD_SIZE != 0){//XXX Assumes the int div rounds to floor
        file_mword_size++;
    }

    file_mword_size++; //for the alignment_word

    last_mword = alignment_word8(size);

    buffer = _newlf(file_mword_size);

    if (buffer == NULL) {
        error("_slurp: malloc returned NULL");
    }

    if ((mword)buffer & (MWORD_SIZE-1)) {
        error("_slurp: expected memory to be MWORD_SIZE aligned");
    }

    // copy the file into the buffer:
    result = fread( (char*)buffer, 1, size, file );

    c(buffer, file_mword_size-1) = last_mword;

//    if (result != size) { fputs ("Reading error",stderr); exit (3); }
    if (result != size) { //fputs ("Reading error",stderr); exit (3); }
        error("_slurp: error reading file")
    }
   
    fclose (file);

    return buffer;

}

//slurp
//
bvm_cache *slurp(bvm_cache *this_bvm){

//    mword *temp_cons = new_cons();
//    mword *filename = _b2c((mword*)TOS_0);
//    mword *result   = _slurp((char*)filename);

    mword *result   = _slurp((mword*)car(TOS_0(this_bvm)));

    //    cons(temp_cons, result, nil);

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, SLURP8);

    

    return this_bvm;

}

//slurp_mword
//
bvm_cache *slurp_mword(bvm_cache *this_bvm){

//    mword *temp_cons = new_cons();
//    mword *filename = _b2c((mword*)TOS_0);
//    mword *result   = _slurp((char*)filename);

    mword *result   = _slurp((mword*)TOS_0(this_bvm));

    _trunc(result, size(result)-1);

//    cons(temp_cons, result, nil);

    hard_zap(this_bvm);
    push_alloc(this_bvm, result, SLURP);

    

    return this_bvm;

}

//cprintf
//
bvm_cache *cprintf(bvm_cache *this_bvm){

    printf((char*)(TOS_0(this_bvm)), (mword)car(TOS_1(this_bvm)));

    zap(this_bvm);
    zap(this_bvm);

    

    return this_bvm;

}

//cprints
//
bvm_cache *cprints(bvm_cache *this_bvm){

    printf("%s", ((char*)TOS_0(this_bvm)));
    zap(this_bvm);

    

    return this_bvm;

}

//
bvm_cache *spit(bvm_cache *this_bvm){

//    mword *filename = _b2c((mword*)TOS_0(this_bvm));
//    _spit((char*)filename, (mword*)TOS_1(this_bvm));
    _spit((char*)TOS_0(this_bvm), (mword*)TOS_1(this_bvm));
    zap(this_bvm);
    zap(this_bvm);

    

    return this_bvm;

}

//
void _spit(char *filename, mword *fileout){

    FILE * pFile;

    mword filesize   = _arlen8(fileout);

    pFile = fopen(filename , "wb");

    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
        error("slurp: file error");
    }

    fwrite(fileout, 1, filesize, pFile);
    //Return Value
    //The total number of elements successfully written is returned as a size_t object, which is an integral data type.
    //If this number differs from the count parameter, it indicates an error.

    fclose (pFile);

}

//
bvm_cache *journal(bvm_cache *this_bvm){

    mword *filename = _b2c((mword*)TOS_0(this_bvm));
    _journal((char*)filename, (mword*)TOS_1(this_bvm));
    zap(this_bvm);
    zap(this_bvm);

    return this_bvm;

}

//
void _journal(char *filename, mword *fileout){

    FILE * pFile;

    mword filesize   = _arlen8(fileout);

    pFile = fopen(filename , "ab"); //First try

    if (pFile==NULL){ //First try fails if file didn't already exist
        pFile = fopen(filename, "wb");
    }

    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
        error("_journal: file error");
    }

    fwrite(fileout, 1, filesize, pFile);
    //Return Value
    //The total number of elements successfully written is returned as a size_t object, which is an integral data type.
    //If this number differs from the count parameter, it indicates an error.

    fclose (pFile);

}

//
bvm_cache *spit_mword(bvm_cache *this_bvm){

    char *filename = (char*)_b2c((mword*)TOS_0(this_bvm));

    FILE * pFile;

    mword filesize   = size((mword*)TOS_1(this_bvm)) * MWORD_SIZE;

    pFile = fopen(filename , "wb");

    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
        error("spit: file error");
    }

    fwrite((char*)TOS_1(this_bvm), 1, filesize, pFile);

    //Return Value
    //The total number of elements successfully written is returned as a size_t object, which is an integral data type.
    //If this number differs from the count parameter, it indicates an error.

    fclose (pFile);

    zap(this_bvm);
    zap(this_bvm);

    

    return this_bvm;

}

//
bvm_cache *stdoutop(bvm_cache *this_bvm){

    mword length = size((mword*)TOS_0(this_bvm));
    int i;

    for(i = 0; i<length; i++){
        putchar((int)(*((mword*)TOS_0(this_bvm)+i)));
    }

    zap(this_bvm);

    

    return this_bvm;

}

// FIXME: Make UTF-8 compliant...
//
bvm_cache *stdinln(bvm_cache *this_bvm){

    int c, i=0;
    char buffer[(1<<16)]; //64K buffer (for now)

    while(1){
        c = fgetc(stdin);
        if(c == EOF || c == '\n'){
            break;
        }
        buffer[i] = c;        
        i++;
    }

    mword arlength = (i / 4) + 1;

    if(i % 4){
        arlength++;
    }

    mword *result = _newlf(arlength);
    memcpy(result, buffer, i);
    free(buffer);

    c(result,arlength-1) = alignment_word8(i);

    push_alloc(this_bvm, result, STDINLN);

    

    return this_bvm;

}

// FIXME: Make UTF-8 compliant...
//
bvm_cache *stdoutop8(bvm_cache *this_bvm){

    _stdoutop8((mword*)TOS_0(this_bvm));
    
    hard_zap(this_bvm);

    

    return this_bvm;

}

void _stdoutop8(mword *string){

    int i;
    mword length = _arlen8(string);
    char *cast_string = (char*)string; //C's casting syntax sucks!

    for(i=0; i<length; i++){
        putchar((int)cast_string[i]);
    }

}

// Clayton Bauman 2011

