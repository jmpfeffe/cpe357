#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include "look.h"

void ParseCmdLine(int argc, char **argv, int *dFlag, int *fFlag, char 
 *termChar, char *string, char *fileName, FILE **file) {
   int c, index;

   while ((c = getopt(argc, argv, "dft:")) != -1)
      switch (c) {
      case 'd':
         *dFlag = 1;
         break;
      case 'f':
         *fFlag = 1;
         break;
      case 't':
         strcpy(termChar, optarg);
         break;
      case '?':
         if (optopt == 't')
            fprintf(stderr, "usage: look [-dfa] [-t char] string [file]\n");
         else if (isprint(optopt))
            fprintf(stderr, "usage: look [-dfa] [-t char] string [file]\n");
         else
            fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
         exit(1);
      default:
         abort();
      }
   
   for (index = optind; index < argc; index++) {
      if (index == optind) {
         strcpy(string, argv[index]);
      }
      else {
         strcpy(fileName, argv[index]);
         break;
      }
   }

   if (!strlen(string)) {
      fprintf(stderr, "usage: look [-dfa] [-t char] string [file]\n");
      exit(1);
   }

   if (!strlen(fileName)) {
      strcpy(fileName, "/usr/share/dict/words");
      *dFlag = *fFlag = 1;
   }

   *file = fopen(fileName, "r");

   if (!(*file)) {
      fprintf(stderr, "look: %s: No such file or directory\n", fileName);
      exit(1);
   }
}

void TermChar(char *termChar, char *string) {
   char termCharacter;
   int index2, foundChar = 0, nullIdx;
   
   if (strlen(termChar)) {
      termCharacter = termChar[0];
      for (index2 = 0; index2 < strlen(string); index2++) {
         if (string[index2] == termCharacter) {
            foundChar = 1;
            nullIdx = index2 + 1;
            break;
         }
      }
      if (foundChar)
         string[nullIdx] = '\0';
   }
}

void DFlag(int dFlag, char *string, char *tempString, char 
 *tempFirstWord, int numLines, Line *linePtr) {
   int idx, index, index2;
   
   if (dFlag) {
      idx = 0;
      for (index2 = 0; index2 < strlen(string); index2++) {
         if (isalnum(string[index2]))
            tempString[idx++] = string[index2];
      }
      tempString[idx] = '\0';
      
      for (index = 0; index < numLines; index++) {
         idx = 0;
         for (index2 = 0; index2 < strlen(linePtr[index].firstWord); index2++) {
            if (isalnum(linePtr[index].firstWord[index2]))
               tempFirstWord[idx++] = linePtr[index].firstWord[index2];
         }
         tempFirstWord[idx] = '\0';

         strcpy(linePtr[index].firstWord, tempFirstWord);
      }
      strcpy(string, tempString);
   }
}

int FFlag(char *tempFirstWord, char *string, int *first, int *middle, 
 int *last, Line *linePtr, int numLines) {
   int count, index2;
   
   if (strcasecmp(tempFirstWord, string) < 0)
      *first = *middle + 1;
   else if (!strcasecmp(tempFirstWord, string)) {
      if (strlen(string) <= strlen(linePtr[*middle].firstWord))
         printf("%s", linePtr[*middle].wholeLine);
            
      for (count = *middle - 1; count >= 0; count--) {
         for (index2 = 0; index2 < strlen(string); index2++) {
            tempFirstWord[index2] = linePtr[count].firstWord[index2];
         }
         tempFirstWord[index2] = '\0';
         if (!strcasecmp(tempFirstWord, string))
            if (strlen(string) <= strlen(linePtr[count].firstWord))
               printf("%s", linePtr[count].wholeLine);
      }
            
      for (count = *middle + 1; count < numLines; count++) {
         for (index2 = 0; index2 < strlen(string); index2++) {
            tempFirstWord[index2] = linePtr[count].firstWord[index2];
         }
         tempFirstWord[index2] = '\0';
         if (!strcasecmp(tempFirstWord, string))
            if (strlen(string) <= strlen(linePtr[count].firstWord))
               printf("%s", linePtr[count].wholeLine);
      }
      return 1;
   }
   else {
      *last = *middle - 1;
   }

   *middle = (*first + *last) / 2;
   
   return 0;
}

int NoFFlag(char *tempFirstWord, char *string, int *first, int *middle, 
 int *last, Line *linePtr, int numLines) {
   int count, index2;
   
   if (strcmp(tempFirstWord, string) < 0)
      *first = *middle + 1;
   else if (!strcmp(tempFirstWord, string)) {
      if (strlen(string) <= strlen(linePtr[*middle].firstWord))
         printf("%s", linePtr[*middle].wholeLine);
            
      for (count = *middle - 1; count >= 0; count--) {
         for (index2 = 0; index2 < strlen(string); index2++) {
            tempFirstWord[index2] = linePtr[count].firstWord[index2];
         }
         tempFirstWord[index2] = '\0';
         if (!strcmp(tempFirstWord, string))
            if (strlen(string) <= strlen(linePtr[count].firstWord))
               printf("%s", linePtr[count].wholeLine);
      }
            
      for (count = *middle + 1; count < numLines; count++) {
         for (index2 = 0; index2 < strlen(string); index2++) {
            tempFirstWord[index2] = linePtr[count].firstWord[index2];
         }
         tempFirstWord[index2] = '\0';
         if (!strcmp(tempFirstWord, string))
            if (strlen(string) <= strlen(linePtr[count].firstWord))
               printf("%s", linePtr[count].wholeLine);
      }

      return 1;
   }
   else {
      *last = *middle - 1;
   }

   *middle = (*first + *last) / 2;

   return 0;
}

void BinarySearch(char *tempFirstWord, Line *linePtr, char *string, int fFlag, 
 int numLines) {
   int first = 0, last = numLines - 1, middle, index2, ret;
   
   middle = (first + last) / 2;

   while (first <= last) {

      for (index2 = 0; index2 < strlen(string); index2++) {
         tempFirstWord[index2] = linePtr[middle].firstWord[index2];
      }
      tempFirstWord[index2] = '\0';

      if (fFlag) {
         ret = FFlag(tempFirstWord, string, &first, &middle, &last, linePtr, 
          numLines);
         
         if (ret)
            break;
      }
      else {
         ret = NoFFlag(tempFirstWord, string, &first, &middle, &last, linePtr, 
          numLines);

         if (ret)
            break;
      }
   }
}

int main(int argc, char **argv) {
   int dFlag = 0, fFlag = 0, index, numLines = 0, seenFirstLine = 0;
   char *termChar = calloc(SIZE, sizeof(char)), *string = calloc(SIZE, 
    sizeof(char)), *fileName = calloc(SIZE, sizeof(char)), *line = NULL, 
    *temp = NULL, *tempFirstWord = calloc(SIZE, sizeof(char)), 
    *tempString = calloc(SIZE, sizeof(char));
   FILE *file = NULL;
   size_t len = 0;
   ssize_t read = 0;
   Line *linePtr = calloc(1, sizeof(Line));

   ParseCmdLine(argc, argv, &dFlag, &fFlag, termChar, string, fileName, &file);

   while ((read = getline(&line, &len, file)) != -1) {
      if (!seenFirstLine) 
         seenFirstLine = 1;
      else 
         linePtr = realloc(linePtr, (numLines + 1) * sizeof(Line));

      linePtr[numLines].wholeLine = calloc(read + 1, sizeof(char));
      strcpy(linePtr[numLines].wholeLine, line);

      linePtr[numLines].firstWord = calloc(SIZE, sizeof(char));

      temp = line;
      
      index = 0;
      while (*temp) {
         if (*temp == ' ')
            break;
        
         linePtr[numLines].firstWord[index++] = *temp;
         temp++;
      }

      numLines++;
   }

   TermChar(termChar, string);

   DFlag(dFlag, string, tempString, tempFirstWord, numLines, linePtr);

   BinarySearch(tempFirstWord, linePtr, string, fFlag, numLines);

   for (index = 0; index < numLines; index++) {
      free(linePtr[index].wholeLine);
      free(linePtr[index].firstWord);
   }

   free(linePtr);
   free(line);

   fclose(file);

   return 0;
}
