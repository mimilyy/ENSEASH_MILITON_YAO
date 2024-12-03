//Question 1

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define SIZE 128

//Function to display a welcome message
void writeMessage(const char* welcomeMessage){
    //Write the message to the standard output
    write(STDOUT_FILENO,welcomeMessage,strlen(welcomeMessage));
}

int main(){
    writeMessage("Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n");
    writeMessage("enseash \n");
}