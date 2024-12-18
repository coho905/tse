/*
 * colin wolfe
 * cs50
*/
#ifndef __INDEX_H
#define __INDEX_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "counters.h"
#include "hashtable.h"
#include "file.h"


typedef struct index index_t;
hashtable_t* index_getHashtable(index_t* index);

/*defines the structure
*/
/*
 * mallocs memory for new index
 * returns pointer to that memory
*/
index_t* index_new(int size); 
/*
 * increments counter for docID at key by 1
 * intilizes key/counter if necessary
 * returns true on success
*/
bool index_add(index_t* index, const char* key, int docID);  

/*
 * saves index to new file
 * file at path
 * returns nothing
*/
void index_save(index_t* index, const char* path); 

/*
 * loads index from file
 * returns true on success
 * Not implemented
 * Use pagedir_load
*/
void index_load(index_t* index, const char* oldIndexFilename); 

/*
 * deletes index & associated pointer
*/
void index_delete(index_t* index);

/* 
 * does the same thing as index_add but allows the option to set a count as well
*/
bool index_addWithCount(index_t* index, const char* key, int docID, int count);  



#endif
