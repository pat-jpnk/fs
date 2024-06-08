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
        fprintf(stderr, "Error opening %s\n", item->path); // possibly permission denied 
    }

  size_t search_term_len = strlen(options.search_term);
  unsigned int size = BF_INITIAL_SIZE;
  int close_status;


  char* buffer = malloc((size * sizeof(char)));  // void pointer returned by malloc is automatically converted to the correct type
  unsigned int buffer_index = 0;

  if(buffer == NULL) {
    fprintf(stderr, "Fatal error, failed to allocate %u bytes", BF_INITIAL_SIZE);
    abort();    // TODO: exit?
  }
  
  while(feof(fp) == 0) {
    if(buffer_index == size) {
      buffer = (char*) realloc(buffer, ((size * 3) / 2));  // TODO: edit
      size *= ((size * 3) / 2);
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
    fprintf(stderr, "Error closing file");
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
                        printf(KGRN"%lu:%lu %s\n"KRESET, line_count, j_cache, item->path);                    
                    }
                    break;
                } 
            }
            j_cache++;
            j++;
        }
    } else {
      fprintf(stderr,KCYN"%s not read, file empty\n"KRESET, item->path); // triggers on empty file
    }

  free(buffer);
}


void _replace(struct searchItem* item, struct options options) {

  // ############## read file content into buffer ######################

  FILE *fp = fopen(item->path, "r");

  if(fp == NULL) {
    fprintf(stderr, "Error opening %s\n", item->path);
    abort();
  }

  size_t search_term_len = strlen(options.search_term);
  size_t replacement_term_len = strlen(options.replacement_term);

  int close_status;
  //int c;

  unsigned int buffer_index = 0; 
  unsigned int size = BF_INITIAL_SIZE;
  unsigned int new_len = 0;     
  unsigned int new_index = 0;

  char* new_content;
  char* buffer =  malloc(BF_INITIAL_SIZE * sizeof(char));

  if(buffer == NULL) {
    fprintf(stderr, "Fatal error, failed to allocate %u bytes", BF_INITIAL_SIZE);
    exit(1);
  }


  while(feof(fp) == 0) {
    if(buffer_index == size) {
      buffer = (char*) realloc(buffer, ((size * 3) / 2));  // TODO: edit 
        size *= ((size * 3) / 2);
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
    fprintf(stderr, "Error closing file");
    exit(1);
  }

  // ##############  record occurrences  ######################

  unsigned int j = 0;
  unsigned int st_occurrences[OI_INITIAL_SIZE];
  unsigned int st_occurrence_index = 0;
  unsigned int st_occurrence_count = 0;

  while(j < buffer_index) {
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
          st_occurrences[st_occurrence_index] = j;   
          st_occurrence_index++;
          st_occurrence_count++;
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

  if(st_occurrence_count > 0) {

    new_len = ((buffer_index) + (((int) st_occurrence_count) * (((int) replacement_term_len) - ((int) search_term_len))));  // index - 1
    st_occurrence_index = 0;

    //  unsigned int buffer_max = buffer_index;
    unsigned int buffer_count = 0;
    unsigned int new_content_count = 0;

    new_content = malloc(new_len * sizeof(char));
    
    if(new_content == NULL) {
      fprintf(stderr, "Fatal error, failed to allocate %u bytes", new_len);
      exit(1);
    }

    memset(new_content, 0, new_len);   // TODO: EDIT

    while(buffer_count < buffer_index) {

      if(buffer_count == st_occurrences[st_occurrence_index]) {

        for(size_t i = 0; i < replacement_term_len; i++) {        
            *(new_content + new_content_count) = *(options.replacement_term + i);
            new_content_count++;
        }
        
        buffer_count += search_term_len;

        if((st_occurrence_index) < st_occurrence_count) {  // index - 1     <=
          st_occurrence_index++;
        }

      } else {
        *(new_content + new_content_count) = *(buffer + buffer_count);
        new_content_count++;
        buffer_count++;
      }      

    }

    fprintf(stdout, KGRN"%.*s - search term occurrence found\n"KRESET, (int)sizeof(item->path), item->path);

    free(buffer);

  } else {
    fprintf(stdout, KRED"%.*s - no search term occurrences found\n"KRESET, (int)sizeof(item->path), item->path);   // %.*s -> convenient to print char array
    free(buffer);
    return;
  }

  // ##############  write output to file  ######################
 
  FILE *rp = fopen(item->path, "w+");

  if(rp == NULL) {
    fprintf(stderr, "Error opening %s\n", item->path);
    exit(1);
  }

  for(unsigned int u = 0; u < new_len; u++) {
    fputc(*(new_content+u),rp);
  }

  close_status = fclose(rp);       

  if(close_status == EOF) {
    fprintf(stderr, "Error closing file");
    exit(1);
  }

  free(new_content);

}


/** 

void _replace(struct searchItem* item, struct options options) {

    // ############## read file content into buffer ######################

    FILE *fp = fopen(item->path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening %s\n", item->path);
        exit(1);
    }

    size_t search_term_len = strlen(options.search_term);
    size_t replacement_term_len = strlen(options.replacement_term);

    unsigned int buffer_index = 0;
    unsigned int size = BF_INITIAL_SIZE;

    char* buffer = malloc(BF_INITIAL_SIZE * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Fatal error, failed to allocate %u bytes", BF_INITIAL_SIZE);
        exit(1);
    }

    while (!feof(fp)) {
        if (buffer_index == size) {   
            size = (size * 3) / 2;            // TODO: edit
            buffer = realloc(buffer, size);
            if (buffer == NULL) {
                fprintf(stderr, "Fatal error, failed to reallocate buffer");
                exit(1);
            }
        }
        int c = fgetc(fp);
        if (c != EOF) {
            buffer[buffer_index++] = (char)c;
        } else {
            break;
        }
    }

    if (fclose(fp) == EOF) {
        fprintf(stderr, "Error closing file");
        exit(1);
    }

    // ############## record occurrences ######################

    unsigned int j = 0;
    int st_occurrences[OI_INITIAL_SIZE];
    int st_occurrence_index = 0;
    int st_occurrence_count = 0;

    while (j < buffer_index) {
        if (buffer[j] == options.search_term[0]) {
            bool mismatch = false;
            for (size_t k = 0; k < search_term_len; k++) {
                if (buffer[j + k] != options.search_term[k]) {
                    mismatch = true;
                    break;
                }
            }
            if (!mismatch) {
                st_occurrences[st_occurrence_index++] = j;
                st_occurrence_count++;
                j += search_term_len;
            } else {
                j++;
            }
        } else {
            j++;
        }
    }

    // ############## write output to new_content ######################

    if (st_occurrence_count > 0) {
        unsigned int new_len = buffer_index + (st_occurrence_count * (replacement_term_len - search_term_len));
        char* new_content = malloc(new_len * sizeof(char));
        if (new_content == NULL) {
            fprintf(stderr, "Fatal error, failed to allocate %u bytes", new_len);
            exit(1);
        }

        memset(new_content, 0, new_len);        // TODO: edit

        unsigned int buffer_count = 0;
        unsigned int new_content_count = 0;
        st_occurrence_index = 0;

        while (buffer_count < buffer_index) {
            if (buffer_count == st_occurrences[st_occurrence_index]) {
                for (size_t i = 0; i < replacement_term_len; i++) {
                    new_content[new_content_count++] = options.replacement_term[i];
                }
                buffer_count += search_term_len;
                if (st_occurrence_index < st_occurrence_count) {
                    st_occurrence_index++;
                }
            } else {
                new_content[new_content_count++] = buffer[buffer_count++];
            }
        }

        fprintf(stdout, "Replacement done: %s\n", item->path);
        free(buffer);

        // ############## write output to file ######################

        FILE *rp = fopen(item->path, "w+");
        if (rp == NULL) {
            fprintf(stderr, "Error opening %s\n", item->path);
            exit(1);
        }

        for (unsigned int u = 0; u < new_len; u++) {
            fputc(new_content[u], rp);
        }

        if (fclose(rp) == EOF) {
            perror("Error closing file");
            fprintf(stderr, "Error closing file");
            exit(1);
        }

        free(new_content);
    } else {
        fprintf(stdout, "No occurrences found: %s\n", item->path);
        free(buffer);
    }
}

**/

void _info(struct searchItem* item, struct options options) {
    printf(KWHT"%lu %s\n"KRESET,item->st_ino, item->path);
}
