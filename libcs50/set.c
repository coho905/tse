/*
 * Colin Wolfe
 * CS50 24W
 * implementation of set.h
 * more information found in set.h
*/
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "set.h"

//node defintion for linked list implementation
typedef struct setnode {
  void* item;
  char* key;
  struct setnode* next;
} setnode_t;

//struct for set type
typedef struct set {
  struct setnode* head;
} set_t;

//not visible outside of module. creates new setnode
static setnode_t* setnode_new(const char* key, void* item);
 
//returns instantiated poitner to set_t, null if not enough memory
set_t* set_new(void)
{
  set_t* set = malloc(sizeof(set_t));
  if(set == NULL){
    return NULL;
  }
  else{
    set->head = NULL;
    return set;
  }
}

//find key in set, return pointer to item. null if it doesnt exist
void* set_find(set_t* set, const char* key)
{
  if(set != NULL && key != NULL){
    for(setnode_t* setNode = set->head; setNode != NULL; setNode = setNode->next){
      if(strcmp(setNode->key, key) == 0){
        return setNode->item;
      }
    }
    return NULL;
  }
  else{
    return NULL;
  }
}

//returns true you can insert it properly, null if not
bool set_insert(set_t* set, const char* key, void* item)
{
  void* tempItem = set_find(set, key);
  if(set != NULL && key != NULL && tempItem == NULL && item != NULL){
    setnode_t* newNode = setnode_new(key, item);
    if(newNode != NULL){
      newNode->next = set->head;
      set->head = newNode;
      return true;
    }
    else{
      return false;
    }
  }
  else{
    return false;
  }
}


//prints out an entire set. calls itemprint on each item. other formats if applicable in edge cases
void set_print(set_t* set, FILE* fp, 
    void(*itemprint)(FILE* fp, const char* key, void* item))
{
  if(fp == NULL){
    ;
  }
  else if(set == NULL){
    fprintf(fp, "NULL set");
  }
  else if(itemprint == NULL){
    fprintf(fp, "{, }");
  }
  else{
    fprintf(fp, "{");
    for(setnode_t* setNode = set->head; setNode != NULL; setNode = setNode->next){
      (*itemprint)(fp, setNode->key, setNode->item);
    }
    fprintf(fp, "}");
  }
}


//iterates itemfunc on each item and key in the set
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void*item))
{
  if(set != NULL && itemfunc != NULL){
    for(setnode_t* setNode = set->head; setNode != NULL; setNode = setNode->next){
      (*itemfunc)(arg, setNode->key, setNode->item);
    }
  }
} 

//deletes the set and frees the memory via itemdelete
void set_delete(set_t* set, void (*itemdelete)(void* item))
{
  if(set != NULL){
    setnode_t* setNode = set->head;
    while(setNode != NULL){     
      if(itemdelete!=NULL){
        (*itemdelete)((void*) setNode->key);
      }
      setnode_t* next = setNode->next;
      free(setNode);
      setNode = next;
    }
    free(set);
  }
}


//creates new setnode pointing to null. not accesible to outside the module.
static setnode_t* setnode_new(const char* key, void* item)
{
  setnode_t* newNode = malloc(sizeof(setnode_t));
  if(newNode == NULL){
    return NULL;
  }
  else{
    newNode->item = item;
    newNode->key = malloc(strlen(key)+1);
    strcpy(newNode->key, key);
    newNode->next = NULL;
    return newNode;
  }
}

