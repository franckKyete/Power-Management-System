#ifndef CLI_H
#define CLI_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// #include "utils/linkedlist.h"

#define MAX_ROOM 20

typedef struct 
{
    bool presence;
    float CO2_level;
    float temperature;
    float power_usage;
    bool light;
    bool ventilation;
    bool natural_light;
    int power_source;
} Room;


void displaySummary(Room *rooms, int size);
void displayMenu();
void awaitCommand();
void fetchData();

#endif