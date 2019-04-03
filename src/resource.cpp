#include <sstream>
#include "resource.h"

int error_check_fread(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream) {
    if (fread(ptr, size, nmemb, stream) != nmemb) {
        return 1;
    }

    return 0;
}

int error_check_fwrite(void* __restrict ptr, size_t size, size_t nmemb, FILE* stream) {
    if (fwrite(ptr, size, nmemb, stream) != nmemb) {
        return 1;
    }
    return 0;
}

u_char convert_base10_to_char(u_int integerValue) {
    if (integerValue < 10) {
        return (u_char) ('0' + integerValue);
    } else {
        return (u_char) ('a' + (integerValue - 10));
    }
}

bool strstarts(std::string string, std::string prefix) {
    return string.rfind(prefix, 0) == 0;
}

int get_sign(int value) {
    if (value < 0) {
        return -1;
    } else {
        return 1;
    }
}

std::string& ltrim(std::string& str, const std::string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars) {
    return ltrim(rtrim(str, chars), chars);
}

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str); // Turn the string into a stream.
    std::string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}