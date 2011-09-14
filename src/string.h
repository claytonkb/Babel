// string.h
// 

#ifndef STRING_H
#define STRING_H

void b2c(void);
mword *_b2c(mword *string);

void ar2str(void);
void str2ar(void);

void c2b(void);
mword *_c2b(char *string, mword max_safe_length);

void catoi(void);
void dec2ci(void);
void cu2dec(void);
void ci2dec(void);
void hex2cu(void);
void cu2hex(void);

#endif //STRING_H

// Clayton Bauman 2011

