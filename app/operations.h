#include "fs.h"

#ifndef OPERATIONS_H
#define OPERATIONS_H

#define OI_INITIAL_SIZE 100     // inital occurrence index size
#define BF_INITIAL_SIZE 200     // intital size of buffer


void _search(struct searchItem* item, struct options options);                
void _replace(struct searchItem* item, struct options options);                
void _info(struct searchItem* item, struct options options);                   

#endif
