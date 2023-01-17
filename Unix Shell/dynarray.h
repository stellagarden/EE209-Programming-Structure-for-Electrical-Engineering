/*--------------------------------------------------------------------*/
/* dynarray.h                                                         */
/* Author: Bob Dondero                                                */
/* Modified by Younghwan Go                                           */
/*--------------------------------------------------------------------*/

#ifndef DYNARRAY_INCLUDED
#define DYNARRAY_INCLUDED

/* shows whether the |, <, > in this token is
   FUNCTIONAL or NON-FUNCTIONAL(i.e. ordinary). */
enum TokenType {TOKEN_FUNCTIONAL_I, TOKEN_FUNCTIONAL_RB, TOKEN_FUNCTIONAL_LB, TOKEN_NON_FUNCTIONAL};

/* A Token is a struct with eType and pcValue.
   eType : shows whether the | in this token is 
   FUNCTIONAL or NON-FUNCTIONAL(i.e. ordinary).
   pcValue : A string that indicates what this token is */
struct Token
{
   enum TokenType eType;
   /* The type of the token. */

   char *pcValue;
   /* The string which is the token's value. */
};

typedef struct Token * Token_T;

/* A DynArray consists of an array, along with its logical and
   physical lengths. */
struct DynArray
{
	/* The number of elements in the DynArray from the client's
	   point of view. */
	int iLength;
	
	/* The number of elements in the array that underlies the
	   DynArray. */
	int iPhysLength;
	
	/* The array that underlies the DynArray. */
	const void **ppvArray;
};

/* A DynArray_T is an array whose length can expand dynamically. */
typedef struct DynArray * DynArray_T;

/* Return a new DynArray_T object whose length is iLength, or
   NULL if insufficient memory is available. */
DynArray_T DynArray_new(int iLength);

/* Free oDynArray. */
void DynArray_free(DynArray_T oDynArray);

/* Return the length of oDynArray. */
int DynArray_getLength(DynArray_T oDynArray);

/* Return the iIndex'th element of oDynArray. */
void *DynArray_get(DynArray_T oDynArray, int iIndex);

/* Assign pvElement to the iIndex'th element of oDynArray.  Return the
   old element. */
void *DynArray_set(DynArray_T oDynArray, int iIndex,
				   const void *pvElement);

/* Add pvElement to the end of oDynArray, thus incrementing its length.
   Return 1 (TRUE) if successful, or 0 (FALSE) if insufficient memory
   is available. */
int DynArray_add(DynArray_T oDynArray, const void *pvElement);

/* Add pvElement to oDynArray such that it is the iIndex'th element.
   Return 1 (TRUE) if successful, or 0 (FALSE) if insufficient memory
   is available. */
int DynArray_addAt(DynArray_T oDynArray, int iIndex,
				   const void *pvElement);

/* Remove and return the iIndex'th element of oDynArray. */
void *DynArray_removeAt(DynArray_T oDynArray, int iIndex);

/* Fill ppvArray with the elements of oDynArray.  ppvArray should point
   to an area of memory that is large enough to hold all elements of
   oDynArray. */
void DynArray_toArray(DynArray_T oDynArray, void **ppvArray);

/* Apply function *pfApply to each element of oDynArray, passing
   pvExtra as an extra argument.  That is, for each element pvElement of
   oDynArray, call (*pfApply)(pvElement, pvExtra). */
void DynArray_map(DynArray_T oDynArray,
				  void (*pfApply)(void *pvElement, void *pvExtra),
				  const void *pvExtra);

/* Sort oDynArray in the order determined by *pfCompare.
   *pfCompare should return <0, 0, or >0 depending upon whether
   *pvElement1 is less than, equal to, or greater than *pvElement2,
   respectively. */
void DynArray_sort(DynArray_T oDynArray,
				   int (*pfCompare)(const void *pvElement1, const void *pvElement2));

/* Linear search oDynArray for *pvSoughtElement using *pfCompare to
   determine equality.  Return the index at which *pvSoughtElement
   is found, or -1 if there is no such index.
   *pfCompare should return 0 if *pvElement1 is equal to pvElement2,
   and non-0 otherwise. */
int DynArray_search(DynArray_T oDynArray, void *pvSoughtElement,
					int (*pfCompare)(const void *pvElement1, const void *pvElement2));

/* Binary search oDynArray for *pvSoughtElement using *pfCompare to
   determine equality.  Return the index at which *pvSoughtElement
   is found, or -1 if there is no such index.
   *pfCompare should return <0, 0, or >0 depending upon whether
   *pvElement1 is less than, equal to, or greater than *pvElement2,
   respectively.  oDynArray should be sorted as determined by
   *pfCompare. */
int DynArray_bsearch(DynArray_T oDynArray, void *pvSoughtElement,
					 int (*pfCompare)(const void *pvElement1, const void *pvElement2));

#endif
