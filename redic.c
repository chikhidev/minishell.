#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    // Step 1: Open the file for writing
    int pfd = open("file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // Step 2: Save the current stdout
    int saved_stdout = dup(STDOUT_FILENO);

    // Step 3: Redirect stdout to the file
    dup2(pfd, STDOUT_FILENO);

    // Step 4: Fork a child process
    pid_t pid = fork();

    if (pid == 0)
        execlp("ls", "ls", NULL);
    else
        waitpid(pid, NULL, 0);

    dup2(saved_stdout, STDOUT_FILENO);

    close(pfd);
    close(saved_stdout);

    // Output to the terminal
    printf("This message is printed to the terminal.\n");

    return EXIT_SUCCESS;
}