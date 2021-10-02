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


// if operation: SEARCH -> _search function = options[bool capitalization;, bool spacing, char* search_term;]
void _search(struct searchItem* item, struct options options) {
    FILE *fp = fopen(item->path, "r");                               

    if (fp == NULL) {
        fprintf(stderr, "Error opening %s\n", item->path); // e.g. permission denied
        return;
    }

    char* buffer = NULL;    
    size_t len;
    size_t search_term_len = strlen(options.search_term);
    ssize_t bytes_read = getdelim(&buffer, &len, EOF, fp); 
    ssize_t errval = -1;

    if (bytes_read != errval) {
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


/**

void _replace(struct searchItem* item, struct options options) {
    size_t search_term_len = strlen(options.search_term);
    size_t replacement_term_len = strlen(options.replacement_term);
    char c;
    
    char content[1028];
    size_t content_index = 0;

    FILE *fp = fopen(item->path, "r"); 

    if (fp == NULL) {
        fprintf(stderr, "Error opening %s %s\n", item->path, strerror(errno)); // e.g. permission denied
        return;
    }

    while(1) {
        c = fgetc(fp);
        
        if (feof(fp)) {
            break;
        }

        content[content_index] = c;
        content_index++;
    }
    fclose(fp);

    bool mismatch;
    size_t j = 0;
    size_t i = content_index;
    size_t k_cache = 0;
    char* buffer = &content[0];

    while(j < i) {
        if(*(buffer+j) == options.search_term[0]) {                                      
                mismatch = false; 

                for(size_t k = 1; k < search_term_len; k++) {           
                    if((*(buffer+j+k) != options.search_term[k])) {     
                        k_cache = k;  
                        mismatch = true;
                        printf("JAVA\n");
                        break;
                    }
                }

                // triggers when full match                                 // TODO: fix case where st > rt
                if(mismatch == false) {    
                    printf("CODE REACHED 2\n");

                    for(size_t i = 0; i < replacement_term_len; i++) {
                        content[content_index] = options.replacement_term[i];
                        content_index++;
                    }

                    j = (j+search_term_len-1);
                }        

            

                // triggers when [0] matched but rest not fully
                if (mismatch == true) {
                    printf("PARTLY\n");
                    for(size_t t = j; t < (j+k_cache); t++) {
                        new_buffer[new_buffer_i] = *(buffer+t);
                        content_index++;
                    }
                    j = j+k_cache-1;
                } 
            } else {
                // triggers when != st[0]
                new_buffer[new_buffer_i] = *(buffer+j);
                new_buffer_i++;
            }
            
            j++;
    }


//    for(size_t i = 0; i < content_index; i++) {
//        printf("%c", content[i]);
//    }

    //printf("YOO %lu\n", content_index);

    //exit(0);
}

**/



// opens files in read and write mode
// if operation: REPLACE -> _replace function = options[bool capitalization;, bool spacing, char* search_term; char* replacement_term]

/**
void _replace(struct searchItem* item, struct options options) {
    FILE *fp = fopen(item->path, "r+"); 

    if (fp == NULL) {
        fprintf(stderr, "Error opening %s %s\n", item->path, strerror(errno)); // e.g. permission denied
        return;
    }
     
    char* buffer = NULL;    
    size_t len;


    size_t search_term_len = strlen(options.search_term);
    size_t replacement_term_len = strlen(options.replacement_term);
    
    ssize_t bytes_read = getdelim(&buffer, &len, EOF, fp); 
    ssize_t errval = -1;

    //printf("len: %lu %s last: %c\n", len, item->path, *(buffer+16));
    

    char new_buffer[len];
    size_t new_buffer_i = 0;

    if (bytes_read != errval) {
        uint64_t i = (uint64_t) len;
        uint64_t j = 0;

        size_t  k_cache = 0;

        bool mismatch;

        while(j < i) {
            if(*(buffer+j) == options.search_term[0]) {                                      
                mismatch = false; 

                for(size_t k = 1; k < search_term_len; k++) {           
                    if((*(buffer+j+k) != options.search_term[k])) {     
                        k_cache = k;  
                        mismatch = true;
                        printf("JAVA\n");
                        break;
                    }
                }

                // triggers when full match                                 // TODO: fix case where st > rt
                if(mismatch == false) {    
                    printf("CODE REACHED 2\n");

                    for(size_t i = 0; i < replacement_term_len; i++) {
                        new_buffer[new_buffer_i] = options.replacement_term[i];
                        new_buffer_i++;
                    }

                    j = (j+search_term_len-1);
                }        

                // triggers when [0] matched but rest not fully
                if (mismatch == true) {
                    printf("PARTLY\n");
                    for(size_t t = j; t < (j+k_cache); t++) {
                        new_buffer[new_buffer_i] = *(buffer+t);
                        new_buffer_i++;
                    }
                    j = j+k_cache-1;
                } 
            } else {
                // triggers when != st[0]
                new_buffer[new_buffer_i] = *(buffer+j);
                new_buffer_i++;
            }
            j++;
        }

        // TODO: if changes made, write new buffer content to file
        //       log 'file altered' in stats

        //printf("NUTRITION");

        
        for(size_t s = 0; s < new_buffer_i; s++) {
            printf("%c", new_buffer[s]);
        }

        //exit(0);
        

        
        fclose(fp);
        
       
        FILE *fp = fopen(item->path, "w");
        if (fp == NULL) {
            fprintf(stderr, "Error opening %s %s for writing\n", item->path, strerror(errno)); // e.g. permission denied
            return;
        }

        for(size_t s = 0; s < new_buffer_i; s++) {
             fprintf(fp,"%c",new_buffer[s]);
        }

        

        
        fclose(fp);
        
        exit(0);

    } else {
        fprintf(stderr,KNRM"%s not read, file empty\n"KRESET, item->path); // triggers on empty file
        fclose(fp);
    }
}
**/

// does not open files
void _info(struct searchItem* item, struct options options) {
    printf(KWHT"%lu %s\n"KRESET,item->st_ino, item->path);
}
