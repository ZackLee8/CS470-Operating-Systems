// Zack Lee
// Lab 2
// 02/01/2026

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

// define number of child processes
#define NUM_CHILDREN 15

int main(){

    // print the parent process ID
    printf("Parent Process: PID = %d\n", getpid());
    pid_t child_pids[NUM_CHILDREN];

    // create a for loop to create 15 child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

            char *argv[6] = {0};
            const char *cmd_label = NULL;
        // check for fork failure
        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);

        }
        // child process
         if (pid == 0) {

            // first child prints my name!
            if(i == 0){
                // set up command to print name
                cmd_label = "echo \"Hello Zack Lee\"";
                argv[0] = "echo";
                argv[1] = "Hello Zack Lee";
                argv[2] = NULL;

                // execute the command
                printf("Child[%d] PID = %d command = %s\n", i, getpid(), cmd_label);
                execvp(argv[0], argv);

                perror("execvp failed");
                exit(127);

            }
            // two children abort() intentionally (signal termination)
            if(i == 11 || i == 12) {
                printf("Child[%d] PID = %d command = abort()\n", i, getpid());
                abort();
            }

            // two children commands that fail on purpose (run invladi commands intentionally)
            if(i == 13 || i == 14){
                cmd_label = "Not a valid command";
                argv[0] = "invalid_command";
                argv[1] = NULL;

                // execute the invalid command
                printf("Child[%d] PID = %d command = %s\n", i, getpid(), cmd_label);
                execvp(argv[0], argv);

                // If execvp returns, it means the command failed
                perror("Execution failed");
                exit(127);
            }

            // set a unique linux command to each child process
            switch(i) {
                case 1:
                    cmd_label = "date";
                    argv[0] = "date";
                    argv[1] = NULL;
                    break;
                case 2:
                    cmd_label = "ls -l";
                    argv[0] = "ls";
                    argv[1] = "-l";
                    argv[2] = NULL;
                    break;
                case 3:
                    cmd_label = "pwd";
                    argv[0] = "pwd";
                    argv[1] = NULL;
                    break;
                case 4:
                    cmd_label = "whoami";
                    argv[0] = "whoami";
                    argv[1] = NULL;
                    break;
                case 5:
                    cmd_label = "uname -a";
                    argv[0] = "uname";
                    argv[1] = "-a";
                    argv[2] = NULL;
                    break;
                case 6:
                    cmd_label = "id";
                    argv[0] = "id";
                    argv[1] = NULL;
                    break;
                case 7:
                    cmd_label = "hostname";
                    argv[0] = "hostname";
                    argv[1] = NULL;
                    break;
                case 8:
                    cmd_label = "uptime";
                    argv[0] = "uptime";
                    argv[1] = NULL;
                    break;
                case 9:
                    cmd_label = "df -h";
                    argv[0] = "df";
                    argv[1] = "-h";
                    argv[2] = NULL;
                    break;
                case 10:
                    cmd_label = "ps";
                    argv[0] = "ps";
                    argv[1] = NULL;
                    break;
                default:
                    cmd_label = "echo Default case";
                    argv[0] = "echo";
                    argv[1] = "Default case";
                    argv[2] = NULL;
                    break;

                
            }

            // execute the command
            printf("Child[%d] PID = %d command= %s\n", i, getpid(), cmd_label);
            execvp(argv[0], argv);
            // If execvp returns, it means the command failed
            perror("Execution failed");
            exit(1);

        }   else {

            child_pids[i] = pid;
        }

        

    }

    // Parent process waits for all child processes to complete and counts their exit statuses
    int normal_exit_0 = 0;
    int normal_exit_nonzero = 0;
    int terminated_by_signal = 0;

    // wait for each child process
    for (int i = 0; i < NUM_CHILDREN; i++) {
        int status = 0;

        // wait for specific child process
        pid_t w = waitpid(child_pids[i], &status, 0);
        if (w < 0) {
            perror("waitpid failed");
            continue;
        }

        // analyze the exit status
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);

            // print normal exit information
            printf("Parent: child[%d] PID=%d exited normally, code=%d\n",
                i, w, code);
            
            // increment counts based on exit code
            if (code == 0) normal_exit_0++;
            else normal_exit_nonzero++;
        }
        //  check if terminated by signal
        else if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);

            // print termination by signal
            printf("Parent: child[%d] PID=%d terminated by signal=%d\n",
                i, w, sig);
            // increment count
            terminated_by_signal++;
        }
        else {
            printf("Parent: child[%d] PID=%d ended in another way\n", i, w);
        }
    }

    // print summary
    printf("\n===== Summary (Parent PID=%d) =====\n", getpid());
    printf("Exited normally with exit code 0: %d\n", normal_exit_0);
    printf("Exited normally with non-zero exit code: %d\n", normal_exit_nonzero);
    printf("Terminated by a signal: %d\n", terminated_by_signal);


    return 0;
}