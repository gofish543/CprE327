#ifndef RESOURCE_H
#define RESOURCE_H

#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define U_CHAR_MIN 0
#define U_CHAR_MAX 255

#define null NULL

#define TIME_HALF_SECOND_MICRO_SECONDS 500000

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

int random_number_between(int min, int max);
unsigned int hash3(unsigned int h1, unsigned int h2, unsigned int h3);
int error_check_fread(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream);
int error_check_fwrite(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream);
u_char convert_base10_to_char(u_int integerValue);
bool strstarts(std::string, std::string prefix);
int get_sign(int value);
int create_home_folders();

#endif
