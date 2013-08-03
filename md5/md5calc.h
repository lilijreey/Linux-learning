#ifndef _MD5CALC_H_
#define _MD5CALC_H_

/** output is the coded character sequence in the character sequence which wants to code string. */
void MD5_String(const char * string, char * output);

/** output is the coded binary in the character sequence which wants to code string. */
void MD5_Binary(const char * string, unsigned char * output);

#endif /* _MD5CALC_H_ */
