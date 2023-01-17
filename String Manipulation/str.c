/* 20190533 Hyemin Lee Assignment 2 str.c */

#include <assert.h>
#include <stdio.h>
#include "str.h"

/* Part 1 */
/*
  Function name : StrGetLength
  Parameter     : *pcSrc (A string that we want to know one's length)
  Return value  : length of the input string
  This function returns the length of input string
*/
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

/*
  Function name : StrCopy
  Parameter     : *pcSrc (A string that we want to copy)
                  *pcDest (A location where copied string will be exist)
  Return value  : Location of a string that is copied
  This function copies pcSrc to pcDest
*/
char *StrCopy(char *pcDest, const char* pcSrc)
{
  assert(pcSrc);
  const char *pcEnd;
  pcEnd = pcSrc;
  while(*pcEnd){
    *pcDest=*pcEnd;
    pcEnd++;
    pcDest++;
  }
  if (*pcDest) *pcDest='\0';
  pcDest-=StrGetLength(pcSrc);
  return pcDest;
}

/*
  Function name : StrCompare
  Parameter     : *pcS1 (A string that we want to compare)
                  *pcS2 (A string that we want to compare)
  Return value  : Integer (Difference of ascii code value between the first string and the second string)
  This function compares two string and returns the difference between those of ascii code value.
  If the first string and second string is completely same, returns 0.
  Else, return the substraction of the ascii code value of first different character.
*/
int StrCompare(const char* pcS1, const char* pcS2)
{
  assert(pcS1);
  assert(pcS2);
  /* -L : Local version of that variable */
  const char* pcS1L=pcS1;
  const char* pcS2L=pcS2;
  int sub=0;
  while(*pcS1L && *pcS2L && sub==0){
    sub=*pcS1L-*pcS2L;
    pcS1L++;
    pcS2L++;
  }
  if (*pcS1L ^ *pcS2L) sub = *pcS1L-*pcS2L;
  return sub;
}

/*
  Function name : StrSearch
  Parameter     : *pcHaystack (A string that we will go out through)
                  *pcNeedle (A string that we will check whether this string exists inside pcHaystack)
  Return value  : NULL or Location of *pcNeedle exist inside of *pcHaystack
  This function finds *pcNeedle inside of *pcHaystack. If it exists, it returns location of that.
  Else, it returns NULL.
*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle)
{
  assert(pcHaystack);
  assert(pcNeedle);
  const char* pcHaystackL=pcHaystack;
  const char* pcNeedleL=pcNeedle;
    while(*pcNeedleL){
      if (!*pcHaystackL) return NULL;
      if(*pcHaystackL!=*pcNeedleL) {
        pcNeedleL=pcNeedle;
        while(*pcHaystackL!=*pcNeedleL) {
          pcHaystackL++;
          if (!*pcHaystackL) return NULL;
        }
      }
      pcHaystackL++;
      pcNeedleL++;
    }
    return (char *)(pcHaystackL-StrGetLength(pcNeedle));
}

/*
  Function name : StrConcat
  Parameter     : *pcDest (A string that will be extended from *pcSrc)
                  *pcSrc (A string that will be added to *pcDest)
  Return value  : Extended string
  This function extends *pcDest with *pcSrc. And returns the extended string
*/
char *StrConcat(char *pcDest, const char* pcSrc)
{
  assert(pcDest);
  assert(pcSrc);
  char* pcDestL=pcDest;
  const char* pcSrcL=pcSrc;
  pcDestL+=StrGetLength(pcDest);
  while(*pcSrcL){
    *pcDestL=*pcSrcL;
    pcDestL++;
    pcSrcL++;
  }
  return pcDest;
}