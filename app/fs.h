#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef FS_H
#define FS_H

#define ROOTPATH "./"              // TODO: replace with parameter?
#define MAX_PATH_SIZE 1024         // in characters
#define MAX_SEARCH_ITEMS FOPEN_MAX // defined in <stdio.h>

typedef enum fileType
{
    SOCKET,
    SYMLINK,
    REGULAR,
    BLOCK,
    DIREC,
    CHAR,
    PIPE,
    UNKNOWN
} fileType;

typedef enum func
{
    NONE,
    SEARCH,
    REPLACE,
    INFO
} func;

typedef struct options
{
    func function;
    char *search_term;
    char *replacement_term;
} options;

typedef struct searchItem
{
    ino_t st_ino; // File serial number defined in <sys/stat.h>
    char path[MAX_PATH_SIZE];
    fileType type; // enum defined above
} searchItem;

typedef struct searchIndex
{
    struct searchItem items[MAX_SEARCH_ITEMS];
    unsigned int size;
} searchIndex;

typedef void (*operation)(
    struct searchItem *,
    struct options); // function pointer parameter accepted by parseFile(...)

struct searchStats initSearchStats();
fileType getFileType(mode_t m);
fileType getFileStatus(const char *path);
bool filterFileName(const char *item_name);
void parseFile(operation op, struct searchIndex *index, struct options options,
               unsigned int size);
void parseIndex(struct searchIndex *index, struct options *options);
void getItemPath(const char *path, const char *item_name, char *directory_path);
void recursive(char *basePath, struct searchIndex *index);

#endif
