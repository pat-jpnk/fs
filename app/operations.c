#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "colors.h"
#include <errno.h>

/**
 * here functions that conform to the defined type 'operation' are defined
 * they are a parameter in the method parseFile(...) defined in fs.c
 * 
 * must have an equivalent in "enum func" in fs.h
 **/


// TODO: add color to print statements

// TODO: change method signature, decide if to return int 

// if operation: SEARCH -> _search function = options[bool capitalization;, bool spacing, char* search_term;]
void _search(struct searchItem* item, struct options options) {
    FILE *fp = fopen(item->path, "r");                               

    if(fp == NULL) {
        fprintf(stderr, "Error opening %s\n", item->path); // e.g. permission denied
        //return EXIT_FAILURE;
    }

    char* buffer = NULL;    
    size_t len;
    size_t search_term_len = strlen(options.search_term);
    ssize_t bytes_read = getdelim(&buffer, &len, EOF, fp); 
    ssize_t errval = -1;                                      // TODO: make global or not a variable, also in other methods 

    if(bytes_read != errval) {
        // thaaacode wont be reached if file aaaempty
        //printf("item :%s -- char of buffer: %c >> SIZE: %zu\n",item->path,*(buffer+1), len);
        uint64_t i = (uint64_t) len;
        uint64_t j = 0;
        uint64_t line_count = 1;
        uint64_t j_cache = 0;

        while(j < i) {
            if(*(buffer+j) == '\n') {
                line_count++;
                j_cache = 0;                // for counting position in line in finding / TODO: add better name 
            }
            if(*(buffer+j) == options.search_term[0]) {                           
                bool mismatch = false;
                while(mismatch == false) {  
                    for(size_t k = 1; k < search_term_len; k++) {
                        if((*(buffer+j+k) != options.search_term[k])) {
                            mismatch = true;
                            break;
                        }
                    }
                    if(mismatch == false) {
                        printf(KWHT"%lu:%lu %s\n"KRESET, line_count, j_cache, item->path);                    
                    }
                    break;
                } 
            }
            j_cache++;
            j++;
        }
    } else {
        fprintf(stderr,KNRM"%s not read, file empty\n"KRESET, item->path); // triggers on empty file
    }
    fclose(fp);
}


// TODO: second argument "" => segfault

void _replace(struct searchItem* item, struct options options) {
  
  int new_indx = 0;
  int new_len = 0;
  // TODO: check that delim is not null ptr



  // store in buffer 

  FILE *fp = fopen(item->path, "r");

  if(fp == NULL) {
    fprintf(stderr, "Error opening %s\n", item->path);
  }

  size_t search_term_len = strlen(options.search_term);
  size_t replacement_term_len = strlen(options.replacement_term);

  char* buffer = NULL;
  size_t len;
  ssize_t bytes_read = getdelim(&buffer, &len, EOF, fp);
  ssize_t errval = -1;

  int st_occurences[100];
  int st_occurence_index = 0;
  int st_occurence_count = 0;
  
  uint64_t i = (uint64_t) len;
  uint64_t j = 0;
  
  char* new_content;

  // store occurence indexes 
  
  if(bytes_read != errval) {
  

    while(j < i) {
      if(*(buffer+j) == options.search_term[0]) {
        
        bool mismatch = false; 
        
        while(mismatch == false) {
          for(size_t k = 1; k < search_term_len; k++) {
            if((*(buffer+j+k) != options.search_term[k])) {
              mismatch = true;
              break;
            }
          }

          if(mismatch == false) {
            st_occurences[st_occurence_index] = j;
            st_occurence_index++;
            st_occurence_count++;
            j = j + replacement_term_len;
          } else {
            j++;
          }
        }
      } 
      j++;
    } 
  } else { 
    fprintf(stderr, KNRM"%s not read, file empty\n"KRESET, item->path);
  }
  
  size_t stored_len = 0;

  while(*(buffer+stored_len)) {
    stored_len++;
  }

  st_occurence_index = 0;

  // len will likely be bigger than needed

  /**
  for(size_t ff = 0; ff < stored_len; ff++) {
    printf("%c",*(buffer+ff));
  }
  **/


  // copy content + insert

  if(st_occurence_count > 0) {
   

    int stored_begin = 0;
    int stored_end = (int) stored_len;
    new_len = ((stored_end - 1) - ((int) search_term_len * (int) st_occurence_count)) + ((int)replacement_term_len * st_occurence_count);
    new_content = (char*) malloc(new_len * sizeof(char));
      
    //printf("%d\n", new_len);
    //printf("%d\n", new_indx);

    while(new_indx < new_len) {
     
    //printf("oi: %d\n", st_occurence_index);
    //printf("oc: %d\n", st_occurence_count);

      if(st_occurence_index <= (st_occurence_count - 1)) {
       // printf("Z");

        if(new_indx == st_occurences[st_occurence_index]) {
       // printf("Y");
          for(size_t i = 0; i < replacement_term_len; i++) {
            *(new_content + new_indx) = *(options.replacement_term + stored_begin);
            new_indx++;
          }

          st_occurence_index++;

        } else {
          *(new_content + new_indx) = *(buffer + stored_begin);
          printf("K");
        }

      } else {  
        printf("R");
        for(size_t j = new_indx; j < stored_end; j++) {
          *(new_content + new_indx) = *(buffer + j);
          new_indx++;
        }
      }
    } 
  } else {
    printf("No occurences to replace\n");
  }


  fclose(fp);

  
  FILE *rp = fopen(item->path, "w+");

  if(rp == NULL) {
    fprintf(stderr, "Error opening %s\n", item->path);
  }
  

  //for(int i = 0; i < new_indx; i++) {
  //  printf("%c\n", *(new_content + i));
  //}

  // printf("%c\n", *(new_content + 0));

  for(size_t u = 0; u < new_len; u++) {
    fputs((new_content + u), rp);
  }

  fclose(rp);

  free(new_content);

}



// does not open files
void _info(struct searchItem* item, struct options options) {
    printf(KWHT"%lu %s\n"KRESET,item->st_ino, item->path);
}
