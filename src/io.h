// io.h
// 

#ifndef IO_H
#define IO_H

void cprintf(void);
void cprints(void);
mword *_slurp(char *filename);
void slurp(void);
mword *_b2c(mword *string);
void spit(void);
void _spit(char *filename, mword *fileout);
void journal(void);
void _journal(char *filename, mword *fileout);
void stdoutop(void);
void stdinln(void);
void spit_mword(void);

#endif //IO_H

// Clayton Bauman 2011

