/*
 * Colin Wolfe
 * CS50 24W
 * This is the Crawler, it will crawl all links connected to a SeedURL to a given max Depth
 * Takes Arguments:
 * ./crawler SeedURL pageDirectory maxDepth
 * Will only work if pageDirectory is pre-existing
 * Returns 0 if successful
 * exits 1 if improper number of arguments
 * exits 2 if impropers argument types
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "bag.h"
#include "webpage.h"
#include "pagedir.h"

//functions needed to keep main clean:
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void namedelete(void* item);
static char* parseArgs(char* one, char* two, char* three);
void name(FILE* fp, void* item);

//main:
int main(int argc, char* argv[])
{
  //verify number of args:
  if(argc != 4){
    fprintf(stderr, "Invalid Number of Args\n");
    exit(1); //invalid number of args
  }
  //parse through args
  char* internalURL = parseArgs(argv[1], argv[2], argv[3]);
  int depth = atoi(argv[3]);
  //verify range of depth
  if(internalURL != NULL){
    crawl(internalURL, argv[2], depth); 
  }
  else{
    free(internalURL);
    exit(2); //invalid parameters
  }
  free(internalURL);
  return 0;
}
/*
 * Crawl does the main driving of the program
 * This "crawls", starting at the seedURL 
 * will comb over every link in every URL that is not at maxDepth
 * only scans each webpage once
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
  char* url = malloc(strlen(seedURL)+1);
  strcpy(url, seedURL);
  hashtable_t* ht = hashtable_new(30);
  hashtable_insert(ht, url, "");
  bag_t* bag = bag_new();
  int docID = 0;
  bag_insert(bag, webpage_new(url, 0, NULL));
  webpage_t* extracted;
  while( (extracted = bag_extract(bag) ) != NULL){
     printf("%d\tFetched:\t%s\n", webpage_getDepth(extracted), webpage_getURL(extracted));
    if(webpage_fetch(extracted)){
      docID+=1;
      pagedir_save(extracted, pageDirectory, docID);
      if(webpage_getDepth(extracted) < maxDepth){
        pageScan(extracted, bag, ht);
      }
    }
    webpage_delete(extracted);
  }
  hashtable_delete(ht, namedelete);
  bag_delete(bag, NULL);
  //freed all potential memory leaks
}
/*
 * This scans all pages that have not hit max depth for new links to add
 * adds links that have not been seen yet
*/ 
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  int start = 0;
  char* newhtml;
  printf("%d\tScanning:\t%s\n", webpage_getDepth(page), webpage_getURL(page));
  while((newhtml = webpage_getNextURL(page, &start)) != NULL){
    printf("%d\tFound:\t%s\n", webpage_getDepth(page)+1, newhtml);
    if(isInternalURL(newhtml)){ 
      char* normal = normalizeURL(newhtml);
      char* tempurl = malloc(strlen(normal)+1);
      strcpy(tempurl, normal);
      free(normal);    
      bool success = hashtable_insert(pagesSeen, tempurl, "");
      if(success){
        printf("%d\tAdded:\t%s\n", webpage_getDepth(page)+1, newhtml);
        //Check for Depth issues
        webpage_t* new = webpage_new(tempurl, webpage_getDepth(page) + 1, NULL);
        bag_insert(pagesToCrawl, new);
      }
      else{
        printf("%d\tIgnDupl:\t%s\n", webpage_getDepth(page)+1, newhtml);
        free(tempurl);
      }
    }
    else{
      printf("%d\tIgnExtern:\t%s\n", webpage_getDepth(page) +1, newhtml);
    }
    free(newhtml);
  }
  //freed all potential leaks
}

//empty function to allow the hashtable_delete to work
static void namedelete(void* item)
{
  if (item != NULL) {
   ;
  }
}

/*
 * parses the arguments for improper inputs
 * must be valid internal url
 * must be existing directory
 * must be depth >= 0
*/
static char* parseArgs(char* one, char* two, char* three)
{
  char* internalURL = normalizeURL(one);
  if(!(isInternalURL(internalURL))){
    free(internalURL);
    fprintf(stderr, "Invalid URL\n");
    return NULL;
  }
  bool success = pagedir_init(two);
  if(!success){
    free(internalURL);
    fprintf(stderr, "Not a valid directory\n");
    return NULL;
  }
  int temp = atoi(three);
  if(temp < 0){ //check if crawler is valid
    free(internalURL);
    fprintf(stderr, "Invalid Range for Depth\n");
    return NULL;
  }
  return internalURL;
}

