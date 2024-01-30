#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 500000 // 20000 * 25

int is_not_printable(char c) {
    return (c <= 31 || c == 127) ? 1 : 0;
}

void print_offset(int n, int i, char *hex) {
    if (i > 7) {
        return;
    }
    print_offset(n / 16, i + 1, hex);
    write(1, &hex[n % 16], 1);
}

int main(int argc, char **argv) {
    int fd, i = 1, j = 0, h = 0, tracker = 1, check = 0, n = 0, bytes = 0;
    char *name;
    char hex[16] = "0123456789abcdef";
    char *buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    char *big_buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE * 2);

    while (i < argc) {
        name = argv[i];
        if (strcmp(name, "-C") == 0) {
            i++;
        } else {
            fd = open(name, O_RDONLY);
            bytes += read(fd, buffer, MAX_BUFFER_SIZE);
            strncat(big_buffer, buffer, bytes);
            i++;
        }
    }

    i = 0;
    while (big_buffer[i]) {
        print_offset(n, 0, hex);
        write(1, " ", 1);
        tracker = 1;
        while (tracker <= 16 && big_buffer[j]) {
            write(1, &hex[big_buffer[j] / 16], 1);
            write(1, &hex[big_buffer[j] % 16], 1);
            write(1, " ", 1);
            if (tracker % 8 == 0) {
                write(1, " ", 1);
            }
            n++;
            j++;
            tracker++;
        }
        while (tracker <= 16) {
            check = 1;
            write(1, "   ", 3);
            tracker++;
        }
        if (check) {
            write(1, " ", 1);
        }
        tracker = 1;
        write(1, "|", 1);
        while (tracker <= 16 && big_buffer[h]) {
            if (!is_not_printable(big_buffer[h])) {
                write(1, &big_buffer[h], 1);
            } else {
                write(1, ".", 1);
            }
            h++;
            tracker++;
        }
        write(1, "|", 1);
        write(1, "\n", 1);
        i += 16;
    }

    print_offset(n, 0, hex);
    write(1, "\n", 1);

    free(big_buffer);
    free(buffer);
    return 0;
}

