// io.h
// 

#ifndef IO_H
#define IO_H

#define _pr(x,y) _output8(x,y,stdout)

void _output8(bvm_cache *this_bvm, mword *string, FILE *stream);
FILE *io_open_file(bvm_cache *this_bvm, mword *filename);
bvm_cache *io_close_file(bvm_cache *this_bvm, FILE *file);
mword *_slurp8(bvm_cache *this_bvm, mword *filename);
mword io_file_size(bvm_cache *this_bvm, FILE *file);
bvm_cache *stdout_8d(bvm_cache *this_bvm);
bvm_cache *foo(bvm_cache *this_bvm);
bvm_cache *_foo(bvm_cache *this_bvm);
bvm_cache *slurp_8d(bvm_cache *this_bvm);
bvm_cache *slurp_md(bvm_cache *this_bvm);
bvm_cache *spit_md(bvm_cache *this_bvm);
bvm_cache *spit_8d(bvm_cache *this_bvm);
bvm_cache *journal_8d(bvm_cache *this_bvm);
mword *_slurp(bvm_cache *this_bvm, mword *filename);
mword *_stdinln(bvm_cache *this_bvm, FILE *stream);
bvm_cache *stdinln8(bvm_cache *this_bvm);
void _spit(bvm_cache *this_bvm, mword *filename, mword *fileout, mword access_size);
void _journal(bvm_cache *this_bvm, mword *filename, mword *fileout, mword access_size);

#endif //IO_H

// Clayton Bauman 2014

