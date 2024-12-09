//Question 4

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#define SIZE 128

//Function to display a welcome message (question 1)
void writeMessage(const char* welcomeMessage){
    //Write the message to the standard output
    write(STDOUT_FILENO,welcomeMessage,strlen(welcomeMessage));
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

void execute(char *message){
    //Child process
    pid_t pid = fork();

    //Check for possible error
    if (pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    //Parent process
    else if (pid != 0){
        int status;
        wait(&status);
    }

    else{
        //execl is used to replace the current process with a new one, specified by the given arguments
        //"/bin/sh" is the path to the Linux shell executable
        //"sh" refers to the shell itself, indicating that the system's shell should be used
        //"-c" tells the shell to execute the command that follows it
        //message is the actual command to be executed by the shell
        execl("/bin/sh","sh","-c",message,(char *)NULL);

        //Error message if not executed
        writeMessage("Execution error");
        exit(EXIT_FAILURE);
    }
}

//Function to display status
void convertMessage(const char *commandName, int exitCodeOrSignal){
    //Write the message to the standard output
    char message[SIZE];
    snprintf(message, sizeof(message), "enseash [%s:%d] %% \n", commandName, exitCodeOrSignal);
    writeMessage(message);
}

void displayPreviousCommand(){
    int status; 

    wait(&status);

    if(WIFEXITED(status)){
        convertMessage("exit", WEXITSTATUS(status));
    }

    else if (WIFSIGNALED(status)){
        convertMessage("sign", WTERMSIG(status));
    }

    writeMessage("enseash");
}

//question 3
void exitFunction(char *message, ssize_t readBytes){
    if (strncmp(message, "exit",4)==0 || readBytes ==0)
    {//'exit' or <ctrl>+d
    writeMessage("Bye bye... \n");
    exit(EXIT_SUCCESS);
    }
}

int main(){
    //Set the buffer size for the input command (can be changed)
    const size_t bufferSize = SIZE;
    char message [bufferSize]; 

    //Display a welcome message (question 1)
    writeMessage("Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n");

    //main loop
    while(1){
        //Display the return code/signal of the previous command 
        displayPreviousCommand();

        //Read the input message
        ssize_t readBytes = readCommand(message,bufferSize);

        //Execute the input message and manage the exit
        exitFunction(message, readBytes);
        execute(message);
    }
    return EXIT_SUCCESS;
}