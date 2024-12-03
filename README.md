# LAB1 - Synthesys Practical Work Ensea in the Shell
### YAO Emilie and MILITON Pierre

## Objectives

The goal of this lab assignment is to create a lightweight shell named enseash, aimed at enhancing our understanding of shell environments and low-level system operations.
This shell will be able to:
- Execute commands and return to the prompt.
- Display the exit status and execution time of each command.
- Support advanced features such as command arguments, input/output redirection, and background process management.

## Answers
### Question 1

Displaying a welcome message in a Shell:
- importing the files unistd.h to use the write() function.
- write(int fd, const void *buf, size_t count);
  - fd is a file descriptor
  - buf is a pointer to the memory containing the data to be written
  - count is the number of bytes to write from the specified memory location.

***Terminal output***

![q1](https://github.com/user-attachments/assets/a5f6aa1a-00f9-43a5-9664-3691a05a52df)

### Question 2

The implementation focuses on coding the REPL (Read-Eval-Print Loop) process:  

**a. Read**  
A readCommand function is defined with two parameters: char *message and size_t size. The size is constrained by a maximum BUFSIZE, and message corresponds to the user input. The function uses the read() system call, defined as read(int fd, const void *buf, size_t count):
  - fd: Set to STDIN_FILENO, the standard input descriptor.
  - buf: Set to message, where the input written by the user is stored.
  - count: Specifies the input size.
  
**b. Execute**  
A child process is created. If the process is the parent, it waits for the child to terminate using the wait function. If the process is the child, the execl function is invoked to execute the command.  
The execl is defined as:  

int execl (const char *path, const char *arg, ...);  
  - Here, execXY is used, where X = l and Y = None. When X = l, arguments are passed individually, and the list is terminated with (char*)NULL.

The arguments used are:

- "/bin/sh": The path to the system shell. The execl function replaces the current process with the shell executable  
- "sh": The shell's call name (first argument)  
- "-c": Indicates that the next argument is a command to execute  
- message: Represents the command to execute  
- (char*)NULL: Signifies the end of arguments

**c. Print Loop**  
A continuous loop is implemented using while(1) in the main function to handle successive user inputs.

![2c](https://github.com/user-attachments/assets/753d0a80-0681-4f4a-93dc-84217f667567)


***Terminal output***

![q2](https://github.com/user-attachments/assets/cd3ea528-a750-4124-85b4-16052cbaca40)

### Question 3

The strncmp() function is utilized, defined as int strncmp(const char *first, const char *second, size_t length); from the string.h library.  
It compares the first and second string argument with the exact same "length" and return 0 if it's the same.

***Terminal output***

![q3](https://github.com/user-attachments/assets/a5855f33-d3e5-4ed4-89d2-692b6f21385b)
