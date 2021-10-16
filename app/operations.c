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

void _search(struct searchItem* item, struct options options) {
    FILE *fp = fopen(item->path, "r");                               

    if(fp == NULL) {
        fprintf(stderr, "Error opening %s\n", item->path); // e.g. permission denied
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


#define INITIAL_SIZE 100

void _replace(struct searchItem* item, struct options options) {
  

  // ############## read file content into buffer ######################

  FILE *fp = fopen(item->path, "r");

  if(fp == NULL){
    fprintf(stderr, "Error opening %s\n", item->path);
  }

  size_t search_term_len = strlen(options.search_term);
  size_t replacement_term_len = strlen(options.replacement_term);

  int close_status;
  int c;

  unsigned int buffer_index = 0; 
  unsigned int size = INITIAL_SIZE;
  unsigned int new_len;
  unsigned int new_index = 0;

  char* new_content;
  char* buffer = (char*) malloc(INITIAL_SIZE * sizeof(char)); 

  if(buffer == NULL) {
    fprintf(stderr, "Fatal error, failed to allocate %u bytes", INITIAL_SIZE);
    abort();
  }

  while(feof(fp) == 0) {
    if(buffer_index == size) {
      buffer = (char*) realloc(buffer, (size * 1.5));
      size *= 1.5;
    }
    int c = fgetc(fp);
    if(c != EOF) {
      *(buffer + buffer_index) = c; 
      buffer_index += 1;
    } else {
      break;
    }
  }

  close_status = fclose(fp);

  if(close_status == EOF) {
    perror("Error closing file");
  }
  
  // ##############  record occurences  ######################

  
  unsigned int j = 0;
  unsigned int i = buffer_index;

  int st_occurences[100];
  int st_occurence_index = 0;

  while(j < i) {
    if(*(buffer+j) == options.search_term[0]) {
      bool mismatch = false;

      while(mismatch == false) {
        for(size_t k = 0; k < search_term_len; k++) {
          if(*(buffer+j+k) != options.search_term[k]) {
            mismatch = true;
            break;
          }
        }
        
        if(mismatch == false) {
          st_occurences[st_occurence_index] = j;   
          st_occurence_index++;
          j += search_term_len;   
        } else {  
          j++;
        }
      }
    }
    else {
      j++;
    } 
  }


  for(int p = 0; p < st_occurence_index; p++) {
    printf("%d:  %d\n", p, st_occurences[p]);

  }

  // ############## write new_content from buffer / occurences ######################

  // st_occurence_count 

  // stored_begin 
  // stored_end

  // new_len



  // ################################################################################

  /**

  if(st_occurence_count > 0) {

    st_occurence_index = 0;
    int stored_begin = 0;     // buffer index
    int stored_end = i;       // buffer end 
    
    new_len = (i - ((int) search_term_len * st_occurence_count)) + ((int) replacement_term_len * st_occurence_count); 
    new_content = (char*) malloc(new_len * sizeof(char));   

    if(new_content == NULL) {
      fprintf(stderr, "Fatal error, failed to allocate %u bytes", new_len);
      abort();
    }

    int sr = ((int) search_term_len) - ((int) replacement_term_len);


    while(new_index < new_len) {
      
      if(st_occurence_index <= (st_occurence_count - 1)) {

        if(new_index == (st_occurences[st_occurence_index])) {

          for(size_t i = 0; i < replacement_term_len; i++) {
            *(new_content + new_index) = *(options.replacement_term + i);
            //stored_begin++;
            new_index++;
          }

          stored_begin += (int) search_term_len;

          // add difference if replacement term is smaller than search term

          st_occurence_index++;
        } else {
            *(new_content + new_index) = *(buffer + stored_begin);
            new_index++;  //
            stored_begin++;
        }      
      } else {
          for(size_t k = stored_begin; k < stored_end; k++) {
            *(new_content + new_index) = *(buffer + k);
            new_index++;
          }
      }
    }
  }

  **/
  free(buffer);     



  // ############## write new_content to file ######################

/**
  
  FILE *rp = fopen(item->path, "w+");
    
  if(rp == NULL) {
    fprintf(stderr, "Error opening %s\n", item->path);
    abort();
  }


  fputs((new_content),rp);

  close_status = fclose(rp);     
  
 
  if(close_status == EOF) {
    perror("Error closing file");
  } 

  **/
  free(new_content); 
  

}


void _info(struct searchItem* item, struct options options) {
    printf(KWHT"%lu %s\n"KRESET,item->st_ino, item->path);
}