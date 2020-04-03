#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int fd1[2];
    pid_t child;

    if (pipe(fd1)==-1){
        exit(1);    // gagal
    }
    child = fork();

    if (child < 0){
        exit(EXIT_FAILURE); //proses membuat anak gagal
    }

    else if (child > 0){
        close(1);  // close writing
        dup(fd1[1]); //write
        close(fd1[0]); //close reading
        execlp("ls","ls",NULL);
    }
    else{
    
        close(0);   //close reading
        dup(fd1[0]); //read
        close(fd1[1]); //close writing
        execlp("wc","wc", "-l", NULL);
    }
}