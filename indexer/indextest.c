/*
 * Colin Wolfe
 * CS50 24W
 * This takes in an index and writes it to a new file
 * Returns 0 on success, non-zero if error
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "index.h"
#include "hashtable.h"
#include "webpage.h"
#include "pagedir.h"
#include "file.h"

/*
 * This parses the arguments, returns 0 on success. 
*/
static int newParseArgs(char* oldIndexFilename, char* newIndexFilename, int argc);
/*
 * This parses the old index file, creates a new index, and then writes it to the new file
*/
static void parseOld(const char* oldIndexFilename, const char* newIndexFilename);

int main(int argc, char* argv[])
{
  int argReturn = newParseArgs(argv[1], argv[2], argc);
  if(argReturn == 0){
    char* arg1 = malloc(strlen(argv[1])+1); //makes a copy
    char* arg2 = malloc(strlen(argv[2])+1); //makes a copy
    strcpy(arg1, argv[1]); 
    strcpy(arg2, argv[2]);
    parseOld(arg1, arg2); //parses it all
    free(arg1); //free copy
    free(arg2); //free copy
  }
  return argReturn; //return exit code
}

static int newParseArgs(char* oldIndexFilename, char* newIndexFilename, int argc)
{
  if(argc != 3){ 
    fprintf(stderr, "Invalid Number of Arguments\n"); //invalid args
    return 1;
  }
  FILE* fp = fopen(oldIndexFilename, "r");
  if(fp == NULL){
    fprintf(stderr, "Invalid Old Index File\n"); //if cant open file
    return 2;
  }
  fclose(fp);
  if(oldIndexFilename == NULL || newIndexFilename == NULL){
    fprintf(stderr, "NULL parameters Not Allowed\n"); // NULL params
    return 3;
  }
  return 0;
}

static void parseOld(const char* oldIndexFilename, const char* newIndexFilename)
{
  FILE* fp = fopen(oldIndexFilename, "r");
  index_t* index = index_new(file_numLines(fp)); //new file with flexible file_numLines
  fclose(fp);
  index_load(index, oldIndexFilename);  //load in function
  index_save(index, newIndexFilename); //save to newFile
  index_delete(index); //delete file
}

