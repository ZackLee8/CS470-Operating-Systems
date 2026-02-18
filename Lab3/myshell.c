// Author: Zack Lee
// Date: 02/16/2026
// Lab 3

// Import necessary libraries for system calls, input/output, string manipulation, and file control
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

// Define a constant for the maximum length of the input line
#define MAX_LINE 1024  

// Main function of the shell
int main() {
    char line[MAX_LINE];
    while (1) { 
        printf("myshell> ");
        fflush(stdout);      
        if (!fgets(line, sizeof(line), stdin))
            break;   
 
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0)
            continue;

        // Handle quoted strings by replacing spaces within quotes with a placeholder character (e.g., '\a')
        int in_quotes = 0;
        for (int j = 0; line[j] != '\0'; j++) {
            if (line[j] == '\"') in_quotes = !in_quotes;
            if (in_quotes && line[j] == ' ') line[j] = '\a';
        }

        // create an array of pointers so that we can save each token of the string to later user execvp()
        char *argv[MAX_LINE / 2 + 1];

        // initialize a counter to keep track of each index or part of the string
        int i = 0;

        // create a char pointer to get the word and split it based on the different delimeters
        char *string = strtok(line, " \t\n");

        // loop through the string until there are no more tokens to process
        while (string != NULL){

            // Handle quoted strings by removing the surrounding quotes and replacing the placeholder character with spaces
            if (string[0] == '\"'){
                string++;
                // Remove the trailing quote if it exists
                size_t len = strlen(string);
                if (len > 0 && string[len - 1] == '\"') {
                    // Remove the trailing quote
                    string[len - 1] = '\0';
                }
            }

            // Replace the placeholder character with spaces
            for (int j = 0; string[j] != '\0'; j++){
                // Replace the placeholder character with a space
                if (string[j] == '\a') {
                    string[j] = ' ';
                }
            }

            // Save the token in the argv array and move to the next token
            argv[i++] = string;
            string = strtok(NULL, " \t\n");

        }

            // Null-terminate the argv array
            argv[i] = NULL;

            // If the user just pressed enter without typing a command, continue to the next iteration of the loop
            if (argv[0] == NULL) continue;

            // If the user types "exit", break out of the loop and end the shell
            if (strcmp(argv[0], "exit") == 0) break;

            // If the user types "cd", change the current working directory. If no directory is specified, change to the home directory
            if (strcmp(argv[0], "cd") == 0) {
                if (argv[1] == NULL) {
                    char *home = getenv("HOME");
                    chdir(home);
                        }
                 else {
                    if (chdir(argv[1]) != 0) {
                        perror("cd failed");
                    }
                }
                continue;
            }

            // create a new child process to run the commands given
            pid_t pid = fork();

            // check for fork failure
            if (pid < 0) {
                // Fork failed
                perror("Fork failed");
                exit(1);

            }

            // If this is the child process, execute the command using execvp
            if (pid == 0) {

                // Loop through the argv array to check for redirection operators and handle them accordingly
                int redirection_index = -1;
                for (int j = 0; argv[j] != NULL; j++) {
                    

                    // Check for output redirection (">"), input redirection ("<"), and append redirection (">>")
                    if (strcmp(argv[j], ">") == 0) {
                        // 1. Open the file for writing (create it if it doesn't exist, and truncate it if it does)
                        int fd = open(argv[j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd < 0) { perror("open"); exit(1); }

                        // 2. Redirect standard output to the file using dup2, and then close the file descriptor
                        dup2(fd, STDOUT_FILENO);
                        close(fd);

                        // 3. Set the redirection index to the current position if it hasn't been set yet
                        if (redirection_index == -1) redirection_index = j;
                    }
                    // Check for input redirection ("<")
                    if (strcmp(argv[j], "<") == 0){
                        // 1. Open the file for reading
                        int fd = open(argv[j+1], O_RDONLY);
                        if (fd < 0) {
                            perror("Opening for input has failed.");
                            exit(1);
                        }
                        // 2. Redirect standard input to the file using dup2, and then close the file descriptor
                        dup2(fd, STDIN_FILENO);
                        close(fd);

                        // 3. Set the redirection index to the current position if it hasn't been set yet
                        if (redirection_index == -1) redirection_index = j;
                    }

                    // Check for append redirection (">>")
                    if(strcmp(argv[j], ">>") == 0){
                        // 1. Open the file for appending (create it if it doesn't exist)
                        int fd = open(argv[j+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                        if (fd < 0) {
                            perror("Opening for append has failed.");
                            exit(1);
                        }

                        // 2. Redirect standard output to the file using dup2, and then close the file descriptor
                        dup2(fd, STDOUT_FILENO);
                        close(fd);

                        // 3. Set the redirection index to the current position if it hasn't been set yet
                        if (redirection_index == -1) redirection_index = j;
                    }
                }
                // After handling redirection, set the argv element at the redirection index to 
                // NULL to ensure that execvp only sees the command and its arguments, and not the redirection operators or file names
                if (redirection_index != -1) {
                argv[redirection_index] = NULL;
    }
                // Execute the command using execvp, passing the command and its arguments. If 
                // execvp returns, it means the command failed, so we print an error message and exit the child process
                execvp(argv[0], argv);
                // If execvp returns, it means the command failed
                perror("Execution failed");
                exit(1);

                
            // If this is the parent process, wait for the child process to finish before continuing to the next iteration of the loop
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
    }

    // When the user types "exit", break out of the loop and print a goodbye message before ending the shell
    printf("\nGoodbye!\n");
    return 0;
}
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = myshell
SRC = myshell.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
