/*
 * CP386 Assignment 4 Submission
 * Student Name: Carson Aziz
 * Student ID: 180800140
 * Student Email: aziz0140@mylaurier.ca
 * github login id: aziz0140
 * github repo url: https://github.com/aziz0140/Assignment-4
 * Student Task Split: Question 1 is done by Raiyan Jugbhery, Question 2 is done by Carson Aziz
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MEM_BLOCK_SIZE 10

typedef struct process_s
{
    char* pid;
    int r_relocation;
    int r_limit;
    int size;
} process_t;

typedef struct hole_s
{
    int start;
    int end;
    int len;
} hole_t;

uint8_t* memory;
int mem_size_in_bytes;
int mem_allocated_in_bytes;
int mem_free_in_bytes;

process_t* processes;
int num_processes = 0;

hole_t* holes;
int num_holes = 0;

void request(char* pid, int size, char strategy);
void release(char* pid);
void status();

int main(int argc, char** argv) {
    // Allocate given number of bytes of memory
    memory = (uint8_t*)malloc(sizeof(uint8_t) * atoi(argv[1]));
    mem_size_in_bytes = atoi(argv[1]);
    mem_allocated_in_bytes = 0;
    mem_free_in_bytes = mem_size_in_bytes;
    printf("Allocated %i bytes of memory\n", mem_size_in_bytes);

    // Initialize processes array to max number of possible processes
    int max_processes = mem_size_in_bytes / MEM_BLOCK_SIZE;
    processes = (process_t*)malloc(sizeof(process_t) * max_processes);
    num_processes = 0;

    // Initialize holes array to max number of possible holes
    int max_holes = ceil((float)(mem_size_in_bytes / MEM_BLOCK_SIZE) / 2);
    holes = (hole_t*)malloc(sizeof(hole_t) * max_holes);
    hole_t hole;
    hole.start = 0;
    hole.len = mem_size_in_bytes;
    hole.end = mem_size_in_bytes - 1;
    holes[0] = hole;
    num_holes++;

    // Accept commands from user and perform corresponding actions
    char command[6];
    while (true)
    {
        printf("command>");
		scanf("%s", command);

        if (strcmp(command, "RQ") == 0)
        {
            char pid[3];
            int size;
            char strategy;
            scanf("%s", pid);
            scanf("%i", &size);
            scanf(" %c", &strategy);

            request(pid, size, strategy);
        }
        else if (strcmp(command, "RL") == 0)
        {
            char pid[3];
            scanf("%s", pid);

            release(pid);
        }
        else if (strcmp(command, "Status") == 0)
        {
            status();
        }
        else if (strcmp(command, "Exit") == 0)
        {
            break;
        }
    }
    


    // Free memory at end of program
    free(memory);
}

void request(char* pid, int size, char strategy)
{
    // Check if there exists a hole with enough memory
    hole_t* hole = NULL;
    for (int i = 0; i < num_holes; i++)
    {
        if (size <= holes[i].len)
        {
            hole = &holes[i];
            break;
        }
    }

    // Return if no hole with sufficient size is available
    if (hole == NULL)
    {
        printf("No hole of sufficient size\n");
        return;
    }

    // Create process and set relocation/limit registers
    process_t process;
    process.pid = (char*)malloc(sizeof(char) * 3);
    strcpy(process.pid, pid);
    process.r_relocation = hole->start;
    process.size = ceil((float)size/MEM_BLOCK_SIZE) * MEM_BLOCK_SIZE;
    process.r_limit = process.size - 1;
    processes[num_processes] = process;

    // Update memory information
    num_processes++;
    mem_free_in_bytes -= process.size;
    mem_allocated_in_bytes += process.size;

    // Update hole information
    hole->len -= process.r_limit + 1;
    if (hole->len != 0)
    {
        hole->start += process.r_limit + 1;
    }
    else
    {
        num_holes--;
    }

    printf("Successfully allocated %i to process %s\n", process.size, process.pid);
}

void release(char* pid)
{
    // Verify process exists
    process_t* process = NULL;
    int idx;
    for (idx = 0; idx < num_processes; idx++)
    {
        if (strcmp(processes[idx].pid, pid) == 0)
        {
            process = &processes[idx];
            break;
        }
    }

    if (process == NULL)
    {
        printf("Process does not exist!\n");
        return;
    }


    // Determine which holes are adjacent to process
    printf("Releasing memory for process %s\n", pid);
    hole_t* hole_before = NULL;
    hole_t* hole_after = NULL;
    int hole_after_idx = -1;
    for (int i = 0; i < num_holes; i++)
    {
        if (holes[i].start == (process->r_relocation + process->size))
        {
            hole_after = &holes[i];
            hole_after_idx = i;
        }
        else if (holes[i].end == process->r_relocation - 1)
        {
            hole_before = &holes[i];
        }
    }

    // Four scenarios for releasing a process
    // 1. holes before and after process
    // 2. hole only before process
    // 3. hole only after process
    // 4. no holes before or after process
    if (hole_before != NULL && hole_after != NULL)
    {
        hole_before->end = hole_after->end;
        hole_before->len += hole_after->len + process->size;

        // Shift hole information in array to keep it contiguous
        for (int i = hole_after_idx; i < num_holes; i++)
        {
            holes[i].start  = holes[i + 1].start;
            holes[i].end    = holes[i + 1].end;
            holes[i].len    = holes[i + 1].len;
        }

        num_holes--;
    }
    if (hole_before != NULL && hole_after == NULL)
    {
        hole_before->end = process->r_relocation + process->r_limit;
        hole_before->len += process->size;
    }
    else if (hole_before == NULL && hole_after != NULL)
    {
        hole_after->start = process->r_relocation;
        hole_after->len += process->size;
    }
    else if (hole_before == NULL && hole_after == NULL)
    {
        hole_t new_hole;
        new_hole.start = process->r_relocation;
        new_hole.end = process->r_relocation + process->r_limit;
        new_hole.len = process->size;

        holes[num_holes] = new_hole;
        num_holes++;
    }

    // Update memory information
    mem_allocated_in_bytes -= process->size;
    mem_free_in_bytes += process->size;

    // Shift process information in array to keep it contiguous
    for (int j = idx; j < num_processes; j++)
    {
        processes[j].pid            = processes[j + 1].pid;
        processes[j].r_limit        = processes[j + 1].r_limit;
        processes[j].r_relocation   = processes[j + 1].r_relocation;
        processes[j].size           = processes[j + 1].size;
    }

    // remove process (actual process information is still in array but will be overwritten when new process is allocated space)
    num_processes--;

    printf("Successfully released memory for process %s\n", pid);
}

void status()
{
    printf("Partitions [Allocated memory = %i]\n", mem_allocated_in_bytes);
    for (int i = 0; i < num_processes; i++)
    {
        printf("Address [%i:%i] Process %s\n", processes[i].r_relocation, processes[i].r_relocation + processes[i].r_limit, processes[i].pid);
    }
    printf("\n");
    
    printf("Holes [Free memory = %i]\n", mem_free_in_bytes);
    for (int i = 0; i < num_holes; i++)
    {
        printf("Address [%i:%i] len = %i\n", holes[i].start, holes[i].end, holes[i].len);
    }
}