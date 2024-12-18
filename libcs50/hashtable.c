/*
 * Colin Wolfe
 * CS50 24W
 * implementation of hashtable.h
 * more specific function details can be viewed in hashtable.h
*/
#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"
#include "hash.h"
#include "set.h"
#include <stdbool.h>

//struct for the hashtable type
typedef struct hashtable{
  int num_count;
  set_t* hasht [];
} hashtable_t;

//instantiate a new empty hashtable, return NULL if memory allocation failed
hashtable_t* hashtable_new(const int num_slots)
{  
 hashtable_t* hash = malloc(sizeof(hashtable_t) + sizeof(set_t*) * num_slots);
 hash->num_count = num_slots;
 if(hash != NULL){
    hash->num_count = num_slots; 
    for(int i = 0; i < num_slots; i++){
      hash->hasht[i] = set_new();
    }
  return hash;
 }
 else{
   return NULL;
  }
}

//insert char* key and void* item. Return false if any conditions fail
bool hashtable_insert(hashtable_t* ht, const char* key, void* item)
{
  void* location = hashtable_find(ht, key);
  if(ht != NULL && key != NULL && item != NULL && location == NULL){
    unsigned long hashLocation = hash_jenkins(key, ht->num_count);
    bool worked = set_insert(ht->hasht[hashLocation], key, item);
    return worked;
  }
  else{
    return false;
  }
}


// return void* associated with key. return null if ht does not exist or item not found
void* hashtable_find(hashtable_t* ht, const char* key)
{
  if(ht == NULL){
    return NULL;
  }
  else{
    for(int i = 0; i < ht->num_count; i++){
      void* item = set_find(ht->hasht[i], key);
      if(item != NULL){
        return item;
      }
    }
    return NULL;
  }
}


//calls itemprint on each object in hashtable + formatting, except on some edge cases
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item))
{
  if(fp == NULL){
    ;
  }
  else if(ht == NULL){
    fprintf(fp, "(null)\n");
  }
  else{
    fprintf(fp, "{");
    for(int i = 0; i < ht->num_count; i++){
      set_print(ht->hasht[i], fp, itemprint);
      fprintf(fp, ",");
    }
    fprintf(fp, "}");
  }
}


// calls a function (item func) on each item expect in some edge cases
void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) )
{
  if(ht != NULL && itemfunc != NULL){
    for(int i = 0; i < ht->num_count; i++){
      set_iterate(ht->hasht[i], arg, itemfunc);
    }
  }
}

//frees the memory and calls the itemdelete on each set
//frees ht at the end
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) )
{
  if(ht != NULL && itemdelete != NULL){
    for(int i = 0; i < ht->num_count; i++){
      set_delete(ht->hasht[i], itemdelete);
    }
    free(ht);
  }
}
