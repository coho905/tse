/*
 * Colin Wolfe
 * CS50 24W
 * This creates an index file
 * returns 0 on success
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "pagedir.h"
#include "index.h"
#include "webpage.h"
#include "hashtable.h"
#include "file.h"
#include "word.h"

/*
 * builds an in-memory index from webpage files it finds in the pageDirectory
*/
static void indexBuild (const char* pageDirectory, const char* indexFilename);
/*
 * scans a webpage document to add its words to the index
 */
static void indexPage(index_t* index, webpage_t* page, int docID);
/*
 * makes sure there are valid arguments
*/
static int parseArgs(const char* pageDirectory, const char* indexFilename, int args);

int main(int argc, char* argv [])
{
  int returnValue = parseArgs(argv[1], argv[2], argc); //return Value
  if(returnValue == 0){
    char* pageDirectory = malloc(strlen(argv[1])+1);
    char* fileName = malloc(strlen(argv[2])+1);
    strcpy(pageDirectory, argv[1]); //makes copies
    strcpy(fileName, argv[2]);
    indexBuild(pageDirectory, fileName); //builds the index + saves it
    free(pageDirectory);
    free(fileName);
    //FREES
  }
  return returnValue; //returned
  
}

static void indexBuild(const char* pageDirectory, const char* indexFilename)
{
  index_t* index = index_new(900); //900 sized hashtable
  int ID = 1;
  char* path = malloc(strlen(pageDirectory) + 50);
  sprintf(path, "%s/%d", pageDirectory, ID); //builds path
  FILE* fp = fopen(path, "r");
  while(fp != NULL){
    char* url = file_readLine(fp); //reads line
    char* depth = file_readLine(fp);
    char* html = file_readFile(fp);
    int realDepth = atoi(depth); //get depth
    webpage_t* webpage = webpage_new(url, realDepth, html);
    if(webpage != NULL){
      indexPage(index, webpage, ID); //pages the webpage
    }
    free(depth);
    ID +=1;
    free(path);
    path = malloc(strlen(pageDirectory) + 50);
    sprintf(path, "%s/%d", pageDirectory, ID);
    fclose(fp);
    fp = fopen(path, "r");
    webpage_delete(webpage);
  }
  index_save(index, indexFilename); //saves index to fileName
  free(path);
  index_delete(index); //deletes the index
}

static void indexPage(index_t* index, webpage_t* page, int docID)
{
  if(page != NULL){
    int count = 0;
    int* pos = &count;
    char* word;
    while((word = webpage_getNextWord(page, pos)) != NULL) { //while there are still words in the page
      
      //char wordT [50];
      //strncpy(wordT, word, 49);
      if(strlen(word) >3){ //size greater than 3
        //for (int i = 0; i < strlen(wordT); i++){
          //(wordT)[i] = tolower((wordT)[i]);
        //}
        normalizeWord(word); //normalize Word
        //char* hi = malloc(50);
        //strcpy(hi, wordT);
        index_add(index, word, docID); //adds word to index
        //free(hi);
      }
 
      free(word); //free word
    }
    /*
    char* word = webpage_getNextWord(page, pointer);
    while(word != NULL){
      char wordT [50];
      strncpy(wordT, word, 49);
      if(strlen(wordT) >3){
        for (int i = 0; i < strlen(wordT); i++){
          (wordT)[i] = tolower((wordT)[i]);
        }
        char* hi = malloc(50);
        strcpy(hi, wordT);
        printf("%s\n", hi);
        index_add(index, hi, docID);
        free(hi);
      }
      (*pointer)+=1;
      free(word);
      word = webpage_getNextWord(page, pointer);
    }
    */
  }
}

static int parseArgs(const char* pageDirectory, const char* indexFilename, int args)
{
  if(args != 3){
    fprintf(stderr, "Invalid Number of Arguments\n");
    return 1;
  }
  if(pageDirectory == NULL || indexFilename == NULL){
    fprintf(stderr, "Null Parameters\n");
    return 2;
  }
  char* crawler = malloc(strlen(pageDirectory) + 1 + strlen("/.crawler"));
  strcpy(crawler, pageDirectory);
  strcat(crawler, "/.crawler");
  bool success = pagedir_valid(crawler);
  free(crawler);
  if(!success){
    return 3;
  }
  return 0;
}
