/*
 * Colin Wolfe
 * CS50
 * Querier
 * See IMPLEMENTATION.md for specifics
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>  // add this to your list of includes
#include "counters.h"
#include "mem.h"
#include "hashtable.h"
#include "index.h"
#include "word.h"
#include "file.h"

//declaring but implemented in stdlib.h
int fileno(FILE *stream);

/*
 * returns the counter associated with the given query
*/
counters_t* getScore(char* words [], int max, index_t* index);

/*
 * parses the sentence for individual words and puts them in array
*/
void parser(char* string, char* array []);

/*
 * calculates the number of words in a given sentence
*/
int numWords(const char* sentence);

/*
 * helps with intersection
*/
void intersect_helper(void *arg, int key, int count);

/*
 * helps with printing
 * formatting the output
*/
void print_help(char* arg, int key, int count);

/*
 * prints the counter out in order
*/
void sort(counters_t* counter, char* arg2);

/* 
 * helper function for sort; stores largest item in a counter
*/
void find_biggest(void* item, int key, int count);

/*
 * gets URL for a docID
*/
char* getURL(char* path, int docID);

/*
 * checks URL for format, returns true if successful
*/
bool checkArray(char* array [], int max);

/* 
 * returns a copy of a given counter
*/
counters_t* counters_copy(counters_t* source);

/*
 * sets individual items into copy
*/
void copy_help(void* arg, const int key, const int count);

/*
 * makes sure all given files can be read
*/
bool parseArgs(char* arg1, char* arg2, int argc);

/*
 * makes sure string only contains spaces and characters
*/
bool checkString(char* string);

/*
 * returns size of counter
*/
int matches (counters_t* counter);

/* 
 * helper function to increase the size ^
*/
void find_total(void* arg, int key, int count);

/*
 * struct to hold current max key & count
*/
typedef struct max{
  int key;
  int count;
} max_t;

/* 
 * function to return the minmium of two integers
*/
int min(int a, int b){
  if(a<=b){
    return a;
  }
  return b;
}

//given by Palmer
static void prompt(void);

/*
 * calls the union iterative over a counter
*/
void counters_union(counters_t* counter, counters_t* counter2);

/* 
 * unions one element at a time
*/
void union_helper(void* arg, int key, int count);

/*
 * struct to hold the result and the other
*/
typedef struct twocount {
    counters_t* result;
    counters_t* other;
} twocount_t;

/*
 * calls intersect helper iterative
*/
void counters_intersect(counters_t* counter, counters_t* counter2);

int main(int argc, char* argv [])
{
  if(parseArgs(argv[2], argv[1], argc)){
    prompt();
    char* response = file_readLine(stdin);
    //if(checkString(response)){
      char* file = malloc(strlen(argv[2])+1);
      strcpy(file, argv[2]);
      FILE* fp = fopen(file, "r");
      index_t* index = index_new(file_numLines(fp));
      fclose(fp);
      char* copy = malloc(strlen(argv[1])+1);
      strcpy(copy, argv[1]);
      index_load(index, file);
      free(file);
      while(response != NULL){
        while(!checkString(response)){
            printf("Invalid Characters Found\n-----------------------------------------------\n");
            free(response);
            /*
            for(int i = 0; i < words; i++){
              if(test[i] != NULL){
                free(test[i]);
              }
            }
            */
            prompt();
            response = file_readLine(stdin);
            if(response == NULL){
              index_delete(index);
              free(copy);
              exit(5);
            }
        } 
        
        int words= numWords(response);
                  //printf("\n");
        if(words> 0){
          char* test [words];
          parser(response, test);
          for(int i = 0; i < words; i++){
            if(test[i] != NULL){
              normalizeWord(test[i]);
            }
            //printf("%s ", test[i]);
          }
          if(checkArray(test, words)){
            counters_t* counter = getScore(test, words, index);
            int number = matches(counter);
            printf("Query: ");
            for(int i = 0; i < words; i++){
              printf("%s ", test[i]);
            }
            printf("\n"); 
            if(number >0){
              printf("Matches: %d (ranked)\n", number);
              sort(counter, copy);
            }
            else{
              printf("No documents match.\n-----------------------------------------------\n");
            }
            /*
            for(int i = 0; i < words; i++){
              free(test[i]);
            }
            */
            free(response);
            counters_delete(counter);
            prompt();
            response = file_readLine(stdin); 
            }
          else{
            /*
            for(int i = 0; i < words; i++){
              free(test[i]);
            }
            */
            printf("Invalid Query\n-----------------------------------------------\n");
            free(response);
            prompt();
            response = file_readLine(stdin);
          //exit(3);
          }
          for(int i =0; i <words; i++){
            free(test[i]);
          }
        }
        else{
          printf("-----------------------------------------------\n"); 
          free(response);
          prompt();
          response = file_readLine(stdin);
        }
      }
      index_delete(index);
      free(copy);
      if(response != NULL){
        free(response);
      }
    //}
    //else{
      //free(response);
      //prompt();
      //response = file_readLine(stdin);
    //}
  }
  else{
    fprintf(stderr, "invalid parameters\n");
    exit(1);
  }
} 


void parser(char* str, char* array [])
{
  //char* array [] = malloc(sizeof(char*));
  //if(!(max == 0)){
    int tracker = 0;
    char cToStr[2];
    cToStr[1] = '\0';
    bool last = false;
    char* temp = malloc(sizeof(char)*100+1);
    strcpy(temp, "\0");
    for(int i = 0; i <= strlen(str); i++){
      if(!isspace(str[i])){
        cToStr[0] = str[i];
        if(strcmp(temp, "\0") == 0){
          strcpy(temp, cToStr);
        }
        else{
          strcat(temp, cToStr);
        }
        last = true;
      }
      else if((isspace(str[i])) && temp != NULL){
        if(last){
          array[tracker] = malloc(sizeof(char)*100+1);
          strcpy(array[tracker], temp);
          tracker +=1;
          free(temp);
          temp = malloc(sizeof(char)*100+1);
          strcpy(temp, "\0");
          last = false;
        }
      }
    }
    if(temp != NULL){
      //free(array[tracker]);
      array[tracker] = malloc(strlen(temp) + 1);
      strcpy(array[tracker], temp);
      free(temp);
    }
  //} 
}
int numWords(const char *sentence)
{
  int count=0;
  char lchar;
  int length = strlen(sentence);
  if(length == 0){
    return 0;
  }
  if(length > 0){
    lchar = sentence[0];
  }
  for(int i = 0; i <= length; i++){
    if((isspace(sentence[i]) || sentence[i]=='\0') && !isspace(lchar)){
      count+=1;
    }
    lchar = sentence[i];
  }
  return count;
}


counters_t* getScore(char* words[], int max, index_t* index) {
  counters_t* result = counters_new();
  counters_t* temp = NULL;
  //counters_t* curr = counters_new();
  for (int i = 0; i < max; i++) {
    if (strcmp(words[i], "and") != 0 && strcmp(words[i], "or") != 0) {
      counters_t* curr = hashtable_find(index_getHashtable(index), words[i]);
      if (temp == NULL) {
        if(curr != NULL){
          temp = counters_copy(curr);
        }
        else{
          temp = counters_new();
        }
        //if(curr != NULL){
          //counters_delete(curr);
        //}
      } 
      else {
        counters_t* tempCopy = counters_copy(temp);
        if(curr != NULL){
          counters_intersect(tempCopy, curr);
          counters_delete(temp);
          temp = counters_copy(tempCopy);
          counters_delete(tempCopy);
          tempCopy = NULL;
        }
        else{
          curr = counters_new();
          counters_intersect(tempCopy, curr);
          counters_delete(temp);
          counters_delete(curr);
          temp = counters_copy(tempCopy);
          counters_delete(tempCopy);
          tempCopy = NULL;
        }
      }
      //if(curr != NULL && i == max-1){
        //counters_delete(curr);
      //}
    } 
    else if (strcmp(words[i], "or") == 0) {
      counters_union(result, temp);
      counters_delete(temp);
      temp = NULL;
    } 
    else if (strcmp(words[i], "and") == 0) {
      continue;
    }

  }
  //if(curr != NULL){
    //counters_delete(curr);
  //}
  counters_union(result, temp);
  counters_delete(temp);
  return result;
}


void counters_intersect(counters_t* counter, counters_t* counter2)
{
    twocount_t args = {counter, counter2}; 
    counters_iterate(counter, &args, intersect_helper);
}

void intersect_helper(void *arg, int key, int count)
{
    twocount_t* newer = arg;
    counters_set(newer->result, key, min(count, counters_get(newer->other, key)));
    //free(two);
}

void counters_union(counters_t* counter, counters_t* counter2){
  twocount_t args = {counter, counter2};
  counters_iterate(counter2, &args, union_helper);
}
void union_helper(void* arg, int key, int count){
  twocount_t* newer = arg;
  //printf("Keys: %d, Scores: %d %d\n", key, counters_get(two->result, key), count);
  counters_set(newer->result, key, count + counters_get(newer->result, key));     // Update result with the addition operation
}

void print_help(char* arg, int key, int count){
  if(count != 0){
    printf("Score\t%d Doc:\t%d\t%s\n", count, key, arg);
  }
}

void sort(counters_t* counter, char* arg2){
  max_t max = {10, 10};
  while(max.count > 0){
    max.count = 0;
    max.key = 0;
    counters_iterate(counter, &max, find_biggest);
    if(max.key != 0){
      counters_set(counter, max.key, 0);
      char* path = malloc(strlen(arg2)+1);
      strcpy(path, arg2);
      char* url = getURL(path, max.key);
      print_help(url, max.key, max.count);
      free(url);
      free(path);
    }
  }
  printf("-----------------------------------------------\n");
}

void find_biggest(void* item, int key, int count){
  max_t* max = item;
  if(count > max->count){
    (*max).key = key;
    (*max).count = count;
    key = 0;
    count = 0;
  }
}

char* getURL(char* path, int docID){
  char* urlPATH = malloc(strlen(path)+100);
  sprintf(urlPATH, "%s/%d", path, docID);
  FILE* fp = fopen(urlPATH, "r");
  char* url = file_readWord(fp);
  fclose(fp);
  free(urlPATH);
  return url;
}
static void
prompt(void)
{
  //print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

counters_t* counters_copy(counters_t* counter)
{
  counters_t* copy = counters_new();
  counters_iterate(counter, copy, copy_help);
  return copy;
}

void copy_help(void* arg, int key, int count)
{
  counters_t* copy = (counters_t*) arg;
  counters_set(copy, key, count);
}

bool parseArgs(char* arg1, char* arg2, int argc)
{
  if(argc >=2){
    char* crawl = malloc(strlen(arg2)+strlen("/.crawler")+1);
    sprintf(crawl, "%s/.crawler", arg2);
    FILE* fp2 = fopen(crawl, "r");
    free(crawl);
    FILE* fp = fopen(arg1, "r");
    if(fp != NULL && fp2 != NULL){
      fclose(fp);
      fclose(fp2);
      return true;
    }
    if(fp != NULL){
      fclose(fp);
    }
    if(fp2 != NULL){
      fclose(fp2);
    }
    return false;
  }
  else{
    return false;
  }
}

bool checkString(char* string)
{
  for(int i = 0; i < strlen(string); i++){
    if(!isalpha(string[i]) && !isspace(string[i])){
      return false;
    }
  }
  return true;
}

bool checkArray(char* array [], int max)
{
  if(strcmp(array[0], "and") == 0 || strcmp(array[0], "or") == 0 || strcmp(array[max-1], "and") == 0 || strcmp(array[max-1], "or") ==0){
    printf("Conjuction cannot be at beginning or end\n");
    return false;
  }
  for(int i = 1; i < max -1; i++){
    if(strcmp(array[i], "and") == 0){
      if(strcmp(array[i-1], "and") == 0 || strcmp(array[i-1], "or") == 0 || strcmp(array[i+1], "and") == 0 || strcmp(array[i+1], "or") == 0){
        printf("Conjuctions cannot be back to back\n");
        return false;  
      }     
    }
    else if(strcmp(array[i], "or") == 0){
      if(strcmp(array[i-1], "and") == 0 || strcmp(array[i-1], "or") == 0 || strcmp(array[i+1], "and") == 0 || strcmp(array[i+1], "or") == 0){
        return false;  
        printf("Conjuctions cannot go back to back\n");
      }    
    }  
  }
  return true;
}

int matches (counters_t* counter){
  int sum = 0;
  counters_iterate(counter, &sum, find_total);
  return sum;
}

void find_total(void* arg, int key, int count){
  int* point = arg;
  (*point) +=1;
}
