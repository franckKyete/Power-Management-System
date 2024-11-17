#ifndef CLI_H
#define CLI_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../building.h"

// #include "utils/linkedlist.h"

#define MAX_ROOM 20
#define PORT 8080


void displaySummary(Building *);
void displayMenu();
void awaitCommand();
void fetchData();

void *cli_socket( Building *building);

#endif