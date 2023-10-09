#include <stdlib.h>
#include <stdio.h>
#include <limits.h> // Include for INT_MAX

typedef struct _process {
    int tickets;      // Number of lottery tickets for the process
    char name;         // Process name
    int length;        // Remaining execution time
    int t_stride;      // Time stride for the lottery scheduling
    int pass;          // Pass value used for lottery scheduling
    struct _process *next;
} process;

typedef struct _queue {
    int count;         // Number of processes in the queue
    process *first;    // Pointer to the first process in the queue
    process *last;     // Pointer to the last process in the queue
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
process *create_process(char name, int tickets, int length) {
    process *result = (process *)malloc(sizeof(process));
    result->tickets = tickets;
    result->length = length;
    result->name = name;
    result->t_stride = 10000 / tickets;  // Adjust as needed
    result->pass = 0;
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

// Lottery Stride Scheduling Function
void lottery_stride(queue *my_queue) {
    if (my_queue == NULL)
        return;

    while (my_queue->count > 0) {
        process *selected_process = NULL;
        int min_pass = INT_MAX;

        process *current_process = my_queue->first;

        // Find the process with the smallest pass value
        while (current_process != NULL) {
            if (current_process->pass < min_pass) {
                min_pass = current_process->pass;
                selected_process = current_process;
            }
            current_process = current_process->next;
        }

        if (selected_process != NULL) {
            printf("\n%s executes (RR)", selected_process->name);

            // Update pass and length
            selected_process->pass += selected_process->t_stride;

            if (selected_process->length > 0) {
                printf("\n%s tickets = %d, length = %d, t_stride = %d, pass = %d",
                       selected_process->name, selected_process->tickets, selected_process->length,
                       selected_process->t_stride, selected_process->pass);

                selected_process->length--;

                if (selected_process->length > 0)
                    add(my_queue, selected_process); // Re-add to the queue if more execution is needed
            } else {
                printf("\n%s is done", selected_process->name);
            }
        }
    }
}

// Main function
int main() {
    queue *my_q = create_queue();
    add(my_q, create_process('A', 100, 10));
    add(my_q, create_process('B', 50, 7));
    add(my_q, create_process('C', 200, 3));

    lottery_stride(my_q);

    return 0;
}
