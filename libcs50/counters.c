/*
 * Colin Wolfe
 * CS50 24W
 * implementation of counters.h
 * see counters.h for even more details
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "counters.h"

//node structure
typedef struct cNode {
  int key;
  int counter;
  struct cNode* next;
} cNode_t;

//head of linked list
typedef struct counters{
  struct cNode* head;  
}counters_t;

//invisible to outside of module, make new node
static cNode_t* counternode_new(const int key, const int value);
 
// return poitner to new counter, null if no memory
counters_t* counters_new(void)
{
  counters_t* counter = malloc(sizeof(counters_t));
  if(counter == NULL){
    return NULL;
  }
  else{
    counter->head = NULL;
    return counter;
  }
}

//if the key exists return the pointer to it, NULL if it doesnt exist. not visible outside of module
static cNode_t* found_key(counters_t* counter, const int key)
{
  if(counter!= NULL && key>= 0){
    for(cNode_t* node = counter->head; node != NULL; node = node->next){
      if(node->key == key){
        return node;
      }
    }
    return NULL;
  }
  return NULL;
}

// adds key if key is not already there returns one. if key is there, add one to count and return that. returns 0 if not valid key
int counters_add(counters_t* ctrs, const int key)
{
  if(ctrs != NULL && key >=0){
    cNode_t* location = found_key(ctrs, key);
    if(location == NULL){
      cNode_t* node = counternode_new(key, 1);
      node->next = ctrs->head;
      ctrs->head = node;
      return 1;
    }
    else{
      location->counter = location->counter + 1;
      return location->counter;
    }
  }
  else{
    return 0;
  }    
}

//returns count for given key, null if not found
int counters_get(counters_t* ctrs, const int key)
{
  if(ctrs != NULL && key>=0){
    cNode_t* location = found_key(ctrs, key);
    if( location != NULL){
      return location->counter;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
//sets count to a certain given value for a given key; returns true. returns false if invalid key, count, or counter pointer
bool counters_set(counters_t* ctrs, const int key, const int count)
{
  if(ctrs != NULL && key >= 0 && count >= 0){
    cNode_t* location = found_key(ctrs, key);
    if(location == NULL){
      cNode_t* node = counternode_new(key, count);
      node->next = ctrs->head;
      ctrs->head = node;
    }
    else{
      location->counter = count;
    }
    return true;
  }
  else{
    return false;
  } 
}

//prints appropriate format for each given possibilty
void counters_print(counters_t* ctrs, FILE* fp)
{
  if(fp == NULL){
    ;
  }
  else{
    if(ctrs == NULL){
      fprintf(fp, "(null)\n");
    }
    else{
      fprintf(fp, "{");
      for(cNode_t* node = ctrs->head; node != NULL; node = node->next){
        fprintf(fp, "%d=%d, ", node->key, node->counter);
      }
      fprintf(fp, "}\n");
    }
  } 
}

//iterates itemfunc over each pairing in set; returns nothing
void counters_iterate(counters_t* ctrs, void* arg,
    void (*itemfunc)(void* arg, const int key, const int count))
{
  if(ctrs != NULL && itemfunc != NULL){
    for(cNode_t* node = ctrs->head; node != NULL; node = node->next){
      (*itemfunc)(arg, node->key, node->counter);
    }
  }
}
//frees memory of given pointer if pointer is not null/
void counters_delete(counters_t* ctrs)
{
  if(ctrs != NULL){
    cNode_t* node = ctrs->head;
    while(node != NULL){
      cNode_t* next = node->next;
      free(node);
      node = next;
    }
    free(ctrs);
  }
}

// not visible outside module, makes new node pointing to nothing
static cNode_t* counternode_new(const int key, const int value)
{
  cNode_t* node = malloc(sizeof(cNode_t));
  if(node == NULL){
    return NULL;
  }
  else{
    node->key = key;
    node->counter = value;
    node->next = NULL;
    return node;
  }
}

