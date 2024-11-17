#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "cli.h"

void displaySummary(Building* building){


    printf("| %-11s | %-11s | %-11s | %-11s | %-11s | %-11s | %-11s |\n", "Piece", "CO2 (\%)", "Presence", "Temp (C)", "Usage (W)", "Lumiere", "Ventilation");
    printf("|-------------|-------------|-------------|-------------|-------------|-------------|-------------|\n");

    for (int i = 0; i < building->size; i++) {


        printf("| %-11d | %-11f | %-11f | %-11f | %-11f | %-11d | %-11d |\n", 
        building->rooms[i].id,
        building->rooms[i].sensors[CO2_SENSOR].value, 
        building->rooms[i].sensors[PRESENCE_SENSOR].value,
        building->rooms[i].sensors[TEMPERATURE_SENSOR].value, 
        building->rooms[i].sensors[POWER_METER].value, 
        building->rooms[i].light, 
        building->rooms[i].ventilation);

    }

}


int main(int argc, char **argv){
 
    Building building;

    pthread_t cli_socket_thread;

    if(pthread_create(&cli_socket_thread, NULL, cli_socket, &building) != 0){
        perror("Failed to create cli_socket client thread");
        exit(EXIT_FAILURE);
    }

    while (1)
    {    
        system("clear");
        displaySummary(&building);
        sleep(1);
    }
    
    return 0;
}