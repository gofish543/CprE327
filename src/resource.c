#include "resource.h"

unsigned short offset = 0;

int randomNumberBetween(int min, int max) {
    int fd = open("/dev/urandom", O_RDWR);
    if (fd == 0) {
        srand(time(NULL));
    } else {
        long long buffer[1];
        lseek(fd, offset, 0);
        if (read(fd, buffer, 8) != 8) {
            srand(time(NULL));
        } else {
            srand(buffer[0]);
            offset += 8;

            if (offset > 2048) {
                offset = 0;
            }
        }
        close(fd);
    }

    return rand() % ((max + 1) - min) + min;
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