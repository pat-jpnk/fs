#include "fs.h"
#include "colors.h"
#include "operations.h"
#include <bsd/string.h>
#include <dirent.h>
#include <errno.h> // defines errno, errno gets set by opendir() from dirent.h
#include <getopt.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int opt = 0;
    struct options options;

    if (argc < 2)
    {
        printf(KGRN "fs\n" KRESET "search" KYEL " -s [search term]" KRESET "\ninfo" KYEL " -i " KRESET "\nreplace " KYEL
                    "-r [target term] [replacement term]" KRESET "\n");
        return 0;
    }

    while ((opt = getopt(argc, argv, "s:r:ih")) != -1)
    {
        switch (opt)
        {
            case 's':
                options.function = SEARCH;
                options.search_term = optarg;
                break;

            case 'r':
                if (argc != 4)
                {
                    fprintf(stderr, "fs: option requires two arguments -- r\n");
                    exit(1);
                }

                if (strlen(argv[2]) == 0)
                {
                    fprintf(stderr, "fs: search term may not be zero-width\n");
                    exit(1);
                }
                options.function = REPLACE;
                options.search_term = optarg;
                options.replacement_term = argv[3];
                break;

            case 'i':
                options.function = INFO;
                break;

            case 'h':
                printf(KGRN "fs\n" KRESET "search" KYEL " -s [search term]" KRESET "\ninfo" KYEL " -i " KRESET
                            "\nreplace " KYEL " -r [search term] [replacement term]" KRESET "\n");
                return 0;

            default:
                options.function = NONE;
                break;
        }
    }

    if (options.function == NONE)
    {
        printf(KGRN "fs\n" KRESET "search" KYEL " -s [search term]" KRESET "\ninfo" KYEL " -i " KRESET "\nreplace " KYEL
                    "-r [target term] [replacement term]" KRESET "\n");
        return 0;
    }

    struct searchIndex index;
    index.size = 0;

    recursive(ROOTPATH, &index);
    parseIndex(&index, &options);

    return 0;
}

void addToIndex(struct searchIndex *si, const char *item_path, ino_t serial, fileType type)
{
    strcpy(si->items[si->size].path, item_path);
    si->items[si->size].st_ino = serial;
    si->items[si->size].type = type;
    si->size++;
}

void recursive(char *path, struct searchIndex *index)
{
    struct dirent *dp;
    DIR *dir = opendir(path);

    while ((dp = readdir(dir)) != NULL)
    {
        char item_path[MAX_PATH_SIZE] = "";
        char directory_path[MAX_PATH_SIZE] = "";
        const char *item_name = dp->d_name;

        if (filterFileName(item_name) == true)
        {
            strlcat(item_path, path, MAX_PATH_SIZE);
            strlcat(item_path, item_name, MAX_PATH_SIZE);
            fileType type = getFileStatus(item_path);

            switch (type)
            {
                case REGULAR:
                    addToIndex(index, item_path, dp->d_ino, type);
                    break;

                case DIREC:
                    getItemPath(path, item_name, &directory_path[0]);
                    recursive(directory_path, index);
                    break;

                case SYMLINK:
                    getItemPath(path, item_name, &directory_path[0]);
                    recursive(directory_path, index);
                    break;

                default:
                    break;
            }
        }
    }
    closedir(dir);
}

// TODO: simplify, make flexible
bool filterFileName(const char *item_name)
{
    if (strcmp(item_name, "a.out") != 0 && strcmp(item_name, ".git") != 0 && strcmp(item_name, ".gitignore") != 0 &&
        strcmp(item_name, ".") != 0 && strcmp(item_name, "..") != 0 && strcmp(item_name, "fs"))
    {
        return true;
    }
    return false;
}

fileType getFileType(mode_t m)
{ // @param m | st_mode attribute of struct stat
    switch (m & S_IFMT)
    { // S_IFMT   | bit mask for file type
        case S_IFSOCK:
            return SOCKET; // socket
        case S_IFLNK:
            return SYMLINK; // symbolic link
        case S_IFREG:
            return REGULAR; // regular file
        case S_IFBLK:
            return BLOCK; // block device
        case S_IFDIR:
            return DIREC; // directory
        case S_IFCHR:
            return CHAR; // char device
        case S_IFIFO:
            return PIPE; // pipe
        default:
            return UNKNOWN; // unknown
    }
}

fileType getFileStatus(const char *path)
{
    struct stat buff;
    struct stat *buffer = &buff;
    fileType fileType;

    int fileStatusRes = stat(path, buffer); // indicates whether it was gotten successfully =>
                                            // success = 0, failure = -1

    if (fileStatusRes == -1)
    {
        fileType = UNKNOWN;
    }
    else
    {
        fileType = getFileType(buffer->st_mode);
    }

    return fileType;
}

void getItemPath(const char *path, const char *item_name, char *directory_path)
{
    strlcat(directory_path, path, MAX_PATH_SIZE);
    strlcat(directory_path, item_name, MAX_PATH_SIZE);
    strlcat(directory_path, "/", MAX_PATH_SIZE);
}

// TODO: remove superfluous parameter for info
void parseFile(operation op, struct searchIndex *index, struct options options, unsigned int size)
{
    for (uint64_t i = 0; i < size; i++)
    {
        op(&(index->items[i]), options);
    }
}

void parseIndex(struct searchIndex *index, struct options *options)
{
    switch (options->function)
    {
        case SEARCH:
            parseFile(_search, index, *options, index->size);
            break;
        case REPLACE:
            parseFile(_replace, index, *options, index->size);
            break;
        case INFO:
            parseFile(_info, index, *options, index->size);
        default:
            break;
    }
}
