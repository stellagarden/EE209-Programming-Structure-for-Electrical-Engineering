/* 20190533 Hyemin Lee Assignment3 customer_manager2.c */
#define _GNU_SOURCE
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define MAX_BUCKET_SIZE 1048576
#define RESIZING_RATIO 0.75
enum {HASH_MULTIPLIER = 65599};
int UNIT_BUCKET_SIZE = 1024;

struct Node {
  char *idKey;
  char *nameKey;
  int idHash;
  int nameHash;
  int purchase;
  int resortChecked;
  struct Node *idNext;
  struct Node *nameNext;
};

struct DB {
  struct Node **idArray;   // pointer to the id array
  struct Node **nameArray;   // pointer to the name array
  int curArrSize;
  int numItems;
};


/*--------------------------------------------------------------------*/
static int hash_function(const char *pcKey, int iBucketCount)

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)(uiHash & ((unsigned int)iBucketCount-1U));
}
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
  d->curArrSize = UNIT_BUCKET_SIZE; // start with 1024 elements
  d->numItems = 0;
  d->idArray = (struct Node **)calloc(d->curArrSize,sizeof(struct Node));
  if (d->idArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for id array of size %d\n",
	    d->curArrSize);
    free(d);
    return NULL;
  }
  d->nameArray = (struct Node **)calloc(d->curArrSize,sizeof(struct Node));
  if (d->nameArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for name array of size %d\n",
	    d->curArrSize);
    free(d->idArray);
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
  struct Node *p;
  struct Node *nextp;

  for(int i=0;i<d->curArrSize;i++){
    for(p=d->idArray[i];p!=NULL;p=nextp){
        nextp=p->idNext;
        free(p->idKey);
        free(p->nameKey);
        free(p);
    }
  }
  free(d->idArray);
  free(d->nameArray);
}
/*--------------------------------------------------------------------*/
DB_T
ResizeD(DB_T preD){
  UNIT_BUCKET_SIZE*=2;
  DB_T newD;
  struct Node *p;
  if((newD = CreateCustomerDB()) == NULL) return NULL;

  for(int i=0;i<preD->curArrSize;i++){
    for(p=preD->idArray[i];p!=NULL;p=p->idNext){
      RegisterCustomer(newD, p->idKey, p->nameKey, p->purchase);
    }
  }
  DestroyCustomerDB(preD);
  *preD=*newD;
  return preD;
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  if(d==NULL || id==NULL || name==NULL || purchase<=0) return -1;
  struct Node *p;

  // Have to resize the array if array is almost(75%) full
  if((d->numItems >= (int)(RESIZING_RATIO * d->curArrSize)) && (d->curArrSize < MAX_BUCKET_SIZE)){
    if((d=ResizeD(d))==NULL) return -1;
  }

  int idHash = hash_function(id, d->curArrSize);
  int nameHash = hash_function(name, d->curArrSize);
  
  // Check whether such id or name of customer already exists
  for(p=d->idArray[idHash];p!=NULL;p=p->idNext){
    if(strcmp(p->idKey, id)==0) return -1;
  }
  for(p=d->nameArray[nameHash];p!=NULL;p=p->nameNext){
    if(strcmp(p->nameKey, name)==0) return -1;
  }

  // Register new customer
  if ((p=(struct Node*)calloc(1, sizeof(struct Node))) == NULL) return -1;
  if ((p->idKey=strdup(id)) == NULL) {
    fprintf(stderr, "Can't allocate a memory for id of length %zu\n", strlen(id));
    return -1;
  }
  if ((p->nameKey=strdup(name)) == NULL) {
    fprintf(stderr, "Can't allocate a memory for name of length %zu\n", strlen(name));
    return -1;
  }
  p->purchase=purchase;
  p->idHash=idHash;
  p->nameHash=nameHash;
  p->idNext=d->idArray[idHash];
  p->nameNext=d->nameArray[nameHash];
  d->idArray[idHash]=p;
  d->nameArray[nameHash]=p;
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
  int idHash = hash_function(id, d->curArrSize);
  // Handle the case when the first node of array has the same key
  if(d->idArray[idHash] && strcmp(d->idArray[idHash]->idKey, id)==0){
    struct Node *q = d->idArray[idHash];
     d->idArray[idHash]= (d->idArray[idHash]->idNext) ? d->idArray[idHash]->idNext : NULL;
    if (q->purchase==-1){
      free(q->idKey);
      free(q->nameKey);
      free(q);
      d->numItems--;
      return 0;
    } else {
      q->purchase=-1;
      int ass = UnregisterCustomerByName(d, q->nameKey);
      assert(ass==0);
    }
    return 0;
  }
  for(struct Node *p=d->idArray[idHash]; p!=NULL;){
    if (p->idNext && strcmp(p->idNext->idKey, id)==0){
      struct Node *q = p->idNext;
      p->idNext= (q->idNext) ? q->idNext : NULL;
      if (q->purchase==-1){
        free(q->idKey);
        free(q->nameKey);
        free(q);
        d->numItems--;
      } else {
        q->purchase=-1;
        int ass = UnregisterCustomerByName(d, q->nameKey);
        assert(ass==0);
      }
      return 0;

    } else {
      p=p->idNext;
    }
  }
  // There's no an such id of customer
  return -1;
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  while(d==NULL || name==NULL){
    return -1;
  }
  int nameHash = hash_function(name, d->curArrSize);
  // Handle the case when the first node of array has the same key
  if(d->nameArray[nameHash] && strcmp(d->nameArray[nameHash]->nameKey, name)==0){
    struct Node *q = d->nameArray[nameHash];
    if (d->nameArray[nameHash]->nameNext) d->nameArray[nameHash]=d->nameArray[nameHash]->nameNext;
    else d->nameArray[nameHash]=NULL;
    if (q->purchase==-1){
      free(q->idKey);
      free(q->nameKey);
      free(q);
      d->numItems--;
    } else {
      q->purchase=-1;
      int ass = UnregisterCustomerByID(d, q->idKey);
      assert(ass==0);
    }
    return 0;
  }
  for(struct Node *p=d->nameArray[nameHash]; p!=NULL;){
    if (p->nameNext && strcmp(p->nameNext->nameKey, name)==0){
      struct Node *q = p->nameNext;
      p->nameNext= (q->nameNext) ? q->nameNext : NULL;
      if (q->purchase==-1){
        free(q->idKey);
        free(q->nameKey);
        free(q);
        d->numItems--;
      } else {
        q->purchase=-1;
        int ass = UnregisterCustomerByID(d, q->idKey);
        assert(ass==0);
      }
      return 0;

    } else {
      p=p->nameNext;
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
  int idHash = hash_function(id, d->curArrSize);
  for(struct Node *p=d->idArray[idHash]; p!=NULL;p=p->idNext){
    if(strcmp(p->idKey, id)==0) return p->purchase;
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
  int nameHash = hash_function(name, d->curArrSize);
  for(struct Node *p=d->nameArray[nameHash]; p!=NULL;p=p->nameNext){
    if(strcmp(p->nameKey, name)==0) return p->purchase;
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
  for(int i=0;i<d->curArrSize;i++){
    for(struct Node *p=d->idArray[i];p!=NULL;p=p->idNext){
      sum+=fp(p->idKey, p->nameKey, p->purchase);
    }
  }
  return sum;
}
