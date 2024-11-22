#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

typedef struct{
    int is_written;
    float values[80];
}shared_memory;

#endif