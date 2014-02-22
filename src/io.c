// io.c
//

#include "babel.h"
#include "io.h"
#include "stack.h"
#include "bstruct.h"
#include "except.h"
#include "list.h"
//#include "bvm_opcodes.h"
#include "io.h"
#include "array.h"
#include "utf8.h"
#include "string.h"
#include "alloc.h"
#include "tptr.h"

//FIXME: Lots of bad things in here...
//
mword *_slurp(bvm_cache *this_bvm, mword *filename){ // _slurp#

    long   size;
    FILE*  file;
    mword* buffer;
    size_t result;
    long   file_mword_size;

    file = fopen ( (char*)_b2c(this_bvm, filename) , "rb" );
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

    last_mword = alignment_word8(this_bvm, size);

    buffer = _newlf(this_bvm, file_mword_size);

    if ((mword)buffer & (MWORD_SIZE-1)) { //FIXME: This shouldn't be here...
        error("expected memory to be MWORD_SIZE aligned");
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


/* i/o operator
**slurp8** (>>>)  
> Named after the Perl idiom  
> 
> Slurps an entire file to an array (or array8)  
> `{"filename"}| -> {X}|`  
> 
> X is the contents of the named file.  
*/
bvm_cache *slurp(bvm_cache *this_bvm){ // slurp#

    mword *filename = _b2c(this_bvm, dstack_get(this_bvm,0));
    mword *result   = _slurp(this_bvm, filename);

    popd(this_bvm);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}

/* i/o operator
**slurp**  
> Named after the Perl idiom  
> 
> Slurps an entire file to an array (or array8)  
> `{"filename"}| -> {X}|`  
> 
> X is the contents of the named file.  
*/
bvm_cache *slurp_mword(bvm_cache *this_bvm){ // slurp_mword // *slurp_mword#

    mword *filename = _b2c(this_bvm, dstack_get(this_bvm,0));
    popd(this_bvm);

    mword *result   = _slurp(this_bvm, filename);

    _trunc(this_bvm, result, size(result)-1);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;


}

/* i/o operator
**cprintf**  
> "C-style printf()"  
> 
> Permits printing of C-style (null-terminated) strings with a format  
> specifier.  
> 
> `{x} {"format-specifier"}| -> |`    
> 
> Prints x according to a standard printf() format specifier.  
> 
> Prefer stdout or stdout8  
*/
bvm_cache *cprintf(bvm_cache *this_bvm){ // cprintf#

    printf((char*)(dstack_get(this_bvm,0)), (mword)car(dstack_get(this_bvm,1)));

    popd(this_bvm);
    popd(this_bvm);

    return this_bvm;

}


/* i/o operator
**cprints**  
> "C-style printf('%s')"  
> 
> Similar to cprintf but prints a plain-string  
> 
> `{"C-string"}| -> |`  
> 
> Prefer stdout  
*/
bvm_cache *cprints(bvm_cache *this_bvm){ // cprints#

    printf("%s", ((char*)dstack_get(this_bvm,0)));
    popd(this_bvm);

    return this_bvm;

}

/* i/o operator
**spit8** (<<<)  
> Play on words - opposite of slurp. Writes an array (or array8)
> to file. Overwrites existing file. Creates file if it does not
> exist.  
> 
> `{X} {"filename"}| -> |`  
*/
bvm_cache *spit(bvm_cache *this_bvm){ // spit#

    _spit(this_bvm, (char*)dstack_get(this_bvm,0), dstack_get(this_bvm,1));
    popd(this_bvm);
    popd(this_bvm);

    return this_bvm;

}


//
//
void _spit(bvm_cache *this_bvm, char *filename, mword *fileout){ // _spit#

    FILE * pFile;

    mword filesize   = _arlen8(this_bvm, fileout);

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

/* i/o operator
**journal8**  
> Same as spit8 but appends to existing file. Creates file if it does
> not exist.  
> 
> `{X} {"filename"}| -> |`  
*/
bvm_cache *journal(bvm_cache *this_bvm){ // journal8#

    mword *filename = _b2c(this_bvm, dstack_get(this_bvm,0));
    _journal(this_bvm, (char*)filename, dstack_get(this_bvm,1));
    popd(this_bvm);
    popd(this_bvm);

    return this_bvm;

}


//
//
void _journal(bvm_cache *this_bvm, char *filename, mword *fileout){ // _journal // _journal#

    FILE * pFile;

    mword filesize   = _arlen8(this_bvm, fileout);

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


/* i/o operator
**spit**  
> Play on words - opposite of slurp. Writes an array (or array8)
> to file. Overwrites existing file. Creates file if it does not
> exist.  
> 
> `{X} {"filename"}| -> |`  
*/
bvm_cache *spit_mword(bvm_cache *this_bvm){ // spit_mword // *spit_mword#

    _spit_mword(this_bvm, (char*)dstack_get(this_bvm,0), dstack_get(this_bvm,1));
    popd(this_bvm);
    popd(this_bvm);

    return this_bvm;

}


//
//
void _spit_mword(bvm_cache *this_bvm, char *filename, mword *fileout){ // _spit_mword#

    FILE * pFile;

    mword filesize   = size(fileout) * MWORD_SIZE;

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



//bvm_cache *spit_mword(bvm_cache *this_bvm){ // spit_mword
//
//    char *filename = (char*)_b2c(this_bvm, dstack_get(this_bvm,0));
//
//    FILE * pFile;
//
//    mword filesize   = size(dstack_get(this_bvm,1)) * MWORD_SIZE;
//
//    popd(this_bvm);
//    popd(this_bvm);
//
//    pFile = fopen(filename , "wb");
//
//    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
//        error("spit: file error");
//    }
//
//    fwrite((char*)TOS_1(this_bvm), 1, filesize, pFile);
//
//    //Return Value
//    //The total number of elements successfully written is returned as a size_t object, which is an integral data type.
//    //If this number differs from the count parameter, it indicates an error.
//
//    fclose (pFile);
//
//    return this_bvm;
//
//}
//

/* i/o operator
**stdout**  
> Prints an array to STDOUT as UCS-4 characters  
> `{"string"}| -> |`  
*/
bvm_cache *stdoutop(bvm_cache *this_bvm){ // stdoutop#

    mword *op0 = dstack_get(this_bvm,0);
    popd(this_bvm);

    mword length = size(op0);
    int i;

    for(i = 0; i<length; i++){
        putchar((int)(*(op0+i)));
    }

    return this_bvm;

}


// FIXME: Make UTF-8 compliant...
/* i/o operator
**stdinln** (>>)  
> Reads from STDIN (standard input) until newline.
> This operator is byte-wise so it places an array8 on TOS  
> `| -> `{"string"}|`  
*/
bvm_cache *stdinln(bvm_cache *this_bvm){ // stdinln#

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

    mword *result = _newlf(this_bvm, arlength);
    memcpy(result, buffer, i);
    free(buffer);

    c(result,arlength-1) = alignment_word8(this_bvm, i);

    pushd(this_bvm, result, IMMORTAL);

    return this_bvm;

}


// FIXME: Make UTF-8 compliant...
/* i/o operator
**stdout** (<<)  
> Prints an array8 to STDOUT in UTF-8 encoding    
> `{"string"}| -> |`  
*/
bvm_cache *stdoutop8(bvm_cache *this_bvm){ // stdoutop8#

    _stdoutop8(this_bvm, dstack_get(this_bvm,0));

    popd(this_bvm);

    return this_bvm;

}


/* i/o operator
**pr/stdout8** (<<)  
> Prints an array8 to STDOUT in UTF-8 encoding   
> `{"string"}| -> |`  
*/
void _stdoutop8(bvm_cache *this_bvm, mword *string){ // _stdoutop8#

    int i;
    mword length = _arlen8(this_bvm, string);

    char *cast_string = (char*)string;

    for(i=0; i<length; i++){
        putchar((int)cast_string[i]);
    }

}

// Clayton Bauman 2011

