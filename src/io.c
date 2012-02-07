// XXX STACK FIX DONE
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

//cprintf
//
void cprintf(void){

    printf((char*)(TOS_0), (mword)car(TOS_1));

    zap();
    zap();

}

//cprints
//
void cprints(void){

    printf("%s", ((char*)TOS_0));
    zap();

}

//_slurp
//
mword *_slurp(char *filename){ // FIXME: Lots of bad things in here...

    FILE * pFile;
    long lSize;
    mword* buffer;
    size_t result;
    long file_mword_size;

    pFile = fopen ( filename , "rb" );
    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
        printf("%s\n", filename);
        except("_slurp: file error", __FILE__, __LINE__);
    }

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    mword last_mword;

    file_mword_size = lSize/MWORD_SIZE; 

    if(lSize % MWORD_SIZE != 0){//XXX Assumes the int div rounds to floor
        file_mword_size++;
    }

    file_mword_size++; //for the alignment_word

    last_mword = alignment_word8(lSize);

    // allocate memory to contain the whole file
    //buffer = (char*) malloc ( (sizeof(char)*file_mword_size*MWORD_SIZE) );
    buffer = _newlf(file_mword_size);

    if (buffer == NULL) {
        except("_slurp: malloc returned NULL", __FILE__, __LINE__);
    }

    if ((mword)buffer & (MWORD_SIZE-1)) {
        except("_slurp: expected memory to be MWORD_SIZE aligned", __FILE__, __LINE__);
    }

    // copy the file into the buffer:
    result = fread ( (char*)buffer, 1, lSize, pFile );

//    char *last_mword_addr = (char*)(buffer+(file_mword_size*MWORD_SIZE));
//    (char*)last_mword_addr = (buffer+file_mword_size+MWORD_SIZE);
//    *last_mword_addr = last_mword;
    c(buffer, file_mword_size-1) = last_mword;

    if (result != lSize) { fputs ("Reading error",stderr); exit (3); }

    /* the whole file is now loaded in the memory buffer. */

    fclose (pFile);
//    return (mword *)(buffer+2*MWORD_SIZE);
    return buffer;

}

//slurp
//
void slurp(void){

//    mword *temp_cons = new_cons();
//    mword *filename = _b2c((mword*)TOS_0);
//    mword *result   = _slurp((char*)filename);
    mword *result   = _slurp((char*)TOS_0);

//    cons(temp_cons, result, nil);

    zap();
    push_alloc(result, SLURP8);

}

//
void slurp_mword(void){

//    mword *temp_cons = new_cons();
//    mword *filename = _b2c((mword*)TOS_0);
//    mword *result   = _slurp((char*)filename);
    mword *result   = _slurp((char*)TOS_0);

    _trunc(result, size(result)-1);

//    cons(temp_cons, result, nil);

    zap();
    push_alloc(result, SLURP);

}

void spit(void){

//    mword *filename = _b2c((mword*)TOS_0);
//    _spit((char*)filename, (mword*)TOS_1);
    _spit((char*)TOS_0, (mword*)TOS_1);
    zap();
    zap();

}

//
//
void _spit(char *filename, mword *fileout){

    FILE * pFile;

    mword filesize   = _arlen8(fileout);

    pFile = fopen(filename , "wb");

    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
        except("slurp: file error", __FILE__, __LINE__);
    }

    fwrite(fileout, 1, filesize, pFile);
    //Return Value
    //The total number of elements successfully written is returned as a size_t object, which is an integral data type.
    //If this number differs from the count parameter, it indicates an error.

    fclose (pFile);

}

//
//
void journal(void){

    mword *filename = _b2c((mword*)TOS_0);
    _journal((char*)filename, (mword*)TOS_1);
    zap();
    zap();

}

//
//
void _journal(char *filename, mword *fileout){

    FILE * pFile;

    mword filesize   = _arlen8(fileout);

    pFile = fopen(filename , "ab"); //First try

    if (pFile==NULL){ //First try fails if file didn't already exist
        pFile = fopen(filename, "wb");
    }

    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
        except("_journal: file error", __FILE__, __LINE__);
    }

    fwrite(fileout, 1, filesize, pFile);
    //Return Value
    //The total number of elements successfully written is returned as a size_t object, which is an integral data type.
    //If this number differs from the count parameter, it indicates an error.

    fclose (pFile);

}

void spit_mword(void){

    char *filename = (char*)_b2c((mword*)TOS_0);

    FILE * pFile;

    mword filesize   = size((mword*)TOS_1) * MWORD_SIZE;

    pFile = fopen(filename , "wb");

    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
        except("spit: file error", __FILE__, __LINE__);
    }

    fwrite((char*)TOS_1, 1, filesize, pFile);

    //Return Value
    //The total number of elements successfully written is returned as a size_t object, which is an integral data type.
    //If this number differs from the count parameter, it indicates an error.

    fclose (pFile);

    zap();
    zap();

}

// FIXME: Make UTF-8 compliant...
//
void stdoutop(void){

    mword length = _arlen8((mword*)TOS_0);
    int i;

    for(i = 0; i<length; i++){
        putchar((int)(*(char*)(TOS_0+i)));
    }

    zap();

}

// FIXME: Make UTF-8 compliant...
//
void stdinln(void){

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

    push_alloc(result, STDINLN);

}

// Clayton Bauman 2011

