/* 20190533 Hyemin Lee Assignment3 customer_manager1.c */
#define _GNU_SOURCE
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define UNIT_ARRAY_SIZE 64

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};


/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  DB_T d;

  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 64 elements
  d->numItems = 0;
  d->pArray = (struct UserInfo *)calloc(d->curArrSize,sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);
    free(d);
    return NULL;
  }
  return d;
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  assert(d);
  if(d==NULL) return;
  for(int i=0;i<d->numItems;i++){
    free(d->pArray[i].name);
    free(d->pArray[i].id);
  }
  free(d->pArray);
  free(d);
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  if(d==NULL || id==NULL || name==NULL || purchase<=0){
    return -1;
  }
  // Check whether such id or name of customer already exists
  for(int i=0;i<d->numItems; i++){
    if(strcmp(d->pArray[i].name, name)==0) return -1;
    if(strcmp(d->pArray[i].id, id)==0) return -1;
  }
  // Have to resize the array if array is full
  if(d->numItems==d->curArrSize){
    d->curArrSize+=UNIT_ARRAY_SIZE;
    d->pArray = (struct UserInfo *)realloc(d->pArray, d->curArrSize * sizeof(struct UserInfo));
    if (d->pArray == NULL) {
      fprintf(stderr, "Can't allocate a memory for array of size %d\n",d->curArrSize);
      return -1;
    }
  }
  // Register new customer
  d->pArray[d->numItems].name=strdup(name);
  if (d->pArray[d->numItems].name == NULL) {
    fprintf(stderr, "Can't allocate a memory for name of length %zu\n", strlen(name));
    return -1;
  }
  d->pArray[d->numItems].id=strdup(id);
  if (d->pArray[d->numItems].id == NULL) {
    fprintf(stderr, "Can't allocate a memory for id of length %zu\n", strlen(id));
    return -1;
  }
  d->pArray[d->numItems].purchase=purchase;
  d->numItems++;
  return 0;
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  if(d==NULL || id==NULL){
    return -1;
  }
  for(int i=0;i<d->numItems; i++){
    // Found such an id of customer
    if(strcmp(d->pArray[i].id, id)==0){
      free(d->pArray[i].name);
      free(d->pArray[i].id);
      d->pArray[i].purchase=0;
      d->numItems--;
      if (i!=d->numItems){
        d->pArray[i] = d->pArray[d->numItems];
      }
      return 0;
    }
  }
  // There's no an such id of customer
  return -1;
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  if(d==NULL || name==NULL){
    return -1;
  }
  for(int i=0;i<d->numItems; i++){
    // Found such a name of customer
    if(strcmp(d->pArray[i].name, name)==0){
      free(d->pArray[i].name);
      free(d->pArray[i].id);
      d->pArray[i].purchase=0;
      d->numItems--;
      if (i!=d->numItems){
        d->pArray[i] = d->pArray[d->numItems];
      }
      return 0;
    }
  }
  // There's no an such name of customer
  return -1;
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  if(d==NULL || id==NULL){
    return -1;
  }
  for(int i=0;i<d->numItems; i++){
    // Found such an id of customer
    if(strcmp(d->pArray[i].id, id)==0){
      return d->pArray[i].purchase;
    }
  }
  // There's no an such id of customer
  return -1;
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  if(d==NULL || name==NULL){
    return -1;
  }
  for(int i=0;i<d->numItems; i++){
    // Found such an name of customer
    if(strcmp(d->pArray[i].name, name)==0){
      return d->pArray[i].purchase;
    }
  }
  // There's no an such name of customer
  return -1;
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  if(d==NULL || fp==NULL){
    return -1;
  }
  int sum=0;
  for(int i=0;i<d->numItems; i++){
    sum+=fp(d->pArray[i].id, d->pArray[i].name, d->pArray[i].purchase);
  }
  return sum;
}
