#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _process {
    int pid;
    char name;
    int length;
    int tickets;
    int t_stride;
    int pass;

    struct _process * next;
} process;

typedef struct _queue {
    int count;
    process * first;
    process * last;
} queue;

queue * create_queue() {
    queue* result = (queue *)malloc(sizeof(queue));
    result->count = 0;
    result->first = NULL;
    result->last = NULL;

    return result;
}

process* create_process(int pid, char name, int length, int tickets, int latency) {
    process* result = (process *)malloc(sizeof(process));
    result->length = length;
    result->name = name;
    result->pid = pid;
    result->tickets = tickets;
    result->t_stride = (latency/tickets);
    result->pass = 0;
    result->next = NULL;

    return result;
}

int add(queue* my_queue, process* new_proces) {
    if(my_queue == NULL) 
        return -1;

    ++my_queue->count;

    if(my_queue->first == NULL){
        my_queue->first = new_proces;
        my_queue->last = new_proces;
    } else {
        my_queue->last->next = new_proces;
        my_queue->last = new_proces;
    }

    return 0;
}


process* remove_process(queue* my_queue) {
    if(my_queue == NULL) 
        return NULL;

    if(my_queue->first == NULL) 
        return NULL;

    --my_queue->count;

    process* result = my_queue->first;

    my_queue->first = result->next;

    if(my_queue->count == 0)
        my_queue->last = NULL;

    result->next = NULL;
    return result;
}

// returns total execution time
int rr(queue* my_queue, int time_slice) {
    int total = 0;
    if(my_queue == NULL) 
        return -1;

        process* run_process = remove_process(my_queue);
        total += 1;

        /*...*/
        
        /*...*/

        //if(time_slice > run_process->length)
        if(time_slice >= run_process->length)
            total += run_process->length;
        else 
            total += time_slice;

        run_process->length -= time_slice;
        run_process->pass += run_process->t_stride;
        printf("\nprocess %c is selected to run (length = %d) (tickets = %d) (pass = %d) (t_stride = %d)", run_process->name, run_process->length, run_process->tickets, run_process->pass, run_process->t_stride);
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

    while (my_queue -> count > 0)
    {
        rr(my_queue, 5);

        if (my_queue -> count == 0)
        {
            printf("All processes finished\n");
            break;
        }
    }

    return 0;
}

int insert(queue* my_queue, process* succ) //Function for inserting a successor process where it should be in the queue (sorted by size of pass)
{
    process* pred_process = my_queue->first; 
    if(my_queue->first->pass>=succ->pass)
        {
            succ->next = my_queue->first;
            my_queue->first = succ;
            return 0;
        }
    if(my_queue->last->pass<=succ->pass)
    {
        succ->next = NULL;
        my_queue->last = succ;
        return 0;
    }
    //for(int i = 0; i > my_queue->count - 1; i++)
    for(int i = 0; i < my_queue -> count - 1; i++)
    {
        if(pred_process->pass<=succ->pass && succ->pass<=pred_process->next->pass)
            {
                succ->next = pred_process->next;
                pred_process->next = succ;
                return 0;
            }
        pred_process = pred_process->next;
    }
    return -1;

}

bool check_pass(queue* my_queue) //return false if there are equal pass, return true if all pass are different
{
    int pass_array[my_queue->count];
    int count = my_queue->count;
    for (int i = 0; i > count; i++) //Loop for cloning pass values to size of queue length
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

int main(int argc, char** argv) {
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