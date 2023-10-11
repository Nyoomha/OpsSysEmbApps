#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _process {           // structure of process
    int pid;                // Process ID
    char name;              // Process name
    int length;             // Remaining execution time
    int tickets;            // Number of lottery tickets for the process
    int t_stride;           // Time stride for the lottery scheduling
    int pass;               // Pass value used for lottery scheduling
    struct _process * next;
} process;

typedef struct _queue {             // stucture for queue of processes
    int count;          // Number of processes in the queue
    process * first;    // Pointer to the first process in the queue
    process * last;     // Pointer to the last process in the queue
} queue;

queue * create_queue() {            // creates empty queue
    queue* result = (queue *)malloc(sizeof(queue));
    result->count = 0;
    result->first = NULL;
    result->last = NULL;

    return result;
}

// creates new process
process* create_process(int pid, char name, int length, int tickets, int latency) {
    process* result = (process *)malloc(sizeof(process)); // memory allocation
    result->length = length;
    result->name = name;
    result->pid = pid;
    result->tickets = tickets;
    result->t_stride = (latency/tickets);
    result->pass = 0;
    result->next = NULL;

    return result;
}

// adds process to the end of the queue
int add(queue* my_queue, process* new_proces) {
    // Check if queue NULL
    if(my_queue == NULL) 
        return -1;

    ++my_queue->count;

    // Check if queue empty
    if(my_queue->first == NULL){
        // If empty set both first and last pointers to new processes
        my_queue->first = new_proces;
        my_queue->last = new_proces;
    } else {
        // If not empty, append new process to the end
        my_queue->last->next = new_proces;
        my_queue->last = new_proces;
    }

    return 0;
}

// remove and returns first process in queue
process* remove_process(queue* my_queue) {
    if(my_queue == NULL) 
        return NULL;
   
    if(my_queue->first == NULL)     // Check if empty
        return NULL;

    --my_queue->count;

    process* result = my_queue->first; // Get reference to first process in queue

    my_queue->first = result->next; // Update first pointer to next process

    // If queue is empty update last pointer to NULL
    if(my_queue->count == 0)
        my_queue->last = NULL;

    // Set next pointer of removed process to NULL
    result->next = NULL;

    return result;
}

// returns total execution time
int rr(queue* my_queue, int time_slice) {
    int total = 0;
    if(my_queue == NULL) 
        return -1;

        // Remove first process from queue
        process* run_process = remove_process(my_queue);
        total += 1;  // increments total execution time

        /*...*/
        
        /*...*/

        //if(time_slice > run_process->length)
        if(time_slice >= run_process->length) // Determine actual time to be executed based on time slice and remaining length
            total += run_process->length;
        else 
            total += time_slice;

        // Update remaining length of process after execution
        run_process->length -= time_slice;
         // Update pass value and simulate progress for lottery scheduling
        run_process->pass += run_process->t_stride;
        printf("\nprocess %c is selected to run (length = %d) (tickets = %d) (pass = %d) (t_stride = %d)", run_process->name, run_process->length, run_process->tickets, run_process->pass, run_process->t_stride);
         // If process still has work to do go back in the queue
        if(run_process->length > 0)
            insert(my_queue, run_process);
    
}

int lottery_stride(queue* my_queue)
{
    
    if (my_queue == NULL)
        return -1;

    //rr(my_queue, 5);
    /*while (!check_pass(my_queue)) //if there are pass values that are equal we run RR until there are
    {
        rr(my_queue, 5);
    }*/

    //Add remaining code to make it loop until processes are finished

    while (my_queue -> count > 0) // keep looping until all processess are complete
    {
        rr(my_queue, 5); // 5 units of time

        if (my_queue -> count == 0)
        {
            printf("\nAll processes finished\n");
            break;
        }
    }

    return 0;
}

// inserts process into queue based on pass value
int insert(queue* my_queue, process* succ)
{
    process* pred_process = my_queue->first;

    if (my_queue->first == NULL || my_queue->first->pass >= succ->pass)
    {
        // insert new process at beginning of queue
        succ->next = my_queue->first;
        my_queue->first = succ;

        // If the queue empty, update last to point to the only element
        if (my_queue->last == NULL)
        {
            my_queue->last = succ;
        }

        return 0;
    }

    if (my_queue->last->pass <= succ->pass) // checks if pass value of last process in queue 
                                            // <= to the pass value of the process being inserted
    {
        succ->next = NULL;      // updates next line as the new last process
        my_queue->last = succ;  
        return 0;
    }

    for (int i = 0; i < my_queue->count - 1; i++)
    {
        // checks if new process goes in middle of queue 
        if (pred_process->pass <= succ->pass && succ->pass <= pred_process->next->pass)
        {
            // insert process in middle of queue
            succ->next = pred_process->next;
            pred_process->next = succ;
            return 0;
        }
        
        pred_process = pred_process->next; // mmove to next process in queue
    }

    return -1;
}

// Checks if all pass values in queue are different
bool check_pass(queue* my_queue)        // return false if there are equal pass
                                        // return true if all pass are different
{
    int pass_array[my_queue->count];
    int count = my_queue->count;
    for (int i = 0; i > count; i++)     //Loop for cloning pass values to size of queue length
    {   process* temp_process = remove_process(my_queue);
        pass_array[i] = temp_process->pass;
        add(my_queue, temp_process);
    }
    int length = sizeof(pass_array)/sizeof(pass_array[0]);
    int min = pass_array[0];
     for (int i = 0; i < length; i++) 
     {        
       if(pass_array[i] < min)    
           min = pass_array[i];
        if(i > 0 && min == pass_array[i])
            return false;
     }
     
    return true;
}

void init_shell()
{    printf("\n\n***********"
        "**********");
    printf("\n\n\tNO NAME");
    printf("\n\n\n\t%s", "\u263A");
    printf("\n\n*********"
        "************\n");
}

int main(int argc, char** argv) {
    init_shell();
    int latency = 10000;
    queue* my_q = create_queue();
    process * p_1 = create_process(1, 'a', 30, 100, latency);
    process * p_2 = create_process(2, 'b', 70, 50, latency);
    process * p_3 = create_process(3, 'c', 10, 200, latency);

    add(my_q, p_1);
    add(my_q, p_2);
    add(my_q, p_3);

    lottery_stride(my_q);

    return 0;
}