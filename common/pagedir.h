#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"
#include "index.h"

/*
 * Parameters: Name of pre-existing directory
 * construct the pathname for the .crawler file in that directory
 * open the file for writing; on error, return false.
 * close the file and return true.
*/
bool pagedir_init(const char* pageDirectory);

/*
 * construct the pathname for the page file in pageDirectory
 * open that file for writing
 * print the URL
 * print the depth
 * print the contents of the webpage
 * close the file
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);


/*
 * check if directory was made by crawler
*/
bool pagedir_valid(const char* fileName);

/*
 * load in index from indexFile
 * closes all possible memory leaks
 * takes in index pointer and fileName
*/
//viid pagedir_load(index_t* index, const char* oldIndexFilename);
//implemented in index now^
#endif //__PAGEDIR_H
