#include <stdlib.h>
#include <stdio.h>

// Structure definition for a process
typedef struct _process {
    int pid;            // Process ID
    char name;          // Process name
    int length;         // Remaining execution time

    struct _process *next;  // Pointer to the next process in the queue
} process;

// Structure definition for a queue
typedef struct _queue {
    int count;          // Number of processes in the queue
    process *first;     // Pointer to the first process in the queue
    process *last;      // Pointer to the last process in the queue
} queue;

// Function to create an empty queue
queue *create_queue() {
    queue *result = (queue *)malloc(sizeof(queue));
    result->count = 0;
    result->first = NULL;
    result->last = NULL;

    return result;
}

// Function to create a new process
process *create_process(int pid, char name, int length) {
    process *result = (process *)malloc(sizeof(process));
    result->length = length;
    result->name = name;
    result->pid = pid;
    result->next = NULL;

    return result;
}

// Function to add a process to the end of the queue
int add(queue *my_queue, process *new_process) {
    if (my_queue == NULL) 
        return -1;

    ++my_queue->count;

    if (my_queue->first == NULL) {
        my_queue->first = new_process;
        my_queue->last = new_process;
    } else {
        my_queue->last->next = new_process;
        my_queue->last = new_process;
    }

    return 0;
}

// Function to remove and return the first process from the queue
process *remove_process(queue *my_queue) {
    if (my_queue == NULL) 
        return NULL;

    if (my_queue->first == NULL) 
        return NULL;

    --my_queue->count;

    process *result = my_queue->first;

    my_queue->first = result->next;

    if (my_queue->count == 0)
        my_queue->last = NULL;

    result->next = NULL;
    return result;
}

// Round Robin Scheduling Function
// Returns total execution time
int rr(queue *my_queue, int time_slice) {
    int total = 0;
    if (my_queue == NULL) 
        return -1;

    while (my_queue->count > 0) {
        process *run_process = remove_process(my_queue);
        total += 1;

        // Print information about the selected process
        printf("\nprocess %c is selected to run (length = %d)", run_process->name, run_process->length);

        // Update total execution time based on the time slice and remaining process length
        if (time_slice > run_process->length)
            total += run_process->length;
        else 
            total += time_slice;

        // Update the remaining length of the process
        run_process->length -= time_slice;

        // Re-add the process to the queue if it still has execution time remaining
        if (run_process->length > 0)
            add(my_queue, run_process);
    }
}

// Main function
int main(int argc, char **argv) {
    // Create a queue and add processes to it
    queue *my_q = create_queue();
    add(my_q, create_process(1, 'a', 30));
    add(my_q, create_process(2, 'b', 70));
    add(my_q, create_process(3, 'c', 10));

    // Perform Round Robin scheduling with a time slice of 5
    rr(my_q, 5);

    return 0;
}
