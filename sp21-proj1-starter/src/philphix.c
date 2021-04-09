/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary = NULL;
static int STRLEN = 64;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string)
 */
unsigned int stringHash(void *s) {
  // -- TODO --
  //fprintf(stderr, "need to implement stringHash\n");
  unsigned int res = 0;
  char * str = (char *)s;
  const int len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    res += 31 * res + str[i]; /// java 8 implemenation
  }
  return res;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  //fprintf(stderr, "You need to implement stringEquals");

  char * str1 = (char *)s1;
  char * str2 = (char *)s2;
  if (0 == strcmp(str1, str2)) return !0;
  return 0;
}

/*
 * This function should read in every word and replacement from the dictionary
 * and store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final bit of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(61)
 * to cleanly exit the program.
 */
void readDictionary(char *dictName) {
  // -- TODO --
  //fprintf(stderr, "You need to implement readDictionary\n");
  FILE * fp = fopen(dictName, "r");
  if (!fp) {
    fprintf(stderr, "open file %s failed.\n", dictName);
    exit(61);
  }
  
  char *word = (char *)calloc(STRLEN, sizeof(char));
  char *repl = (char *)calloc(STRLEN, sizeof(char));
  
  while (2 == fscanf(fp, "%s %s\n", word, repl)) {
    if (NULL == findData(dictionary, word))
    {
      insertData(dictionary, (void *)word, (void *)repl);
    } 

    memset(word, 0, STRLEN * sizeof(char));
    memset(repl, 0, STRLEN * sizeof(char));
  };

  fclose(fp);
}

/*
 * This should process standard input (stdin) and perform replacements as 
 * described by the replacement set then print either the original text or 
 * the replacement to standard output (stdout) as specified in the spec (e.g., 
 * if a replacement set of `taest test\n` was used and the string "this is 
 * a taest of  this-proGram" was given to stdin, the output to stdout should be 
 * "this is a test of  this-proGram").  All words should be checked
 * against the replacement set as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the replacement set shoud 
 * it report the original word.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final bit of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
  // -- TODO --
  //fprintf(stderr, "You need to implement processInput\n");

  char *s1 = (char *)calloc(STRLEN, sizeof(char));
  char *s2 = (char *)calloc(STRLEN, sizeof(char));
  char *s3 = (char *)calloc(STRLEN, sizeof(char));

  int idx = 0;
  int ch = 0;
  while (EOF != (ch = fgetc(stdin)))
  {
    if (idx == STRLEN) 
    {
      STRLEN *= 2; /// doule length of buffer
      s1 = realloc(s1, STRLEN);
      s2 = realloc(s2, STRLEN);
      s3 = realloc(s3, STRLEN);
    }

    if (isalpha(ch))
    {
      s1[idx] = (char)ch;/// case 1
      s2[idx] = idx == 0 ? (char)ch: (char)tolower(ch);///case 2
      s3[idx] = (char)tolower(ch);/// case 3
      idx++;
    }
    else
    {
      if (isalpha(s1[0]))
      {
        s1[idx] = '\0';
        s2[idx] = '\0';
        s3[idx] = '\0';

        void * repl1 = NULL;
        void * repl2 = NULL;
        void * repl3 = NULL;
        if ( NULL == (repl1 = findData(dictionary, (void *)s1)) &&
             NULL == (repl2 = findData(dictionary, (void *)s2)) &&
             NULL == (repl3 = findData(dictionary, (void *)s3)) )
        {
          fprintf(stdout, "%s%c", s1, ch);
        }
        else
        {
          fprintf(stdout, "%s%c", repl1 ? (repl1) : (repl2 ? repl2 : repl3), ch);
        }
      }
      else
      {
        fprintf(stdout, "%c", ch);
      }
      idx = 0;

      memset(s1, 0, strlen(s1));
      memset(s2, 0, strlen(s2));
      memset(s3, 0, strlen(s3));
    }
  }

  free(s1);
  free(s2);
  free(s3);
}
