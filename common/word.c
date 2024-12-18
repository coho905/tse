/*
 * colin wolfe
 * cs50 24w
*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//normalize word
//no returns
void normalizeWord(char* word)
{
  char edited [strlen(word)+1];
  strncpy(edited, word, strlen(word)); //edit in place
  for(int i = 0; i < strlen(word); i++){
    edited[i] = tolower(edited[i]); //make lowercase
  }
  strncpy(word, edited, strlen(word));  //copy back to original pointer
}
