// Zack Lee
// 03/03/2026
// LibFS.h

#ifndef LIBFS_H
#define LIBFS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void createFile(void);
bool openFile(char *fileName);
void writeFile(const char *fileName);
void readFile(const char *fileName);
void closeFile(char *fileName);
void deleteFile(void);
void pressEnterToContinue(void);

#endif
