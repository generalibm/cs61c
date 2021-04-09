#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  newTable->size = size;
  newTable->data = malloc(sizeof(struct HashBucket *) * size);
  for (i = 0; i < size; i++) {
    newTable->data[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/*
 * This inserts a key/data pair into a hash table.  To use this
 * to store strings, simply cast the char * to a void * (e.g., to store
 * the string referred to by the declaration char *string, you would
 * call insertData(someHashTable, (void *) string, (void *) string).
 */
void insertData(HashTable *table, void *key, void *data) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  // 2. Allocate a new hash bucket struct.
  // 3. Append to the linked list or create it if it does not yet exist. 
  
  const char * k = (const char*)key;
  const char * v = (const char*)data;
  const int k_len = strlen(k);
  const int v_len = strlen(v);

  struct HashBucket * newBucket = (struct HashBucket *)malloc(sizeof(struct HashBucket));
  newBucket->key = calloc(k_len, sizeof(char));
  newBucket->data = calloc(v_len, sizeof(char));
  strncpy(newBucket->key, k, k_len);
  strncpy(newBucket->data, v, v_len);
  newBucket->next = NULL;

  unsigned int hashBucketIndex = table->hashFunction(key);
  hashBucketIndex %= table->size;

  if (!table->data[hashBucketIndex])
  {
    table->data[hashBucketIndex] = newBucket;
  }
  else
  {
    struct HashBucket **lst = &table->data[hashBucketIndex];
    newBucket->next = *lst;
    *lst = newBucket;
  }
}

/*
 * This returns the corresponding data for a given key.
 * It returns NULL if the key is not found. 
 */
void *findData(HashTable *table, void *key) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  // 2. Walk the linked list and check for equality with table->equalFunction.
  unsigned int hashBucketIndex = table->hashFunction(key);
  hashBucketIndex %= table->size;
  struct HashBucket *listHead = table->data[hashBucketIndex];
  
  for (struct HashBucket * x = listHead; x != NULL; x = x->next)
  {
    int equalRet = table->equalFunction(x->key, key);
    
    // TODO : defination of ret of equal
    if (0 != equalRet) return x->data;
  }
  
   return NULL;
}
