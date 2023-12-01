#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


// strace 
int main() { 
    char filename1[100], filename2[100];

    if (scanf("%100s", filename1) <= 0 || scanf("%100s", filename2) <= 0) {
        perror("scanf error");
        return -1;
    }
    
    int file1 = open(filename1, O_WRONLY | O_CREAT, 0644);
    int file2 = open(filename2, O_WRONLY | O_CREAT, 0644);

    if (file1 < 0 || file2 < 0) {
        perror("open error");
        return -1;
    }

    int fd1[2], fd2[2];

    if (pipe(fd1) != 0 || pipe(fd2) != 0) {
        perror("pipe error");
        return -1;
    }

    pid_t child1 = fork();

    if (child1 == -1) { // child1
        perror("fork error");
        return -1;
    } else if (child1 == 0) {
        close(file2);
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[1]);
        if (dup2(fd1[0], 0) == -1) { 
            perror("dup2 error");
            return -1;
        }
        if (dup2(file1, 1) == -1) {
            perror("dup2 error");
            return -1;
        }  
        close(file1);

        char *const *null = NULL;
        if (execv("./child", null) == -1) {
            perror("execv error");
            return -1;
        }

    } else { // child2
        pid_t child2 = fork();

        if (child2 == -1) { 
            perror("fork error");
            return -1;

        } else if (child2 == 0) {
            close(file1);
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[1]);
            if (dup2(fd2[0], 0) == -1) {
                perror("dup2 error");
                return -1;
            }
            if (dup2(file2, 1) == -1) {
                perror("dup2 error");
                return -1;
            }  
            close(file2);

            char *const *null = NULL;
            if (execv("./child", null) == -1) {
                perror("execv error");
                return -1;
            }

        } else { // parent 
            close(fd1[0]);
            close(fd2[0]);
            close(file1);
            close(file2);

            char c;
            int counter = 1;
            while (scanf("%c", &c) > 0) {
                if (counter % 2 == 0)
                    write(fd1[1], &c, sizeof(char)); 
                else
                    write(fd2[1], &c, sizeof(char));

                if (c == '\n')
                    counter++;    
            }  

            close(fd1[1]);
            close(fd2[1]);

        }
    }
    return 0;
}