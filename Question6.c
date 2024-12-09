//Question 6

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 128


///Function to display a welcome message (question 1)
void writeMessage(const char* welcomeMessage){
    //Write the message to the standard output
    write(STDOUT_FILENO,welcomeMessage,strlen(welcomeMessage));
}

// Function to display status
void convertMessage(const char *output, int val, float executionTime) {
    // Write the message to the standard output
    char message[SIZE]; 
    snprintf(message, sizeof(message), "enseash [%s:%d|%fms] %% \n", output, val, executionTime);
    writeMessage(message);
}

void displayPreviousCommand(float executionTime){
    int status;

    wait(&status);

    if (WIFEXITED(status)){
        convertMessage("exit", WEXITSTATUS(status), executionTime);
    }

    else if (WIFSIGNALED(status)){
        convertMessage("sign", WTERMSIG(status), executionTime);
    }

    writeMessage("enseash");
}

//Function to read the command entered by user (question 2)
ssize_t readCommand(char *message, size_t size){

    //Read the input
    ssize_t readBytes = read(STDERR_FILENO,message,size-1);

    //Check possible input issue
    if (readBytes <0){
        writeMessage("Read error occurred\n");
        exit(EXIT_FAILURE);
    }

    //Null-terminate the string to ensure it's properly ended
    message[readBytes] = '\0';

    //Return the number of bytes read
    return readBytes;
}

double execute(char *message){
    struct timespec startTime, endTime;
    float executionTime = 0;
    
    //Start time    
    if (clock_gettime(CLOCK_REALTIME, &startTime) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    //Child process
    pid_t pid = fork();

    // Check for possible error
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    //Parent process
    else if (pid != 0) {
        int status;
        wait(&status);
    }

    else{
        char *args[SIZE];
        int argc = 0;

        //break down into tokens (~word/argument)
        char *token = strtok(message, "");
        while (token !=NULL && argc <SIZE -1){
            args[argc++] = token;
            token = strtok(NULL, "");
        }
        args[argc] = NULL;
        execvp(args[0], args);

        //Error message if not executed
        writeMessage("Execution error");
        exit(EXIT_FAILURE);
    }

    //End time
    if (clock_gettime(CLOCK_REALTIME, &endTime) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    // Calculate the execution time
    executionTime = ((endTime.tv_sec - startTime.tv_sec)*1000) + (endTime.tv_nsec - startTime.tv_nsec) / 1e6;

    return executionTime;
}

//question 3
void exitFunction(char *message, ssize_t readBytes){
    if(strncmp(message,"exit",4)==0 || readBytes == 0)
    { // 'exit' or <ctrl>+d
        writeMessage("Bye bye...\n");
        exit(EXIT_SUCCESS);
    } 
}

int main() {
    //Set the buffer size for the input command (can be changed)
    const size_t bufferSize = SIZE; 
    char message[bufferSize];
    float executionTime = 0;
    
    //Display a welcome message (question 1)
    writeMessage("Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n");

    //main loop
    while (1) {

        ///Display the return code/signal of the previous command 
        displayPreviousCommand(executionTime);

        //Read the input message
        ssize_t readBytes = readCommand(message, bufferSize);

	    //Execute the input message and manage the exit
        exitFunction(message, readBytes);
        executionTime = execute(message);
    }
    return EXIT_SUCCESS;
}
