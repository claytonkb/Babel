// pearson16.h

#ifndef PEARSON16_H
#define PEARSON16_H

char *pearson16_byte_perm;
mword *new_hash(void);
void pearson16_init(void);
void pearson16(void);
mword *_pearson16(mword* sinit, mword *skey);

#define HASH_SIZE 4

#endif //PEARSON16_H

//Clayton Bauman 2011

