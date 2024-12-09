//Question 7

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SIZE 128

/// Function to display a welcome message (question 1)
void writeMessage(const char* welcomeMessage) {
    write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
}

// Function to read the command entered by user (question 2)
ssize_t readCommand(char *message, size_t size) {
    ssize_t readBytes = read(STDERR_FILENO, message, size - 1);
    if (readBytes < 0) {
        writeMessage("Read error occurred\n");
        exit(EXIT_FAILURE);
    }
    message[readBytes] = '\0';
    return readBytes;
}

// Function to parse the command and arguments for execvp
void parseCommand(char *message, char **argv) {
    char *token = strtok(message, " \n");
    int i = 0;
    while (token != NULL) {
        argv[i++] = token;
        token = strtok(NULL, " \n");
    }
    argv[i] = NULL;
}

// Function to check for redirections and manage file descriptors
void handleRedirections(char **argv) {
    int i = 0;
    while (argv[i] != NULL) {
        if (strcmp(argv[i], ">") == 0) {
            // Output redirection
            int fd = open(argv[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            argv[i] = NULL; // Remove the redirection operator and file from arguments
        } else if (strcmp(argv[i], "<") == 0) {
            // Input redirection
            int fd = open(argv[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            argv[i] = NULL; // Remove the redirection operator and file from arguments
        }
        i++;
    }
}

double execute(char *message) {
    struct timespec startTime, endTime;
    float executionTime = 0;

    if (clock_gettime(CLOCK_REALTIME, &startTime) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else if (pid != 0) {
        int status;
        wait(&status);
    }

    else {
        char *argv[SIZE];
        parseCommand(message, argv);
        handleRedirections(argv); // Handle redirections before executing

        execvp(argv[0], argv);

        perror("Execution error");
        exit(EXIT_FAILURE);
    }

    if (clock_gettime(CLOCK_REALTIME, &endTime) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    executionTime = ((endTime.tv_sec - startTime.tv_sec) * 1000) + (endTime.tv_nsec - startTime.tv_nsec) / 1e6;
    return executionTime;
}

// Function to display status
void convertMessage(const char *output, int val, float executionTime) {
    char message[SIZE];
    snprintf(message, sizeof(message), "enseash [%s:%d|%fms] %% \n", output, val, executionTime);
    writeMessage(message);
}

void displayPreviousCommand(float executionTime) {
    int status;
    wait(&status);

    if (WIFEXITED(status)) {
        convertMessage("exit", WEXITSTATUS(status), executionTime);
    } else if (WIFSIGNALED(status)) {
        convertMessage("sign", WTERMSIG(status), executionTime);
    }

    writeMessage("enseash");
}

// Question 3
void exitFunction(char *message, ssize_t readBytes) {
    if (strncmp(message, "exit", 4) == 0 || readBytes == 0) {
        writeMessage("Bye bye...\n");
        exit(EXIT_SUCCESS);
    }
}

int main() {
    const size_t bufferSize = SIZE;
    char message[bufferSize];
    float executionTime = 0;

    writeMessage("Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n");

    while (1) {
        displayPreviousCommand(executionTime);

        ssize_t readBytes = readCommand(message, bufferSize);

        exitFunction(message, readBytes);
        executionTime = execute(message);
    }
    return EXIT_SUCCESS;
}
        executionTime = execute(message);
    }
    return EXIT_SUCCESS;
}
