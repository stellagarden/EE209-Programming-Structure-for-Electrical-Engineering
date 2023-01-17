#ifndef _STR_H_
#define _STR_H_
#include <unistd.h> /* for typedef of size_t */

/* Part 1 */
size_t StrGetLength(const char* pcSrc);
char *StrCopy(char *pcDest, const char* pcSrc);
int StrCompare(const char* pcS1, const char* pcS2);
char *StrSearch(const char* pcHaystack, const char *pcNeedle);
char *StrConcat(char *pcDest, const char* pcSrc);

#endif /* _STR_H_ */