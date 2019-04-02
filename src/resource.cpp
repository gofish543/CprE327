#include <sstream>
#include "resource.h"

bool global_ncurses = false;
unsigned short global_offset = 0;

void set_global_ncurses(bool value) {
    global_ncurses = value;
}

void debug_terminate() {
    if (global_ncurses) {
        endwin();
    }
    std::cout << "Stopping here" << std::endl;
    exit(1);
}
int random_number_between(int min, int max) {
    int fd = open("/dev/urandom", O_RDWR);
    struct timeval time = {};
    ::gettimeofday(&time, null);
    u_int defaultSeed;
    if (fd == 0) {
        defaultSeed = hash3((u_int) time.tv_sec, (u_int) time.tv_usec, (u_int) getpid());
    } else {
        size_t buffer[1];
        lseek(fd, global_offset, 0);
        if (read(fd, buffer, sizeof(size_t)) != sizeof(size_t)) {
            defaultSeed = hash3((u_int) time.tv_sec, (u_int) time.tv_usec, (u_int) getpid());
        } else {
            defaultSeed = (u_int) buffer[0];
            global_offset += 8;

            if (global_offset > 2048) {
                global_offset = 0;
            }
        }
        close(fd);
    }

    std::random_device randomDevice;
    std::default_random_engine randomEngine(randomDevice());
    randomEngine.seed(defaultSeed);

    return ((u_int) randomEngine()) % ((max + 1) - min) + min;
}

unsigned int hash3(unsigned int h1, unsigned int h2, unsigned int h3) {
    return (((h1 * 2654435789U) + h2) * 2654435789U) + h3;
}

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