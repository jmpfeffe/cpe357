#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>

typedef struct {
   char *firstWord;
   char *wholeLine;
} Line;


int main (int argc, char **argv) {
   int c, dFlag = 0, fFlag = 0, index, index2, numLines = 0, seenFirstLine = 0, foundChar = 0, nullIdx = 0, stringsEqual = 0, idx = 0;
   char *termChar = NULL, *string = NULL, *fileName = NULL, *line = NULL, *temp = NULL, termCharacter, *tempFirstWord, *tempString;
   FILE *file = NULL;
   size_t len = 0;
   ssize_t read = 0;
   Line *linePtr = calloc(1, sizeof(Line));

   while ((c = getopt(argc, argv, "dft:")) != -1)
      switch(c) {
         case 'd':
            dFlag = 1;
            break;
         case 'f':
            fFlag = 1;
            break;
         case 't':
            termChar = optarg;
            break;
         case '?':
            if (optopt == 't')
               fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
               fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
               fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            return 1;
         default:
            abort();
      }
   
   for (index = optind; index < argc; index++) {
      if (index == optind)
         string = argv[index];
      else {
         fileName = argv[index];
         break;
      }
   }

   if (!fileName)
      fileName = "/usr/share/dict/words";

   file = fopen(fileName, "r");

   if (!file) {
      fprintf(stderr, "look: can't open %s\n", fileName);
      exit(1);
   }

   while ((read = getline(&line, &len, file)) != -1) {
      if (!seenFirstLine) 
         seenFirstLine = 1;
      else 
         linePtr = realloc(linePtr, (numLines + 1) * sizeof(Line));

      linePtr[numLines].wholeLine = calloc(read + 1, sizeof(char));
      strcpy(linePtr[numLines].wholeLine, line);

      linePtr[numLines].firstWord = calloc(250, sizeof(char));

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

   if (termChar) {
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

   tempString = calloc(50, sizeof(char));
   tempFirstWord = calloc(50, sizeof(char));

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

   for (index = 0; index < numLines; index++) {
      stringsEqual = 0;

      if (strlen(string) > strlen(linePtr[index].firstWord))
         continue;
      
      for (index2 = 0; index2 < strlen(string); index2++) {
         tempFirstWord[index2] = linePtr[index].firstWord[index2];
      }
      tempFirstWord[index2] = '\0';

      if (fFlag) {
         if (!strcasecmp(tempFirstWord, string))
            stringsEqual = 1;
      }
      else {
         if (!strcmp(tempFirstWord, string))
            stringsEqual = 1; 
      }

      if (stringsEqual) {
         printf("%s", linePtr[index].wholeLine);
      }
   }


   for (index = 0; index < numLines; index++) {
      free(linePtr[index].wholeLine);
      free(linePtr[index].firstWord);
   }


   free(linePtr);
   free(line);

   fclose(file);

   return 0;
}
