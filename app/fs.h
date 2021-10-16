#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#ifndef FS_H
#define FS_H

#define ROOTPATH "./"             
#define MAX_PATH_SIZE 1024                  // in characters
#define MAX_SEARCH_ITEMS 150                // set to something smart, like OPEN_MAX
#define MAX_PREVIEW_SIZE 512                // ????

typedef enum fileType {
    SOCKET,
    SYMLINK,                
    REGULAR,                 
    BLOCK,                  
    DIREC,                      
    CHAR,               
    PIPE,         
    UNKNOWN                  
} fileType;

typedef enum func {
    SEARCH,
    REPLACE,
    INFO
} func;

typedef struct options {
    func function;
    char* search_term;
    char* replacement_term;
} options;

typedef struct searchItem {
    ino_t st_ino;                           // File serial number defined in <sys/stat.h>
    char path[MAX_PATH_SIZE];               // changed from char*
    fileType type;                          // enum defined above
} searchItem;

typedef struct searchIndex {
    struct searchItem items[MAX_SEARCH_ITEMS];
    uint64_t size;
} searchIndex;

typedef void (*operation) (struct searchItem*, struct options);         // function pointer parameter accepted by parseFile(...)

struct searchStats initSearchStats();
fileType getFileType(mode_t m);
fileType getFileStatus(const char* path);
bool filterFileName(const char * item_name); 
void parseFile(operation op, struct searchIndex* index, struct options options, uint64_t size);
void parseIndex(struct searchIndex* index, struct options* options);
void getItemPath(const char* path, const char* item_name, char* directory_path);
void recursive(char *basePath, struct searchIndex* index);

#endif
