#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair));

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int index = hash(key, ht->capacity);

  if (*(ht->storage + index))     // this hash bucket already has at least one LinkedPair
  {
    // Check whether key already exists in linked list; update it if so
    LinkedPair *current_node = *(ht->storage + index);
    int found = 0;
    while (current_node->next != NULL)
    {
      if (!strcmp(key, current_node->key))
      {
        current_node->value = value;
        found = 1;
        break;
      }
      else
      {
        current_node = current_node->next;
      }
    }
    // If key was not found, add it to the start of the linked list
    LinkedPair *lp = create_pair(key, value);
    lp->next = *(ht->storage + index);
    *(ht->storage + index) = lp;
  }
  else        // hash bucket is empty or NULL
  {
    LinkedPair *lp = create_pair(key, value);
    *(ht->storage + index) = lp;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  int found = 0;

  if (*(ht->storage + index))
  {
    LinkedPair *current_node = *(ht->storage + index);

    // Delete the first node(s) if they match the key
    while (current_node && !strcmp(key, current_node->key))
    {
      found = 1;
      LinkedPair *second_node = current_node->next;
      destroy_pair(current_node);
      current_node = second_node;
    }
    *(ht->storage + index) = current_node;

    // Then, delete any subsequent nodes that match the key
    while (current_node && current_node->next != NULL)
    {
      if (current_node->next && !strcmp(key, current_node->next->key))
      {
        found = 1;
        LinkedPair *next_node = current_node->next->next;
        destroy_pair(current_node->next);
        current_node->next = next_node;
        current_node = current_node->next;
      }
      else
      {
        current_node = current_node->next;
      }
    }
  }
  if (!found)
  {
    fprintf(stderr, "Key %s not found in hash table to be deleted.", key);
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{

}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht;

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
