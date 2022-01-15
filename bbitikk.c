#include <stdio.h>

#define max_size 255
#define max_size_bin 10000
#define module 10

typedef struct {
    int size;
    char* s;
} long_number;

long_number division_by_two(long_number l) {
    int k = 0;
    for (int i = 0; i < l.size; i++) {
        int x = (int) (l.s[i] - '0') + k * module;
        l.s[i] = (char) (x/2 + '0');
        k = x % 2;
    }
    return l;
}

int comparison_with_zero(long_number l) {
    int f = 0;
    for (int i = 0; i < l.size; i++) {
        if (l.s[i] != '0') {
            f = 1;
            return f;
        }
    }
    return f;
}


void print(long_number l) {
    char y[max_size_bin];
    int i = 0;
    while (comparison_with_zero(l)) {
        int percent = (int) (l.s[l.size - 1] - '0') % 2;
        if (percent == 1) {
            y[i] = '1';
        }
        else {
            y[i] = '0';
        }
        i += 1;
        l = division_by_two(l);
    }
    for (int t = i - 1; t >= 0; t--) {
        printf("%c", y[t]);
    }
}

int main() {
    int size = 0;
    char s[max_size];
    for (int i = 0; i < max_size; i++) {
        char c = 0;
        scanf("%c", &c);
        if (size == 0) {
            if (c == '\n' || c == '\0') {
                break;
            }
            if (c >= '0' && c <= '9') {
                s[size] = c;
                size += 1;
            } else {
                printf("%c", c);
            }
        } else {
            long_number l;
            l.s = s;
            l.size = size;
            if (c == '\n' || c == '\0') {
                print(l);
                break;
            }
            if (c >= '0' && c <= '9') {
                s[size] = c;
                size += 1;
            } else {
                print(l);
                printf("%c", c);
                size = 0;
            }
        }
    }
    return 0;
}
