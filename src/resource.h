#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <string>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/time.h>

int error_check_fread(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream);
int error_check_fwrite(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream);
u_char convert_base10_to_char(u_int integerValue);
bool strstarts(std::string, std::string prefix);
int get_sign(int value);
std::string trim(std::string str, const std::string& chars = "\t\n\v\f\r ");
std::vector<std::string> split(std::string str, char delimiter);

#endif
