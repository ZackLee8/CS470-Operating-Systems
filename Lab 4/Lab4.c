// Zack Lee
// Lab 4
// 02/24/2026

#include <stdio.h>
#include <stdlib.h>

// Define the process structure to hold information about each process, including its PID, 
// arrival time, burst time, waiting time, turnaround time, remaining time, and completion time.
struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
    int completion_time;
};

// Define a queue structure to manage the ready queue for SJF scheduling. The queue will hold the index 
// of the process in the processes array and a pointer to the next node in the queue.
struct queue {
    int process_index;
    struct queue *next;
};

// Function to copy an array of processes from a source array to a destination array. This function is used to 
// create separate copies of the process data for SJF and Round Robin scheduling, allowing both algorithms to run independently without modifying the original process data.
void copy_processes(struct process dest[], struct process src[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// Function to enqueue a process index into the SJF ready queue in sorted order based on remaining time, arrival time, and PID. 
// This function is used to maintain the ready queue for SJF scheduling, ensuring that processes are ordered according to the SJF criteria.
void enqueue_sjf_sorted(struct queue **head, struct process processes[], int idx) {

    // Create a new queue node for the process index being enqueued. The program allocates memory for the new node and initializes its process
    // index and next pointer. If memory allocation fails, the program will print an error message and exit.
    struct queue *node = (struct queue *)malloc(sizeof(struct queue));
    if (node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Initialize the new node with the process index and set its next pointer to NULL. The program will use this node to insert the process 
    // index into the ready queue in the correct order.
    node->process_index = idx;
    node->next = NULL;

    // If the ready queue is empty, the new node becomes the head of the queue. The program checks if the head pointer is NULL and, if so, assigns 
    // the new node to be the head of the queue and returns.
    if (*head == NULL) {
        *head = node;
        return;
    }

    // Traverse the ready queue to find the correct position for the new node based on the SJF scheduling criteria. The program compares the remaining time,
    // arrival time, and PID of the processes to determine where to insert the new node in
    struct queue *curr = *head;
    struct queue *prev = NULL;

    // The program will continue to traverse the queue until it finds a node where the new process has a shorter remaining time, an earlier arrival time, or a 
    // smaller PID (in case of ties).
    while (curr != NULL) {
        int a = curr->process_index;

        // Compare the remaining time of the new process with the current node's process. If the new process has a shorter remaining time, it should be inserted before the current node.
        if (processes[idx].remaining_time < processes[a].remaining_time) {
            break;
        }

        // If the remaining times are equal, compare the arrival times. The process with the earlier arrival time should be inserted first.
        // If the arrival times are also equal, compare the PIDs. The process with the smaller PID should be inserted first. This ensures a consistent ordering of processes in the ready queue.
        if (processes[idx].remaining_time == processes[a].remaining_time) {
            if (processes[idx].arrival_time < processes[a].arrival_time) {
                break;
            }
            if (processes[idx].arrival_time == processes[a].arrival_time &&
                processes[idx].pid < processes[a].pid) {
                break;
            }
        }

        // Move to the next node in the queue if the new process does not have a higher priority than the current node. The program updates
        // the prev and curr pointers to continue traversing the queue.
        prev = curr;
        curr = curr->next;
    }

    node->next = curr;
    if (prev == NULL) {
        *head = node;
    } else {
        prev->next = node;
    }
}

// Function to dequeue the head of the queue and return the process index. This function is used to retrieve the next
// process to execute based on the SJF scheduling criteria,
int dequeue_head(struct queue **head) {
    if (*head == NULL) {
        return -1;
    }

    // Store the process index of the head node, update the head pointer to the next node in the queue, free the memory 
    // allocated for the dequeued node, and return the process index.
    struct queue *temp = *head;
    int idx = temp->process_index;
    *head = temp->next;
    free(temp);
    return idx;
}

// Function to clear the queue and free allocated memory. This function is used to clean up the queue after scheduling is complete, 
// ensuring that all dynamically allocated memory for the queue nodes is properly freed to prevent memory leaks.
void clear_queue(struct queue **head) {
    // Loop through the queue and free each node until the head is NULL. The program will continue to free nodes until the entire 
    // queue has been cleared, ensuring that all memory allocated for the queue is released.
    while (*head != NULL) {
        struct queue *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

// Function to print the results of the scheduling algorithms, including waiting time and 
// turnaround time for each process, as well as average waiting time and average turnaround time.
void print_results(const char *title, struct process processes[], int n) {
    // Validate input
    double total_waiting = 0.0;
    double total_turnaround = 0.0;

    // Print the header for the results and then loop through each process to print its details, including PID,
    // arrival time, burst time, waiting time, and turnaround time.
    printf("\n%s Results:\n", title);
    printf("PID\tArrival\tBurst\tWaiting\tTurnaround\n");

    // Loop through each process to print its details and calculate the total waiting time and total turnaround time for all processes. 
    // The program will display the results in a tabular format for easy comparison.
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time);
        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
    }

    // Print the average waiting time and average turnaround time for all processes. The program calculates these averages by 
    // dividing the total waiting time and total turnaround time by the number of processes (n) and displays the results with two decimal places.
    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
}

// Function to run the preemptive SJF scheduling algorithm
void run_preemptive_sjf(struct process processes[], int n) {
    // Validate input
    if (n <= 0) {
        printf("\nNo processes to schedule.\n");
        return;
    }

    // Initialize variables for SJF scheduling, including the count of completed processes, 
    // current time, an array to track which processes are in the ready queue, the last process 
    // ID that was executed, and a counter for printed segments in the execution order. A queue is 
    // also initialized to manage the processes based on their remaining time.
    int completed = 0;
    int current_time = 0;
    int in_ready[n];
    int last_pid = -2;
    int printed_segments = 0;
    struct queue *head = NULL;

    // Initialize the in_ready array to track which processes are currently in the ready queue. Initially, 
    // all processes are not in the ready queue, so the array is set to 0 for all processes.
    for (int i = 0; i < n; i++) {
        in_ready[i] = 0;
    }

    // Print the execution order for SJF scheduling. The program will display the order in which processes are executed based on the
    printf("\nSJF (Preemptive) Execution Order:\n");

    // Main loop to process the SJF scheduling until all processes are completed. The program checks for processes that have arrived and are not yet completed
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0 &&
                in_ready[i] == 0) {
                enqueue_sjf_sorted(&head, processes, i);
                in_ready[i] = 1;
            }
        }

        // If the ready queue is empty, the program will print "Idle" if the last executed process was not idle, 
        //and then increment the current time to continue checking for arriving processes in the next iteration.
        if (head == NULL) {
            if (last_pid != -1) {
                if (printed_segments > 0) {
                    printf(" -> ");
                }
                printf("Idle");
                printed_segments++;
                last_pid = -1;
            }
            current_time++;
            continue;
        }

        // Dequeue the next process index from the ready queue and update the in_ready array to indicate that this process is no longer in the ready queue.
        int idx = dequeue_head(&head);
        in_ready[idx] = 0;

        // If the process being executed is different from the last executed process, the program will print the process ID. This helps to visualize the 
        // execution order of processes, including any idle periods.
        if (last_pid != processes[idx].pid) {
            if (printed_segments > 0) {
                printf(" -> ");
            }
            // Print the process ID of the currently executing process. The program uses the process ID to identify which process is being executed at each step in the SJF scheduling.
            printf("P%d", processes[idx].pid);
            printed_segments++;
            last_pid = processes[idx].pid;
        }

        // Update the remaining time of the currently executing process and increment the current time. The program simulates the execution of the process by decrementing its 
        //remaining time and advancing the current time accordingly.
        processes[idx].remaining_time--;
        current_time++;

        // If the process has completed execution (remaining time is 0), the program will mark it as completed, calculate its completion time, turnaround time, and waiting time.
        if (processes[idx].remaining_time == 0) {
            completed++;
            processes[idx].completion_time = current_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        } else {
            enqueue_sjf_sorted(&head, processes, idx);
            in_ready[idx] = 1;
        }
    }

    // Print a newline after the execution order and then print the results for SJF scheduling. The program will display the waiting time and turnaround time for each process, 
    // as well as the average waiting time and average turnaround time for all processes.
    printf("\n");
    print_results("SJF", processes, n);
    clear_queue(&head);
}

// Function to run the Round Robin scheduling algorithm with a specified time quantum
void run_round_robin(struct process processes[], int n, int time_quantum) {

    // Validate inputs
    if (n <= 0 || time_quantum <= 0) {
        printf("\nInvalid number of processes or time quantum.\n");
        return;
    }

    // Initialize variables for Round Robin scheduling
    int completed = 0;
    int current_time = 0;
    int front = 0;
    int rear = 0;
    int in_queue[n];
    int queue[10000];

    // Initialize in_queue array to track which processes are currently in the queue
    for (int i = 0; i < n; i++) {
        in_queue[i] = 0;
    }

    // Print the execution order for Round Robin scheduling
    printf("\nRR (Time Quantum = %d) Execution Order:\n", time_quantum);

    // Main loop to process the Round Robin scheduling until all processes are completed
    while (completed < n) {
        // Enqueue processes that have arrived and are not yet completed
        for (int i = 0; i < n; i++) {

            // Check if the process has arrived, has remaining time, and is not already in the queue
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && in_queue[i] == 0) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        // If the queue is empty, increment the current time and continue to the next iteration
        if (front == rear) {
            current_time++;
            continue;
        }

        // Dequeue the next process index from the queue
        int idx = queue[front++];
        in_queue[idx] = 0;

        // If the process has no remaining time, skip it and continue to the next iteration
        if (processes[idx].remaining_time <= 0) {
            continue;
        }

        // Print the process being executed
        printf("P%d ", processes[idx].pid);

        // Calculate the actual run time for the process, which is the minimum of the time quantum and the remaining time of the process
        int run_time = time_quantum;
        if (processes[idx].remaining_time < run_time) {
            run_time = processes[idx].remaining_time;
        }

        // Update the remaining time of the process and the current time
        processes[idx].remaining_time -= run_time;
        current_time += run_time;

        // Enqueue any new processes that have arrived during the execution of the current process
        for (int i = 0; i < n; i++) {
            // Check if the process has arrived, has remaining time, is not already in the queue, and is not the currently executing process
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && in_queue[i] == 0 && i != idx) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        // If the current process still has remaining time after its time quantum, re-enqueue it to the end of the queue. Otherwise, mark it as completed and calculate its completion, turnaround, and waiting times.
        if (processes[idx].remaining_time > 0) {
            queue[rear++] = idx;
            in_queue[idx] = 1;
        } else {
            // Process has completed execution
            completed++;
            processes[idx].completion_time = current_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        }
    }

    // Print a newline after the execution order and then print the results for Round Robin scheduling
    printf("\n");
    print_results("RR", processes, n);
}

// Main function to execute the scheduling algorithms
int main(void) {
    int n;
    printf("How many processes? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes.\n");
        return 0;
    }

    // Define an array of processes to hold the input data for each process, including arrival time and burst time. 
    // The user will be prompted to enter these values for each process, and the program will validate the input to 
    // ensure that arrival times are non-negative and burst times are positive.
    struct process processes[n];

    // Loop to get the arrival time and burst time for each process from the user. The program checks for valid input
    // and initializes the process structure with the provided data.
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        if (scanf("%d %d", &processes[i].arrival_time, &processes[i].burst_time) != 2) {
            printf("Invalid input.\n");
            return 0;
        }

        // Validate that the arrival time is non-negative and the burst time is positive. If the input is invalid, the program will print an error message and exit.
        if (processes[i].arrival_time < 0 || processes[i].burst_time <= 0) {
            printf("Arrival time must be >= 0 and burst time must be > 0.\n");
            return 0;
        }

        // Initialize the process structure with the provided data, including setting the process ID, remaining time, 
        // waiting time, turnaround time, and completion time.
        processes[i].pid = i + 1;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completion_time = 0;
    }

    // Create separate arrays for SJF and Round Robin scheduling to avoid modifying the original process data. The program
    struct process sjf_processes[n];
    struct process rr_processes[n];
    int quantum;

    // Copy the original process data into the separate arrays for SJF and Round Robin scheduling. This allows the program 
    // to run both scheduling algorithms independently without affecting each other's data.
    copy_processes(sjf_processes, processes, n);
    copy_processes(rr_processes, processes, n);

    // Run the preemptive SJF scheduling algorithm and print the results. The program will execute the SJF scheduling based on the
    run_preemptive_sjf(sjf_processes, n);

    // Prompt the user to enter the time quantum for Round Robin scheduling and validate the input. The program checks that the time
    // quantum is a positive integer before proceeding with the Round Robin scheduling.
    printf("\nEnter time quantum for RR: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Invalid time quantum.\n");
        return 0;
    }

    // Run the Round Robin scheduling algorithm with the specified time quantum and print the results. The program will execute the 
    // Round Robin scheduling based on the user-defined time quantum and display the results accordingly.
    run_round_robin(rr_processes, n, quantum);

    return 0;
}
