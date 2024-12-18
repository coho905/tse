/*
 * colin wolfe
 * cs50 24w
 * see index.h for more implementation specs
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "counters.h"
#include "file.h"
#include "index.h"
/*
 * has a size variable
 * has a hashtable that is stored
*/
typedef struct index{
  hashtable_t* hashtable;
  int size;
} index_t;

/*
 * helper functions for deleting and printing
*/
static void printToFile(void* arg, const char* key, void* item);
static void helper(void* arg, const int key, const int count);
static void iDelete(void* item);



/*
 * creates new index and returns pointer
*/
index_t* index_new(int size)
{
  //index_t* index =  malloc(sizeof(index_t));
  hashtable_t* ht = hashtable_new(size);
  index_t* index = malloc(sizeof(index_t) + sizeof(size) + sizeof(ht));
  index->hashtable = ht;
  index->size = size;
  if(index != NULL){ //if mem is alloc
    if(index->hashtable != NULL){ //if mem is alloced
      return index;
    }
    return NULL;
  }
  else{
    return NULL;
  }
}

hashtable_t* index_getHashtable(index_t *index)
{
  return index->hashtable;
}
//increment counter not set
bool index_add(index_t* index, const char* key, int docID)
{
  void* found = hashtable_find(index->hashtable, key);
  if(found != NULL){
    bool success = counters_add(hashtable_find(index->hashtable, key), docID); //increment
    return success;
  }
  else{
    counters_t* counters = counters_new(); //create new
    bool suc1 = hashtable_insert(index->hashtable, key, counters); //add key
    if(suc1){
      bool suc2 = counters_add(hashtable_find(index->hashtable, key), docID); //added
      return suc2; //return success
    }
    return false;
  }
}

//same as index_add but sets to count instead of increasing by 1
bool index_addWithCount(index_t* index, const char* key, int docID, int count)
{
  void* found = hashtable_find(index->hashtable, key);
  if(found != NULL){
    bool success = counters_set(hashtable_find(index->hashtable, key), docID, count);
    return success;
  }
  else{
    counters_t* counters = counters_new();
    bool suc1 = hashtable_insert(index->hashtable, key, counters);
    if(suc1){
      bool suc2 = counters_set(hashtable_find(index->hashtable, key), docID, count);
      return suc2;
    }
    return false;
  }
}

//deletes the index
void index_delete(index_t* index)
{
  if(index != NULL){
    hashtable_delete(index->hashtable, iDelete); //delete the hashtable
    free(index); //free the memory
  }
}


//saves the index to a file (prints to file)
void index_save(index_t* index, const char* path)
{
  if(index != NULL){
    FILE* fp = fopen(path, "w");
    if(fp != NULL){
      hashtable_iterate(index->hashtable, fp, printToFile); //iterate with helper function
      fclose(fp);
    }
    else{
      fprintf(stderr, "Could not open file to save\n");
    }
  }
}


// helper function 1 to handle each hashtable entry
static void printToFile(void* arg, const char* key, void* item)
{
  fprintf(arg, "%s ", key);
  counters_iterate(item, arg, helper);
  fprintf(arg, "\n");
}

//helper 2 to handle each counter
static void helper(void* arg, const int key, const int count)
{
  fprintf(arg, "%d %d ", key, count);
}

//helper to delete counters
static void iDelete(void* item){
  counters_delete(item);
}

void index_load(index_t* index, const char* oldIndexFilename)
{
  char* old = malloc(strlen(oldIndexFilename) + 1);
  strcpy(old, oldIndexFilename);
  FILE* fp = fopen(old, "r");
  if (fp != NULL) {
    char* keyWord;
    while ((keyWord = file_readLine(fp))) {
      char* key = malloc(strlen(keyWord) + 1);
      char* temp = malloc(strlen(keyWord) + 1);
      char* temp2 = malloc(strlen(keyWord) + 1);
      sscanf(keyWord, "%s %[^\n]", key, temp);
      int docID, count;
      while ((sscanf(temp, "%d %d %[^\n]", &docID, &count, temp2)) >= 3) {
        free(temp);
        temp = malloc(strlen(keyWord) + 1);
        index_addWithCount(index, key, docID, count);
        strcpy(temp, temp2);
        free(temp2);
        temp2 = malloc(strlen(keyWord) + 1);
      }
      free(keyWord);
      free(temp2);
      index_addWithCount(index, key, docID, count);
      free(key);
      free(temp);
    }
    fclose(fp);
  }
  free(old);
}
