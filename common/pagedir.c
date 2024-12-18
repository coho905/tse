/*
 * Colin Wolfe
 * CS50 24W
 * This is the implementation of pagedir.h
 * See pagedir.h for more specs
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "webpage.h" 
#include "index.h"
#include "file.h"
/*
 * creates a .crawler file in pageDirectory
*/
bool pagedir_init(const char* pageDirectory)
{
  FILE* fp;
  char* literal = "/.crawler";
  char* file = malloc(strlen(pageDirectory) + strlen(literal) +1);
  strcpy(file, pageDirectory);
  strcat(file, literal);
  fp = fopen(file, "w");
  if(fp == NULL){
    free(file);
    fprintf(stderr, "Could not produce .crawler file\n");
    return false;
  }
  fprintf(fp, "This was created by the crawler\n");
  fclose(fp);
  free(file); //free all possible leaks
  return true;
}

/*
 * save the URL, Depth, HTML of the page with the given docID
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
  bool success = pagedir_init(pageDirectory);
  if(!success){
    ;
  }
  else if(page == NULL || webpage_getURL(page) == NULL || pageDirectory == NULL){
    ;
  }
  else{
    char* fileName = malloc(strlen(pageDirectory)+51);
    sprintf(fileName, "%s/%d", pageDirectory, docID); 
    FILE* fp = fopen(fileName, "w");
    if(fp == NULL){
      fprintf(stderr, "Trying to open file that cannot be accessed.\n");
      free(fileName);
      exit(6);
    }
    else{
      fprintf(fp,"%s\n", webpage_getURL(page));
      fprintf(fp,"%d\n", webpage_getDepth(page));
      fprintf(fp,"%s\n", webpage_getHTML(page));
      fclose(fp);
      free(fileName);
    }
  }
}


/*
 * validates that the directory was made by .crawler
*/
bool pagedir_valid(const char* fileName)
{
  char* crawler = malloc(strlen(fileName)+1); 
  strcpy(crawler, fileName);
  FILE* fp = fopen(crawler, "r");
  if(fp == NULL){ // cannot open
    free(crawler);
    fprintf(stderr, "Directory not created by cralwer\n");
    return false;
  }
  fclose(fp);
  free(crawler);
  return true;
}

/*
 * loads in indexFile into the index pointer given

void pagedir_load(index_t* index, const char* oldIndexFilename)
{
  char* old = malloc(strlen(oldIndexFilename)+1);
  strcpy(old, oldIndexFilename);
  
  FILE* fp = fopen(old, "r");
  if(fp != NULL){
    char* keyWord;
    while((keyWord = file_readLine(fp)) != NULL){
      char* key = malloc(strlen(keyWord)+1);
      char* temp = malloc(strlen(keyWord)+1);
      char* temp2 = malloc(strlen(keyWord)+1);
      sscanf(keyWord, "%s %[^\n]", key, temp);
      int docID = 0, count = 0, found = 0;
      while( (found = sscanf(temp, "%d %d %[^\n]", &docID, &count, temp2)) >=2){
        free(temp);
        temp = malloc(strlen(keyWord)+1);
        index_addWithCount(index, key, docID, count);
        if(found == 3){
          strcpy(temp, temp2);
        }
        free(temp2);
        temp2 = malloc(strlen(keyWord)+1);
      }
      free(keyWord);
      free(temp2);
      free(key);
      free(temp);
    }
    fclose(fp);
  }
  free(old);
  
  FILE* fp = fopen(old, "r");
  if(fp != NULL){ // can open file 
    char* keyWord;
    while((keyWord = file_readLine(fp))){ //not EOF
              
                      *       if(keyWord != NULL){
                      *               char* first;
                      *                       char* second;
                      *                               while((first = file_readWord(fp)) != NULL && (second = file_readWord(fp)) != NULL){
                      *                                         int docID = atoi(first);
                      *                                                   free(first);
                      *                                                            int count = atoi(second);
                      *                                               char* key = malloc(strlen(keyWord)+1);
      char* temp = malloc(strlen(keyWord)+1);
      char* temp2 = malloc(strlen(keyWord)+1);
      sscanf(keyWord, "%s %[^\n]", key, temp);
      int docID, count;
      while( (sscanf(temp, "%d %d %[^\n]", &docID, &count, temp2)) >=3){ //parse the string
        free(temp);
        temp = malloc(strlen(keyWord)+1);
        index_addWithCount(index, key, docID, count); //add to index
        strcpy(temp, temp2);
        free(temp      temp2 = malloc(strlen(keyWord)+1);
      }
      free(keyWord);
      free(temp2);
      index_addWithCount(index, key, docID, count); //one last time for the ones skipped
      free(key);
      free(temp);
    }
    fclose(fp); //close file
  }
  free(old);
}

*/

