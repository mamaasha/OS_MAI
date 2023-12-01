#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *add(char *str, int capacity, int n, char c) {
    if (n == capacity) {
        capacity *= 2;
        str = (char *)realloc(str, sizeof(char) * capacity);
        if (str == NULL) {
            perror("realloc error");
            exit(1);
        }
    }
    str[n] = c;
    return str;
}

int main() {
    char c;
    int capacity = 100;
    int n = 0;
    char *str = (char *)malloc(sizeof(char) * capacity);
    if (str == NULL) {
        perror("malloc error");
        return 1;
    }
    while(read(0, &c, sizeof(char)) > 0) {
        if (c != '\n') {
            str = add(str, capacity, n, c);
            n++;
        } else {
            for (int i = n -1; i >= 0; i--)
                printf("%c", str[i]);
            
            printf("%c", c);
            n = 0;
        }
    }
    free(str);
    return 0;
}