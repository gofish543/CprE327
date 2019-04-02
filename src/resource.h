#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/time.h>

#define U_CHAR_MIN 0
#define U_CHAR_MAX 255

#define U_SHORT_MIN 0
#define U_SHORT_MAX 65535

#define U_INT_MIN 0
#define U_INT_MAX 4294967295

#define null NULL

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

void set_global_ncurses(bool value);
void debug_terminate();
int random_number_between(int min, int max);
unsigned int hash3(unsigned int h1, unsigned int h2, unsigned int h3);
int error_check_fread(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream);
int error_check_fwrite(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream);
u_char convert_base10_to_char(u_int integerValue);
bool strstarts(std::string, std::string prefix);
int get_sign(int value);
std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::vector<std::string> split(std::string str, char delimiter);
#endif
