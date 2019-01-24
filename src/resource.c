#include "resource.h"

int offset = 0;

int randomNumberBetween(int min, int max) {
    int fd = open("/dev/urandom", O_RDWR);
    if (fd == 0) {
        srand(time(NULL));
    } else {
        long long buffer[1];
        lseek(fd, offset, 0);
        read(fd, buffer, 8);
        close(fd);
        srand(buffer[0]);
    }

    return rand() % ((max + 1) - min) + min;
}