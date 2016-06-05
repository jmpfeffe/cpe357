#ifndef LOOK_H
#define LOOK_H

#include <stdio.h>

#define SIZE 50

typedef struct {
   char *firstWord;
   char *wholeLine;
} Line;

void ParseCmdLine(int argc, char **argv, int *dFlag, int *fFlag, char 
 *termChar, char *string, char *fileName, FILE **file);
void TermChar(char *termChar, char *string);
void DFlag(int dFlag, char *string, char *tempString, char 
 *tempFirstWord, int numLines, Line *linePtr);
int FFlag(char *tempFirstWord, char *string, int *first, int *middle, 
 int *last, Line *linePtr, int numLines);
int NoFFlag(char *tempFirstWord, char *string, int *first, int *middle, 
 int *last, Line *linePtr, int numLines);
void BinarySearch(char *tempFirstWord, Line *linePtr, char *string, int fFlag, 
 int numLines);

#endif
