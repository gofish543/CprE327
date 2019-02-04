#ifndef RESOURCE_H
#define RESOURCE_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

int randomNumberBetween(int min, int max);
int error_check_fread(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream);
int error_check_fwrite(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream);
int check_if_file_exists(const char* filename);

#endif
