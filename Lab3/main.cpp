#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    const int BUFFER_SIZE = 1024;

    int memoryd;
    memoryd = open("memory.txt", O_RDWR | O_CREAT, 0666);

    if (memoryd == -1) {
        perror("Failed to open memory.txt");
        return -1;
    }

    if (ftruncate(memoryd, BUFFER_SIZE) == -1) { // размер файла (0/1)
        perror("Failed to truncate memory.txt");
        close(memoryd);
        return -1;
    }

    // отбрпжение файла в память
    char* buffer = (char*)mmap( NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memoryd, 0);

    if (buffer == MAP_FAILED) {
        perror("Failed to mmap buffer");
        close(memoryd);
        return -1;
    }
    close(memoryd);

    sem_t* sem = sem_open("mmap_sem", O_CREAT, 0777, 0); // семафор(0)

    if (sem == SEM_FAILED) {
        perror("Could not open semaphore");
        munmap(buffer, BUFFER_SIZE); // освобождает ресурсы
        return -1;
    }

    pid_t id = fork();

    if (id == -1) {
        perror("Fork failed");
        sem_close(sem);
        munmap(buffer, BUFFER_SIZE);
        return -1;
    } else if (id == 0) {  // ребенок 1

        if (fork() == 0) {  
            if (execl("./child", "./child", "mmap_sem", NULL) == -1) {
                perror("Failed to execl");
                sem_close(sem);
                munmap(buffer, BUFFER_SIZE);
                return -1;
            }
        } else {
            char c;
            c = getchar();
            int i = 0;

            while (c != EOF) {
                buffer[i++] = c;
                c = getchar();
            }
            buffer[i] = c;

            if (sem_post(sem) == -1) {
                perror("Failed to post sem");
                sem_close(sem);
                munmap(buffer, BUFFER_SIZE);
                return -1;
            }

            sem_close(sem);

            if (munmap(buffer, BUFFER_SIZE) == -1) {
                perror("Failed to munmap buffer");
                return -1;
            }

            int status;
            if (waitpid(0, &status, 0) == -1) {
                perror("Failed to wait for child process");
                return -1;
            }

            if (status != 0)
                perror("Child process exited with an error");

            return status;
        }
    } else {  // мама и пап

        if (fork() == 0) {  // 2 ребенок
            if (execl("./child", "./child", "mmap_sem", NULL) == -1) {
                perror("Failed to execl");
                sem_close(sem);
                munmap(buffer, BUFFER_SIZE);
                return -1;
            
        } else {  // Родитель
            if (waitpid(0, NULL, 0) == -1) {
                perror("Failed to wait for child process 1");
                sem_close(sem);
                munmap(buffer, BUFFER_SIZE);
                return -1;
            }
            if (waitpid(0, NULL, 0) == -1) {
                perror("Failed to wait for child process 2");
                sem_close(sem);
                munmap(buffer, BUFFER_SIZE);
                return -1;
            }
            sem_close(sem);
            munmap(buffer, BUFFER_SIZE);

            return 0;
        }
        
    }
    
    }

}

