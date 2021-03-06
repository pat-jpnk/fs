#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "colors.h"
#include <errno.h>

void _search(struct searchItem* item, struct options options) {
  FILE *fp = fopen(item->path, "r");
  
  if(fp == NULL) {
        fprintf(stderr, "Error opening %s\n", item->path); // e.g. permission denied
    }

  size_t search_term_len = strlen(options.search_term);
  unsigned int size = BF_INITIAL_SIZE;
  int close_status;


  char* buffer = (char*) malloc((size * sizeof(char)));
  unsigned int buffer_index = 0;

  if(buffer == NULL) {
    fprintf(stderr, "Fatal error, failed to allocate %u bytes", BF_INITIAL_SIZE);
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


  uint64_t i = (uint64_t) buffer_index;
  uint64_t j = 0;
  uint64_t line_count = 1;
  uint64_t j_cache = 0;

  if(buffer_index > 0) {
        while(j < i) {
            if(*(buffer+j) == '\n') {
                line_count++;
                j_cache = 0;             
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
}

void _replace(struct searchItem* item, struct options options) {


  // ############## read file content into buffer ######################

  printf("%s\n", item->path);

  FILE *fp = fopen(item->path, "r");

  if(fp == NULL){
    fprintf(stderr, "Error opening %s\n", item->path);
    abort();
  }

  size_t search_term_len = strlen(options.search_term);
  size_t replacement_term_len = strlen(options.replacement_term);

  int close_status;
  int c;

  unsigned int buffer_index = 0; 
  unsigned int size = BF_INITIAL_SIZE;
  unsigned int new_len;
  unsigned int new_index = 0;

  char* new_content;
  char* buffer = (char*) malloc(BF_INITIAL_SIZE * sizeof(char)); 

  if(buffer == NULL) {
    fprintf(stderr, "Fatal error, failed to allocate %u bytes", BF_INITIAL_SIZE);
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

  int st_occurences[OI_INITIAL_SIZE];
  int st_occurence_index = 0;
  int st_occurence_count = 0;

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
          st_occurence_count++;
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

  // ##############  write output to new_content  ######################

  if(st_occurence_count > 0) {
    size_t stored_index = 0;
    int stored_end = i;
    st_occurence_index = 0;


    new_len = ((i - 1) - ((int) search_term_len * st_occurence_count)) + ((int) replacement_term_len * st_occurence_count); 
    new_content = (char*) malloc(new_len * sizeof(char));

    if(new_content == NULL) {
      fprintf(stderr, "Fatal error, failed to allocate %u bytes", new_len);
      abort();
    }

    while(new_index < new_len) {

      if(st_occurence_index < (st_occurence_count)) {
        if(new_index == (st_occurences[st_occurence_index])) {
          for(size_t i = 0; i < replacement_term_len; i++) {
            *(new_content + new_index) = *(options.replacement_term + i);
            new_index++;
          }

          stored_index += search_term_len;
          st_occurence_index++;
        } else {
            *(new_content + new_index) = *(buffer + stored_index);
            new_index++;  
            stored_index++;
        }      
      } else {
          for(size_t k = stored_index; k < stored_end; k++) {
            *(new_content + new_index) = *(buffer + k);
            new_index++;
          }
      }
    }
  }

  free(buffer);

  // ##############  write output to file  ######################
  
  FILE *rp = fopen(item->path, "w+");
  if(rp == NULL) {
    fprintf(stderr, "Error opening %s\n", item->path);
    abort();
  }
  
  for(int u = 0; u <= new_len; u++) {
    fputc(*(new_content+u),rp);
  }

  close_status = fclose(rp);       
  if(close_status == EOF) {
    perror("Error closing file");
  } 

  free(new_content);

}


void _info(struct searchItem* item, struct options options) {
    printf(KWHT"%lu %s\n"KRESET,item->st_ino, item->path);
}
