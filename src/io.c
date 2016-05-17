// io.c
//

#include "babel.h"
#include "io.h"
#include "array.h"
#include "string.h"
#include "bstruct.h"
#include "stack.h"
#include "introspect.h"
#include "list.h"
#include "bvm.h"
#include "operator.h"


//
//
mword *_stdinln(bvm_cache *this_bvm, FILE *stream){ // _stdinln#

    int c, i=0;
    char buffer[(1<<16)]; //FIXME: make buffer size an argument to this function

    while(1){ //FIXME unsafe, wrong
        c = fgetc(stream);
        if(c == EOF || c == '\n'){
            break;
        }
        buffer[i] = c;
        i++;
    }

    mword file_mword_size = _array8_size(this_bvm,i);

    mword *result = _newlfi(this_bvm, file_mword_size, 0);

    memcpy(result, buffer, i); // XXX WAIVER(memcpy) XXX //

    lcl(result, file_mword_size-1) = _alignment_word8(this_bvm, i);

    return result;

}


//
//
void _output8(bvm_cache *this_bvm, mword *string, FILE *stream){ // _output8#

    if(!is_nil(string)){
        fprintf(stream, (char*)string);
    }

}


// returns array-8
//
mword *_slurp8(bvm_cache *this_bvm, mword *filename){ // _slurp8#

    FILE *file = io_open_file(this_bvm, filename);
    mword file_size = io_file_size(this_bvm, file);

    mword file_mword_size = _array8_size(this_bvm,file_size);

    mword *buffer = _newlfi(this_bvm, file_mword_size, 0);

    mword alignment_mword = _alignment_word8(this_bvm, file_size);

    fread( (char*)buffer, 1, file_size, file );

    lcl(buffer, file_mword_size-1) = alignment_mword;

    io_close_file(this_bvm,file);

    return buffer;

}


//
//
FILE *io_open_file(bvm_cache *this_bvm, mword *filename){ // io_open_file#

    FILE*  file;

    char *bfilename = (char*)string_b2c(this_bvm, filename);

    file = fopen( bfilename, "rb" );

    // FIXME: set a flag instead? cat_ex is overkill...
    if(file==NULL){ 
        _fatal((char*)filename);
    }

    return file;

}


//
//
bvm_cache *io_close_file(bvm_cache *this_bvm, FILE *file){ // io_close_file#

    fclose(file); // Return value can be checked to see if there were any issues closing the file...

    return this_bvm;

}


//
//
mword io_file_size(bvm_cache *this_bvm, FILE *file){ // io_file_size#

    fseek(file, 0L, SEEK_END);
    mword size = ftell(file);
    rewind(file);

    return size;

}


//
//
mword *_slurp(bvm_cache *this_bvm, mword *filename){ // _slurp#

    mword *result = _slurp8(this_bvm, filename);

    _trunc(this_bvm, result, size(result)-1);

    return result;

}


//
//
void _spit(bvm_cache *this_bvm, mword *filename, mword *fileout, mword access_size){ // _spit#

    FILE *pFile;

    mword filesize;

    if(access_size == ACCESS_SIZE_MWORD){
        filesize = size(fileout) * MWORD_SIZE;
    }
    else{ //access_size == ACCESS_SIZE_BYTE
        filesize = _arlen8(this_bvm, fileout);
    }

    pFile = fopen((char*)filename, "wb");

    if(pFile==NULL){
        _fatal("file error");
    }

    fwrite(fileout, 1, filesize, pFile);

    fclose(pFile);

}


//
//
void _journal(bvm_cache *this_bvm, mword *filename, mword *fileout, mword access_size){ // _journal#

    FILE *pFile;

    mword filesize;

    if(access_size == ACCESS_SIZE_MWORD){
        filesize = size(fileout) * MWORD_SIZE;
    }
    else{ //access_size == ACCESS_SIZE_BYTE
        filesize = _arlen8(this_bvm, fileout);
    }

    pFile = fopen((char*)filename, "ab");

    if(pFile==NULL){
        _fatal("file error");
    }

    fwrite(fileout, 1, filesize, pFile);

    fclose(pFile);

}


////
////
//void _spit8(bvm_cache *this_bvm, char *filename, mword *fileout){ // _spit8#
//
//    FILE * pFile;
//
//    mword filesize   = _arlen8(this_bvm, fileout);
//
//    pFile = fopen(filename , "wb");
//
//    if (pFile==NULL) {//fputs ("File error",stderr); exit (1);}
//        error("slurp: file error");
//    }
//
//    fwrite(fileout, 1, filesize, pFile);
//    //Return Value
//    //The total number of elements successfully written is returned as a size_t object, which is an integral data type.
//    //If this number differs from the count parameter, it indicates an error.
//
//    fclose (pFile);
//
//}
//

/*****************************************************************************
 *                                                                           *
 *                             IO OPERATORS                                  *
 *                                                                           *
 ****************************************************************************/

#define STDOUT_8D_OPERATIONS \
    _pr(this_bvm, oi0.data);

//OPERATOR_R1_W0_8D(stdout_8d, STDOUT_8D_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)
OPERATORA_R1_W0_D(stdout_8d, STDOUT_8D_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)


/* i/o operator
**slurp8** (>>>)  
> Named after the Perl idiom  
> 
> Slurps an entire file to array8  
> `"filename"| -> X|`  
> 
> X is the contents of the named file.  
*/
#define SLURP_8D_OPERATIONS \
    result0 = _slurp8(this_bvm, oi0.data); 

//OPERATOR_R1_W1_8D(slurp_8d, SLURP_8D_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)
OPERATORA_R1_W1_D(slurp_8d, SLURP_8D_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)


/* i/o operator
**slurp**  
> Named after the Perl idiom  
> 
> Slurps an entire file to an array. Assumes that the file-size is
> mword-aligned.
> `"filename"| -> X|`  
> 
> X is the contents of the named file.  
*/
#define SLURP_MD_OPERATIONS \
   result0 = _slurp(this_bvm, oi0.data); 

//OPERATOR_R1_W1_MD(slurp_md, SLURP_MD_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)
OPERATORA_R1_W1_D(slurp_md, SLURP_MD_OPERATIONS, nil, OI_MASK_LEAF, 0, 0)


#define SPIT_MD_OPS \
    _spit(this_bvm, oi0.data, oi1.data, ACCESS_SIZE_MWORD);

OPERATORA_R2_W0_D(spit_md,SPIT_MD_OPS,
        nil,OI_MASK_LEAF,0,0,
        nil,OI_MASK_LEAF,0,1)


#define SPIT_8D_OPS \
    _spit(this_bvm, oi0.data, oi1.data, ACCESS_SIZE_BYTE);

OPERATORA_R2_W0_D(spit_8d,SPIT_8D_OPS,
        nil,OI_MASK_LEAF,0,0,
        nil,OI_MASK_LEAF,0,1)


#define JOURNAL_8D_OPS \
    _journal(this_bvm, oi0.data, oi1.data, ACCESS_SIZE_BYTE);

OPERATORA_R2_W0_D(journal_8d,JOURNAL_8D_OPS,
        nil,OI_MASK_LEAF,0,0,
        nil,OI_MASK_LEAF,0,1)


#define STDINLN_8_OPERATIONS \
   result0 = _stdinln(this_bvm, stdin);

OPERATORA_R0_W1(stdinln8, STDINLN_8_OPERATIONS)


// Clayton Bauman 2014

