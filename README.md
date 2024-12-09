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

### Question 4

The prompt includes the return code or signal of the previously executed program.

To determine whether the program terminated due to a signal, conditional checks are used:
- WIFEXITED(status) evaluates to true if the child process terminated normally.
- WIFSIGNALED(status) evaluates to true if the child process was terminated by a signal.
- WTERMSIG(status) provides the number of the signal that caused the process to terminate.  

Displaying the message:
Instead of using printf, which can interfere with read()/write() operations, snprintf is used to store the message in a C string. This string is then output using a custom writeMessage function.

***Terminal output***

![q4](https://github.com/user-attachments/assets/af31a99d-551c-4454-a44f-4be622cc341e)

### Question 5

The execution time of a command is measured using the clock_gettime function from the time.h library. The reference clock utilized is CLOCK_REALTIME, provided by the Linux kernel.

Calling clock_gettime(CLOCK_REALTIME, &startTime) initializes the timer, and clock_gettime(CLOCK_REALTIME, &endTime) stops the timer. The execution time is then calculated in milliseconds, reflecting the typical duration of command execution.

The result is displayed by incorporating an "executionTime" parameter into the relevant functions.

***Terminal output***

![q5](https://github.com/user-attachments/assets/a8b1ad59-0944-4b1d-b904-55135c502bce)

### Question 6

Executing a complex command with arguments requires separating the command from its associated arguments. This is achieved by tokenizing the input string, splitting it based on spaces (" "), and storing the resulting tokens in a list.

The execvp() function is then used to execute the command, with all arguments stored sequentially in a (char *) array. The final element of the array is set to (char *)NULL to indicate the end of the argument list.

***Terminal output***

![q62](https://github.com/user-attachments/assets/d693bda0-830b-4027-8ac6-56f949e72d69)


![q61](https://github.com/user-attachments/assets/dd4cc9dd-c374-4dfe-b5e9-9cf1e2ac7995)

### Question 7

**Management of redirections for stdin and stdout using < and >**

Redirection of stdin and stdout is handled through a redirection function, which processes the < and > symbols and removes them from the argument list, ensuring compatibility with the functionality described in question 6. File handling is performed using the open() function.

**Input redirection (<):**

The < symbol is replaced with NULL to remove it from the argument list.
The file specified is opened in read-only mode using O_RDONLY.
The standard input (STDIN_FILENO) is redirected to the file using dup2.
The file descriptor is closed using close(fd) after redirection.

**Output redirection (>):**

The > symbol is replaced with NULL to remove it from the argument list.
The file is opened in write mode (O_WRONLY), created if it does not exist (O_CREAT), or truncated (O_TRUNC) if it already exists.
The standard output (STDOUT_FILENO) is redirected to the file using dup2.
The file descriptor is closed using close(fd) after redirection.

***Terminal output***

![q71b](https://github.com/user-attachments/assets/32f855cf-10bd-4e2d-b956-23d68a28ea4d)

![q72](https://github.com/user-attachments/assets/e99fae5f-83c5-48ad-a0fb-757eb0991564)



