#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string>
#include <chrono>
#include <stdexcept>

std::string reverse(const std::string& s) {
    std::string str;
    for (int i = s.size() - 1; i >= 0; i--)
        str += s[i];
    return str;
}

int main(int argc, char** argv) {
    FILE* f1 = fopen("output1.txt", "a");
    if (!f1) {
        fprintf(stderr, "Failed to open output1.txt\n");
        return -1;
    }

    FILE* f2 = fopen("output2.txt", "a");
    if (!f2) {
        fclose(f1);
        fprintf(stderr, "Failed to open output2.txt\n");
        return -1;
    }

    int memoryd;
    memoryd = open("memory.txt", O_RDWR, 0777);
    if (memoryd == -1) {
        fclose(f1);
        fclose(f2);
        fprintf(stderr, "Failed to open memory.txt\n");
        return -1;
    }

    char* buffer = (char*)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, memoryd, 0); 
    close(memoryd);

    sem_t* sem = sem_open(argv[1], O_CREAT, 0777, 0);  // семафор ура
    if (sem == SEM_FAILED) {
        munmap(buffer, 1024);
        fclose(f1);
        fclose(f2);
        fprintf(stderr, "Failed to open semaphore\n");
        return -1;
    }

    size_t i = 0;

    sem_wait(sem);  // ждем

    std::string s = "";
    int line_number = 0;

    while (buffer[i] != '\0') {
        if (buffer[i] != ' ' && buffer[i] != '\n') {
            s += buffer[i];
        } else if (buffer[i] == '\n') {
            line_number++;

            if (line_number % 2 == 0) { 
                s = reverse(s);
                fprintf(f2, "%s\n", s.c_str());
            } else { 
                s = reverse(s);
                fprintf(f1, "%s\n", s.c_str());
            }

            s = "";
        }

        i++;
    }

    fflush(f1);
    fflush(f2);

    sem_close(sem);
    munmap(buffer, 1024);
    fclose(f1);
    fclose(f2);

    return 0;
}

